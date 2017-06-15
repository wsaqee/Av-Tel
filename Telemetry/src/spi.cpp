#include "spi.h"
#include <wiringPiSPI.h>
#define DEF_CH 0

spi::spi()
{
    //ctor
    fd = wiringPiSPISetup(0, 1e6);

}

spi::~spi()
{
    //dtor
}

void spi::spiRW(unsigned char *data, int len)
{
    wiringPiSPIDataRW(DEF_CH, data, len);
}


unsigned char spi::write(unsigned char address, unsigned char data)
{
    address |= 1<<7;
    unsigned char buffer[2] = {address, data};
    wiringPiSPIDataRW (DEF_CH, buffer, 2);
    return buffer [1];
}


void spi::read(unsigned char address, unsigned char *data)
{
    address &= ~(1<<7);
    unsigned char buffer[2] = {address, 0};
    wiringPiSPIDataRW (DEF_CH, buffer, 2);
    *data = buffer[1];
}

unsigned char spi::read(unsigned char address)
{
    unsigned char data = 0;
    read(address, &data);
    return data;
}


