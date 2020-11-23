import RPi.GPIO as GPIO

# first try for a driver control class
class DriveMotor:
    # stop motor output
    def stop(self):
        # init gpio pins (stop mode)
        GPIO.output(self.in1,GPIO.LOW)
        GPIO.output(self.in2,GPIO.LOW)
        GPIO.output(self.in3,GPIO.LOW)
        GPIO.output(self.in4,GPIO.LOW)

    def forward(self):
        # motor 1
        GPIO.output(self.in1,GPIO.HIGH)
        GPIO.output(self.in2,GPIO.LOW)
        # motor 2
        GPIO.output(self.in3,GPIO.HIGH)
        GPIO.output(self.in4,GPIO.LOW)

    def backward(self):
        # motor 1
        GPIO.output(self.in1,GPIO.LOW)
        GPIO.output(self.in2,GPIO.HIGH)
        # motor 2
        GPIO.output(self.in3,GPIO.LOW)
        GPIO.output(self.in4,GPIO.HIGH)

    def turn_right(self):
        # motor 1
        GPIO.output(self.in1,GPIO.HIGH)
        GPIO.output(self.in2,GPIO.LOW)
        # motor 2
        GPIO.output(self.in3,GPIO.LOW)
        GPIO.output(self.in4,GPIO.HIGH)

    def turn_left(self):
        # motor 1
        GPIO.output(self.in1,GPIO.LOW)
        GPIO.output(self.in2,GPIO.HIGH)
        # motor 2
        GPIO.output(self.in3,GPIO.HIGH)
        GPIO.output(self.in4,GPIO.LOW)


    # set new motor powers to correct for drive angle
    def powerUpdate(self,power1,power2):
        self.p1.start(power1)
        self.p2.start(power2)

    # init driver class
    def __init__(self):
        print("open driver motor")
        self.in1 = 1
        self.in2 = 7
        self.in3 = 11
        self.in4 = 10
        self.en2 = 9
        self.en1 = 0

        # setup motor GPIO pins
        GPIO.setup(self.in1,GPIO.OUT)
        GPIO.setup(self.in2,GPIO.OUT)
        GPIO.setup(self.en1,GPIO.OUT)
        GPIO.setup(self.in3,GPIO.OUT)
        GPIO.setup(self.in4,GPIO.OUT)
        GPIO.setup(self.en2,GPIO.OUT)

        # init gpio pins (stop mode)
        GPIO.output(self.in1,GPIO.LOW)
        GPIO.output(self.in2,GPIO.LOW)
        GPIO.output(self.in3,GPIO.LOW)
        GPIO.output(self.in4,GPIO.LOW)

        # init pwm interface for motor speed
        self.p1=GPIO.PWM(self.en1,1000)
        self.p2=GPIO.PWM(self.en2,1000)

        # set initial speed
        self.p1.start(65)
        self.p2.start(85)
