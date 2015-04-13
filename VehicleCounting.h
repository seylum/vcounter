#ifndef VEHICLECOUNTING_H
#define VEHICLECOUNTING_H

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <cv.h>
#include <highgui.h>
#include <cmath>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "cvblob.h"
#include "quantile.h"
#include "mysql.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <cstring>

enum LaneOrientation
{
  LO_NONE       = 0,
  LO_HORIZONTAL = 1,
  LO_VERTICAL   = 2
};

enum VehiclePosition
{
  VP_NONE = 0,
  VP_A  = 1,
  VP_B  = 2,
  VP_D  = 4
};

class VehicleCouting
{
private:
  bool firstTime;
  bool showOutput;
  int key;
  cv::Mat img_input;
  cvb::CvTracks tracks;
  std::map<cvb::CvID, std::vector<CvPoint2D64f> > points;
  LaneOrientation LaneOrientation;
  std::map<cvb::CvID, VehiclePosition> positions;

  int img_w;
  int img_h;
  int showAB;
  int showCD;

public:
  long countBA;long countBA2;

  long countBAC;long countBAC2;long countBAC3;
  long countBAD;long countBAD2;long countBAD3;
  long countBAE;long countBAE2;long countBAE3;

  long countAB;long countAB2;long countAB3;long countAB4;
  long countABC;long countABC2;long countABC3;long countABC4;
  long countABD;long countABD2;long countABD3;long countABD4;
  long countABE;long countABE2;long countABE3;

  long countBC;long countBC2; long countBC3; long countBC4;
  long countBCC;long countBCD;long countBCE;
  long countBCC2;long countBCD2; long countBCE2;
  long countBCC3;long countBCD3;long countBCE3;
  long countBCC4;long countBCD4;

  int areaX;  int areaY;  int areaZ;
  int areaA;int areaB;int areaC;int areaE; int areaF;  int areaG;
  int areaP; int areaQ; int areaR;
  int areaXX; int areaYY;
  int areaSS; int areaTT;

  int idken1; int idken2;
  int idkend1; int idkend2;

  int idken1b; int idken2b;
  int idkend1b; int idkend2b;

  int idken1c; int idken2c;
  int idkend1c; int idkend2c;

  int i; float sum; float avg;
  int i2; float sum2; float avg2;
  int i3; float sum3; float avg3;

  float aaa; float bbb; float ddd; float eee; float hhh; float iii;

  float jarak; float jarak2; float jarak3; float jarakv;

  int ticki; int tickf;
  int tocki; int tockf;

  int ticki2; int tickf2;
  int tocki2; int tockf2;

  int ticki3; int tickf3;
  int tocki3; int tockf3;

  int tack; int teck; int tackv; int teckv;
  int tack2; int teck2; int tackv2; int teckv2;
  int tack3; int teck3; int tackv3; int teckv3;

  float waktu; float waktu2; float waktu3;
  float waktuv; float waktuv2; float waktuv3;

  float kec; float kec2; float kec3;
  float kecv; float kecv2; float kecv3;

  float gap; float gap2; float gap3;
  float gapv; float gapv2; float gapv3;

  float sumgap; float avggap; long countBCCmin;
  float sumgap2; float avggap2; long countBCDmin;
  float sumgap3; float avggap3; long countBCEmin;

  int kecvint;

  CvPoint2D64f areaD;
  VehicleCouting();
  ~VehicleCouting();

  int milli()
  {
      timeval curTime;
      gettimeofday(&curTime, NULL);
      return (curTime.tv_usec / 1000);
  }

  unsigned long GetTickCount()
  {
          struct timeval tv;
          if(gettimeofday(&tv, NULL) != 0)
                  return 0;

          return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  }

  /*const std::string currentDateTime(){
      time_t now = time(0);
      struct tm tstruct;
      char buf[80];
      tstruct = *localtime(&now);
      strftime(buf, sizeof(buf), "%d/%m/%Y %X", &tstruct);
      return buf;
  }*/

  time_t now;
  struct tm *current;

  const std::string currentDateTime(){
      now = time(0);
      current = localtime(&now);
      char buf[80];
      strftime(buf,sizeof(buf), "%d/%m/%Y %X", current);
      return buf;
  }

  int detiks(){
      now = time(0);
      current = localtime(&now);
      int detik = current->tm_sec;
      return detik;
  }

  int menits(){
      now = time(0);
      current = localtime(&now);
      int menit = current->tm_min;
      return menit;
  }

  int jams(){
      now = time(0);
      current = localtime(&now);
      int jam = current->tm_hour;
      return jam;
  }

  int percentile(){
      std::istream_iterator<int> input(std::cin);
      std::istream_iterator<int> last;
      int val = clx::percentile(input, last, 85);
      return val;
  }

  void setInput(const cv::Mat &i);
  void setTracks(const cvb::CvTracks &t);
  void process();

//private:
  VehiclePosition getVehiclePosition(const CvPoint2D64f centroid);

};

#endif // VEHICLECOUNTING_H
