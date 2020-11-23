import RPi.GPIO as GPIO
import time

# Base class for 28BYJ-48 stepper motor
# should have 2038 steps a round for this sequence
class Stepper:

    # init driver class
    def __init__(self):
        self.pins    = [22,23,24,25]
        self.seq     = [[1,1,0,0],[0,1,1,0],[0,0,1,1],[1,0,0,1]]
        self.seqsize = len(self.seq)
        self.seqidx  = 0
        GPIO.setup(22,GPIO.OUT)
        GPIO.setup(23,GPIO.OUT)
        GPIO.setup(24,GPIO.OUT)
        GPIO.setup(25,GPIO.OUT)

    # rotate s steps in d directory (1,-1)
    def rotate(self,s,d):
        for count in range(0,s):
            for pin in range(0,4):
                xpin = self.pins[pin]
                if self.seq[self.seqidx][pin] != 0:
                    GPIO.output(xpin,True)
                else:
                    GPIO.output(xpin,False)
            # inc step position
            self.seqidx += d
            # check border
            if (self.seqidx >= self.seqsize):
                self.seqidx = 0
            if (self.seqidx < 0):
                self.seqidx = self.seqsize - 1
            time.sleep(0.003)
        for pin in range(0,4):
            xpin = self.pins[pin]
            GPIO.output(xpin,False)
