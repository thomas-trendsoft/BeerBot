import RPi.GPIO as GPIO
import DriveMotor
import Stepper
import Distance
import GyroSensor
import EyeScanner
import time
import math

#
# simple drive around first try
#
def driveAround():
    print("eye calibration: " + str(eye.calibrate()))
    print("remove calibration component")
    time.sleep(5)

    mindist = eye.simple_scan(50)
    print("min dist: " + str(mindist))

    driver  = DriveMotor.DriveMotor()

    # small drive around
    for i in range(0,4):
        print("next lane: " + str(i))

        # drive until no more space
        while mindist > 24.0:
            driver.forward()
            dval = mindist / 100.0
            if (dval < 0.3):
                dval = 0.3
            print("forward: " + str(dval))
            time.sleep(dval)
            driver.stop()
            time.sleep(0.1)
            mindist = eye.simple_scan(20)

        driver.backward()
        time.sleep(0.4)
        driver.stop()

        while mindist < 60.0:
            driver.turn_left()
            time.sleep(0.4)
            driver.stop()
            mindist = eye.simple_scan(20)


#
# test the gyro calibration
#
def testGyro():
    x_sum = 0
    y_sum = 0
    z_sum = 0
    for i in range(0,100):
        data = gyro.read_gyro_data()
        print(str(data))
        x_sum += data['x']
        y_sum += data['y']
        z_sum += data['z']
        time.sleep(0.1)

    df = 0.00763359

    print(str(x_sum) + "/" + str(y_sum) + "/" + str(z_sum))
    print(str(x_sum*df) + "/" + str(y_sum*df) + "/" + str(z_sum*df))

# main entry
if __name__ == "__main__":
    # setup gpio basic
    #GPIO.setmode(GPIO.BCM)

    gyro = GyroSensor.MPU6050(0x68)

    print("test con: " + str(gyro.testConnection()))
    print("read temp: " + str(gyro.get_temperature()))

    time.sleep(15)
    print("start calibration")
    #gyro.gyro_calibration()
    gyro.set_x_gyro_useroffset(73)
    gyro.set_y_gyro_useroffset(0)
    gyro.set_z_gyro_useroffset(7)
    print("start test")
    testGyro()

    # clean up gpios
    #GPIO.cleanup()
