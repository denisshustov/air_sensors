import time
from MS5611 import MS5611
#https://github.com/jfosnight/jonahsystems/blob/master/python/MS5611_example_usage.py

import mh_z19
#https://github.com/UedaTakeyuki/mh-z19

import os
import glob 
#https://www.circuitbasics.com/raspberry-pi-ds18b20-temperature-sensor-tutorial/


import MySQLdb



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

conn = MySQLdb.connect(host= "localhost",
                  user="pi_user",
                  passwd="pi_userP@$",
                  db="measure")

def write_to_db(data):
	x = conn.cursor()
	try:
	   q_insert = '\
		INSERT INTO data \
		(ms5611_temp, \
	     	ms5611_pressue, \
	     	mhz19_co2, \
	     	mhz19_temp, \
	     	ds18b20_temp) \
		VALUES \
		(%.5f,%.5f,%.5f,%.5f,%.5f) \
		' % (data['ms5611_temp'], data['ms5611_pressue'], data['mhz19_co2'], data['mhz19_temp'], data['ds18b20_temp'])
	   print q_insert
	   x.execute(q_insert)
	   conn.commit()
 	except MySQLdb.Error as error:
	    print "ERROR!!!!:"
	    print(x._last_executed)
	    print(error)
	    conn.rollback()

	finally:
            x.close()
            conn.close()
	

while True:
    sensorMS5611.read()
    tempC = sensorMS5611.getTempC()
    press = sensorMS5611.getPressureAdj()*0.75006375541921
    #print tempC, "C", press, "mmHg"

    mhz19 = mh_z19.read_all()
    #print mhz19['co2']
    #print mhz19['temperature']
    #print "temperature - ", read_temp()
    temp = read_temp()
    write_to_db({ \
	'ms5611_temp': tempC, \
	'ms5611_pressue': press, \
	'mhz19_co2': mhz19['co2'], \
	'mhz19_temp': mhz19['temperature'], \
	'ds18b20_temp': temp })

    time.sleep(15)






