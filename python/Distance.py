import RPi.GPIO as GPIO
import time


class Distance:
    # init driver class
    def __init__(self):
        self.trigger = 18
        self.echo    = 17
        #Richtung der GPIO-Pins festlegen (IN / OUT)
        GPIO.setup(self.trigger, GPIO.OUT)
        GPIO.setup(self.echo, GPIO.IN)

    def measure(self):
        # setze Trigger auf HIGH
        GPIO.output(self.trigger, True)

        # setze Trigger nach 0.01ms aus LOW
        time.sleep(0.00001)
        GPIO.output(self.trigger, False)

        StartZeit = time.time()
        StopZeit = time.time()

        # speichere Startzeit
        while GPIO.input(self.echo) == 0:
            StartZeit = time.time()

        # speichere Ankunftszeit
        while GPIO.input(self.echo) == 1:
            StopZeit = time.time()

        # Zeit Differenz zwischen Start und Ankunft
        TimeElapsed = StopZeit - StartZeit
        print("time: " + str(TimeElapsed))
        
        # mit der Schallgeschwindigkeit (34300 cm/s) multiplizieren
        # und durch 2 teilen, da hin und zurueck
        distanz = (TimeElapsed * 34300) / 2

        if (distanz > 400):
            distanz = 0

        return distanz
