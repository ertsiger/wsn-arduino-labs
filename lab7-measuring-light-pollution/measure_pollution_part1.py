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

    # Create a JSON file and fill it with the received samples
    json_data = {}
    json_data['version'] = '0.2'
    json_data['datastreams'] = ()
    json_data['datastreams'] = json_data['datastreams'] + ({'id': data['samples'][0].keys()[0], 'current_value': str(data['samples'][0].values()[0])},)
    # Add more datastreams if needed
    with open('Xbee2Xively.json', mode='w') as f:
        json.dump(json_data, f, indent = 4)
    # Upload information to Xively. Use your own Api Key and feed identifier
    os.popen('curl --request PUT --data-binary @Xbee2Xively.json --header "X-ApiKey:g14aNzsNGLD0UOvi4wFXpTjzqfliK0JcfgpU0GnubyGUBzKi" --verbose https://api.xively.com/v2/feeds/779242912')

zigbee = ZigBee(serial_port, escaped = True, callback = print_data)

time.sleep(200)

zigbee.halt();
serial_port.close()
