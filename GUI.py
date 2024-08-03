import tkinter as tk
from tkinter import messagebox
import serial
import threading

# Serial communication setup
ser = serial.Serial('COM5', 9600)  # Update with your COM port
ser.flushInput()

# GUI setup
root = tk.Tk()
root.title("Andon System")

# Colors
NORMAL_COLOR = "green"
ATTENTION_COLOR = "blue"
CRITICAL_COLOR = "red"

# Create labels for status display
status_label = tk.Label(root, text="System Status", font=("Arial", 100))
status_label.pack(pady=20)

status_message = tk.Label(root, text="Normal", font=("Arial", 70), fg=NORMAL_COLOR)
status_message.pack(pady=20)

# Function to update the GUI based on serial data
def update_gui():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line == "ATTENTION":
                status_message.config(text="Attention Needed", fg=ATTENTION_COLOR)
            elif line == "CRITICAL":
                status_message.config(text="Critical Error", fg=CRITICAL_COLOR)
            elif line == "NORMAL":
                status_message.config(text="Normal", fg=NORMAL_COLOR)
            else:
                status_message.config(text="Unknown Status", fg="gray")
        root.update_idletasks()
        root.update()

# Run the update_gui function in a separate thread
thread = threading.Thread(target=update_gui, daemon=True)
thread.start()

# Start the GUI loop
root.mainloop()