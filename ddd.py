import time
from MS5611 import MS5611
#https://github.com/jfosnight/jonahsystems/blob/master/python/MS5611_example_usage.py

import mh_z19
#https://github.com/UedaTakeyuki/mh-z19

import os
import glob 
#https://www.circuitbasics.com/raspberry-pi-ds18b20-temperature-sensor-tutorial/

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


while True:
    sensorMS5611.read()
    tempC = sensorMS5611.getTempC()
    press = sensorMS5611.getPressureAdj()
    print tempC, "C", press*0.75006375541921, "mmHg"
    print  mh_z19.read_all()
    print "temperature - ", read_temp()
    time.sleep(5)




