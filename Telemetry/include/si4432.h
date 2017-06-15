#ifndef SI4432_H
#define SI4432_H

#include <spi.h>
#include <wiringPi.h>

#define IRQ_PIN 05


class si4432 : public spi
{
    public:
        si4432();
        virtual ~si4432();
        void init();
        void to_tx_mode(const char tx_buf[]);
        bool IRQ();
        void to_rx_mode(void);
        void rx_reset(void);
    protected:
    private:
};

#endif // SI4432_H
