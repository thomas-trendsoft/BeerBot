import Distance
import Stepper

# front eye controller
class EyeScanner:

    # util method to get stepper motor to front direction
    def calibrate(self):
        i     = 0 # security to avoid complete rotation
        check = self.dist.measure()
        print(str(check))
        while (i < 200 and check > 8.0 and check < 1000.0):
            self.stepper.rotate(5,-1)
            check = self.dist.measure()
        self.stepper.rotate(self.caldist,1)
        self.pos = 0
        return i < 200

    # first method to scan minimum dist on given step area (scan from (0 - mstep) to (msteps)) 0 is from calibration
    # one step is 5 steps on stepper motor
    def simple_scan(self,msteps):
        minval = 2000.0
        self.stepper.rotate(msteps*10,1)
        for i in range(0-msteps,msteps):
            val = self.dist.measure()
            if (val < minval):
                minval = val
                print(str(minval))
            self.stepper.rotate(10,-1)
        self.stepper.rotate(msteps*10,1)
        return minval


    def __init__(self,calibratevalue=405):
        self.caldist = calibratevalue
        self.pos     = 0
        self.stepper = Stepper.Stepper()
        self.dist    = Distance.Distance()
