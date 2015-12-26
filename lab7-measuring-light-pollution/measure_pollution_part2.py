# Derived from code by Alejandro Andreu
import commands
import json
import serial
import time
import os
from serial import SerialException
from xbee import ZigBee

print 'Asynchronously printing data from remote XBee'

serial_port = serial.Serial('COM5', 9600)

def print_data(data):
    """
    This method is called whenever data is received.
    Its only argument is the data within the frame.
    """
    print data['samples'][0].keys()[0]
    print data['samples'][0].values()[0]
    print data['samples'][0].keys()[1]
    print data['samples'][0].values()[1]
    
    # Create a JSON file and fill it with the received samples
    json_data = {}
    json_data['version'] = '0.2'
    json_data['datastreams'] = ()
    json_data['datastreams'] = json_data['datastreams'] + ({'id': data['samples'][0].keys()[0], 'current_value': str(data['samples'][0].values()[0])},)
 
    with open('Xbee2Xively.json', mode='w') as f:
        json.dump(json_data, f, indent = 4)
  
 # Add data from adc-1 pin
    json_data1 = {}
    json_data1['version'] = '0.2'
    json_data1['datastreams'] = ()
    json_data1['datastreams'] = json_data['datastreams'] + ({'id': data['samples'][0].keys()[1], 'current_value': str(data['samples'][0].values()[1])},)
 
    with open('Xbee2Xively.json', mode='w') as f:
        json.dump(json_data1, f, indent = 4)
   
 
zigbee = ZigBee(serial_port, callback = print_data)
while True:
    try:
        time.sleep(500)
    except KeyboardInterrupt:
        break

zigbee.halt();
serial_port.close()
