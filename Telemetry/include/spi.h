#ifndef SPI_H
#define SPI_H


class spi
{
    public:
        spi();
        virtual ~spi();
        unsigned char write(unsigned char address, unsigned char data);
        void read(unsigned char address, unsigned char *data);
        unsigned char read(unsigned char address);
        void spiRW(unsigned char *data, int len);

    protected:
    private:
        int fd;
};

#endif // SPI_H
