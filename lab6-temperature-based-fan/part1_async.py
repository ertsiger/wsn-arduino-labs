import serial
import time
from xbee import ZigBee

print 'Asynchronously printing data from remote XBee'

serial_port = serial.Serial('/dev/ttyUSB0', 9600)

def print_data(data):
    """
    This method is called whenever data is received.
    Its only argument is the data within the frame.
    """
    print data['samples']

zigbee = ZigBee(serial_port, escaped=True, callback = print_data)

while True:
    try:
        time.sleep(0.001)
    except KeyboardInterrupt:
        break

zigbee.halt();
serial_port.close()