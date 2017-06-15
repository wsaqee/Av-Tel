#include "GPS.h"
#include <string>
#include <iostream>

GPS::GPS():serial("/dev/ttyS0", 9600)
{
    //ctor
    RMC_raw = "";
    spd = 0;
    crs = 0;
    lat = 0;
    latIndicator = ERROR;
    lon = 0;
    lonIndicator = ERROR;
    alt = 0;
    UTC = 0;
    date = 0;
    dataValid = false;
}

GPS::~GPS()
{
    //dtor
}

/*
void GPS::splitInArray (std::string str, std::string arr[], std::string delimiter)
{
    size_t k;
    while(( k = str.find(delimiter))!= str.npos)
    {
        arr.append(str.substr(0, k));
        str = str.substr(k + delimiter.length());
    }
}*/

bool GPS::update()
{
    if (availData() == 0) return false;

        std::string str = "";
        std::string arr[12] ;
    try
    {


        getString(str);

        if (str.empty()) return false;
        size_t index = str.find("$GPRMC");
        if (index == std::string::npos) return false;
        str = str.substr(index);                                  //substring $GPRMC to end
        size_t endStr = str.find("\r\n",1);
        if ( endStr == std::string::npos) return false;
        str = str.substr(0, endStr);            //substring from $RMC to CR LF
        //update data

        RMC_raw = str;
        for (int i=0; i<12; i++)
        {
            size_t k = str.find(',');
            if (k ==std::string::npos) return false;

            arr[i] = str.substr(0,k);
            str = str.substr(k + 1);
        }


        //UTC time
        (arr[ADDR_UTC]).empty() ? UTC = 0: UTC = std::stod(arr[ADDR_UTC]);


        //dataValid
        if ((arr[ADDR_STAT]).empty())
        {
            dataValid = false;
        }else if ((arr[ADDR_STAT]).compare("A")== 0)
        {
            dataValid = true;
        }else
        {
            dataValid = false;
        }

        //Latitude
        (arr[ADDR_LAT]).empty() ? lat = 0: lat = std::stod(arr[ADDR_LAT]);

        //Latitude indicator
        if ((arr[ADDR_LATNS]).empty())
        {
            latIndicator = ERROR;
        }else if ((arr[ADDR_LATNS]).compare("N")== 0)
        {
            latIndicator = N;
        }else
        {
            latIndicator = S;
        }

        //longtitude
        (arr[ADDR_LON]).empty() ? lon = 0: lon = std::stod(arr[ADDR_LON]);

        //longtitude indicator
        if ((arr[ADDR_LONEW]).empty())
        {
            lonIndicator = ERROR;
        }else if ((arr[ADDR_LONEW]).compare("E")== 0)
        {
            lonIndicator = E;
        }else
        {
            lonIndicator = W;
        }
        //speed
        (arr[ADDR_SPD]).empty() ? spd = 0: spd = std::stod(arr[ADDR_SPD]);

        //course
        (arr[ADDR_CRS]).empty() ? crs = 0: spd = std::stod(arr[ADDR_CRS]);

        //date
        (arr[ADDR_DATE]).empty() ? date = 0: date = std::stoi(arr[ADDR_DATE]);




        return true;
    }
    catch (...)
    {
        std::cout<<"def ex"<<std::endl;
        std::cout<<RMC_raw<<std::endl<<RMC_raw.length()<<std::endl;
        for (int i=0; i<13; i++)
        {
            std::cout<<i<<". "<<arr[i]<<"is Empty"<<(arr[i]).empty()<<std::endl;
        }
        std::cin.get();
    }

}

double GPS::getUTC(void)
{
    return UTC;
}

bool GPS::getDataValid(void)
{
    return dataValid;
}

bool GPS::getLat(double& latitude, latLonIndicator& NS)
{
    latitude = lat;
    NS = latIndicator;
    return dataValid;
}

bool GPS::getLong(double& longtitude, latLonIndicator& EW)
{
    longtitude = lon;
    EW = lonIndicator;
    return dataValid;
}

double GPS::getSpeed()
{
    return spd;
}

double GPS::getCourse()
{
    return crs;
}

int GPS::getDate()
{
    return date;
}

std::string GPS::getRAW ()
{
    return RMC_raw;
}

void GPS::getRAW (std::string& str)
{
    str = getRAW();
}


