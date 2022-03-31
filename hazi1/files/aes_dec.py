
import os

def xor(in1, in2):
    result = int(in1, 16) ^ int(in2, 16) # convert to integers and xor them together
    return '{:x}'.format(result)     # convert back to hexadecimal


hexdata1 = ""
hexdata2 = ""
with open("..\\..\\LabProfile-v1.crypt", 'rb') as f:
    hexdata1 = f.read().hex()

with open("..\\..\\LabProfile-v1.1.crypt", 'rb') as f:
    hexdata2 = f.read().hex()

hexdata1 = ' '.join(hexdata1[i: i+32] for i in range(0, len(hexdata1), 32))
hexdata2 = ' '.join(hexdata2[i: i+32] for i in range(0, len(hexdata2), 32))
hexdata1 = hexdata1.split(" ")
hexdata2 = hexdata2.split(" ")

hextemp = 16 * '20'
chunk10 = xor(hexdata1[8], hexdata2[8])
chunk10 = xor(hextemp, chunk10)

prev_chunk = chunk10
for i in range(9, len(hexdata2)):
    hextemp = xor(hexdata1[i], hexdata2[i])
    hextemp = xor(prev_chunk, hextemp)
    prev_chunk = hextemp
    print(hextemp)

