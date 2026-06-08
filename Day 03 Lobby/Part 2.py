vfid = open("input.txt")

totaljoltage = 0

#Go through each bank of batteries of our input
for bank in vfid:
    joltage = ''
    digit = 9

    #Update our joltage until we reach a length of 12
    while len(joltage) < 12:
        battery = bank.find(str(digit))

        #If the digit is found, we compute the resulting searchable bank
        if battery != -1:
            subbank = bank[battery + 1 :] 

            #If this new bank is large enough, we update the joltage, the bank, and reset the digit
            if len(subbank) >= 12 - len(joltage):
                joltage += str(digit)
                bank = subbank
                digit = 9
            #If this new bank is not large enough, we try with the next digit
            else:
                digit -= 1
        #If the digit is not found, we try with the next digit
        else:
            digit -= 1

    totaljoltage += int(joltage)

vfid.close()
    
print("The new total output joltage is", totaljoltage)
