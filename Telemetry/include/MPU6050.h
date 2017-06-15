#ifndef MPU6050_H
#define MPU6050_H
#include "i2cComm.h"
#include <stdlib.h>


#define MPU6050_ADDRESS 0x68

//register address definitions
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE 0x38
#define INT_STATUS 0x39
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define USER_CRTL 0x6A
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C



#define DEFAULT_CONFIG 0x00
#define DEFAULT_GYRO_CONFIG 0x00
#define DEFAULT_ACCEL_CONFIG 0x00
#define DEFAULT_INT_ENABLE 0x00
#define DEFAULT_USER_CRTL 0x00
#define PWR_MGMT_1_RESET 0x80
#define DEFAULT_PWR_MGMT_1 0x01
#define DEFAULT_PWR_MGMT_2 0x00


class MPU6050:public i2cComm
{
    public:
        MPU6050();
        virtual ~MPU6050();
        void resetDevice();
        void init();

        //acceleration
        bool getAccelData(int &accelX, int &accelY, int &accelZ);
        bool getAccelDataX(int &accelX);
        bool getAccelDataY(int &accelY);
        bool getAccelDataZ(int &accelZ);

        //gyro
        bool getGyroData(int &gyroX, int &gyroY, int &gyroZ);
        bool getGyroDataX(int &gyroX);
        bool getGyroDataY(int &gyroY);
        bool getGyroDataZ(int &gyroZ);
        void gyroCal();
        void getGyroOffset(int& oX, int& oY, int& oZ);
        void setGyroOffsetX(int X);
        void setGyroOffsetY(int Y);
        void setGyroOffsetZ(int Z);

        //temp
        bool getTemp(double &temp);
        bool getTemp(int &temp);
        bool getTempRAW(int &temp);



    protected:
    private:
        int gyroOffsetX;
        int gyroOffsetY;
        int gyroOffsetZ;
};

#endif // MPU6050_H
