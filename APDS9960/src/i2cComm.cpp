#include <wiringPiI2C.h>
#include "i2cComm.h"

i2cComm::i2cComm(int devId)
{
    //ctor
    this -> DevId = devId;
	this -> fd = getFileHandle(DevId);
}

i2cComm::~i2cComm()
{
    //dtor
}

bool i2cComm::ReadByte(uint8_t reg, uint8_t *data)
{
	int val = wiringPiI2CReadReg8(fd, reg);
	if (val == -1) return false;

	*data = val;
	return true;
}

bool i2cComm::ReadByte(int reg, int *data)
{
    uint8_t val;
    bool isOk = ReadByte((uint8_t) reg, &val);
    *data = val;
    return isOk;

}

bool i2cComm::ReadByte(int reg, uint8_t *data)
{
    return ReadByte((uint8_t)reg, data);
}

bool i2cComm::ReadMemoryBlock(int reg, uint8_t *data, int len)
{
	if (wiringPiI2CWrite(fd, reg)) return false;
    for(int i = 0; i < len; i++) {
        data[i] = wiringPiI2CRead(fd);
    }
    return true;
}

bool i2cComm::WriteByte(uint8_t reg, uint8_t data)
{
	if (wiringPiI2CWriteReg8(fd, reg, data) == -1) return false;
    return true;
}

bool i2cComm::WriteByte(int reg, int data)
{
    return WriteByte((uint8_t)reg, (uint8_t) data);
}

bool i2cComm::WriteByte(int reg, uint8_t data)
{
    return WriteByte((uint8_t)reg, data);
}


bool i2cComm::WriteMemoryBlock(int reg, uint8_t *dataIn, int len)
{
	for (int i=0; i<len; i++)
    {
		if (wiringPiI2CWriteReg8(fd, reg + i, dataIn[i])) return false;
	}
    return true;
}

int i2cComm::setDevId(int devId)
{
	this -> DevId = devId;
	this -> fd = getFileHandle(DevId);
	return fd;
}

int i2cComm::getFileHandle(int devId)
{
	return wiringPiI2CSetup(devId);
}

