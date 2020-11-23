import RPi.GPIO as GPIO
import DriveMotor
import Stepper
import Distance
import GyroSensor
import EyeScanner
import time
import math

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

if __name__ == "__main__":
    # setup gpio basic
    GPIO.setmode(GPIO.BCM)

    eye  = EyeScanner.EyeScanner()
    gyro = GyroSensor.MPU6050(0x68)

    print("test con: " + str(gyro.testConnection()))
    print("read temp: " + str(gyro.get_temperature()))

    time.sleep(12)
    print("start calibration")
    print(str(gyro.get_x_gyro_offset()))
    print(str(gyro.get_y_gyro_offset()))
    print(str(gyro.get_z_gyro_offset()))

    gyro.set_x_gyro_useroffset(325)
    gyro.set_y_gyro_useroffset(197)
    gyro.set_z_gyro_useroffset(254)

    x_sum = 0
    y_sum = 0
    z_sum = 0
    for i in range(0,100):
        data = gyro.read_gyro_data()
        print(str(data))
        x_sum += data['x']
        y_sum += data['y']
        z_sum += data['z']
        time.sleep(0.2)

    df = 0.00763359

    print(str(x_sum) + "/" + str(y_sum) + "/" + str(z_sum))
    print(str(x_sum*df) + "/" + str(y_sum*df) + "/" + str(z_sum*df))

    # clean up gpios
    GPIO.cleanup()
