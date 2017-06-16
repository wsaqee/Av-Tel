#include "../include/HMC5883.h"
#include <wiringPi.h>

HMC5883::HMC5883():i2cComm(HMC5883_ADDRESS)
{
    //ctor
    init();
}

HMC5883::~HMC5883()
{
    //dtor
}

void HMC5883::init()
{
    WriteByte(CONFIG_A, DEFAULT_CONFIG_A);
    WriteByte(CONFIG_B, DEFAULT_CONFIG_B);
    WriteByte(MODE_REG, DEFAULT_MODE_REG);
    delay(200);
}

bool HMC5883::update ()
{
    int X=0, Y=0, Z=0;
    return getMagData(X, Y, Z);
}

bool HMC5883::getMagData(int &magX, int &magY, int &magZ)
{
    if (!(getMagDataX(magX) && getMagDataY(magY) && getMagDataZ(magZ))) return false;
    return true;
}

bool HMC5883::getMagDataX(int &magX)
{
    if (!get16bitSignValue(DATA_XOUT_H, DATA_XOUT_L, magX)) return false;
    magF[X_AXIS] = magX;
    return true;
}

bool HMC5883::getMagDataY(int &magY)
{
    if (!get16bitSignValue(DATA_YOUT_H, DATA_YOUT_L, magY)) return false;
    magF[Y_AXIS] = magY;
    return true;
}

bool HMC5883::getMagDataZ(int &magZ)
{
    if (!get16bitSignValue(DATA_ZOUT_H, DATA_ZOUT_L, magZ)) return false;
    magF[Z_AXIS] = magZ;
    return true;
}

double HMC5883::getHeading(double pitch, double roll, bool getNewData)
{
    if (getNewData) update();                       //get new data
    tiltCompensation(pitch, roll);  //tiltCompensation

    //calc heading from compass mag field

        double headingComp;
        if (std::abs(magFx)>std::abs(magFy))
        {
            headingComp = asin((double)std::abs(magFx)/(sqrt(magFx*magFx + magFy*magFy)));
        }else
        {
            headingComp = acos((double)std::abs(magFy)/(sqrt(magFx*magFx + magFy*magFy)));
        }

        if (magFy>0)
        {
            if (magFx<0)
            {
                       //if in first quadrant - no change;
            }else
            {
                headingComp = 2*PI - headingComp;   //second quadrant
            }
        }else
        {
            if (magFx>0)
            {
                headingComp += PI;                      //third quadrant
            }else
            {
                headingComp = PI - headingComp;         //forth quadrant
            }
        }

        return headingComp;
}

void HMC5883::tiltCompensation (double pitch, double roll)
{
        //heading compensation
        magFx = magF[X_AXIS]*cos(pitch)+magF[Y_AXIS]*sin(roll)*sin(pitch)-magF[Z_AXIS]*cos(roll)*sin(pitch);
        magFy = magF[Y_AXIS]*cos(roll) + magF[Z_AXIS]*sin(roll);
}

