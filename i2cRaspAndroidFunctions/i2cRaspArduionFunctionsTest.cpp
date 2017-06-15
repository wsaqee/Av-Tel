 /**
  * @file i2cRaspArduinoFunctionsTest.cpp
  * @brief i2c functions for sparkfun apds9960 i2c Arduino communication
  * @author Matija Nestic
  * @date April 19th, 2017.
  * 
  * 
  * 
  *
  */
 
 #include <stdint.h>
 #include <iostream>
 #include <wiringPiI2C.h>
 
 //i2c filehandle
 int fd;
 
 using namespace std;



 bool wireReadDataByte(uint8_t reg, uint8_t &val);	
 int wireReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len);
 bool wireWriteDataByte(uint8_t reg, uint8_t val);
 bool wireWriteDataBlock(uint8_t reg, uint8_t *val, unsigned int len);


 int main()
 {
	 //init i2c
	 //fd standard linux filehandle
	 fd = wiringPiI2CSetup(0x39);
	 
	 
	 
	 uint8_t data = 0;
	 #define BLOCKDATALENGHT 3
	 uint8_t blockData[BLOCKDATALENGHT];
	 cout << hex;
	 
	 //Caption
	 cout<<"I2C functions test"<<endl<<endl;
	 cout<<"All values are hex based"<<endl;
	 cout<<endl<<endl;
	 //..................
	 
	 
	 
	 
	 //testing reading functions
	 //functions: wireReadDataByte(), wireReadDataBlock()
	 
	 cout << "READ TEST"<<endl;
	 cout<<"-----------------"<<endl<<endl;
	 
	 //wireReadDataByte test code
	 //1)read device ID from register 0x92
	 //2)compare value with defined device_id
	 
	 #define DEVICE_ID 0xAB
	 #define ID_ADDRESS 0x9C
	 cout << "wireReadDataByte()"<<endl;
	 wireReadDataByte(ID_ADDRESS, data)?cout<<"Read OK" << endl : cout<<"Read Error"<<endl;
	 cout << "Read data: " <<(int)data<< endl;
	 data==DEVICE_ID ? cout<<"Data OK"<<endl : cout<<"Read Error (ID mismatch)"<<endl;
	 //................
	 return 0;
	 cout<<endl<<endl<<endl;
	 
	 //wireReadDataBlock() test code
	 //1)Read block of memory from i2c device
	 //2)Compare values with i2cdump script from shell
	 
	 cout << "wireReadDataBlock()"<<endl;
	 cout << "Read status ";
	 wireReadDataBlock(0x92, blockData, BLOCKDATALENGHT)?cout<<"Read OK"<<endl:(cout<<"Read Error"<<endl);
	 
	 cout << "Block data: ";
	 for (int i=0; i<BLOCKDATALENGHT; i++)
	 {
		cout << (int)blockData[i] << " ";
		 
	 }
	 //....................
	 
	 
	 cout << endl;
	 cout << endl;
	 cout << endl;
	 
	 
	 //testing write functions
	 //functions: wireWriteDataByte(), wireWriteDataBlock()
	 
	 cout << "WRITE TEST"<<endl;
	 cout<<"-----------------"<<endl;
	 
	 
	 
	 //wireWriteDataByte testing code:
	 
	 //1)Read data from register
	 //2)Write to register
	 //3)Read value from register and compare
	 
	 #define WRITE_ADDRESS 0x80
	 #define TEST_DATA 0x30
	 
	 
	 cout<<"wireWriteDataByte()"<<endl;
	 wireReadDataByte(WRITE_ADDRESS, data)?cout<<"":cout<<"ERROR"<<endl;
	 cout<<"Read reg 0x"<<WRITE_ADDRESS<<", register data: 0x"<<(int)data<<endl;
	 cout<<"Write 0x"<<TEST_DATA<<" to 0x"<<WRITE_ADDRESS<<"....";
	 wireWriteDataByte(WRITE_ADDRESS, TEST_DATA)?cout<<"SUCCESS"<<endl:cout<<"ERROR"<<endl;
	 cout<<"READBACK from 0x"<<WRITE_ADDRESS<<"....";
	 wireReadDataByte(WRITE_ADDRESS, data)?cout<<"":cout<<"ERROR"<<endl;
	 cout<<"0x"<<(int)data<<endl;
	 data==TEST_DATA?cout<<"DATA OK"<<endl:cout<<"WRITE ERROR"<<endl;
	 
	 //..........................
	 
	 cout<<endl;
	 cout<<endl;
	 cout<<endl;
	 
	 
	 
	 //wireWriteDataBlock testing code:
	 
	 //1)read block of memory from i2c device
	 //2)write a block a data to i2c device memory
	 //3)readback writen data from i2c device memory and compare
	 
	 
	 
	 cout<<"wireWriteDataBlock()"<<endl;
	 cout << "Data (register address: 0x83, 0x84, 0x85) ...";
	 wireReadDataBlock(0x83, blockData, BLOCKDATALENGHT)?cout<<"":cout<<"ERROR"<<endl;
	 for (int i=0; i<BLOCKDATALENGHT; i++)
	 {
		cout << (int)blockData[i] << " ";
		 
	 }
	 cout<<endl;
	 cout<<"Writing 0x10,0x11, 0x12 .."<<endl;
	 uint8_t blockDataWrite[] = {0x10, 0x11, 0x12};
	 wireWriteDataBlock(0x83, blockDataWrite, BLOCKDATALENGHT)?cout<<"WRITE SUCCESSUFUL"<<endl:cout<<"WRITE ERROR"<<endl;
	 cout<<"READBACK from 0x83..0x85 ...";
	 wireReadDataBlock(0x83, blockData, BLOCKDATALENGHT)?cout<<"":cout<<"ERROR"<<endl;
	 for (int i=0; i<BLOCKDATALENGHT; i++)
	 {
		cout << (int)blockData[i] << " ";
		 
	 }
	 cout<<endl;
	 (((int)blockData[0] + ((int)blockData[1]<<8) + ((int)blockData[2]<<16)) - (0x10 + (0x11<<8) + (0x12<<16)))==0?cout<<"DATA OK"<<endl:cout<<"WRITE ERROR"<<endl;
	 
	 //..........................
	 
	 
	 return 0;
 }
 
 



/*******************************************************************************
 * Raw I2C Reads and Writes
 ******************************************************************************/

/**
 * @brief Writes a single byte to the I2C device (no register)
 *
 * @param[in] val the 1-byte value to write to the I2C device
 * @return True if successful write operation. False otherwise.
 */
bool wireWriteByte(uint8_t val)
{
    /*Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    
    return true;*/
    
    if (wiringPiI2CWrite(fd, val)) return false;
    return true;
}

/**
 * @brief Writes a single byte to the I2C device and specified register
 *
 * @param[in] reg the register in the I2C device to write to
 * @param[in] val the 1-byte value to write to the I2C device
 * @return True if successful write operation. False otherwise.
 */
bool wireWriteDataByte(uint8_t reg, uint8_t val)
{
    /*Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }

    return true;*/
    
    if (wiringPiI2CWriteReg8(fd, reg, val)) return false;
    return true;
    
}

/**
 * @brief Writes a block (array) of bytes to the I2C device and register
 *
 * @param[in] reg the register in the I2C device to write to
 * @param[in] val pointer to the beginning of the data byte array
 * @param[in] len the length (in bytes) of the data to write
 * @return True if successful write operation. False otherwise.
 */
bool wireWriteDataBlock(  uint8_t reg, 
                                        uint8_t *val, 
                                        unsigned int len)
{
    /*unsigned int i;

    Wire.beginTransmission(APDS9960_I2C_ADDR);
    Wire.write(reg);
    for(i = 0; i < len; i++) {
        Wire.beginTransmission(val[i]);
    }
    if( Wire.endTransmission() != 0 ) {
        return false;
    }

    return true;*/
    
    
    for (uint32_t i=0; i<len; i++)
    {
		if (wiringPiI2CWriteReg8(fd, reg + i, val[i])) return false;
	}
    return true;
}

/**
 * @brief Reads a single byte from the I2C device and specified register
 *
 * @param[in] reg the register to read from
 * @param[out] the value returned from the register
 * @return True if successful read operation. False otherwise.
 */
bool wireReadDataByte(uint8_t reg, uint8_t &val)
{
    
    /* Indicate which register we want to read from */
    /*if (!wireWriteByte(reg)) {
        return false;
    }
    */
    /* Read from register */
    /*Wire.requestFrom(APDS9960_I2C_ADDR, 1);
    while (Wire.available()) {
        val = Wire.read();
    }

    return true;
    * */
    
    int data;
    if ((data = wiringPiI2CReadReg8 (fd, reg)) == -1) return false;
    val = data;
    return true;
}

/**
 * @brief Reads a block (array) of bytes from the I2C device and register
 *
 * @param[in] reg the register to read from
 * @param[out] val pointer to the beginning of the data
 * @param[in] len number of bytes to read
 * @return Number of bytes read. -1 on read error.
 */
int wireReadDataBlock(   uint8_t reg, 
                                        uint8_t *val, 
                                        unsigned int len)
{
    /*unsigned char i = 0;
    */
    /* Indicate which register we want to read from */
    /*if (!wireWriteByte(reg)) {
        return -1;
    }
    */
    /* Read block data */
    /*Wire.requestFrom(APDS9960_I2C_ADDR, len);
    while (Wire.available()) {
        if (i >= len) {
            return -1;
        }
        val[i] = Wire.read();
        i++;
    }

    return i;
    */
    
    unsigned char i = 0;
    
    if (wiringPiI2CWrite(fd, reg)) return false;
    for(i = 0; i < len; i++) {
        val[i] = wiringPiI2CRead(fd);
    }
    return true;
    
}
