# Gantry System 23

An automated storage system built around a two-axis gantry. When a package arrives at the intake point, a colour sensor detects it. A Flask web server then identifies the package from its colour, gives it a free storage slot in a MySQL database, and tells the gantry to move it there. You can export a stored package from the web interface, and the gantry brings it back out.

A demo video (`V1/video.mov`) and a technical drawing (`V1/TechnicalDrawing.pdf`) are included.

## How it works

```
 ┌─────────────────────┐    USB serial (9600 baud)    ┌──────────────────────┐
 │  Flask server       │ ─── commands ─────────────▶ │  Arduino firmware    │
 │  (V1/app_v1.1)      │ ◀── INFO / ERROR / IMPORT ─ │  (V1/v1)             │
 └─────────┬───────────┘                              └──────────┬───────────┘
           │                                                    │
     MySQL database                          X/Z steppers, limit switches,
   (slots, items, stock)                     lift + claw servos, TCS34725
                                             colour sensor
```

**Firmware ([V1/v1](V1/v1))**
- At power-up it drives the X and Z axes to their limit switches to set the origin (homing), retracts the lift, opens the claw, and records the ambient colour reading.
- Each loop it reads one command from serial, runs it, and replies with a line of `;`-separated messages in the form `INFO(...)`, `ERROR(<code> ...)` or `IMPORT(r,g,b)`.
- It sends `IMPORT` whenever the colour sensor reading moves away from the ambient baseline, which means a package is present.
- It checks bounds on every move (X: 0–16000 steps, Z: 0–44000 steps, lift: 1000–2000 µs servo pulse). It also refuses a `move` while the claw is holding something.

| Command | Description |
| --- | --- |
| `moveGantry X,Z` | Move the gantry to step position (X, Z) |
| `pick H` / `place H` | Lower the lift to height H, close/open the claw, retract |
| `move iX,iZ,iY fX,fZ,fY` | Pick up at the first position, place at the second |
| `stepStepper X\|Z <dir>` | Jog an axis 100 steps |
| `height H` | Set the lift servo directly |
| `sudo shutdown` | Put the microcontroller into power-down sleep |

**Server ([V1/app_v1.1](V1/app_v1.1))**
- A background thread reads serial output. A second thread watches for `IMPORT` messages.
- An import is accepted once five readings in a row agree to within 0.5. The server matches the averaged RGB against known items (`item_info`), logs the arrival, reserves the first free slot, and sends a `move` from the intake to that slot.
- An export clears the slot in the database and sends a `move` from the slot to the drop-off point.
- Pages:
  - `/`: home page
  - `/warehouse`: current stock, with an export button per item
  - `/serial_monitor`: live serial and server logs, plus manual command entry

**Database ([V1/GantrySystem23.sql](V1/GantrySystem23.sql))**

| Table | Contents |
| --- | --- |
| `coords_table` | X/Z/Y coordinates for each of the 15 storage slots |
| `position_table` | Which stored item (if any) is in each slot |
| `item_info` | Known item types, their reference colour, and descriptions |
| `item_colors` | Items currently in storage, with measured colour and time in |
| `item_tags`, `logs` | Present in the schema; not yet used |

## Repository layout

```
V1/
  v1/                      Arduino firmware for the gantry
  app_v1.1/                Flask web server (current)
  app_v1.0/                Earlier GTK serial monitor (deprecated)
  GantrySystem23.sql       MySQL schema and seed data
  GantrySystemPCB2023.dip  PCB design (DipTrace)
  TechnicalDrawing.pdf     Mechanical drawing (Fusion 360)
  video.mov                Demo video
Testing/                   Standalone sketches used to bring up each subsystem
                           (steppers, servos, limit switches, colour sensor,
                           presence detection, serial protocol, Bluetooth)
```

## Running it

**Hardware wiring (from the firmware):**

| Pin | Function |
| --- | --- |
| 2 / 3 | X stepper direction / step |
| 15 / 16 | Z stepper direction / step |
| 14 / 17 | Z / X limit switch (`INPUT_PULLUP`) |
| 10 / 9 | Lift (linear) servo / claw (rotary) servo |
| I²C | TCS34725 colour sensor |

1. **Firmware.** Open `V1/v1/v1.ino` in the Arduino IDE, install the `DFRobot_TCS34725` library, and upload. The `Servo` and `Wire` libraries ship with the IDE.
2. **Database.** Import `V1/GantrySystem23.sql` into a local MySQL server. The server connects to `localhost` as `root` with no password; to change that, edit `create_connection()` in `v1.1.py`.
3. **Server.**
   ```sh
   pip install flask pymysql pyserial
   cd V1/app_v1.1
   python v1.1.py
   ```
   - The serial port is hardcoded near the top of `v1.1.py`; change it to match your machine.
   - The server listens on `localhost:5555` by default. Set the `SERVER_HOST` and `SERVER_PORT` environment variables to change this.

## Known limitations

- The `/logs` link on the home page has no matching route, and the `logs` table is never written to.
- The two axes move one after the other rather than at the same time, and there is no acceleration ramp.
- Packages are identified by colour alone. Two items whose reference colours are within ±2 of each other can't be told apart.
