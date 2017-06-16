#include "../include/serial.h"
#include <wiringSerial.h>
#include <cstring>

serial::serial(std::string str, int baudRate)
{
    serial(str.c_str(), baudRate );
}

serial::serial(char *devID, int baudRate)
{
    //ctor
    fd = serialOpen(devID, baudRate);

}

serial::~serial()
{
    //dtor
    serialClose (fd) ;
}


void serial::sendByte(char byte)
{
    serialPutchar(fd, byte);
}

//functions sendStrign is null terminated
void serial::sendString(char *data)
{
    serialPuts(fd, data);
}



int serial::availData()
{
    return serialDataAvail(fd);
}

char serial::getChar(void)
{
    return serialGetchar(fd) ;
}

void serial::getChar(char *character)
{
    *character = serialGetchar(fd) ;
}

void serial::getString(std::string &str)
{
    str = "";
    for (int i=0; i<availData(); i++)
    {
        str += getChar();
    }
}

void serial::getString(char str[])
{
    /*for (int i=0; i<availData(); i++)
    {
        getChar(str + i);
    }*/

    std::string s = "";
    getString(s);
    strcpy(str,s.c_str());
}


void serial::flushBuffer()
{
    serialFlush(fd);
}



