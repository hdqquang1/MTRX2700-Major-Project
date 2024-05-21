import serial
import time
import tkinter as tk
import threading
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import re

# def close_serial_port(port):
#     try:
#         ser = serial.Serial(port)
#         if ser.is_open:
#             ser.close()
#             print(f"Serial port {port} closed successfully.")
#     except serial.SerialException as e:
#         print(f"Error closing serial port {port}: {e}")

def read_serial(port, baudrate, display_callback, update_plot_callback, update_leaderboard_callback, update_time_callback, stop_event):
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        ser.reset_input_buffer()
        print("Port opened")

        while not stop_event.is_set():
            try:
                if ser.in_waiting > 0:
                    data = ser.readline().decode('utf-8', errors='ignore').strip()
                    print(f"Data received: '{data}'")  # Debug print

                    # Check if data is sensor data using regex
                    if re.match(r'^-?\d+\.\d{3},-?\d+\.\d{3},-?\d+\.\d{3},-?\d+\.\d{3},-?\d+\.\d{3},-?\d+\.\d{3}$', data):
                        try:
                            values = [float(value) for value in data.split(',')]
                            gyro_values = values[:3]
                            acc_values = values[3:]
                            update_plot_callback(gyro_values, acc_values)
                        except ValueError as e:
                            print(f"Error parsing data: {e} | Data: {data}")
                    elif data.startswith("Remaining time:"):
                        update_time_callback(data)
                    else:
                        display_callback(data)
                        if "total score" in data or data.startswith("Time's Up!"):
                            update_leaderboard_callback(data)

                time.sleep(0.01)

            except serial.SerialException as e:
                print(f"Serial read error: {e}")
                break

    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")

    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        if ser.is_open:
            ser.close()
            print("Port closed")

def create_display():
    root = tk.Tk()
    root.title("Serial Data Display")
    root.geometry("2560x1664")

    # Configure the grid to ensure equal dimensions for all sections
    root.grid_columnconfigure(0, weight=1)
    root.grid_columnconfigure(1, weight=1)
    root.grid_rowconfigure(0, weight=1)
    root.grid_rowconfigure(1, weight=1)

    # Create frames for each section
    top_left_frame = tk.Frame(root, width=1706, height=832)
    top_left_frame.grid(row=0, column=0, sticky="nsew")

    bottom_left_frame = tk.Frame(root, width=1706, height=832)
    bottom_left_frame.grid(row=1, column=0, sticky="nsew")

    top_right_frame = tk.Frame(root, width=854, height=832)
    top_right_frame.grid(row=0, column=1, sticky="nsew")

    bottom_right_frame = tk.Frame(root, width=854, height=832)
    bottom_right_frame.grid(row=1, column=1, sticky="nsew")

    # Top left section: Correct, pass, random word, other messages
    status_label = tk.Label(top_left_frame, text="", font=("Helvetica", 30))
    status_label.pack(expand=True, fill="both")

    # Top right section: Leaderboard
    leaderboard_label = tk.Label(top_right_frame, text="Leaderboard", font=("Helvetica", 34))
    leaderboard_label.pack()
    player1_score_label = tk.Label(top_right_frame, text="Player 1: 0", font=("Helvetica", 30))
    player1_score_label.pack()
    player2_score_label = tk.Label(top_right_frame, text="Player 2: 0", font=("Helvetica", 30))
    player2_score_label.pack()

    # Bottom left section: Remaining time display
    remaining_time_label = tk.Label(bottom_left_frame, text="Remaining time: 0 s", font=("Helvetica", 30))
    remaining_time_label.pack(expand=True, fill="both")

    # Bottom right section: Accelerometer 3D model
    acc_figure = Figure()
    acc_ax = acc_figure.add_subplot(111, projection='3d')
    acc_ax.set_xlim([-1, 1])
    acc_ax.set_ylim([-1, 1])
    acc_ax.set_zlim([-1, 1])
    acc_ax.set_box_aspect([1,1,1])  # Aspect ratio is 1:1:1

    def draw_prism(ax):
        # Define the vertices of a rectangular prism
        vertices = np.array([
            [-0.5, -0.5, -0.5],
            [ 0.5, -0.5, -0.5],
            [ 0.5,  0.5, -0.5],
            [-0.5,  0.5, -0.5],
            [-0.5, -0.5,  0.5],
            [ 0.5, -0.5,  0.5],
            [ 0.5,  0.5,  0.5],
            [-0.5,  0.5,  0.5]
        ])

        # Define the 6 faces of the prism (each face is a list of 4 vertices)
        faces = [
            [vertices[j] for j in [0, 1, 2, 3]],
            [vertices[j] for j in [4, 5, 6, 7]],
            [vertices[j] for j in [0, 1, 5, 4]],
            [vertices[j] for j in [2, 3, 7, 6]],
            [vertices[j] for j in [1, 2, 6, 5]],
            [vertices[j] for j in [4, 0, 3, 7]]
        ]

        prism = Poly3DCollection(faces, alpha=0.5, linewidths=1, edgecolors='r')
        prism.set_facecolor((0, 1, 0, 0.1))  # Set the face color of the prism
        ax.add_collection3d(prism)

        return prism, vertices

    prism, prism_vertices = draw_prism(acc_ax)
    acc_canvas = FigureCanvasTkAgg(acc_figure, master=bottom_right_frame)
    acc_canvas.draw()
    acc_canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    def rotate_prism(prism, vertices, pitch, roll):
        # Create rotation matrices
        Rx = np.array([[1, 0, 0],
                       [0, np.cos(pitch), -np.sin(pitch)],
                       [0, np.sin(pitch), np.cos(pitch)]])
        
        Ry = np.array([[np.cos(roll), 0, np.sin(roll)],
                       [0, 1, 0],
                       [-np.sin(roll), 0, np.cos(roll)]])
        
        # Rotate the vertices
        R = np.dot(Ry, Rx)
        new_vertices = np.dot(vertices, R.T)
        
        new_faces = [
            [new_vertices[j] for j in [0, 1, 2, 3]],
            [new_vertices[j] for j in [4, 5, 6, 7]],
            [new_vertices[j] for j in [0, 1, 5, 4]],
            [new_vertices[j] for j in [2, 3, 7, 6]],
            [new_vertices[j] for j in [1, 2, 6, 5]],
            [new_vertices[j] for j in [4, 0, 3, 7]]
        ]
        
        prism.set_verts(new_faces)

    def update_display(data):
        if data == "Correct!" or data == "Pass!":
            status_label.config(text=data, bg="green" if data == "Correct!" else "red", fg="white")
        else:
            status_label.config(text=data, bg="black", fg="white")

    alpha = 0.1  # Smoothing factor for the exponential moving average
    smoothed_acc = [0, 0, 0]  # Initial smoothed accelerometer values

    def update_plot(gyro_data, acc_data):
        nonlocal smoothed_acc

        # Apply exponential moving average to the accelerometer data
        smoothed_acc = [(1 - alpha) * smoothed_acc[i] + alpha * acc_data[i] for i in range(3)]

        # scatter_gyro.set_data([gyro_data[0]], [gyro_data[1]])
        # scatter_gyro.set_3d_properties([gyro_data[2]])

        pitch = np.arctan2(smoothed_acc[1], np.sqrt(smoothed_acc[0]**2 + smoothed_acc[2]**2))
        roll = np.arctan2(-smoothed_acc[0], smoothed_acc[2])
        rotate_prism(prism, prism_vertices, pitch, roll)
        # gyro_canvas.draw()
        acc_canvas.draw()

    # Initialize player scores
    player1_score = 0
    player2_score = 0

    def update_leaderboard(data):
        nonlocal player1_score, player2_score
        if "total score" in data or data.startswith("Time's Up!"):
            if "Player 1" in data:
                score = int(data.split("score is ")[1].split('!')[0])
                player1_score += score
                player1_score_label.config(text=f"Player 1: {player1_score}")
            elif "Player 2" in data:
                score = int(data.split("score is ")[1].split('!')[0])
                player2_score += score
                player2_score_label.config(text=f"Player 2: {player2_score}")

    def update_time(data):
        # Extract the numeric part from the string and convert to seconds
        match = re.search(r'\d+', data)
        if match:
            milliseconds = int(match.group())
            seconds = milliseconds // 1000
            remaining_time_label.config(text=f"Remaining time: {seconds} s")

    def periodic_update():
        update_plot([0, 0, 0], [0, 0, 0])  # Initial dummy update
        root.after(100, periodic_update)  # Schedule next update

    periodic_update()  # Start the periodic update loop

    return root, update_display, update_plot, update_leaderboard, update_time

if __name__ == "__main__":
    serial_port = '/dev/cu.usbmodem1303'  # Replace with your serial port
    baud_rate = 115200  # Replace with your baud rate
    stop_event = threading.Event()

    # close_serial_port(serial_port)
    time.sleep(1)

    root, update_display, update_plot, update_leaderboard, update_time = create_display()

    serial_thread = threading.Thread(target=read_serial, args=(serial_port, baud_rate, update_display, update_plot, update_leaderboard, update_time, stop_event))
    serial_thread.start()

    try:
        root.mainloop()
    finally:
        stop_event.set()
        serial_thread.join()
