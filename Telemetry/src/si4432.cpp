#include "si4432.h"

#include <iostream>

si4432::si4432():spi()
{
    //ctor
    pinMode(IRQ_PIN, INPUT);
    init();
}

si4432::~si4432()
{
    //dtor
}

void si4432::init()
{
    write(0x07, 0x80);  //reset
    delay(50);
    write(0x07, 0x00);

    write(0x06, 0x00);	// Disable all interrupts
	write(0x07, 0x01);		// Set READY mode
	write(0x09, 0x7F);		// Cap = 12.5pF
	write(0x0A, 0x05);		// Clk output is 2MHz

	/*write(0x0B, 0xF4);		// GPIO0 is for RX data output
	write(0x0C, 0xEF);		// GPIO1 is TX/RX data CLK output
	write(0x0D, 0x00);		// GPIO2 for MCLK output
	write(0x0E, 0x00);		// GPIO port use default value*/

	write(0x0F, 0x00);		// NO ADC used
	write(0x10, 0x00);		// no ADC used
	write(0x12, 0x00);		// No temp sensor used
	write(0x13, 0x00);		// no temp sensor used

	write(0x70, 0x20);		// No manchester code, no data whiting, data rate < 30Kbps

	write(0x1C, 0x1D);		// IF filter bandwidth
	write(0x1D, 0x40);		// AFC Loop
	//write(0x1E, 0x0A);	// AFC timing

	write(0x20, 0xA1);		// clock recovery
	write(0x21, 0x20);		// clock recovery
	write(0x22, 0x4E);		// clock recovery
	write(0x23, 0xA5);		// clock recovery
	write(0x24, 0x00);		// clock recovery timing
	write(0x25, 0x0A);		// clock recovery timing

	//write(0x2A, 0x18);
	write(0x2C, 0x00);
	write(0x2D, 0x00);
	write(0x2E, 0x00);

	write(0x6E, 0x27);		// TX data rate 1   0x27
	write(0x6F, 0x52);		// TX data rate 0

	write(0x30, 0x8C);		// Data access control

	write(0x32, 0xFF);		// Header control

	write(0x33, 0x42);		// Header 3, 2, 1, 0 used for head length, fixed packet length, synchronize word length 3, 2,

	write(0x34, 64);		// 64 nibble = 32 byte preamble
	write(0x35, 0x20);		// 0x35 need to detect 20bit preamble
	write(0x36, 0x2D);		// synchronize word
	write(0x37, 0xD4);
	write(0x38, 0x00);
	write(0x39, 0x00);
	write(0x3A, 's');		// set tx header 3
	write(0x3B, 'o');		// set tx header 2
	write(0x3C, 'n');		// set tx header 1
	write(0x3D, 'g');		// set tx header 0
	write(0x3E, 50);		// set packet length to 17 bytes

	write(0x3F, 's');		// set rx header
	write(0x40, 'o');
	write(0x41, 'n');
	write(0x42, 'g');
	write(0x43, 0xFF);		// check all bits
	write(0x44, 0xFF);		// Check all bits
	write(0x45, 0xFF);		// check all bits
	write(0x46, 0xFF);		// Check all bits

	//write(0x56, 0x01);

	write(0x6D, 0x07);		// Tx power to max

	write(0x79, 0x00);		// no frequency hopping
	write(0x7A, 0x00);		// no frequency hopping

	write(0x71, 0x22);		// GFSK, fd[8]=0, no invert for TX/RX data, FIFO mode, txclk-->gpio

	write(0x72, 0x48);		// Frequency deviation setting to 45K=72*625

	write(0x73, 0x00);		// No frequency offset
	write(0x74, 0x00);		// No frequency offset

	write(0x75, 0x53);		// frequency set to 434MHz
	write(0x76, 0x64);		// frequency set to 434MHz
	write(0x77, 0x00);		// frequency set to 434Mhz

	/*write(0x5A, 0x7F);
	write(0x59, 0x40);
	write(0x58, 0x80);

	write(0x6A, 0x0B);
	write(0x68, 0x04);*/
	write(0x1F, 0x03);
}


void si4432::to_tx_mode(const char tx_buf[])
{

	unsigned char i;

	write(0x07, 0x02);	// To ready mode
	//cbi(PORTD, RXANT);
	//sbi(PORTD, TXANT);

	write(0x08, 0x03);	// FIFO reset
	write(0x08, 0x00);	// Clear FIFO

	//write(0x34, 64);	// preamble = 64nibble
	//write(0x3E, 50);	// packet length = 17bytes
	for (i=0; i<50; i++)
	{
		write(0x7F, tx_buf[i]);	// send payload to the FIFO
	}

	write(0x05, 0x04);	// enable packet sent interrupt
	i = read(0x03);		// Read Interrupt status1 register
	i = read(0x04);

	write(0x07, 0x0A);	// Start TX

	while (IRQ()) ;	 //need to check interrupt here
	//write(0x07, 0x02);	// to ready mode
	//cbi(PORTD, RXANT);	// disable all interrupts
	//cbi(PORTD, TXANT);

}

bool si4432::IRQ ()
{
    return digitalRead(IRQ_PIN);
}


void si4432::to_rx_mode(void)
{
	rx_reset();
}

void si4432::rx_reset(void)
{


	write(0x07, 0x01);	// to ready mode

	read(0x03);
	read(0x04);

	write(0x7E, 50);

	write(0x08, 0x03);
	write(0x08, 0x00);

	write(0x07, 5);

	write(0x05, 2);
}

