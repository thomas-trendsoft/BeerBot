import smbus
import math
import time

# first try to get a gyro driver
class MPU6050:

    #base register
    USER_CTRL  = 0x6a
    PWR_MGMT_1 = 0x6b
    PWR_MGMT_2 = 0x6c
    WHO_AM_I   = 0x75
    SMPRT_DIV  = 0x19 # sample rate divider (0 = 8kHz -> rate = 8kHz / (val + 1))

    # data register für accel
    ACCEL_CONFIG = 0x1C
    ACCEL_XOUT_H = 0x3B
    ACCEL_YOUT_H = 0x3D
    ACCEL_ZOUT_H = 0x3F

    # accel config values
    ACCEL_FS_2   = 0x00
    ACCEL_FS_4   = 0x08
    ACCEL_FS_8   = 0x10
    ACCEL_FS_16  = 0x18

    # temp sensor
    TEMP_OUT_H = 0x41

    # gyro registers
    GYRO_CONFIG = 0x1b
    GYRO_XOUT_H = 0x43
    GYRO_YOUT_H = 0x45
    GYRO_ZOUT_H = 0x47

    # offset gyro register
    GYRO_X_OFFSET = 0x00
    GYRO_Y_OFFSET = 0x01
    GYRO_Z_OFFSET = 0x02

    # user gyro offset values
    GYRO_X_USEROFF = 0x13
    GYRO_Y_USEROFF = 0x15
    GYRO_Z_USEROFF = 0x17

    # gyro register values
    GYRO_FS_250   = 0x00
    GYRO_FS_500   = 0x08
    GYRO_FS_1000  = 0x10
    GYRO_FS_2000  = 0x18

    # selft test register
    SELF_TEST_X = 0x0D
    SELF_TEST_Y = 0x0E
    SELF_TEST_Z = 0x0F

    # fifo registers
    FIFO_EN     = 0x23
    FIFO_COUNTH = 0x72
    FIFO_COUNTL = 0x73
    FIFO_R_W    = 0x74

    #interrupt control
    INT_PIN_CFG = 0x37
    INT_ENABLE  = 0x38
    INT_STATUS  = 0x3A


    # rotation y axis
    def get_x_rotation(self):
        data    = self.read_accel_data()
        x       = data['x'] / 16384.0
        y       = data['y'] / 16384.0
        z       = data['z'] / 16384.0
        return math.atan2(y, self.dist(x,z))

    # rotation x axis
    def get_y_rotation(self):
        data    = self.read_accel_data()
        x       = data['x'] / 16384.0
        y       = data['y'] / 16384.0
        z       = data['z'] / 16384.0
        return math.atan2(x, self.dist(y,z))

    # read current acc values
    def read_accel_data(self):
        data = {}
        data['x'] = self.read_word_2c(self.ACCEL_XOUT_H)
        data['y'] = self.read_word_2c(self.ACCEL_YOUT_H)
        data['z'] = self.read_word_2c(self.ACCEL_ZOUT_H)
        return data

    # set accel scale (see defined values)
    def set_accel_scale(self,value):
        self.bus.write_byte_data(self.address,self.ACCEL_CONFIG,value)

    # read current gyro values
    def read_gyro_data(self):
        data = {}
        data['x'] = self.read_word_2c(self.GYRO_XOUT_H)
        data['y'] = self.read_word_2c(self.GYRO_YOUT_H)
        data['z'] = self.read_word_2c(self.GYRO_ZOUT_H)
        return data

    # set gyro scale (value by definitions)
    def set_gyro_scale(self,value):
        self.bus.write_byte_data(self.address, self.GYRO_CONFIG, value)

    # get x gyro user offset
    def get_x_gyro_useroffset(self):
        return self.read_word_2c(self.GYRO_X_USEROFF)

    # get x gyro user offset
    def get_y_gyro_useroffset(self):
        return self.read_word_2c(self.GYRO_Y_USEROFF)

    # get x gyro user offset
    def get_z_gyro_useroffset(self):
        return self.read_word_2c(self.GYRO_Z_USEROFF)

    # sets user defined gyro offset value (seems to be given values / 4) not sure how the shift is handled
    def set_x_gyro_useroffset(self,val):
        self.write_word_2c(self.GYRO_X_USEROFF,val)

    def set_y_gyro_useroffset(self,val):
        self.write_word_2c(self.GYRO_Y_USEROFF,val)

    def set_z_gyro_useroffset(self,val):
        self.write_word_2c(self.GYRO_Z_USEROFF,val)

    # get x gyro offset value
    def get_x_gyro_offset(self):
        val = self.bus.read_byte_data(self.address,self.GYRO_X_OFFSET)
        return (val | 0x7E) >> 1

    # set own x gyro offset
    def set_x_gyro_offset(self,value):
        return self.bus.write_byte_data(self.address,self.GYRO_X_OFFSET,value << 1)

    # get y gyro offset value
    def get_y_gyro_offset(self):
        val = self.bus.read_byte_data(self.address,self.GYRO_Y_OFFSET)
        return (val | 0x7E) >> 1

    # set own x gyro offset
    def set_y_gyro_offset(self,value):
        return self.bus.write_byte_data(self.address,self.GYRO_Y_OFFSET,value << 1)

    # get z gyro offset value
    def get_z_gyro_offset(self):
        val = self.bus.read_byte_data(self.address,self.GYRO_Z_OFFSET)
        return (val | 0x7E) >> 1

    # set own z gyro offset
    def set_z_gyro_offset(self,value):
        return self.bus.write_byte_data(self.address,self.GYRO_Z_OFFSET,value << 1)

    # get measured temperature
    def get_temperature(self):
        val = self.read_word_2c(self.TEMP_OUT_H)
        return (val / 340) + 36.53

    # check connection
    def testConnection(self):
        return self.bus.read_byte_data(self.address,self.WHO_AM_I)

    # set sample rate
    def samplerate(self,val):
        dv = int(8000 / val) - 1
        self.bus.write_byte_data(self.address, self.SMPRT_DIV, dv)
        print("sample rate set to " + str(val) + " ... divide: " + str(dv))

    # util method
    def read_word(self,reg):
        h = self.bus.read_byte_data(self.address, reg)
        l = self.bus.read_byte_data(self.address, reg+1)
        value = (h << 8) + l
        return value

    # util to check neg values
    def read_word_2c(self,reg):
        val = self.read_word(reg)
        if (val >= 0x8000):
            return -((65535 - val) + 1)
        else:
            return val

    def write_word_2c(self,reg,val):
        aval = abs(val)
        hb = (aval >> 8 & 0xFF)
        if (val < 0):
            hb |= 0x80
        self.bus.write_byte_data(self.address,reg,hb)
        self.bus.write_byte_data(self.address,reg+1,aval & 0xFF)


    # calc util function for dist
    def dist(self,a,b):
        return math.sqrt((a*a)+(b*b))

    # check for if needed
    def gyro_calibration(self):
        # init with current set values
        x_g_off = self.get_x_gyro_offset()
        y_g_off = self.get_y_gyro_offset()
        z_g_off = self.get_z_gyro_offset()

        x_g_dir = 0
        y_g_dir = 0
        z_g_dir = 0

        x_done = False
        y_done = False
        z_done = False

        i = 0

        # update values until flip
        while (not x_done or not y_done or not z_done):
            gdata = self.read_gyro_data()

            print("active value gyro x: " + str(gdata['x']) + " / " + str(x_g_off))
            print("active value gyro y: " + str(gdata['y']) + " / " + str(y_g_off))
            print("active value gyro z: " + str(gdata['z']) + " / " + str(z_g_off))

            # update x offset and check flip
            if (gdata['x'] > 0):
                x_g_off -= 1
                if (i > 10 and x_g_dir != -1):
                    x_done = True
                x_g_dir  = -1

            if (gdata['x'] < 0):
                x_g_off += 1
                if (i > 10 and x_g_dir != 1):
                    x_done = True
                x_g_dir  = 1

            # update y offset
            if (gdata['y'] > 0):
                y_g_off -= 1
                if (i > 10 and y_g_dir != -1):
                    y_done = True
                y_g_dir  = -1

            if (gdata['y'] < 0):
                y_g_off += 1
                if (i > 10 and y_g_dir != 1):
                    y_done = True
                y_g_dir  = 1

            # update z offset
            if (gdata['z'] > 0):
                z_g_off -= 1
                if (i > 10 and z_g_dir != -1):
                    z_done = True
                z_g_dir  = -1

            if (gdata['z'] < 0):
                z_g_off += 1
                if (i > 10 and z_g_dir != 1):
                    z_done = True
                z_g_dir  = 1


            self.set_x_gyro_offset(x_g_off)
            self.set_y_gyro_offset(y_g_off)
            self.set_z_gyro_offset(z_g_off)
            i += 1
            time.sleep(0.2)

        print("gyro calibration done")



    # init gyro interface
    def __init__(self, address, bus=1):
        self.address = address
        self.bus     = smbus.SMBus(bus)

        self.set_gyro_scale(self.GYRO_FS_250)
        self.set_accel_scale(self.ACCEL_FS_2)
        self.samplerate(7)
        print("gyro scale set to 250 dps")
        print("accel set to 2G")

        time.sleep(0.1)

        # Wake up the MPU-6050 since it starts in sleep mode and
        # change clock source to adviced (see register pdf) gyro source
        self.bus.write_byte_data(self.address, self.PWR_MGMT_1, 0x01)
