#ifndef GPS_H
#define GPS_H

#include "serial.h"
#include <array>

#define ADDR_UTC    1
#define ADDR_STAT   2
#define ADDR_LAT    3
#define ADDR_LATNS  4
#define ADDR_LON    5
#define ADDR_LONEW  6
#define ADDR_SPD    7
#define ADDR_CRS    8
#define ADDR_DATE   9


enum latLonIndicator {
                                N,
                                E,
                                S,
                                W,
                                ERROR};


class GPS : public serial
{
    public:
        GPS();
        virtual ~GPS();

        bool update();
        double getUTC(void);
        bool getDataValid(void);
        bool getLat(double& latitude, latLonIndicator& NS);
        bool getLong(double& longtitude, latLonIndicator& EW);
        double getSpeed();
        double getCourse();
        int getDate();
        std::string getRAW ();
        void getRAW (std::string& str);




    protected:
        //void splitInArray (std::string str, std::string arr[], std::string delimiter);
    private:


        std::string RMC_raw;
        double spd;
        double crs;
        double lat;
        latLonIndicator latIndicator;
        double lon;
        latLonIndicator lonIndicator;
        double alt;
        double UTC;
        int date;
        bool   dataValid;

};

#endif // GPS_H
