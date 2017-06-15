//20.04.2017
//Matija Nestic
//i2c library

class i2cComm 
{
	
	public:
		i2cComm(int devId);
		~i2cComm();
		
		
		int getDevId(){return DevId;}
		int setDevId(int devId);
		int getfd(){return fd;}
		bool ReadByte(int reg, int *data);
		bool ReadMemoryBlock(int reg, int *data, int len);
		bool WriteByte(int reg, int data);
		bool WriteMemoryBlock(int reg, int *dataIn, int len);
		static int getFileHandle(int devId);
		
	private:
		int fd;
		int DevId;
		
		
	
};
