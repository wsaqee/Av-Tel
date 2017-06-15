#ifndef I2CCOMM_H
#define I2CCOMM_H

#include <stdint.h>

class i2cComm
{
    public:
        i2cComm(int devId);
        virtual ~i2cComm();
        int getDevId(){return DevId;}
		int setDevId(int devId);
		int getfd(){return fd;}
		bool ReadByte(uint8_t reg, uint8_t *data);
		bool ReadByte(int reg, int *data);
		bool ReadByte(int reg, uint8_t *data);
		bool ReadMemoryBlock(int reg, uint8_t *data, int len);
		bool WriteByte(uint8_t reg, uint8_t data);
		bool WriteByte(int reg, uint8_t data);
		bool WriteByte(int reg, int data);
		bool WriteMemoryBlock(int reg, uint8_t *dataIn, int len);
		bool get16bitSignValue(int regMSB, int regLSB, int16_t &data);
        bool get16bitSignValue(int regMSB, int regLSB, int &data);
		static int getFileHandle(int devId);

    protected:

    private:
        int fd;
		int DevId;
};

#endif // I2CCOMM_H
