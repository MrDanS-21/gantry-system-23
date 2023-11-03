import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib
import serial
import threading

class SerialThread(threading.Thread):
    def __init__(self, textbuffer):
        threading.Thread.__init__(self)
        self.ser = serial.Serial('/dev/tty.usbmodem14101', 9600)
        self.textbuffer = textbuffer
        self.auto_scroll = True

    def run(self):
        while True:
            if self.ser.in_waiting > 0:
                line = self.ser.readline().decode('utf-8').strip()
                GLib.idle_add(self.append_text, line + '\n')

    def append_text(self, line):
        end_iter = self.textbuffer.get_end_iter()
        self.textbuffer.insert(end_iter, line)
        if self.auto_scroll:
            self.textview.scroll_to_mark(self.textbuffer.get_insert(), 0.0, True, 0.0, 1.0)

class MyWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Serial Monitor")

        self.maximize()  # Start in full-screen

        self.box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.add(self.box)

        self.menu_bar_box = Gtk.Box()
        self.box.pack_start(self.menu_bar_box, False, False, 0)

        self.clear_button = Gtk.Button(label="Clear Output")
        self.clear_button.connect("clicked", self.on_clear_output)
        self.menu_bar_box.pack_end(self.clear_button, False, False, 0)

        self.auto_scroll_check_button = Gtk.CheckButton(label="Auto Scroll")
        self.auto_scroll_check_button.set_active(True)
        self.auto_scroll_check_button.connect("toggled", self.on_toggle_auto_scroll)
        self.menu_bar_box.pack_end(self.auto_scroll_check_button, False, False, 0)

        self.scrollable_treelist = Gtk.ScrolledWindow()
        self.scrollable_treelist.set_border_width(10)
        self.scrollable_treelist.set_policy(Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC)
        self.box.pack_start(self.scrollable_treelist, True, True, 0)

        self.textview = Gtk.TextView()
        self.textview.set_editable(False)
        self.textbuffer = self.textview.get_buffer()
        self.scrollable_treelist.add(self.textview)

        self.entry = Gtk.Entry()
        self.entry.set_text("Type here...")
        self.entry.connect("activate", self.on_activate)
        self.box.pack_start(self.entry, False, False, 0)

        self.ser_thread = SerialThread(self.textbuffer)
        self.ser_thread.textview = self.textview
        self.ser_thread.start()

    def on_clear_output(self, widget):
        self.textbuffer.set_text('')

    def on_toggle_auto_scroll(self, widget):
        self.ser_thread.auto_scroll = widget.get_active()

    def on_activate(self, widget):
        self.ser_thread.ser.write((self.entry.get_text() + '\n').encode())
        self.entry.set_text('')

win = MyWindow()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()