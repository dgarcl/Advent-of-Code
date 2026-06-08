vfid = open("input.txt")

ranges = []

blankline = False

# Read through the database and append the line into the correct list
for line in vfid:
    # The empty line separates the ranges from the IDs
    if line == "\n":
        blankline = True
    # If the blank line has not been reached, we append the line into our ingredient ID ranges
    elif not blankline:
        dash = line.find("-")

        # We find the start and the end of our range
        start = int(line[0 : dash])
        end = int(line[dash + 1 : -1])

        ranges.append((start, end))

vfid.close()

# Sort our igredient ID ranges to find the fresh ingredient ID ranges
ranges.sort()
IDs = []

# Loop through our igredient ID ranges
for start, end in ranges:
    # If our range doesnt overlap with the previous one, we append it
    if (len(IDs) == 0) or (IDs[-1][1] < start):
        IDs.append([start, end])  
    # If our range overlaps with the previous one, we modify it by taking their union
    else:
        IDs[-1][1] = max(IDs[-1][1], end)

fresh = 0

# Loop through our fresh ingredient ID ranges to find how many ingredient IDs are considered to be fresh
for start, end in IDs:
    fresh += (end + 1) - start

print("According to the fresh ingredient ID ranges,", fresh, "ingredient IDs are considered to be fresh")
