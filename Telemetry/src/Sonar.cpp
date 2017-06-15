#include "Sonar.h"
#include <wiringPi.h>


#define TRIGGER_PIN 18
#define ECHO_PIN 23


#define SOUND_VELOCITY 340


Sonar::Sonar()
{
    //ctor
    init();

}

Sonar::~Sonar()
{
    //dtor
}

void Sonar::init()
{
    fd = wiringPiSetupGpio();
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void Sonar::trigger()
{
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
}

bool Sonar::readDistance(double &d)
{
    double t = 0;
    trigger();
    if (!readEchoTime(t)) return false;
    d = t * SOUND_VELOCITY/2;
    return true;

}
bool Sonar::readEchoTime(double &t)
{

    if (!waitForPinChange(ECHO_PIN)) return false;
    int startTime = micros();
    if (!waitForPinChange(ECHO_PIN)) return false;

    //time in sec
    t = double(micros() - startTime) / 1e6;
    return true;
}

bool Sonar::waitForPinChange(int pin)
{
    int oldState = digitalRead(pin);
    for (int i=0; i<3000;i++)
    {
        if (digitalRead (pin) != oldState)
        {
            return true;
        }
        delayMicroseconds(10);
    }
    return false;
}
