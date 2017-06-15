#include <iostream>
#include "include/i2cComm.h"
#include <iomanip>
#include "MPU6050.h"
#include "HMC5883.h"
#include "serial.h"
#include "Sonar.h"
#include "GPS.h"
#include "si4432.h"

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

/*
    //TX
    while(1)
    {
    string str;
    while(getline (cin, str))
    {
        str +="\n";
        txRx.to_tx_mode(str.c_str());
        str = "";
    }

    }
*/
//
//    /////RX
//
//
//    txRx.rx_reset();
//    cout<< "rx_reset()"<<endl;
//    while(1)
//    {
//        if(txRx.IRQ() == 0)	//Interrupt will be generated if data is recieved
//		{
//			cout << "int generated"<< endl;
//			// Read 18-byte packet into RF_RXBUF[]:
//			for(int i=0; i<17; i++)
//			{
//				buf[i] = txRx.read(0x7F);
//			}
//
//			// Check that the checksum matches up:
//			/*unsigned char chksum = 0;
//			for(int i=0; i<16; i++)
//				chksum += buf[i];
//            */
//			// If checksum is good, print out what was received into the terminal
//			//if(chksum == buf[16])
//			{
//				txRx.write(0x07, 0x01);	// To ready mode
//				cout<<"Received: ";
//				for(int i=0; i<17; i++)
//					cout << buf[i]; // Print characters if passed the checksum
//                cout<<endl;
//			}
//			// otherwise reset and start listening again
//			/*else
//			{
//				cout <<"Bad checksum RX RESET...Listening again\n";
//			}*/
//			txRx.rx_reset();
//			delay(50);
//		}
//		delay(500);
//		cout << (int)txRx.read(0x31)<<endl;
//    }
//
//
//    //si4432 testing END
//

    ////////////////////////////////////////////
    MPU6050 ag = MPU6050();
    HMC5883 comp = HMC5883();
    GPS gps = GPS();
    Sonar son = Sonar();
    si4432 txRx = si4432();
    cout << setprecision(2) << fixed;
    ofstream f_out ("LoggedData.txt");
    if (!f_out.is_open() )
    {
        cout<<"Error"<<endl;
        return -1;
    }

    //variables
    //accel
    int aX=0, aY = 0, aZ = 0;
    //gyro
    int gX=0, gY = 0, gZ = 0;
    //compass
    int magX = 0, magY = 0, magZ = 0;
    //ag - temp
    int temp = 0;
    //sonar
    double dist = 0;

    //get test data
    ag.getAccelData(aX, aY, aZ);
    ag.getGyroData(gX, gY, gZ);
    ag.getTemp(temp);
    comp.getMagData(magX, magY, magZ);
    if (!son.readDistance(dist)) cout<<"Distance Error"<<endl;
    delay(1000);
    gps.update();

    //output test data
    cout<<"X: "<<aX<<", " <<"Y: "<<aY<<", "<<"Z: "<<aZ<<endl;
    cout<<"gX: "<<gX<<", " <<"gY: "<<gY<<", "<<"gZ: "<<gZ<<endl;
    cout<<"magX: "<<magX<<", "<<"magY: "<<magY<<", "<<"magZ: "<<magZ<<endl;
    cout<<"Temp: "<<t<<endl;
    cout<<"Temp: "<<temp<<endl;
    cout<<"Sonar Distance: "<<dist<<endl;
    cout<<"UTC"<<gps.getUTC()<<endl;
    cout<<"Serial Out: "<<gps.getRAW()<<endl;


    //header for log file:
    f_out << "Start of Flight [Flight#] : [yyyy/mm/dd], hh:mm:ss" << endl;
    f_out << "data delimiter ';'"<<endl;
    f_out << "[time], GPSUTC, GPSdate, accelX, accelY, accelZ, gyroX, gyroY, gyroZ, magX, magY, magZ, Temp, SonarDist, accelPitch, accelRoll, gyroAngleChangeX, gyroAngleChangeY, gyroAngleChangeZ, HeadingCompensation, pitch, roll, heading, GPSlat, GPSlon, GPScrs, GPSspd, [GPSalt]"<<endl;
    //end of header for log file


    //wait for start cmd
    string in = "";
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

        double dist = 0;            //sonar distance
        double lat = 0, lon = 0;    //GPS position
        double spd = 0;             //GPS spd
        double crs = 0;             //GPS crs
        //double alt = ;              //GPS alt
        latLonIndicator latIncd = N;
        latLonIndicator lonIncd = N;


        ag.getAccelData(a[0], a[1], a[2]);              //get data, x, y, z
        ag.getGyroData(w[0], w[1], w[2]);               //get data
        ag.getTemp(temp);                               //get data
        gps.getLat(lat, latIncd);
        gps.getLong(lon, lonIncd);
        spd = gps.getSpeed();
        crs = gps.getCourse();
        son.readDistance(dist);                          //get data - sonar distance measurement
        //log
        f_out<<timeStamp<<","<<gps.getUTC()<<","<<gps.getDate()<<",";
        for (int i=0; i<3; i++) f_out << a[i] << ",";
        for (int i=0; i<3; i++) f_out << w[i] << ",";
        //for (int i=0; i<3; i++) f_out << magF[i] << ",";
        f_out<<temp<<","<<dist<<",";

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
        f_out<<lat<<","<<lon<<","<<crs<<","<<spd<<";"<<endl;


        //console display:
        static int counter;
        counter++;
        //if (counter == 10 || counter == 20)
        {

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
            cout << "Distance(Sonar): "<< dist*100 << endl;
            cout << gps.getRAW()<< endl;

            //TX packet
            auto txTimeStart = std::chrono::high_resolution_clock::now();
            ostringstream strs;
            strs << pitch*180/PI << ";" << roll*180/PI << ";" << heading*180/PI << ";" << lat << ";" << latIncd << ";" << lon << ";" << lonIncd << ";" << dist*100 << endl;
            string RIO = strs.str();
            txRx.to_tx_mode(RIO.c_str());
            cout << "Packet sent"<<endl;
            auto txTimeEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> txTime = txTimeEnd - txTimeStart;
            cout << "Tx duration: " << txTime.count()*1000 << "ms" <<endl;
            std::cout << "Elapsed avg time: " << avgT*1000 << " ms\n";

        }
        if (counter >= 5)
        {
            gps.update();
            counter = 0;
        }



        //delay
        delay(delayT);

        //cout << "Max dT: "<<(float)maxT/CLOCKS_PER_SEC*1000<<", Min dT: "<<minT<<", dT: "<<(float)dT/CLOCKS_PER_SEC*1000.0<<", avg: "<<(double)avg/CLOCKS_PER_SEC*1000<<endl;

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
