import serial
from xbee import XBee
import cv2
from networktables import NetworkTables as NT
import numpy as np
import socket
import sys
import pickle

PORT = #insert port here

serial_port = serial.Serial(PORT, 9600)
xbee = XBee(serial_port)






while True:
    try:
    	print xbee.wait_read_frame()
		bob = cv2.imread(xbee.wait_read_frame(),1)
		imgCount = 1
		while xbee.wait_read_frame() != []:
	


	k = cv2.waitKey(5)
	print("thing")    
	if k == 27 or imgCount >= 4:
	if k == 27:
		break

serial_port.close()