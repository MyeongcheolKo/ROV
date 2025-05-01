import serial
import csv
import matplotlib.pyplot as plt
import time
import numpy as np 

arduinoData = serial.Serial('/dev/cu.usbmodem1101',9600)

start_mission = False
first_row = True

while True:
    if start_mission:
        with open("float_data.csv", "w", newline='') as file:
            print("Waiting for data...", end = "", flush = True)
            last_time = time.time()
            while True:
                if arduinoData.in_waiting > 0:
                    data = arduinoData.readline()
                    decoded_data = data.decode('utf-8').strip()
                    print(decoded_data)
                    if decoded_data == "Data received":
                        break
                    else:
                        if first_row:
                            file.write(decoded_data)
                            file.write("\n")
                            first_row = False
                        else:
                            split_data = decoded_data.split(",")
                            file.write(split_data[0])
                            file.write(",")
                            file.write(str(float(split_data[1]) / 1000))
                            file.write(",")
                            file.write(split_data[2])
                            file.write("\n")
                            file.flush()
                else:
                    if time.time() >= last_time + 3:
                        last_time = time.time()
                        print(".", end = "", flush = True)
                    

        with open("float_data.csv", "r") as file:
            print("Generating plot...")
            csv_reader = csv.reader(file)
            next(csv_reader)  
            x_values = []
            y_values = []
            for row in csv_reader:
                x_values.append(int(float(row[1])))
                y_values.append(float(row[2]))
            # print(x_values)
            # print(y_values)
            plt.plot(x_values, y_values, linestyle = '-', marker= '.')
            plt.xlabel("Time Since Start(s)")
            plt.ylabel("Depth(m)")
            plt.title("Hammerhead ROV Float Data")
            x_min = min(x_values)
            x_max = max(x_values)
            plt.xticks(np.arange(x_min, x_max, 5))
            plt.plot([x_min, x_max], [3, 3], color='red', linestyle='--')
            plt.plot([x_min, x_max], [2, 2], color='red', linestyle='--')
            plt.grid()
            plt.show()

            start_mission = False
    else:
        while input("Enter Command: ") != "start":
            print("Please enter 'start' to start the mission.")
        
        start_mission = True
        first_row = True
        arduinoData.write(b"s")
        print("Starting mission...")
        
    
        
    



