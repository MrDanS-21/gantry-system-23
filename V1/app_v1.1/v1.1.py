from flask import Flask, render_template, redirect, url_for, request, session, flash, jsonify
from datetime import datetime
import pymysql
import serial
import threading
import time
import queue
import os
import re

ser = serial.Serial('/dev/tty.usbmodem14101', 9600)
time.sleep(2)  # wait for the serial connection to initialize

app = Flask(__name__)
# Make the WSGI interface available at the top level so wfastcgi can get it.
wsgi_app = app.wsgi_app
app.config['SECRET_KEY'] = 'qwertyuiop1234567890:D'

def create_connection():
    return pymysql.connect(
        host='localhost',
        user='root',
        password='',
        db='GantrySystem23',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor)
    
serial_queue = queue.Queue()
serial_queue2 = queue.Queue()
server_queue = queue.Queue()

def log(message):
    server_queue.put(message)
    print(message)

def read_from_serial():
    while True:
        message = ser.readline().decode('utf-8').strip()
        serial_queue.put(message)
        serial_queue2.put(message)

def ideal_position():
    connection = create_connection()
    try:
        with connection.cursor() as cursor:
            # Find the first unoccupied position
            cursor.execute("SELECT position_id FROM position_table WHERE id=0 ORDER BY position_id LIMIT 1")
            result = cursor.fetchone()

            if result:
                position_id = result['position_id']

                # Fetch coordinates for the position_id
                cursor.execute("SELECT x, z, y FROM coords_table WHERE position_id=%s", (position_id))
                coords = cursor.fetchone()

                return coords['x'], coords['z'], coords['y'], position_id
            else:
                raise Exception("Warehouse is full!")
    except Exception as e:
        log(f"Error: {e}")
    finally:
        connection.close()
    

import_buffer = []
def process_imports():
    connection = create_connection()
    try:
        while True:
            while not serial_queue2.empty():
                message = serial_queue2.get()
                # Check if the message matches the expected IMPORT pattern
                match = re.match(r'IMPORT\((\d+(\.\d+)?),(\d+(\.\d+)?),(\d+(\.\d+)?)\);', message)
                if match:
                    r, g, b = map(float, match.groups()[::2])  # Extract every second group to get the RGB values
                    import_buffer.append((r, g, b))
                    if len(import_buffer) > 5:
                        import_buffer.pop(0)  # remove the oldest entry
                    # Check if the last 5 imports have similar RGB values
                    if len(import_buffer) == 5 and all(
                            abs(import_buffer[-1][i] - import_buffer[-2][i]) <= 0.5 and
                            abs(import_buffer[-1][i] - import_buffer[-3][i]) <= 0.5 and
                            abs(import_buffer[-1][i] - import_buffer[-4][i]) <= 0.5 and
                            abs(import_buffer[-1][i] - import_buffer[-5][i]) <= 0.5
                            for i in range(3)):
                        # Calculate the average of the last four RGB values
                        avg_r = round(sum(val[0] for val in import_buffer[1:]) / 4, 2)
                        avg_g = round(sum(val[1] for val in import_buffer[1:]) / 4, 2)
                        avg_b = round(sum(val[2] for val in import_buffer[1:]) / 4, 2)
                        log(f"Importing Package: ({avg_r}, {avg_g}, {avg_b})")
                        # Clear the buffer
                        import_buffer.clear()
                        
                        with connection.cursor() as cursor:
                            cursor.execute("SELECT uid FROM item_info WHERE ABS(eR - %s) <= 2 AND ABS(eG - %s) <= 2 AND ABS(eB - %s) <= 2", (avg_r, avg_g, avg_b))
                            result = cursor.fetchone()
                            if result:
                                uid = result['uid']
                                
                            cursor.execute("INSERT INTO item_colors (r, g, b, uid, timein) VALUES (%s, %s, %s, %s, %s)", (avg_r, avg_g, avg_b, uid, datetime.utcnow()))
                            item_id = cursor.lastrowid
                            
                            # Get the ideal position
                            x, z, y, position_id = ideal_position()
                            # Update position_table to indicate the spot is taken
                            cursor.execute("UPDATE position_table SET id=%s WHERE position_id=%s", (item_id, position_id))

                            # Send the command to the Arduino
                            data = f"move 190,44000,1840 {x},{z},{y}"
                            ser.write(data.encode('utf-8'))

                            connection.commit()  # Commit changes to the database
    except Exception as e:
        log(f"Failed to process imports due to: {e}")
        return

def export(position_id):
    connection = create_connection()
    try:
        with connection.cursor() as cursor:
            # Check if there is a package at the provided position_id
            cursor.execute("SELECT id FROM position_table WHERE position_id=%s", (position_id))
            result = cursor.fetchone()

            if not result:
                raise Exception(f'No position with ID: {position_id}')
            
            package_id = result['id']

            if package_id == 0:
                raise Exception(f'No package at position {position_id}')

            # Fetch the x, z, y coordinates of the package
            cursor.execute("SELECT x, z, y FROM coords_table WHERE position_id=%s", (position_id,))
            coords = cursor.fetchone()

            if not coords:
                raise Exception(f'Coordinates not found for position ID: {position_id}')

            x, z, y = coords['x'], coords['z'], coords['y']

            # Delete the entry from item_colors table
            cursor.execute("DELETE FROM item_colors WHERE id=%s", (package_id))

            # Update position_table to mark the spot as unoccupied
            cursor.execute("UPDATE position_table SET id=0 WHERE position_id=%s", (position_id))

            # Command to move the robot
            move_command = f"move {x},{z},{y} 3500,44000,2000"
            ser.write(move_command.encode('utf-8'))

        connection.commit()  # Don't forget to commit changes to the database
        
    except Exception as e:
        log(f"Error: {e}")
        raise Exception(str(e))
    
    finally:
        connection.close()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_serial_data', methods=['GET'])
def get_serial_data():
    messages = []
    while not serial_queue.empty():
        messages.append(serial_queue.get())
    return jsonify(messages)

@app.route('/get_server_data', methods=['GET'])
def get_server_data():
    messages = []
    while not server_queue.empty():
        messages.append(server_queue.get())
    return jsonify(messages)

@app.route('/send_serial_data', methods=['POST'])
def send_serial_data():
    data = request.json.get('data', '')
    ser.write(data.encode('utf-8'))
    return jsonify({'status': 'sent', 'data': f"Sent: {data}"})

@app.route('/send_server_data', methods=['POST'])
def send_server_data():
    data = request.json.get('data', '')
    if not data.startswith("export "):
        return jsonify({'status': 'error', 'message': 'Invalid command'})

    # Extract position_id from the data string
    try:
        position_id = int(data.split(" ")[1])
        export(position_id)
        return jsonify({'status': 'success', 'message': f'Package at position {position_id} is being exported.'})
    except (IndexError, ValueError):
        return jsonify({'status': 'error', 'message': 'Invalid position ID'})
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)})

@app.route('/serial_monitor', methods=['GET', 'POST'])
def serial_monitor():
    if request.method == 'POST':
        data = request.form.get('data_to_send')
        if data:
            ser.write(data.encode('utf-8'))
        return redirect(url_for('serial_monitor'))

    return render_template('serial_monitor.html')

@app.route('/warehouse', methods=['GET'])
def warehouse():
    connection = create_connection()
    with connection.cursor() as cursor:
        cursor.execute("SELECT item_colors.uid, item_colors.timein, item_info.* FROM item_colors INNER JOIN item_info ON item_colors.uid = item_info.uid")
        data = cursor.fetchall()
        
    return render_template('warehouse.html', data=data)

@app.route('/warehouse/<int:uid>', methods=['POST'])
def warehouse_export(uid):
    connection = create_connection()
    try:
        with connection.cursor() as cursor:
            cursor.execute("SELECT pt.position_id FROM position_table AS pt JOIN item_colors AS ic ON pt.id = ic.id WHERE ic.uid = %s", (uid))
            result = cursor.fetchone()
            export(result['position_id'])
            return redirect(url_for('warehouse'))
    except Exception as e:
        msg = f"An error occoured trying to export position {result['position_id']}: {e}"
        log(msg)
        flash(msg)
        return redirect(url_for('warehouse'))
        

@app.route('/testing')
def testing():
    return render_template('testing.html')

if __name__ == "__main__":
    serial_thread = threading.Thread(target=read_from_serial)
    serial_thread.start()
    
    process_thread = threading.Thread(target=process_imports)
    process_thread.start()
    
    HOST = os.environ.get('SERVER_HOST', 'localhost')
    try:
        PORT = int(os.environ.get('SERVER_PORT', '5555'))
    except ValueError:
        PORT = 5555
    app.run(HOST, PORT)