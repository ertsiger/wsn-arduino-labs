import serial
from xbee import ZigBee

print 'Printing data from remote XBee'

serial_port = serial.Serial('COM4', 9600)
zigbee = ZigBee(serial_port, escape=True)

while True:
    try:
        print zigbee.wait_read_frame()
    except KeyboardInterrupt:
        break

zigbee.halt()
serial_port.close()