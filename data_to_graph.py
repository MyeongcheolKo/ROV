import csv
import matplotlib.pyplot as plt

def cast_to_float(input_list):
    for i in range(len(input_list)):
        input_list[i] = float(input_list[i])

with open('data.csv', newline='') as csvfile:
    data = csv.reader(csvfile)
    next(data)
    i = 0
    x = []
    y = []

    for row in data:
        if i == 0:
            x = row[1:]
            cast_to_float(x)

        else:
            y = row[1:]
            cast_to_float(y)
            plt.plot(x, y, linestyle = '-', marker= '.', label = row[0])
        # print(x)
        # print(y)
        i += 1
    plt.xticks(x)



plt.xlabel("Day")
plt.ylabel("# of Sturgeon")
plt.title("Number of Sturgeon detected at each receiver over time")
plt.legend()
plt.grid()
plt.show()
