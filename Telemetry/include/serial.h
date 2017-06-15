#ifndef SERIAL_H
#define SERIAL_H

#include <string>

class serial
{
    public:
        serial(std::string str, int baudRate);
        serial(char *devID, int baudRate);
        virtual ~serial();

        void sendByte(char byte);
        void sendString(char *data);
        int availData();
        char getChar(void);
        void getChar(char *character);
        void getString(std::string &str);
        void getString(char str[]);
        void flushBuffer();

    protected:
    private:

        int fd;     //serial file handle
};

#endif // SERIAL_H
