import os
import picamera
import shutil
import RPi.GPIO as GPIO
from time import sleep as delay

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

pin = 21
src = "/home/pi/Intruder"
dst = "/home/pi/gdrivefs/Intruder/Dir"
a = 0
camera = picamera.PiCamera()

GPIO.setup(pin, GPIO.IN)
GPIO.setup(20, GPIO.OUT)
GPIO.output(20, 1)
delay(5)
GPIO.output(20, 0)

while True:
	i = GPIO.input(pin)
	#i = input("INPUT: ")

	if i:
		if not os.path.exists(src):
			print("Source doesnt exist")
			os.makedirs(src)
		else:
			print("Contains source")
			shutil.rmtree(src)
			os.makedirs(src)
		camera.vflip = True
		for i in range(0, 5):
			x = str(i)
			camera.capture('Intruder/Intruda'+x+'.png')
			print("Done: "+x)
			delay(1)
		print("Captured")
		if not os.path.exists(dst):
			os.makedirs(dst)
			ndst = dst
		else:
			c = str(a)
			ndst = dst + c
		print("Sending")
		shutil.move(src, ndst)
		print("Done!")
		a+=1

