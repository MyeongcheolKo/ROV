import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Load the image
img = plt.imread("/Users/krisko/Documents/ROV/invasion_carps/illinois_river_map.png")
fig, ax = plt.subplots(figsize=(15, 10))  


#data, # column: region | row: year
data_set = [
    [2016,1,0,0,0,0],
    [2017,0,0,0,0,0],
    [2018,1,0,0,0,0],
    [2019,1,0,0,0,0],
    [2020,1,1,1,0,0],
    [2021,1,1,1,0,0],
    [2022,1,1,1,0,0],
    [2023,1,1,1,1,0],
    [2024,1,1,1,1,0],
    [2025,1,1,1,1,0]
]

regions = {
    "1": {"points": [(220, 815), (212, 821), (208, 817), (202, 804), (201, 777), (203, 740), (205, 703), (199,685), (203, 665), (212, 642), (222, 622), (232, 619), (238, 609)], "color": "red"},
    "2": {"points": [(238, 609), (248,599), (264, 596), (269, 586), (278, 577), (290, 555), (304.5, 545), (316, 528), (338, 520)], "color": "green"},
    "3": {"points": [(342,520), (359, 491), (367, 463), (372, 457), (373, 450), (379, 447), (380, 429), (395, 410), (392, 384), (400, 379), (431, 377)], "color": "orange"},
    "4": {"points": [(431, 377), (465, 373), (500, 380), (548, 363), (559, 353), (569, 335), (571, 319), (592, 300), (617, 279), (639, 269)], "color": "blue"},
    "5": {"points": [(611, 155), (609, 179), (607, 186.6), (609, 199), (613, 211), (629, 236), (631, 247), (635, 254), (636.5, 264), (639, 271), (641, 282), (646, 292), (658, 308), (666, 311), (682, 321), (696, 320), (733, 306.5), (753, 291), (773, 264), (778, 248)], "color": "purple"},
}

#show all region
# for region, data in regions.items():
#     x, y = zip(*data["points"])
#     ax.plot(x, y, linestyle="-", linewidth=3, color=data["color"], label=region)

text = ax.text(700, 656, "", weight="bold", fontsize=15, color='red')
plt.imshow(img)  

def update(r):
    ax.clear()
    ax.imshow(img)
    
    for c in range(1, len(data_set[0])):
        if data_set[r][c] == 1:
            region = str(c)
            x, y = zip(*regions[region]["points"])
            ax.plot(x, y, linestyle="-", linewidth=3, color=regions[region]["color"], label=f"Region {region}")    

    text = ax.text(700, 656, f"Year: {data_set[r][0]}", weight="bold", fontsize=15, color='red') 
    return text, 


ani = animation.FuncAnimation(fig, update, frames=len(data_set), interval=1500, repeat=True)


ax.set_xticks([])
ax.set_yticks([])
plt.show()