import time
import sys
import serial


from MS5611 import MS5611
#https://github.com/jfosnight/jonahsystems/blob/master/python/MS5611_example_usage.py

import mh_z19
#https://github.com/UedaTakeyuki/mh-z19

import os
import glob 
#https://www.circuitbasics.com/raspberry-pi-ds18b20-temperature-sensor-tutorial/


import MySQLdb

#-----------------arguments--------------

print "-v Verbose"
verbose_mode = len(sys.argv) > 1 and sys.argv[1] == '-v'
if verbose_mode:
	print "Verbose mode ON!!!"
#-----------------arguments--------------

#-----------------arduino sensers--------------

usbPort='/dev/ttyUSB0'
ser = serial.Serial(usbPort, 9600)

#-----------------arduino sensers--------------



#-----------------MS5611-----------------
sensorMS5611 = MS5611(1, 0x77, 432.8)
sensorMS5611.setElevationFt(1420)
#-----------------MS5611-----------------

#-----------------ds18b20-----------------
 
os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')
 
base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'
 
def read_temp_raw():
    f = open(device_file, 'r')
    lines = f.readlines()
    f.close()
    return lines
 
def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        return temp_c
#-----------------ds18b20-----------------


def write_to_db(data):	
	conn = MySQLdb.connect(host= "localhost",
                  user="pi_user",
                  passwd="pi_userP@$",
                  db="measure")
                  
	x = conn.cursor()
	try:
	   q_insert = '\
		INSERT INTO data \
		(\
			mq7, \
			mq135, \
			mq3, \
			mq8, \
			ms5611_temp, \
	     	ms5611_pressue, \
	     	mhz19_co2, \
	     	mhz19_temp, \
	     	ds18b20_temp) \
		VALUES \
		(%s,%s,%s,%s,%.5f,%.5f,%.5f,%.5f,%.5f) \
		' % (data['mq7'], \
		 data['mq135'], \
		 data['mq3'], \
		 data['mq8'], \
		 data['ms5611_temp'], \
		 data['ms5611_pressue'], \
		 data['mhz19_co2'], \
		 data['mhz19_temp'], \
		 data['ds18b20_temp'])

       	   if verbose_mode:
	   	print q_insert

	   x.execute(q_insert)
	   conn.commit()
 	except MySQLdb.Error as error:
	    if verbose_mode:
	    	print(x._last_executed)
	    	print(error)
	    conn.rollback()

	finally:
		x.close()
		conn.close()
	
mh_z19.abc_on()

while True:
    sensorMS5611.read()
    tempC = sensorMS5611.getTempC()
    press = sensorMS5611.getPressureAdj()*0.75006375541921
    
    mhz19 = mh_z19.read_all()
    temp = read_temp()
    


	 #--------arduino sensors-----------
   
    bytesToRead = ser.inWaiting()
    ard_data_str = ser.read(bytesToRead)

    ard_data_ar = ard_data_str.split('\r\n')
    
    mq_values = ard_data_ar[len(ard_data_ar)-1].split(' ')
	 #--------arduino sensors-----------
    
    obj = { \
      'mq135': int(mq_values[0]), \
      'mq7': int(mq_values[1]), \
      'mq3': int(mq_values[2]), \
      'mq8': int(mq_values[3]), \
		'ms5611_temp': tempC, \
		'ms5611_pressue': press, \
		'mhz19_co2': mhz19['co2'], \
		'mhz19_temp': mhz19['temperature'], \
		'ds18b20_temp': temp }
	
    if verbose_mode:
	    print "obj: ", obj
    
    write_to_db(obj) 	
    time.sleep(60)
