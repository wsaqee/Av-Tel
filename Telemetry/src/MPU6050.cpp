#include "MPU6050.h"
#include <wiringPi.h>

MPU6050::MPU6050():i2cComm(MPU6050_ADDRESS)
{
    init();
}

MPU6050::~MPU6050()
{
    //dtor
}

void MPU6050::resetDevice()
{
    WriteByte(PWR_MGMT_1, PWR_MGMT_1_RESET);

    setGyroOffsetX(0);
    setGyroOffsetY(0);
    setGyroOffsetZ(0);

    delay(100);
}

void MPU6050::init()
{
    resetDevice();
    WriteByte(PWR_MGMT_1, DEFAULT_PWR_MGMT_1);
    /*
    additional reg init
    */

    gyroCal();

    delay(200);
}

//simple Gyro software calibration
void MPU6050::gyroCal()
{
    int X = 0, Y = 0, Z = 0;
    int sumX = 0, sumY = 0, sumZ = 0;
    int const N = 20;

    for (int i=0; i<N; i++)
    {
        getGyroData(X, Y, Z);
        sumX +=X;
        sumY +=Y;
        sumZ +=Z;
        delay(50);
    }
    setGyroOffsetX(sumX/N);
    setGyroOffsetY(sumY/N);
    setGyroOffsetZ(sumZ/N);
}


//acceleration functions

bool MPU6050::getAccelData(int &accelX, int &accelY, int &accelZ)
{
    if (!(getAccelDataX(accelX) && getAccelDataY(accelY) && getAccelDataZ(accelZ))) return false;
    return true;
}

bool MPU6050::getAccelDataX(int &accelX)
{
    if (!get16bitSignValue(ACCEL_XOUT_H, ACCEL_XOUT_L, accelX)) return false;
    return true;
}

bool MPU6050::getAccelDataY(int &accelY)
{
    if (!get16bitSignValue(ACCEL_YOUT_H, ACCEL_YOUT_L, accelY)) return false;
    return true;
}

bool MPU6050::getAccelDataZ(int &accelZ)
{
    if (!get16bitSignValue(ACCEL_ZOUT_H, ACCEL_ZOUT_L, accelZ)) return false;
    return true;
}

//..........................................................................


//gyro functions

bool MPU6050::getGyroData(int &gyroX, int &gyroY, int &gyroZ)
{
    if (!(getGyroDataX(gyroX) && getGyroDataY(gyroY) && getGyroDataZ(gyroZ))) return false;
    return true;
}


bool MPU6050::getGyroDataX(int &gyroX)
{
    if (!get16bitSignValue(GYRO_XOUT_H, GYRO_XOUT_L, gyroX)) return false;
    gyroX -= gyroOffsetX;
    return true;
}

bool MPU6050::getGyroDataY(int &gyroY)
{
    if (!get16bitSignValue(GYRO_YOUT_H, GYRO_YOUT_L, gyroY)) return false;
    gyroY -= gyroOffsetY;
    return true;
}

bool MPU6050::getGyroDataZ(int &gyroZ)
{
    if (!get16bitSignValue(GYRO_ZOUT_H, GYRO_ZOUT_L, gyroZ)) return false;
    gyroZ -= gyroOffsetZ;
    return true;
}

void MPU6050::setGyroOffsetX(int X)
{
    gyroOffsetX = X;
}

void MPU6050::setGyroOffsetY(int Y)
{
    gyroOffsetY = Y;
}

void MPU6050::setGyroOffsetZ(int Z)
{
    gyroOffsetZ = Z;
}

void MPU6050::getGyroOffset(int& oX, int& oY, int& oZ)
{
    oX = gyroOffsetX;
    oY = gyroOffsetY;
    oZ = gyroOffsetZ;
}
//...........................................................................


//temp

bool MPU6050::getTemp(double &temp)
{
    int val = 0;
    if (!getTempRAW(val)) return false;
    temp = val/340.0 + 36.53;
    return true;
}

bool MPU6050::getTemp(int &temp)
{
    double val = 0;
    if (!getTemp(val)) return false;
    temp = val;
    return true;
}

bool MPU6050::getTempRAW(int &temp)
{
    return get16bitSignValue(TEMP_OUT_H, TEMP_OUT_L, temp);
}

//...........................................................................



