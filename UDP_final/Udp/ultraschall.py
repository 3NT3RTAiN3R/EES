#Bibliotheken einbinden
import RPi.GPIO as GPIO
import time
from threading import Timer
 
#GPIO Modus (BOARD / BCM)
GPIO.setmode(GPIO.BCM)
 
#GPIO Pins zuweisen
GPIO_TRIGGERA = 18
GPIO_ECHOA = 24
GPIO_TRIGGERB =9
GPIO_ECHOB =10
GPIO_TRIGGER1 = 21
GPIO_ECHO1 = 20
GPIO_TRIGGER2 = 19
GPIO_ECHO2 = 26
 
#Richtung der GPIO-Pins festlegen (IN / OUT)
GPIO.setup(GPIO_TRIGGERA, GPIO.OUT)
GPIO.setup(GPIO_ECHOA, GPIO.IN)
GPIO.setup(GPIO_TRIGGERB, GPIO.OUT)
GPIO.setup(GPIO_ECHOB,GPIO.IN)
GPIO.setup(GPIO_TRIGGER1, GPIO.OUT)
GPIO.setup(GPIO_ECHO1, GPIO.IN)
GPIO.setup(GPIO_TRIGGER2,GPIO.OUT)
GPIO.setup(GPIO_ECHO2,GPIO.IN)
 
def distanz(Trigger, Echo):
    # setze Trigger auf HIGH
    GPIO.output(Trigger, True)
 
    # setze Trigger nach 0.01ms aus LOW
    time.sleep(0.00001)
    GPIO.output(Trigger, False)
 
    StartZeit = time.time()
    StopZeit = time.time()
 
    # speichere Startzeit
    while GPIO.input(Echo) == 0:
        StartZeit = time.time()
 
    # speichere Ankunftszeit
    while GPIO.input(Echo) == 1:
        StopZeit = time.time()
 
    # Zeit Differenz zwischen Start und Ankunft
    TimeElapsed = StopZeit - StartZeit
    # mit der Schallgeschwindigkeit (34300 cm/s) multiplizieren
    # und durch 2 teilen, da hin und zurueck
    distanz = (TimeElapsed * 34300) / 2
 
    return distanz

def printAbstand(Abstand = 0, ID ='ID'):
    print ("Sensor %c = %.1f cm"  % (ID, Abstand))
    i = 0
    return 

i = 0
abstandA = 0
abstandB = 0
abstand1 = 0
abstand2 = 0
abstandAfirst = 0
abstandBfirst = 0
abstand1first = 0
abstand2first = 0
first = 1
if __name__ == '__main__':
    try:
        while True:

            abstandA = distanz(Trigger=GPIO_TRIGGERA, Echo=GPIO_ECHOA)
            time.sleep(1)
            abstand1 = distanz(Trigger=GPIO_TRIGGER1, Echo=GPIO_ECHO1)
            time.sleep(1)
            abstandB = distanz(Trigger=GPIO_TRIGGERB, Echo=GPIO_ECHOB)
            time.sleep(1)
            abstand2 = distanz(Trigger=GPIO_TRIGGER2, Echo=GPIO_ECHO2)
            
	    abstandA = round(abstandA,3)
            abstandB = round(abstandB,3)
            abstand1 = round(abstand1,3)
	    abstand2 = round(abstand2,3)
            if first == 1:
		abstandAfirst = abstandA
		abstandBfirst = abstandB
		abstand1first = abstand1
		abstand2first = abstand2
		first = 0 
            if abstandAfirst < abstandA:
		abstandAfirst = abstandA
	    if abstandBfirst < abstandB:
		abstandBfirst = abstandB
	    if abstand1first <  abstand1:
		abstand1first = abstand1
	    if abstand2first < abstand2:
		abstand2first = abstand2

            printAbstand(Abstand=abstandA, ID='A')
            printAbstand(Abstand=abstandB, ID='B')
            printAbstand(Abstand=abstand1, ID='1')
	    printAbstand(Abstand=abstand2, ID='2')
            time.sleep(1)
	    text_file = open("uSensor.txt", "w")
            text_file.write("%s;%s;%s;\n" % ('A',abstandA,abstandAfirst))
            text_file.write("%s;%s;%s;\n" % ('B',abstandB,abstandBfirst))
            text_file.write("%s;%s;%s;\n" % ('1',abstand1,abstand1first))
            text_file.write("%s;%s;%s;\n" % ('2',abstand2,abstand2first))
       	    text_file.close()
        # Beim Abbruch durch STRG+C resetten
    except KeyboardInterrupt:
        print("Messung vom User gestoppt")
        GPIO.cleanup()


