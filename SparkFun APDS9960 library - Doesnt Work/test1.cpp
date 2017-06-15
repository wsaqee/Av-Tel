#include <iostream>
#include <cstdlib>
#include <string>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdint.h>
#include "SparkFun_APDS9960.h"

using namespace std;



int main()
{
	cout<<hex;	//set otput to hex based system
	
	/*
	int fd, dataProx, dataUp, dataDown, dataLeft, dataRight;
wiringPiSetupSys();
if (!(fd = wiringPiI2CSetup(slave_address))) 
	{
	* 
		cout << "wiringPiI2CSetup failed. Root?\n";
		return -1;
	}
cout<<"Init result"<<fd<<endl;

while(1)
{
	system("clear");
	dataProx = wiringPiI2CReadReg8(fd, 0x9c);
	dataUp = wiringPiI2CReadReg8(fd, 0xFC);
	dataDown = wiringPiI2CReadReg8(fd, 0xFD);
	dataLeft = wiringPiI2CReadReg8(fd, 0xFE);
	dataRight = wiringPiI2CReadReg8(fd, 0xFF);
	cout<<"Proximity		" << hex<< dataProx <<endl;
	cout<<"Gesture FIFO UP		"<<dataUp<<endl;
	cout<<"Gesture FIFO DOWN	"<<dataDown<<endl;
	cout<<"Gesture FIFO LEFT 	"<<dataLeft<<endl;
	cout<<"Gesture FIFO RIGHT 	"<<dataRight<<endl;
	delay(500);	
}



*/
   
   int fd = wiringPiI2CSetup(0x39);
   cout << wiringPiI2CReadReg8(fd, 0x9C)<<endl;
   
   uint8_t prox = 2;
  
 SparkFun_APDS9960 apds = SparkFun_APDS9960();

 apds.init();//?cout << "Init Complete"<<endl:cout << "Init Error"<<endl;
 
 
 
 /*if ( apds.enableGestureSensor(true) ) {
    cout << "Gesture sensor is now running"<<endl;
  } else {
    cout << "Something went wrong during gesture sensor init!"<<endl;
  }*/
  
  apds.enableProximitySensor();
  //apds.enableLightSensor(false);
  
  
  //uint16_t amb = 40;
  apds.readProximity(prox)?cout<<"true"<<endl:cout<<"false"<<endl;
  cout << "Proximity data: 0x"<< (uint32_t)(prox) <<endl;
  //cout << (int)apds.getMode()<<endl;
  /*
  apds.readAmbientLight(amb);
  cout <<"Ambient Data 0x" <<(int)amb<<endl;*/
}

