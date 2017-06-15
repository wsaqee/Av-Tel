#include <iostream>
#include "i2cComm.h"


using namespace std;

int main()
{
	i2cComm i2cDev = i2cComm(0x39);
	#define BLOCKDATALENGHT 3
	 int blockData[BLOCKDATALENGHT];
	 cout << hex;
	 
	 #define WRITE_ADDRESS 0x80
	 #define TEST_DATA 0x30
	 
	 int data;
	 
	 cout<<"wireWriteDataBlock()"<<endl;
	 cout << "Data (register address: 0x83, 0x84, 0x85) ...";
	 i2cDev.ReadMemoryBlock(0x83, blockData, BLOCKDATALENGHT)?cout<<"":cout<<"ERROR"<<endl;
	 for (int i=0; i<BLOCKDATALENGHT; i++)
	 {
		cout << (int)blockData[i] << " ";
		 
	 }
	 cout<<endl;
	 cout<<"Writing 0x10,0x11, 0x12 .."<<endl;
	 int blockDataWrite[] = {0x10, 0x11, 0x12};
	 i2cDev.WriteMemoryBlock(0x83, blockDataWrite, BLOCKDATALENGHT)?cout<<"WRITE SUCCESSUFUL"<<endl:cout<<"WRITE ERROR"<<endl;
	 cout<<"READBACK from 0x83..0x85 ...";
	 i2cDev.ReadMemoryBlock(0x83, blockData, BLOCKDATALENGHT)?cout<<"":cout<<"ERROR"<<endl;
	 for (int i=0; i<BLOCKDATALENGHT; i++)
	 {
		cout << (int)blockData[i] << " ";
		 
	 }
	 cout<<endl;
	 (((int)blockData[0] + ((int)blockData[1]<<8) + ((int)blockData[2]<<16)) - (0x10 + (0x11<<8) + (0x12<<16)))==0?cout<<"DATA OK"<<endl:cout<<"WRITE ERROR"<<endl;
	 
	return 0;
}
