vfid = open("input.txt")

ranges = []
IDs = []

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
    # If the blank line has been reached, we append the line into our ingredient IDs
    else:
        IDs.append(int(line[: -1]))

vfid.close()

fresh = 0

# Loop through our ingredient IDs
for ingredient in IDs:
    spoilt = True

    # Check for each of our ingredient ID ranges
    for start, end in ranges:
        # If the ingredient ID is inside any ID range, then it is fresh
        if (start <= ingredient) and (ingredient <= end):
            spoilt = False
        
    # If an ingredient ID is fresh, then we take note of it
    if not spoilt:
        fresh += 1

print(fresh, "of the available ingredient IDs are fresh")
