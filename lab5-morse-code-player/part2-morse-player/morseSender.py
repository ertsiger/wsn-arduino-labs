#! /usr/bin/python

from xbee import ZigBee
from array import *
import serial
import time
from datetime import datetime

ser = serial.Serial('COM4', 9600)
xbee = ZigBee(ser, escaped=True)

while True:
	try:
		input = raw_input('Enter your letter:')
		for c in input:
			tstart = datetime.now()
			xbee.send('tx',
			frame_id='A',
			dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x45\xC8',
			dest_addr='\xFF\xFE',
			data=c)
			response = xbee.wait_read_frame()
			tend = datetime.now()
			print c
			print tend-tstart
			time.sleep(2)
	except KeyboardInterrupt:
		break
		
ser.close()