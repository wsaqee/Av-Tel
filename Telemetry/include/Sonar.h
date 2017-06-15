#ifndef SONAR_H
#define SONAR_H


class Sonar
{
    public:
        Sonar();
        virtual ~Sonar();
        void init();

        void trigger();
        bool readDistance(double &d);
        bool readEchoTime(double &t);

        bool waitForPinChange(int pin);
    protected:
    private:

        int fd; //linux filehandle


};

#endif // SONAR_H
