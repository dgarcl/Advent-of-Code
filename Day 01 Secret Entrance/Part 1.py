vfid = open("input.txt")

pwd = 0
dial = 50

#Loop through the sequence of rotations
for rotation in vfid:
    #Extract the direction and the distance of the rotation
    dir = rotation[0]
    dist = int(rotation[1:-1])

    #Move the dial left or right depending on the direction
    if dir == 'R':
        dial += dist
    else:
        dial -= dist

    #Use the modulus to update the number the dial points to
    dial %= 100

    #Count the number of times the dial is left pointing at 0
    if dial == 0:
        pwd += 1

vfid.close()

print("The actual password to open the door is", pwd)