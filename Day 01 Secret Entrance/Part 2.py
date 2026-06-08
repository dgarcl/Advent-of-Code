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
    
    #Count the number of times the dial points at 0 throughout the rotation
    rem = dial % 100
    zeros = abs((dial - rem) / 100)

    #Update the password and the number the dial points to
    pwd += zeros
    dial = rem

vfid.close()

print("The password to open the door is", int(pwd))