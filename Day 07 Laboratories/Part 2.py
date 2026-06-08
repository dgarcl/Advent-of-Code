vfid = open("input.txt")

diagram = []

# Read the contents of our input
for line in vfid:
    diagram.append(line[: -1])

vfid.close()

# Loop through all lines of our diagram
for i in range(len(diagram) - 1):
    # Loop through the characters of each line
    for k in range(len(diagram[i])):
        # If our character is "S", a tachyon beam extends downwards from it
        if diagram[i][k] == "S":
            diagram[i + 1] = diagram[i + 1][0 : k] + "|" + diagram[i + 1][k + 1 :]
        # If our character is a tachyon beam, we have to look at whats below it
        elif diagram[i][k] == "|":
            # If the character below is a splitter, two new beams are emitted from the splitter
            if diagram[i + 1][k] == "^":
                diagram[i + 1] = diagram[i + 1][0 : k - 1] + "|^|" + diagram[i + 1][k + 2 :] 
            # If not, the beam continues downwards
            else:
                diagram[i + 1] = diagram[i + 1][0 : k] + "|" + diagram[i + 1][k + 1 :]

possiblePaths = []

# Iterate through our completed diagram to find all possible paths the particle can take
for i in range(len(diagram)):
    beam = diagram[i].find("|")

    # New information is only added at even lines of the diagram
    if (i % 2 == 0):
        temp = []

        # Loop through the possible beams in that line
        for j in range(int(i / 2) + 1):
            paths = 0

            # If there was no beams, then we are in the "S" line so we have 1 path
            if beam == -1:
                paths += 1

            # Add to avoid index out of range error
            if beam - 1 > 0:
                # If theres a splitter left of the beam, then we add the paths that go through that splitter
                if diagram[i][beam - 1] == "^":
                    paths += possiblePaths[-1][j - 1]
            
            # Add to avoid index out of range error
            if beam + 1 < len(diagram[i]):
                # If theres a splitter right of the beam, then we add the paths that go through that splitter
                if diagram[i][beam + 1] == "^":
                    paths += possiblePaths[-1][j]

            # Add to avoid index out of range error
            if i > 0:
                # If there is a beam on top, then we add the paths that go through that beam
                if diagram[i - 1][beam] == "|":
                    paths += possiblePaths[-2][j - 1]

            temp.append(paths)
            beam += 2

        possiblePaths.append(temp)

timelines = 0

# Loop through the possible paths that a particle can take to the last line in our diagram
for i in range(len(diagram[-1])):
    # If theres a beam, we add all the paths that go through it
    if diagram[-1][i] == "|":
        # If is even, then its stored on the last row
        if i % 2 == 0:
            timelines += possiblePaths[-1][int(i / 2)]
        # It its odd, its stored in the second to last row
        else:
            timelines += possiblePaths[-2][int(i / 2)]

print("In total, a single tachyon particle will end up on", timelines, "different timelines")
