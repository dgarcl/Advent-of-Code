vfid = open("input.txt")

redTiles = []

for line in vfid: # Read the red tiles from our input
    comma = line.find(",")

    # Append the location of each red tile to a list
    redTiles.append([int(line[: comma]), int(line[comma + 1 : -1])])

vfid.close()

largestArea = 0

# Calculate the largest area of any rectangle
for i in range(len(redTiles)):
    for j in range(i + 1, len(redTiles)):
        area = (1 + abs(redTiles[i][0] - redTiles[j][0])) * (1 + abs(redTiles[i][1] - redTiles[j][1]))

        if (area > largestArea): # If we find a new largest area, we update the largest area
            largestArea = area

print("The largest area of any rectangle you can make is", largestArea)