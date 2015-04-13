#include "VehicleCounting.h"

namespace FAV1
{
  IplImage* img_input1 = 0; //ambil capture gambar
  IplImage* img_input2 = 0; //beri capture gambar counting line

  int roi_x0 = 0;
  int roi_y0 = 0;
  int roi_x1 = 0;
  int roi_y1 = 0;

  int roi_x2 = 0;
  int roi_y2 = 0;
  int roi_x3 = 0;
  int roi_y3 = 0;

  int numOfRec = 0;
  int numOfRec2 =0;
  int startDraw = 0;
  int startDraw2 = 0;
  bool roi_defined = false;
  bool roi_defined2 = false;
  bool use_roi = true;
  bool use_roi2 = true;

  //fungsi untuk menggambar garis 1
  void VehicleCouting_on_mouse(int evt, int x, int y, int flag, void* param)
  {
    if(!use_roi)
      return;
    //menarik garis diawali klik kiri mouse
    if(evt == CV_EVENT_LBUTTONDOWN)
    {
      if(!startDraw)
      {
        roi_x0 = x;
        roi_y0 = y;
        startDraw = 1;
      }
      else
      {
        roi_x1 = x;
        roi_y1 = y;
        startDraw = 0;
        roi_defined = true;
      }
    }

    //menampikan garis yang dibuat
    if(evt == CV_EVENT_MOUSEMOVE && startDraw)
    {
      //redraw ROI selection
      img_input2 = cvCloneImage(img_input1);
      cvLine(img_input2, cvPoint(roi_x0,roi_y0), cvPoint(x,y), CV_RGB(255,0,255));
      cvShowImage("VehicleCouting", img_input2);
      cvReleaseImage(&img_input2);
    }
  }

  //fungsi untuk menggambar garis 2
  void VehicleCouting_on_mouse2(int evt2, int x2, int y2, int flag, void* param)
  {
    if(!use_roi2)
      return;

    if(evt2 == CV_EVENT_LBUTTONDOWN)
    {
      if(!startDraw2)
      {
        roi_x2 = x2;
        roi_y2 = y2;
        startDraw2 = 1;
      }
      else
      {
        roi_x3 = x2;
        roi_y3 = y2;
        startDraw2 = 0;
        roi_defined2 = true;
      }
    }

    if(evt2 == CV_EVENT_MOUSEMOVE && startDraw2)
    {
      //orangedraw ROI selection
      img_input2 = cvCloneImage(img_input1);
      cvLine(img_input2, cvPoint(roi_x2,roi_y2), cvPoint(x2,y2), CV_RGB(255,153,255));
      cvShowImage("VehicleCouting", img_input2);
      cvReleaseImage(&img_input2);
    }
  }
    }


struct connection_details
{
    char *server;
    char *user;
    char *password;
    char *database;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
     // first of all create a mysql instance and initialize the variables within
    MYSQL *connection = mysql_init(NULL);

    // connect to the database with the details attached.
    if (!mysql_real_connect(connection,mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {
      printf("Conection error : %s\n", mysql_error(connection));
      exit(1);
    }
    return connection;
}


VehicleCouting::VehicleCouting()
{
  std::cout << "VehicleCouting()" << std::endl;
  firstTime=true;
  showOutput=true;
  key=0;
  showAB=1;
  showCD=1;

  countBA=0;countBA2=0;
  countAB=0;countAB2=0; countAB3=0; countAB4=0;
  countBC=0;countBC2=0; countBC3=0; countBC4=0;

  countABC=0; countABC2=0; countABC3=0; countABC4=0;
  countABD=0; countABD2=0; countABD3=0; countABD4=0;
  countABE=0; countABE2=0; countABE3=0;
  countBCC=0; countBCC2=0; countBCC3=0; countBCC4=0;
  countBCD=0; countBCD2=0; countBCD3=0; countBCD4=0;
  countBCE=0; countBCE2=0; countBCE3=0;

  jarakv=0;
  jarak=0; jarak2=0; jarak3=0;
  waktu=0; waktu2=0;waktu3=0; waktuv=0; waktuv2=0; waktuv3=0;
  kec=0; kec2=0; kec3=0; kecv=0; kecv2=0; kecv3=0;
  gap=0; gap2=0; gap3=0; gapv=0; gapv2=0; gapv3=0;
  tack=0; tack2=0; tack3=0; tackv=0; tackv2=0; tackv3=0;
  teck=0; teck2=0; teck3=0; teckv=0; teckv2=0; teckv3=0;
  areaA=0; areaB=0; areaC=0; areaE=0; areaF=0; areaG=0; areaP=0; areaX=0;
  areaYY=0; areaXX=0;
  areaSS=0; areaTT=0;

  i=0; sum=0; avg=0; i2=0; sum2=0; avg2=0; i3=0; sum3=0; avg3=0;

  sumgap=0; avggap=0; countBCCmin=0;
  sumgap2=0; avggap2=0; countBCDmin=0;
  sumgap3=0; avggap3=0; countBCEmin=0;
}

VehicleCouting::~VehicleCouting()
{
  std::cout << "~VehicleCouting()" << std::endl;
}

//fungsi setInput
void VehicleCouting::setInput(const cv::Mat &i)
{
  //i.copyTo(img_input);
  img_input = i;
}

//fungsi setTracks
void VehicleCouting::setTracks(const cvb::CvTracks &t)
{
  tracks = t;
}

//untuk mengetahui posisi kendaraan apakah berada di A atau B atau C
VehiclePosition VehicleCouting::getVehiclePosition(const CvPoint2D64f centroid)
{
  VehiclePosition vehiclePosition = VP_NONE;

  if(LaneOrientation == LO_HORIZONTAL)
  {
    if(centroid.x < FAV1::roi_x0 && centroid.x < FAV1::roi_x2)
    {
      cv::putText(img_input, "STATE: A", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_A;
    }

    if(centroid.x > FAV1::roi_x0 && centroid.x < FAV1::roi_x2)
    {
      cv::putText(img_input, "STATE: B", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_B;
    }

    if(centroid.x > FAV1::roi_x2 && centroid.x > FAV1::roi_x0)
    {
      cv::putText(img_input, "STATE: C", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_D;
    }
  }

  if(LaneOrientation == LO_VERTICAL)
  {
    if(centroid.y > FAV1::roi_y0 && centroid.y > FAV1::roi_y2)
    {
      cv::putText(img_input, "STATE: A", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_A;
    }

    if(centroid.y < FAV1::roi_y0 && centroid.y > FAV1::roi_y2)
    {
      cv::putText(img_input, "STATE: B", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_B;
    }

    if(centroid.y < FAV1::roi_y2 && centroid.y < FAV1::roi_y0)
    {
      cv::putText(img_input, "STATE: C", cv::Point(10,img_h/4), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      vehiclePosition = VP_D;
    }
  }

  return vehiclePosition;
}

//fungsi process
void VehicleCouting::process()
{
  if(img_input.empty())
    return;

  img_w = img_input.size().width;
  img_h = img_input.size().height;

  //--------------------------------------------------------------------------

  //untuk mengecek apakah garis hitung 1 sudah dibuat
  if(FAV1::use_roi == true && FAV1::roi_defined == false && firstTime == true)
  {
    do
    {
      cv::putText(img_input, "masukan garis 1", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,255));
      cv::imshow("VehicleCouting", img_input);
      FAV1::img_input1 = new IplImage(img_input);
      cvSetMouseCallback("VehicleCouting", FAV1::VehicleCouting_on_mouse, NULL);
      key = cvWaitKey(0);
      delete FAV1::img_input1;

      if(FAV1::roi_defined)
      {
        std::cout << "garis hitung sudah terdefinisi (" << FAV1::roi_x0 << "," << FAV1::roi_y0 << "," << FAV1::roi_x1 << "," << FAV1::roi_y1 << ")" << std::endl;
        break;
      }
      else
        std::cout << "garis hitung belum terdefinisi!" << std::endl;
    }while(1);
  }

  //klo garis hitung 1 sudah terdefinisi maka tampilkan!
  if(FAV1::use_roi == true && FAV1::roi_defined == true)
    cv::line(img_input, cv::Point(FAV1::roi_x0,FAV1::roi_y0), cv::Point(FAV1::roi_x1,FAV1::roi_y1), cv::Scalar(0,0,255));

    bool ROI_OK = false;

    if(FAV1::use_roi == true && FAV1::roi_defined == true)
      ROI_OK = true;

  //untuk mengecek apakah garis hitung 2 sudah dibuat
  if(FAV1::use_roi2 == true && FAV1::roi_defined2 == false && firstTime == true)
  {
    do
    {
      cv::putText(img_input, "masukan garis 2", cv::Point(10,30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(51,153,255));
      cv::imshow("VehicleCouting", img_input);
      FAV1::img_input1 = new IplImage(img_input);
      cvSetMouseCallback("VehicleCouting", FAV1::VehicleCouting_on_mouse2, NULL);
      key = cvWaitKey(0);
      delete FAV1::img_input1;

      if(FAV1::roi_defined2)
      {
        std::cout << "garis hitung2 sudah terdefinisi (" << FAV1::roi_x2 << "," << FAV1::roi_y2 << "," << FAV1::roi_x3 << "," << FAV1::roi_y3 << ")" << std::endl;
        break;
      }
      else
        std::cout << "garis hitung2 belum terdefinisi!" << std::endl;
    }while(1);
   }

  //klo garis hitung 2 sudah terdefinisi maka tampilkan!
  if(FAV1::use_roi2 == true && FAV1::roi_defined2 == true)
      cv::line(img_input, cv::Point(FAV1::roi_x2,FAV1::roi_y2), cv::Point(FAV1::roi_x3,FAV1::roi_y3), cv::Scalar(51,153,255));

  bool ROI_OK2 = false;

  if(FAV1::use_roi2 == true && FAV1::roi_defined2 == true)
    ROI_OK2 = true;

  //untuk menentukan tipe line orientation (vertical atau horizontal)
  if(ROI_OK && ROI_OK2)
  {
    LaneOrientation = LO_NONE;

    if(abs(FAV1::roi_x0 - FAV1::roi_x1) < abs(FAV1::roi_y0 - FAV1::roi_y1) && abs(FAV1::roi_x2 - FAV1::roi_x3) < abs(FAV1::roi_y2 - FAV1::roi_y3))
    {
      if(!firstTime)
        cv::putText(img_input, "HORIZONTAL", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      LaneOrientation = LO_HORIZONTAL;

      cv::putText(img_input, "(A)", cv::Point(FAV1::roi_x0-32,FAV1::roi_y0), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      cv::putText(img_input, "(B)", cv::Point(FAV1::roi_x0+12,FAV1::roi_y0), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      cv::putText(img_input, "(C)", cv::Point(FAV1::roi_x2+12,FAV1::roi_y0), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
    } else {std::cout << "orientasi garis 1 dengan garis 2 tidak sama!";}

    if(abs(FAV1::roi_x0 - FAV1::roi_x1) > abs(FAV1::roi_y0 - FAV1::roi_y1) && abs(FAV1::roi_x2 - FAV1::roi_x3) > abs(FAV1::roi_y2 - FAV1::roi_y3))
    {
      if(!firstTime)
        cv::putText(img_input, "VERTICAL", cv::Point(10,15), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      LaneOrientation = LO_VERTICAL;

      cv::putText(img_input, "(A)", cv::Point(FAV1::roi_x0,FAV1::roi_y0+13), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      cv::putText(img_input, "(B)", cv::Point(FAV1::roi_x0,FAV1::roi_y0-3), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
      cv::putText(img_input, "(C)", cv::Point(FAV1::roi_x0,FAV1::roi_y2-3), cv::FONT_HERSHEY_PLAIN, 0.75, cv::Scalar(255,255,255));
    } else {std::cout << "orientasi garis 1 dengan garis 2 tidak sama!";}
  }

  //--------------------------------------------------------------------------

  //untuk menentukan posisi centroid
  for(std::map<cvb::CvID,cvb::CvTrack*>::iterator it = tracks.begin() ; it != tracks.end(); it++)
  {
    //std::cout << (*it).first << " => " << (*it).second << std::endl;
    cvb::CvID id = (*it).first;
    cvb::CvTrack* track = (*it).second;

    CvPoint2D64f centroid = track->centroid;
    //CvPoint2D64f areaD = track->centroid;

    //std::cout << "---------------------------------------------------------------" << std::endl;
    //std::cout << "id:" << id << " (" << centroid.x << "," << centroid.y << ")" << std::endl;
    //std::cout << "track->active:" << track->active << std::endl;
    //std::cout << "track->inactive:" << track->inactive << std::endl;
    //std::cout << "track->lifetime:" << track->lifetime << std::endl;


    //----------------------------------------------------------------------------

    //pengambilan nilai variabel2 yg diperlukan untuk hitung kecepatan based distance
        jarakv = abs(FAV1::roi_y0 - FAV1::roi_y2) ;

    //mereset nilai rata2 secara periodik
        int mil = milli();
        int det = detiks();
        int men = menits();
        //int ja  = jams();
        if (men % 15 == 0 && det % 60 == 0 && mil < 500)
        {
            //i=1; i2=1; i3=1;
            //sum = kecv; sum2 = kecv2; sum3 = kecv3;
            i=0; i2=0; i3=0;
            kecv=0; kecv2=0; kecv3=0;
            kec=0; kec2=0; kec3=0;
            sum = 0; sum2 = 0; sum3 = 0;
            avg = 0; avg2 = 0; avg3 = 0;
            countBCCmin=0; sumgap=0; avggap=0; gapv=0;
            countBCDmin=0; sumgap2=0; avggap2=0; gapv2=0;
            countBCEmin=1; sumgap3=0; avggap3=0; gapv3=0;
            gap=0; gap2=0; gap3=0;
            countBCC=0; countBCC2=0; countBCC3=0; countBCC4=0;
            countBCD=0; countBCD2=0; countBCD3=0; countBCD4=0;
            countABC=0; countABC2=0; countABC3=0; countABC4=0;
            countABD=0; countABD2=0; countABD3=0; countABD4=0;
            countAB=0; countAB2=0; countAB3=0; countAB4=0;
            countBC=0; countBC2=0; countBC3=0; countBC4=0;
            tackv=0; teckv=0; tackv2=0; teckv2=0;
            tack=0; teck=0; tack2=0; teck2=0;
            //tockf=0; tocki=0;
            waktu=0; waktu2=0;
            jarak=0; jarak2=0;
            waktuv=0; waktuv2=0;
            jarakv=0;
            areaSS=0; areaTT=0; areaX=0; areaP=0;
        }

    //pengambilan nilai variabel2 yg diperlukan untuk hitung kecepatan based lifetime
    //if(77 > track->centroid.x)
      if((108 < track->centroid.x) && (138 > track->centroid.x))
    {
        if(track->active == 4)
        {
           aaa = centroid.y;
           ticki = GetTickCount();
           idken1 = track -> id;
        }
        else if(track->active == 8)
            {
                bbb = centroid.y;
                tickf = GetTickCount();
                idken2 = track -> id;
            }
    }

    //if((77 < track->centroid.x) && (113 > track->centroid.x))
      if((138 < track->centroid.x) && (175 > track->centroid.x))
    {
        if(track->active == 4)
        {
           ddd = centroid.y;
           ticki2 = GetTickCount();
           idken1b = track -> id;
        }
        else if(track->active == 8)
            {
                eee = centroid.y;
                tickf2 = GetTickCount();
                idken2b = track -> id;
            }
    }
    /*
    if(113 < track->centroid.x)
    {
        if(track->active == 4)
        {
           hhh = centroid.y;
           ticki3 = GetTickCount();
           idken1c = track -> id;
        }
        else if(track->active == 8)
            {
                iii = centroid.y;
                tickf3 = GetTickCount();
                idken2c = track -> id;
            }
    }
    */
    //----------------------------------------------------------------------------

    if(track->inactive == 0)
    {
      if(positions.count(id) > 0)
      {
        std::map<cvb::CvID, VehiclePosition>::iterator it2 = positions.find(id);
        VehiclePosition old_position = it2->second;

        VehiclePosition current_position = getVehiclePosition(centroid);

        areaXX=track->centroid.x;
        areaYY=track->centroid.y;

    //----------------------------------------------------------------------------

        //menentukan apakah centroid bergerak dari A->B atau dari B->A
        if(old_position == VP_A && current_position == VP_B  && current_position != VP_D && old_position != VP_B)
        {
        //menghitung jumlah kendaraan dari a ke b
            countAB++;

        //menentukan id kendaraan yang melintas dari a ke b
            areaX = track->id;

        //menentukan lebar kendaraan yang melintas dari a ke b (vertical!) ->untuk horizontal perlu cond if
            areaY = track->maxx;
            areaZ = track->minx;
            areaB = areaY-areaZ;

        //mentrack koordinat centroid ketika melewati garis hitung
            areaA = track->centroid.x;
            areaC = track->centroid.y;

        //menghitung total kendaraan ukuran besar dari a ke b
            if(areaB >= 30)
            countAB2++;
        //menghitung total kendaraan ukuran sedang dari a ke b
            if(areaB < 30 && areaB >=13)
            countAB3++;
        //menghitung total kendaraan ukuran kecil dari a ke b
            if(areaB <13)
            countAB4++;

        //hitung total kendaraan & kecepatan based lifetime di line 1
            //if(77>areaA)
            if((108<areaA) && (138>areaA))
            {
            countABC++;
            areaSS = track->id;
            tocki = GetTickCount();
            if (countABC % 2 == 0)
            {
                tack = tocki;
                gap = (tocki - teck) * 0.001;
            }
            else if (countABC == 1)
            {
                teck = tocki;
                gap = (tocki - tocki - tack) * 0.001;
            }
            else
            {
                teck = tocki;
                gap = (tocki - tack) * 0.001;
            }
            if(idken1 == idken2)
            {
                jarak = std::abs(bbb - aaa);
                waktu = (tickf-ticki) * 0.001;
                kec = (jarak/waktu) * 2.5;
            }           

        //hitung kendaraan besar di line 1
            //if(77>areaA && areaB >= 32)
            if((108<areaA) && (138>areaA) && (areaB >= 30))
            countABC2++;
        //hitung kendaraan sedang di line 1
            //if(77>areaA && areaB < 32)
            if((108<areaA) && (138>areaA) && (areaB < 30) && (areaB >=13))
            countABC3++;
        //hitung kendaraan kecil di line 1
            if((108<areaA) && (138>areaA) && (areaB < 13))
            countABC4++;

            std::ofstream outfile;
            outfile.open("line1a.csv", std::ios_base::app);
            outfile << currentDateTime() << ',' << countABC << ',' << countABC2 << ',' << countABC3 << ',' << countABC4 << ',' << gap << ',' << kec << '\n';
            }

        //hitung total kendaraan & kecepatan based lifetime di line 2
            //if((77<areaA) && (113>areaA))
            if((138<areaA) && (175>areaA))
            {
            countABD++;
            tocki2 = GetTickCount();
            if (countABD % 2 == 0)
            {
                tack2 = tocki2;
                gap2 = (tocki2 - teck2) * 0.001;
            }
            else if (countABD == 1)
            {
                teck2 = tocki2;
                gap2 = (tocki2 - tocki2 - tack2) * 0.001;
            }
            else
            {
                teck2 = tocki2;
                gap2 = (tocki2 - tack2) * 0.001;
            }
            if(idken1b == idken2b)
            {
                jarak2 = std::abs(eee-ddd);
                waktu2 = (tickf2-ticki2) * 0.001;
                kec2 = (jarak2/waktu2) * 2.5;
            }

        //hitung kendaraan besar di line 2
            //if((77<areaA) && (113>areaA) && areaB >= 32)
            if((138<areaA) && (175>areaA) && (areaB >= 30))
            countABD2++;
        //hitung kendaraan sedang di line 2
            //if((77<areaA) && (113>areaA) && areaB < 32)
            if((138<areaA) && (175>areaA) && (areaB < 30) && (areaB >=13))
            countABD3++;
        //hitung kendaraan kecil di line 2
            if((138<areaA) && (175>areaA) && (areaB < 13))
            countABD4++;

            std::ofstream outfile;
            outfile.open("line2a.csv", std::ios_base::app);
            outfile << currentDateTime() << ',' << countABD << ',' << countABD2 << ',' << countABD3 << ',' << countABD4 << ',' << gap << ',' << kec << '\n';
            }

        //hitung total kendaraan & kecepatan based lifetime di line 3
            /*
            if(113<areaA)
            {
            countABE++;
            tocki3 = GetTickCount();
            if (countABE % 2 == 0)
            {
                tack3 = tocki3;
                gap3 = (tocki3 - teck3) * 0.001;
            }
            else if (countABE == 1)
            {
                teck3 = tocki3;
                gap3 = (tocki3 - tocki3 - tack3) * 0.001;
            }
            else
            {
                teck3 = tocki3;
                gap3 = (tocki3 - tack3) * 0.001;
            }
            if(idken1c == idken2c)
            {
                jarak3 = std::abs(iii-hhh);
                waktu3 = (tickf3-ticki3) * 0.001;
                kec3 = (jarak3/waktu3) * 2.5;
            }
            }
        //hitung kendaraan besar di line 3
            if(113<areaA && areaB >= 32)
            countABE2++;
        //hitung kendaraan sedang di line 3
            if(113<areaA && areaB < 32)
            countABE3++;
        */

        /*if(old_position == VP_B && current_position == VP_A)
        //menghitung jumlah kendaraan dari b ke a
            countBA++;
        //menentukan id kendaraan yang melintas dari b ke a
            areaX=track->id;
        //menentukan lebar kendaraan yang melintas dari b ke a (horizontal!) ->untuk vertical perlu cond if
            areaY=track->maxx;
            areaZ=track->minx;
            areaB=areaY-areaZ;
        //mentrack koordinat centroid ketika melewati garis hitung
            areaA=track->centroid.x;
            areaC=track->centroid.y;
        //menghitung jumlah kendaraan ukuran sedang dari b ke a
            if(old_position == VP_A && current_position == VP_B && areaB >= 32)
            countBA2++; */

          positions.erase(positions.find(id));
        }

    //----------------------------------------------------------------------------

        if(old_position == VP_B && current_position == VP_D && old_position != VP_A && current_position != VP_B)
        {
            //menghitung jumlah kendaraan dari b ke c
                countBC++;
                //menentukan id kendaraan yang melintas dari b ke c
                    areaP=track->id;
                //menentukan lebar kendaraan yang melintas dari b ke c (vertical!) ->untuk horizontal perlu cond if
                    areaQ=track->maxx;
                    areaR=track->minx;
                    areaF=areaQ-areaR;
                //mentrack koordinat centroid ketika melewati garis hitung
                    areaG=track->centroid.x;
                    areaE=track->centroid.y;
                //menghitung total kendaraan ukuran besar dari b ke c
                    if(areaF >= 30)
                    countBC2++;
                //menghitung total kendaraan ukuran sedang dari b ke c
                    if(areaF < 30 && areaF >=13)
                    countBC3++;
                //menghitung total kendaraan ukuran kecil dari b ke c
                    if(areaF < 13)
                    countBC4++;

                    //hitung total kendaraan di line 1
                    //if(77>areaG)
                    if((106<areaG) && (132>areaG))
                    {
                    countBCC++;
                    areaTT = track->id;
                    //sumgap += gapv;
                    tockf = GetTickCount();
                    if (countBCC % 2 == 0)
                    {
                        tackv = tockf;
                        gapv = (tockf - teckv) * 0.001;
                    }
                    else if (countBCC == 1)
                    {
                        teckv = tockf;
                        gapv = (tockf - tockf - tackv) * 0.001;
                    }
                    else
                    {
                        teckv = tockf;
                        gapv = (tockf - tackv) * 0.001;
                    }

                    sumgap += gapv;
                    countBCCmin++;
                    if(sumgap != 0){
                    avggap = sumgap / (countBCCmin-1);
                    }
                    else {
                        avggap=0;
                    }

                    if(areaSS == areaTT || tockf > tocki)
                    {                     
                        waktuv = (tockf-tocki) * 0.001;
                        kecv = (jarakv/waktuv) * 2.5;
                        i++;
                        sum += kecv;
                        avg = sum / i;
                    }

                    //}
                    //hitung kendaraan besar di line 1
                        //if(areaF >= 32)
                        if((106<areaG) && (132>areaG) && (areaF >= 30))
                        countBCC2++;
                    //hitung kendaraan sedang di line 1
                        //if(areaF < 32)
                        if((106<areaG) && (132>areaG) && (areaF < 30) && (areaF >=13))
                        countBCC3++;
                    //hitung kendaraan kecil di line 1
                        if((106<areaG) && (132>areaG) && (areaF < 13))
                        countBCC4++;

                        MYSQL *conn;
                        struct connection_details mysqlD;
                        mysqlD.server = "localhost";  // where the mysql database is
                        mysqlD.user = "root";		// the root user of mysql
                        mysqlD.password = ""; // the password of the root user in mysql
                        mysqlD.database = "Vtraco";
                        //mysqlD.database = "latihan";	// the database to pick

                        // connect to the mysql database
                        conn = mysql_connection_setup(mysqlD);
                        std::stringstream ss;
                        std::string jojo = "line2";
                        ss << "INSERT INTO "<<jojo<<" (waktu,total,sedang,besar,headway,kecepatan,avgheadway,avgkecepatan) VALUES(now(),"<<countBCC<<","<<countBCC2<<","<<countBCC3<<","<< gapv <<","<< kecv <<","<<avggap<<","<<avg<<");";
                        //ss << "INSERT INTO pertama (id,nama) VALUES("<< gapv <<","<< kecv <<");";
                        std::string aaa = ss.str();
                        mysql_query(conn, aaa.c_str() );

                        std::ofstream outfile;
                        outfile.open("line1b.csv", std::ios_base::app);
                        outfile << currentDateTime() << ',' << countBCC << ',' << countBCC2 << ',' << countBCC3 << ',' << countBCC4 << ',' << gapv << ',' << kecv << ',' << avggap << ',' << avg << '\n';
                    }

                    //hitung total kendaraan di line 2
                    //if((77<areaG) && (113>areaG))
                    if((132<areaG) && (163>areaG))
                    {
                    countBCD++;
                    tockf2 = GetTickCount();
                    if (countBCD % 2 == 0)
                    {
                        tackv2 = tockf2;
                        gapv2 = (tockf2 - teckv2) * 0.001;
                    }
                    else if (countBCD == 1)
                    {
                        teckv2 = tockf2;
                        gapv2 = (tockf2 - tockf2 - tackv2) * 0.001;
                    }
                    else
                    {
                        teckv2 = tockf2;
                        gapv2 = (tockf2 - tackv2) * 0.001;
                    }

                    sumgap2 += gapv2;
                    countBCDmin++;
                    if(sumgap2 != 0){
                    avggap2 = sumgap2 / (countBCDmin-1);
                    }
                    else {
                        avggap2=0;
                    }

                    if(areaX == areaP || tockf2 > tocki2)
                    {
                        waktuv2 = (tockf2-tocki2) * 0.001;
                        kecv2 = (jarakv/waktuv2) * 2.5;
                        i2++;
                        sum2 += kecv2;
                        avg2 = sum2 / i2;
                    }

                    //hitung kendaraan besar di line 2
                        //if((77<areaG) && (113>areaG) && areaF >= 32)
                        if((132<areaG) && (163>areaG) && (areaF >= 30))
                        countBCD2++;
                    //hitung kendaraan sedang di line 2
                        //if((77<areaG) && (113>areaG) && areaF < 32)
                        if((132<areaG) && (163>areaG) && (areaF < 30) && (areaF >=13))
                        countBCD3++;
                    //hitung kendaraan kecil di line 2
                        if((132<areaG) && (163>areaG) && (areaF < 13))
                        countBCD4++;

                        std::ofstream outfile;
                        outfile.open("line2b.csv", std::ios_base::app);
                        outfile << currentDateTime() << ',' << countBCD << ',' << countBCD2 << ',' << countBCD3 << ',' << countBCD4 << ',' << gapv2 << ',' << kecv2 << ',' << avggap2 << ',' << avg2 << '\n';
                    }

                    //hitung total kendaraan di line 3
                    /*
                    if(113<areaG)
                    {
                    countBCE++;
                    tockf3 = GetTickCount();
                    if (countBCE % 2 == 0)
                    {
                        tackv3 = tockf3;
                        gapv3 = (tockf3 - teckv3) * 0.001;
                    }
                    else if (countBCE == 1)
                    {
                        teckv3 = tockf3;
                        gapv3 = (tockf3 - tockf3 - tackv3) * 0.001;
                    }
                    else
                    {
                        teckv3 = tockf3;
                        gapv3 = (tockf3 - tackv3) * 0.001;
                    }

                    sumgap3 += gapv3;
                    countBCEmin++;
                    if(sumgap3 != 0){
                    avggap3 = sumgap3 / (countBCEmin-1);
                    }
                    else {
                        avggap3=0;
                    }

                    if(areaX == areaP)
                    {
                        waktuv3 = (tockf3-tocki3) * 0.001;
                        kecv3 = (jarakv/waktuv3) * 2.5;
                        i3++;
                        sum3 += kecv3;
                        avg3 = sum3 / i3;
                    }
                    }
                    //hitung kendaraan besar di line 3
                        if(113<areaG && areaF >= 32)
                        countBCE2++;
                    //hitung kendaraan sedang di line 3
                        if(113<areaG && areaF < 32)
                        countBCE3++;
                    */

                positions.erase(positions.find(id));
        }

      }
      else
      {
        VehiclePosition vehiclePosition = getVehiclePosition(centroid);

        if(vehiclePosition != VP_NONE)
          positions.insert(std::pair<cvb::CvID, VehiclePosition>(id,vehiclePosition));
      }
    }
    else
    {
      if(positions.count(id) > 0)
      positions.erase(positions.find(id));
    }

    //----------------------------------------------------------------------------

    if(points.count(id) > 0)
    {
      std::map<cvb::CvID, std::vector<CvPoint2D64f> >::iterator it2 = points.find(id);
      std::vector<CvPoint2D64f> centroids = it2->second;

      std::vector<CvPoint2D64f> centroids2;
      if(track->inactive == 0 && centroids.size() < 30)
      {
        centroids2.push_back(centroid);

        for(std::vector<CvPoint2D64f>::iterator it3 = centroids.begin() ; it3 != centroids.end(); it3++)
        {
          centroids2.push_back(*it3);
        }

        for(std::vector<CvPoint2D64f>::iterator it3 = centroids2.begin() ; it3 != centroids2.end(); it3++)
        {
          cv::circle(img_input, cv::Point((*it3).x,(*it3).y), 1, cv::Scalar(0,0,255), -1);
        }

        points.erase(it2);
        points.insert(std::pair<cvb::CvID, std::vector<CvPoint2D64f> >(id,centroids2));
      }
      else
      {
        points.erase(it2);
      }
    }
    else
    {
      if(track->inactive == 0)
      {
        std::vector<CvPoint2D64f> centroids;
        centroids.push_back(centroid);
        points.insert(std::pair<cvb::CvID, std::vector<CvPoint2D64f> >(id,centroids));
      }
    }

    //cv::waitKey(0);
  }

  //--------------------------------------------------------------------------
  //http://stackoverflow.com/questions/332111/how-do-i-convert-a-double-into-a-string-in-c
  //mengconvert double ke string dengan menggunakan boost

  //menampilkan hasil counting dalam bentuk teks di video hasil processing
  /*
  if(showAB == 0)
  {
    cv::putText(img_input, "A->B: " + boost::lexical_cast<std::string>(countAB), cv::Point(10,img_h-20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
    cv::putText(img_input, "B->A: " + boost::lexical_cast<std::string>(countBA), cv::Point(10,img_h-8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
  }

  if(showAB == 1)
    cv::putText(img_input, "A->B: " + boost::lexical_cast<std::string>(countAB), cv::Point(10,img_h-8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));

  if(showAB == 2)
    cv::putText(img_input, "B->A: " + boost::lexical_cast<std::string>(countBA), cv::Point(10,img_h-8), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
  */

  if(showOutput)
    cv::imshow("Vehicle Counting", img_input);

  firstTime = false;
}





