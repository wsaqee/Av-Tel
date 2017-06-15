#ifndef HMC5883_H
#define HMC5883_H

#include <i2cComm.h>
#include <cmath>

#define PI 3.14159265359
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2


#define HMC5883_ADDRESS 0x1E

//register address definitons
#define CONFIG_A 0x00
#define CONFIG_B 0x01
#define MODE_REG 0x02
#define DATA_XOUT_H 0x03
#define DATA_XOUT_L 0x04
#define DATA_ZOUT_H 0x05
#define DATA_ZOUT_L 0x06
#define DATA_YOUT_H 0x07
#define DATA_YOUT_L 0x08
#define STATUS_REG 0x09
#define ID_REG_A 0x0A
#define ID_REG_B 0x0B
#define ID_REG_C 0x0C

//default values
#define DEFAULT_CONFIG_A 0x10
#define DEFAULT_CONFIG_B 0x00
#define DEFAULT_MODE_REG 0x00


class HMC5883 : public i2cComm
{
    public:
        HMC5883();
        virtual ~HMC5883();
        void init();

        bool update ();
        bool getMagData(int &magX, int &magY, int &magZ);
        bool getMagDataX(int &magX);
        bool getMagDataY(int &magY);
        bool getMagDataZ(int &magZ);

        double getHeading(double pitch = 0, double roll = 0, bool getNewData = true);
    protected:
    private:
        void tiltCompensation (double pitch, double roll);

        int magFx, magFy;           //mag field vectors
        int magF[3];                //mag field 3 axis
};

#endif // HMC5883_H
