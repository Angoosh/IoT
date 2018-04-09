import os
import picamera
import shutil
import RPi.GPIO as GPIO
import smtplib
import pickle
import base64
from time import sleep, gmtime, strftime

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

pin = 21
src = "/home/pi/survelliance/Intruder"
dst = "/home/pi/gdrivefs/Intruder/Dir"
a = 0
camera = picamera.PiCamera()
server = smtplib.SMTP('smtp.gmail.com', 587)
psswd = base64.b64decode(pickle.load(open("psswrd.p", "rb"))).decode("utf-8")
mail = pickle.load(open("mail.p", "rb"))
mailRecieve = pickle.load(open("mailR.p", "rb"))

GPIO.setup(pin, GPIO.IN)
GPIO.setup(20, GPIO.OUT)
GPIO.output(20, 1)
sleep(5)
GPIO.output(20, 0)

while True:
	i = GPIO.input(pin)

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
			camera.annotate_text = strftime("%d-%m-%Y %H:%M:%S", gmtime())
			camera.capture('Intruder/Intruda'+x+'.png')
			print("Done: "+x)
			sleep(1)
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

		server.starttls()
		server.login(mail, psswd)
		msg = "Prisel nezvany host. Kdo? To si najdes na googlu."
		server.sendmail(mail, mailRecieve, msg)
		server.quit()
		a+=1
