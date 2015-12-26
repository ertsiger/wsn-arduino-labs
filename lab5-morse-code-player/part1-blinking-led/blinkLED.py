#! /usr/bin/python

from xbee import ZigBee
import serial
import time

ser = serial.Serial('COM5', 9600)
xbee = ZigBee(ser, escaped=True)

while True:
    try:
        xbee.send('remote_at',
                  frame_id='A',
                  dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x45\xC8',
                  dest_addr='\xFF\xFE',
                  options='\x02',
                  command='D1',
                  parameter='\x05')
        print("Open port, LED ON on other side")  
        time.sleep(5)
        
        xbee.send('remote_at',
                  frame_id='A',
                  dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x45\xC8',
                  dest_addr='\xFF\xFE',
                  options='\x02',
                  command='D1',
                  parameter='\x04')
        print("Closed port, LED OFF on the other side")
          
        time.sleep(5)
 
	 
    except KeyboardInterrupt:
        break

xbee.send('remote_at',
          frame_id='A',
          dest_addr_long='\x00\x00\x00\x00\x00\x00\xFF\xFF',
          dest_addr='\xFF\xFE',
          options='\x02',
          command='D1',
          parameter='\x04')

ser.close()
