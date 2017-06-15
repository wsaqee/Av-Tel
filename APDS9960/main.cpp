#include <iostream>
#include <apds9960.h>
#include <wiringPi.h>
#include <string>
using namespace std;


#define STD_DELAY 100
#define PROX_LOW_THD 0xA
#define PROX_HIGH_THD 0x15
#define TIMER_DEFAULT_VAL 20

class swipe
{
    public:
        swipe()
        {
            reset();
        }
        virtual ~swipe(){}


    protected:
        bool readSwipe()
        {
            if (!_swipe) return false;

            bool val = _swipe;
            reset();
            return val;
        }

        void updateSwipe(int proxVal)
        {
            if (!_isHigh)
            {
                if (proxVal>PROX_HIGH_THD)
                {
                    _isHigh = true;
                }
            }else
            {
                if (proxVal<PROX_LOW_THD)
                {
                    _swipe = true;
                }
            }
        }


    private:
        void reset()
            {
                _swipe = false;
                _isHigh = false;
            }
        bool _isHigh;
        bool _swipe;



};

class LED:public swipe
{
    public:
        LED():swipe()
        {
            reset();
        }
        virtual ~LED(){}

        void reset()
        {
            _isLedOn = false;
            _oneSwipe = false;
            _timer = TIMER_DEFAULT_VAL;
        }
        void LedOn()
        {
            _isLedOn = true;
            getLEDstate()?cout<<"LED on"<<endl:cout<<"Error - !LED on"<<endl;
        }
        void LedOff()
        {
            _isLedOn = false;
            !getLEDstate()?cout<<"LED off"<<endl:cout<<"Error - !LED off"<<endl;
        }
        bool getLEDstate()
        {
            return _isLedOn;
        }
        /*bool getSwipeStatus()
        {
            return _oneSwipe;
        }*/
        bool processSwipe(int proxVal)
        {
            updateSwipe(proxVal);
            if (getLEDstate())
            {
                if (_oneSwipe)
                {
                    //if timer runs out, reset swipe
                    if (_timer--<=0)
                    {
                        _oneSwipe = false;
                    }else
                    {
                        if (readSwipe()) return true;
                    }

                }else
                {
                    _oneSwipe = readSwipe();
                    _timer = TIMER_DEFAULT_VAL;
                }
                return false;
            }else
            {
                _oneSwipe = false;
                return readSwipe();
            }

        }


    private:
        bool _isLedOn;
        bool _oneSwipe;
        int _timer;

};




int main()
{
    apds9960 s1 = apds9960();
    s1.enableLightSensor();
    s1.enableProximitySensor();
    //s1.enableGestureSensor();
    s1.enablePower();

    LED lampa = LED();

    uint8_t prox;
    uint16_t amb;
    int gest = 0;

    delay(200);



    while(1)
    {
        cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
        cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;

        s1.readProximity(prox);
        cout<<"Proximity :"<<(int)prox<<endl;
        s1.readAmbientLight(amb);
        cout<<"Ambient Light: "<<(int)amb<<endl;
        s1.readRedLight(amb);
        cout<<"Red Light: "<<(int)amb<<endl;
        int a ;
        /*if ((a = s1.readGesture())!=0)
        {
            gest = a;
        }
        cout<<gest;

        cout<<endl;*/
        if (lampa.processSwipe(prox))
        {
            lampa.getLEDstate()?lampa.LedOff():lampa.LedOn();
            //return 0;
        }else
        {
            lampa.getLEDstate()?cout<<"LED on"<<endl:cout<<"LED off"<<endl;
        }



        delay(STD_DELAY);
    }



    return 0;
}
