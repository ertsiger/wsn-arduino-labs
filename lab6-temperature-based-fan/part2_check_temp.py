import serial
from xbee import ZigBee

def getTemperatureFromData(data):
	temperature = data['samples'][0]['adc-0']
	# temperature = temperature*18.0/212.0
	temperature = 3.3 * temperature * 100.0 / 1024.0
	return temperature

def turnFanOn(xbee):
    print 'Threshold exceeded! Activating Fans'
    xbee.send('remote_at',
                  frame_id='A',
                  dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x96\x2E',
                  dest_addr='\xFF\xFE',
                  options='\x02',
                  command='D1',
                  parameter='\x05')

def turnFanOff(xbee):
    xbee.send('remote_at',
                  frame_id='A',
                  dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x96\x2E',
                  dest_addr='\xFF\xFE',
                  options='\x02',
                  command='D1',
                  parameter='\x04')	  
			
def main():
    print 'Receiving temperature values...'
    serialPort = serial.Serial('COM4', 9600)
    xbee = ZigBee(serialPort, escaped=True)
	
    while True:
		try:
			data = xbee.wait_read_frame()
			if data.keys().count('samples') != 0:
				temperature = getTemperatureFromData(data)
				print temperature
				if (temperature > 21.0):
					turnFanOn(xbee)
				else:
					turnFanOff(xbee)
			
		except KeyboardInterrupt:
			xbee.send('remote_at',
					  frame_id='A',
					  dest_addr_long='\x00\x13\xA2\x00\x40\x8B\x96\x2E',
					  dest_addr='\xFF\xFE',
					  options='\x02',
					  command='D1',
					  parameter='\x04')
			break
	
	xbee.halt()
	serialPort.close()
	
if __name__ == '__main__':
	main()
else:
	pass	
		