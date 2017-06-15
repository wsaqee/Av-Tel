/*
 * i2cComms.cpp
 * 
 * Copyright 2017  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
//20.04.2017
//Matija Nestic
//i2c library

#include <wiringPiI2C.h>
#include "i2cComm.h"



i2cComm::i2cComm(int devId)
{
	this -> DevId = devId;
	this -> fd = getFileHandle(DevId);
	
}

i2cComm::~i2cComm(){}

bool i2cComm::ReadByte(int reg, int *data)
{
	int val = wiringPiI2CReadReg8(fd, reg);
	if (val == -1) return false;
	
	*data = val;
	return true;
}

bool i2cComm::ReadMemoryBlock(int reg, int *data, int len)
{
	if (wiringPiI2CWrite(fd, reg)) return false;
    for(int i = 0; i < len; i++) {
        data[i] = wiringPiI2CRead(fd);
    }
    return true;
}

bool i2cComm::WriteByte(int reg, int data)
{
	if (wiringPiI2CWriteReg8(fd, reg, data) == -1) return false;
    return true;
}

bool i2cComm::WriteMemoryBlock(int reg, int *dataIn, int len)
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





