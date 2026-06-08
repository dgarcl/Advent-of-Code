vfid = open("input.txt")

diagram = []

# Read the contents of our input
for line in vfid:
    diagram.append(line[: -1])

vfid.close()

splits = 0

# Loop through all lines of our diagram
for i in range(len(diagram) - 1):
    # Loop through the characters of each line
    for j in range(len(diagram[i])):
        # If our character is "S", a tachyon beam extends downwards from it
        if diagram[i][j] == "S":
            diagram[i + 1] = diagram[i + 1][0 : j] + "|" + diagram[i + 1][j + 1 :]
        # If our character is a tachyon beam, we have to look at whats below it
        elif diagram[i][j] == "|":
            # If the character below is a splitter, two new beams are emitted from the splitter
            if diagram[i + 1][j] == "^":
                splits += 1
                diagram[i + 1] = diagram[i + 1][0 : j - 1] + "|^|" + diagram[i + 1][j + 2 :] 
            # If not, the beam continues downwards
            else:
                diagram[i + 1] = diagram[i + 1][0 : j] + "|" + diagram[i + 1][j + 1 :]

print("The beam will be split", splits, "times")
