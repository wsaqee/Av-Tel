#include <iostream>
#include "../include/i2cComm.h"
#include <iomanip>
#include "../include/MPU6050.h"
#include "../include/HMC5883.h"
#include "../include/GPS.h"

#include <wiringPi.h>
#include <cmath>
#include <chrono>
#include <fstream>

#include <string>
#include <sstream>



using namespace std;


//global

double roll, pitch, heading;
const double cFilt[] = {0.9, 0.9, 0.96};       //complentary filter constants {pitch, roll, heading}
const double t = 200./1000;                   //sample time in sec

//global_end
int main()
{

    MPU6050 ag = MPU6050();
    HMC5883 comp = HMC5883();
    GPS gps = GPS();
    cout << setprecision(2) << fixed;
    ofstream f_out("LoggedData.txt");
    if (!f_out.is_open() )
    {
        cout<<"Error"<<endl;
        return -1;
    }


    //header for log file:
    //string flightNum;
    //cout << "Enter Flight Number: ";
    //cin >> flightNum;

    //flight info save to another file

    //f_out << "Start of Flight [Flight" << flightNum << "] : [yyyy/mm/dd], hh:mm:ss" << endl;
    //f_out << "data delimiter ';'"<<endl;

    f_out << "time, GPSUTC, GPSdate, accelX, accelY, accelZ, gyroX, gyroY, gyroZ, temp, accelPitch, accelRoll, gyroAngleChangeX, gyroAngleChangeY, gyroAngleChangeZ, headingCompensation, pitch, roll, heading, GPSlat, GPSlon, GPScrs, GPSspd"<<endl;
    //end of header for log file


    //wait for start cmd
    string in;
    cout << "Enter 'y' and press enter to start logger\n";
    do
    {
        cin >> in;
    }while(in.compare("y"));

    //start sampling and logging
    while(1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        static double avgT, timeStamp, delayT = t*1000;
        int a[3];                   //accel
        int w[3];                   //gyro
        double dw [3];              //gyro angle change
        int temp = 0;
        double headingComp;         //compass heading

        double lat = 0, lon = 0;    //GPS position
        double spd = 0;             //GPS spd
        double crs = 0;             //GPS crs
        double alt = 0;              //GPS alt
        latLonIndicator latIncd = N;
        latLonIndicator lonIncd = N;


        ag.getAccelData(a[0], a[1], a[2]);              //get data, x, y, z
        ag.getGyroData(w[0], w[1], w[2]);               //get data
        ag.getTemp(temp);                               //get data
        gps.getLat(lat, latIncd);
        gps.getLong(lon, lonIncd);
        spd = gps.getSpeed();
        crs = gps.getCourse();

        //log
        f_out<<timeStamp<<","<<gps.getUTC()<<","<<gps.getDate()<<",";
        for (int i=0; i<3; i++) f_out << a[i] << ",";
        for (int i=0; i<3; i++) f_out << w[i] << ",";
        //for (int i=0; i<3; i++) f_out << magF[i] << ",";
        f_out<<temp<<",";

        //pitch, roll angle from acceleration
        double pitch_accel = atan2(a[0], a[2]);         //atan(ax,az)
        double roll_accel = atan2(a[1], a[2]);          //atan(ay,az)
        //log
        f_out<<pitch_accel<<","<<roll_accel<<",";

        //gyro angle change (integration)
        for (int i=0; i<3; i++) dw[i] = w[i]*t/(32768*180)*PI;
        //log
        for (int i=0; i<3; i++) f_out << dw[i] << ",";


        heading = headingComp = comp.getHeading(pitch, roll);
        //log
        f_out<<headingComp<<",";


        //complementary filter
        pitch = cFilt[0]*(pitch + dw[0]) + (1-cFilt[0])*pitch_accel;
        roll = cFilt[1]*(roll + dw[1]) + (1-cFilt[1])*roll_accel;
        //heading = cFilt[2]*(heading - dw[2]) + (1-cFilt[2])*headingComp;
        //log
        f_out<<pitch<<","<<roll<<","<<heading<<",";
        f_out<<lat<<","<<lon<<","<<crs<<","<<spd<<";\n";


        //console display:
        static int counter;
        counter++;

        system("clear");
        cout<<"pitch: "<<pitch*180/PI<<endl;
        cout<<"roll: "<<roll*180/PI<<endl;
        cout<<"heading: "<<heading*180/PI<<endl;
        cout<<"lat: "<<lat;
        latIncd == N ? cout <<"N"<<endl: cout<<"S"<<endl;
        cout<<"lon: "<<lon;
        lonIncd == E ? cout <<"E"<<endl: cout<<"W"<<endl;
        cout << "Spd: "<<spd<<endl;
        cout << "Crs: "<< crs<<endl;
        cout << gps.getRAW()<< endl;

        std::cout << "Elapsed avg time(refresh rate): " << avgT*1000 << " ms\n";

        if (counter >= 5)
        {
            gps.update();
            counter = 0;
        }



        //delay
        delay(delayT);

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        avgT = (elapsed.count() + avgT)/2;
        delayT += (t - avgT)*1000;
        if (delayT<0) delayT = 0;
        timeStamp+=elapsed.count();


    }


    f_out.close();


    return 0;
}
