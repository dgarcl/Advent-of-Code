vfid = open("input.txt")

serverRack = {}

for line in vfid: # Read the list of the devices and their outputs
    # Find the label of the device 
    device = line[: line.find(":")]
    line = line[line.find(" ") + 1 :]
    outputs = []

    # Append the labels of all the outputs of the device to a list
    while(True):
        if (line.find(" ") != -1):
            outputs.append(line[: line.find(" ")])
            line = line[line.find(" ") + 1 :]
        else:
            outputs.append(line[: -1])
            break

    serverRack[device] = outputs # Add the device and its outputs to a dictionary

vfid.close()

# Construct a queue of the devices left to explore 
deviceQueue = serverRack["you"]
paths = len(serverRack["you"])

while (len(deviceQueue) > 0):
    # Pop the first device of the queue and add a path for each of its output 
    device = deviceQueue.pop(0)
    paths += len(serverRack[device]) - 1 # We substract the path from where we came from

    for output in serverRack[device]: # Add the devices' outputs to the queue
        if output != "out":
            deviceQueue.append(output)

print(paths, 'different paths lead from "you" to "out"')
