vfid = open("input.txt")

RedTiles = []

for line in vfid:  # Read the red tiles from our input
    comma = line.find(",")

    # Append the location of each red tile to a list
    RedTiles.append([int(line[: comma]), int(line[comma + 1 : -1])])

vfid.close()

cols = []
rows = []

for i in range(len(RedTiles)): # We create two arrays that keep track of the locations of the red tiles
    if i % 2 == 0: # Tiles are connected, so we only consider every other tile to avoid duplicates 
        cols.append(RedTiles[i][0])
        rows.append(RedTiles[i][1])

# Sort the arrays
cols.sort()
rows.sort()

Floor = []

# With our red tiles location information, we build a compressed diagram of the theater floor
for i in range(len(rows) - 1):
    temp = []

    for j in range(len(cols) - 1):
        # Choose a representative tile of this rectangle of the floor
        tile = [int((cols[j] + cols[j + 1]) / 2), int((rows[i] + rows[i + 1]) / 2)]

        green = False

        # Use ray casting to determine wether tiles in this rectangle are green or not
        for k in range(len(RedTiles)):
            if (RedTiles[k][0] == RedTiles[k - 1][0]) and (tile[0] < RedTiles[k][0]): # Only account for red tiles connected vertically to the right of our tile
                if min(RedTiles[k - 1][1], RedTiles[k][1]) < tile[1] < max(RedTiles[k - 1][1], RedTiles[k][1]): # Check if our tile crosses vertical lines formed by red tiles
                    green = not green

        if green: # We store the color of the tiles of the floor using a binary matrix
            temp.append(1)
        else:
            temp.append(0)

    Floor.append(temp)

GreenTiles = []

# Construct a zero matrix with one more row and one more column than our floor matrix 
for i in range(len(Floor) + 1): 
    GreenTiles.append([0] * (len(Floor) + 1))

# Build a padded prefix sum matrix of green tiles
for i in range(len(Floor)):
    for j in range(len(Floor[i])):
        if i > 0 and j > 0: # Use the previously computed entries to compute the new ones
            GreenTiles[i + 1][j + 1] = GreenTiles[i][j + 1] + GreenTiles[i + 1][j] - GreenTiles[i][j] + Floor[i][j]
        elif i > 0: # If we are at the first column, only use the entry from the previous row
            GreenTiles[i + 1][j + 1] = GreenTiles[i][j + 1] + Floor[i][j]
        elif j > 0: # If we are at the first row, only use the entry from the previous column
            GreenTiles[i + 1][j + 1] = GreenTiles[i + 1][j] + Floor[i][j]
        else: # If we are at the first entry, start the prefix sum with the first entry of the floor matrix
            GreenTiles[i + 1][j + 1] = Floor[i][j]

largestArea = 0

# Calculate the largest area of any rectangle that only uses red and green tiles
for i in range(len(RedTiles)): 
    for j in range(i + 1, len(RedTiles)):
        # Find the top-left and bottom-right tiles of our rectangle
        starttile = [min(RedTiles[i][0], RedTiles[j][0]), min(RedTiles[i][1], RedTiles[j][1])]
        endtile = [max(RedTiles[i][0], RedTiles[j][0]), max(RedTiles[i][1], RedTiles[j][1])]

        # Map those tiles to an entry of our diagram of the floor
        startindex = [cols.index(starttile[0]), rows.index(starttile[1])]
        endindex = [cols.index(endtile[0]), rows.index(endtile[1])]

        # Calculate the expected number of green tiles of our rectangle if it were valid
        expectedGreenTiles = (endindex[0] - startindex[0]) * (endindex[1] - startindex[1])

        # Calculate the actual number of green tiles of our rectangle using our prefix sum
        actualGreenTiles = GreenTiles[endindex[1]][endindex[0]]
        actualGreenTiles -= GreenTiles[startindex[1]][endindex[0]] + GreenTiles[endindex[1]][startindex[0]]
        actualGreenTiles += GreenTiles[startindex[1]][startindex[0]]

        if expectedGreenTiles == actualGreenTiles: # If both quantities match, we have a valid rectangle
            area = (1 + abs(RedTiles[i][0] - RedTiles[j][0])) * (1 + abs(RedTiles[i][1] - RedTiles[j][1]))

            if (area > largestArea): # If we find a new largest area, we update the largest area
                largestArea = area

print("Using only red and green tiles, the largest area of any rectangle you can make is", largestArea)