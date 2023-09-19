#Code by GVV Sharma
#Jan 16, 2021
#Released under GNU GPL
#Program to find the bit length for a 20 MHz clock

fre = int(20e6)

print(bin(fre),fre.bit_length())

