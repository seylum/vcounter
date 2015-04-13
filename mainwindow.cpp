#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include <time.h>
#include <math.h>
#include <QtGui>

#include "BlobTracking.h"
#include "VehicleCounting.h"
#include "PixelBased.h"
#include "SJN_MultiCueBGS.h"
#include "MixtureOfGaussianV2BGS.h"
#include "SigmaDeltaBGS.h"
#include "SuBSENSE.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bkgnd("images/background.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);
    //ui->lcdNumber->setPalette(Qt::red);
    ui->label_16->setPixmap(QPixmap("images/smart.png", 0, Qt::AutoColor));
    ui->label_17->setPixmap(QPixmap("images/cctv.png", 0, Qt::AutoColor));
    ui->label_21->setPixmap(QPixmap("images/id.png", 0, Qt::AutoColor));
    ui->label_22->setPixmap(QPixmap("images/width.png", 0, Qt::AutoColor));
    ui->label_38->setPixmap(QPixmap("images/jalan1.jpg", 0, Qt::AutoColor));
    ui->label_44->setPixmap(QPixmap("images/car.png", 0, Qt::AutoColor));
    ui->label_45->setPixmap(QPixmap("images/truck.png", 0, Qt::AutoColor));
    ui->label_52->setPixmap(QPixmap("images/car.png", 0, Qt::AutoColor));
    ui->label_53->setPixmap(QPixmap("images/truck.png", 0, Qt::AutoColor));
    ui->label_56->setPixmap(QPixmap("images/jalan2.jpg", 0, Qt::AutoColor));
    ui->label_75->setPixmap(QPixmap("images/car.png", 0, Qt::AutoColor));
    ui->label_76->setPixmap(QPixmap("images/truck.png", 0, Qt::AutoColor));
    ui->label_46->setPixmap(QPixmap("images/motor.png", 0, Qt::AutoColor));
    ui->label_81->setPixmap(QPixmap("images/motor.png", 0, Qt::AutoColor));
    ui->label_82->setPixmap(QPixmap("images/motor.png", 0, Qt::AutoColor));
    //ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    //waktu lama
    clock_t t;
        int ts2;
        int ts = 0;
        int beda = 0;

    //waktu baru
        time_t start = time(0);
        //time_t now;
        //time_t start = time(&now);


        cv::VideoCapture capture;
        //capture.open("http://root:bbic@192.168.0.90/mjpg/video.mjpg");
        capture.open("/Users/yohanssen/Desktop/build-vcounter-Desktop-Debug/videos/c.m4v");
        //capture.open(0);

                if(!capture.isOpened())
        {
                //std::cerr adalah standard output stream for errors
                std::cerr << "Tidak Dapat Membuka Video!" << std::endl;
                //return 1;
        }

                cv::Mat frame_aux;

     //---------------------------------------konstruktor baru-------------------------------------//
                //mog2 *bgs;
                //bgs = new mog2;
                SJN_MultiCueBGS S;
                SigmaDeltaBGS s;
                SuBSENSE Su;

                //PixelBased *bgs;
                //bgs = new PixelBased;
                PixelBased P;

                //BlobTracking *blobTracking;
                //blobTracking = new BlobTracking;
                BlobTracking B;

                //VehicleCouting *vehicleCouting;
                //vehicleCouting = new VehicleCouting;
                VehicleCouting V;



     //--------------------------------------------------------------------------------------------//

                double fps=capture.get(CV_CAP_PROP_FPS);
                std::cout<< "fps: "<<fps<<endl;

                int key = 0;
                //akan terus looping asal jangan pencet esc (ascii 27) atau c
                while(key != 'c')
                {

                t = clock();
                ts2 = ((float)t)/CLOCKS_PER_SEC;
                if(ts2 != ts)
                {
                beda = 1;
                ts = ts2;
                }
                else
                {
                beda = 0;
                }

                //waktu baru
                double seconds_since_start = difftime(time(0),start);
                //double seconds_since_start = difftime(time(&now),start);

                    //double frameRate = capture.get(CV_CAP_PROP_FPS);
                    //std::cout << frameRate << std::endl;

                //The function cvQueryFrame grabs a frame from camera or video file, decompresses and returns it.
                //This function is just a combination of cvGrabFrame and cvRetrieveFrame in one call.
                //The returned image should not be released or modified by user.
                capture >> frame_aux;
                //Ran out of film!
                if(!frame_aux.data) break;

                cv::Mat img_input(frame_aux);
                //cv::resize(img_input,img_input,cv::Size(320,240));
                //cv::imshow("input",img_input);

                cv::Mat img_mask;
                //uncomment untuk multicue krn pd process hrs ada bgmodel
                cv::Mat img_bgmodel;
                //bgs->process(img_input,img_mask);
                P.process(img_input,img_mask);

                cv::Mat img_blob;
                if(!img_mask.empty())
                {
                    //blobTracking->process(img_input,img_mask,img_blob);
                    B.process(img_input,img_mask,img_blob);

                    //vehicleCouting->setInput(img_blob);
                    V.setInput(img_blob);
                    //vehicleCouting->setTracks(blobTracking->getTracks());
                    V.setTracks(B.getTracks());
                    //vehicleCouting->process();
                    V.process();
                }

                //menunggu selama 33ms sampai frame berikutnya menggantikan frame sebelumnya
                key = cvWaitKey(1000/fps);

                //cout<<"waktu yang sudah berjalan: "<<t <<" clicks "<<ts<<" s"<<endl;

                capture.read(frame_aux);
                QImage img = QImage((const unsigned char*)(frame_aux.data),frame_aux.cols,frame_aux.rows,
                                     QImage::Format_RGB888);
                ui->label_10->setPixmap(QPixmap::fromImage(img));

                if(ts2%60==0 && beda==1)
                {
                    //....print to database here
                    cout<<"jumlah kendaraan yang lewat pada detik ke: "<< ts<<" adalah "<<V.countAB<<endl;
                    int cc=V.countAB;
                    int dd=ts/60;
                    ui->lcdNumber_7->display(cc);
                    ui->lcdNumber_8->display(dd);
                }
                int aa=V.countAB;
                int bb=ts;
                int ee=V.countBCC2;
                int ff=V.countBC2;
                int gg=V.countAB2;
                //untuk menghitung jumlah kendaraan kecil (selisih dari total kendaraan dgn kendaraan sedang)
                //int hh=V.countAB-V.countAB2;
                //int cc=V.countBC-V.countBC2;
                int hh=V.countAB3;
                int cc=V.countBC3;
                int dd=V.countBCC3;
                int ii=V.areaYY;
                int jj=V.areaXX;
                int kk=V.countABC;
                int ll=V.countABD;
                int mm=V.countABE;
                int nn=V.countABC2;
                int oo=V.countABD2;
                int pp=V.countABE2;
                int qq=V.countABC3;
                int rr=V.countABD3;
                int ss=V.countABE3;
                int tt=V.countBC;
                int uu=V.countBCC;
                int vv=V.countBCD;
                int ww=V.countBCE;
                int xx=V.countBCD2;
                int yy=V.countBCD3;
                int zz=V.countBCE2;
                int yz=V.countBCE3;
                int kecila=V.countABC4;
                int kecilb=V.countABD4;
                int kecilc=V.countBCC4;
                int kecild=V.countBCD4;
                int kecile=V.countAB4;
                int kecilf=V.countBC4;

                //VehiclePosition sekarang = V.getVehiclePosition(V.areaD);
                string waktu = V.currentDateTime();
                V.kecvint= V.kecv;
                //int percent = V.percentile();

                /*time_t now;
                struct tm *current;
                now = time(0);
                current = localtime(&now);
                int jams = current->tm_hour;
                int menits = current->tm_min;
                int detiks = current->tm_sec;*/

                        //ui->lcdNumber_->display(sekarang);
                        /*
                        ui->lcdNumber->display(aa);
                        ui->lcdNumber_2->display(bb);
                        ui->lcdNumber_3->display(hh);
                        ui->lcdNumber_4->display(seconds_since_start);
                        ui->lcdNumber_5->display(gg);
                        ui->lcdNumber_6->display(cc);
                        ui->lcdNumber_9->display(ff);
                        ui->lcdNumber_10->display(V.gap);
                        ui->lcdNumber_11->display(V.areaX);
                        ui->lcdNumber_12->display(V.areaB);
                        ui->lcdNumber_13->display(V.gap2);
                        ui->lcdNumber_14->display(V.areaA);
                        ui->lcdNumber_15->display(V.areaC);
                        ui->lcdNumber_16->display(70);
                        ui->lcdNumber_17->display(ii);
                        ui->lcdNumber_18->display(jj);
                        ui->lcdNumber_19->display(kk);
                        ui->lcdNumber_20->display(ll);
                        ui->lcdNumber_21->display(mm);
                        ui->lcdNumber_22->display(qq);
                        ui->lcdNumber_23->display(rr);
                        ui->lcdNumber_24->display(ss);
                        ui->lcdNumber_25->display(nn);
                        ui->lcdNumber_26->display(oo);
                        ui->lcdNumber_27->display(pp);
                        ui->lcdNumber_28->display(V.kec2);
                        ui->lcdNumber_29->display(V.jarak2);
                        ui->lcdNumber_30->display(V.waktu2);
                        ui->lcdNumber_31->display(V.jarak);
                        ui->lcdNumber_32->display(V.waktu);
                        ui->lcdNumber_33->display(V.kec);
                        ui->lcdNumber_34->display(V.kec3);
                        ui->lcdNumber_35->display(V.jarak3);
                        ui->lcdNumber_36->display(V.waktu3);
                        ui->lcdNumber_37->display(tt);
                        ui->lcdNumber_38->display(xx);
                        ui->lcdNumber_39->display(V.kecv2);
                        ui->lcdNumber_40->display(V.gap3);
                        ui->lcdNumber_41->display(zz);
                        ui->lcdNumber_42->display(V.jarakv);
                        ui->lcdNumber_43->display(V.gapv);
                        ui->lcdNumber_44->display(V.waktuv);
                        ui->lcdNumber_45->display(dd);
                        ui->lcdNumber_46->display(V.kecv);
                        ui->lcdNumber_47->display(yy);
                        ui->lcdNumber_48->display(V.kecv3);
                        ui->lcdNumber_49->display(yz);
                        ui->lcdNumber_50->display(vv);
                        ui->lcdNumber_51->display(V.jarakv);
                        ui->lcdNumber_52->display(ee);
                        ui->lcdNumber_53->display(ww);
                        ui->lcdNumber_54->display(V.jarakv);
                        ui->lcdNumber_55->display(V.waktuv3);
                        ui->lcdNumber_56->display(V.areaE);
                        ui->lcdNumber_57->display(V.gapv2);
                        ui->lcdNumber_58->display(V.waktuv2);
                        ui->lcdNumber_59->display(V.areaG);
                        ui->lcdNumber_60->display(uu);
                        ui->lcdNumber_61->display(V.areaP);
                        ui->lcdNumber_62->display(V.areaF);
                        ui->lcdNumber_63->display(V.gapv3);
                        ui->lcdNumber_64->display(V.avg3);
                        ui->lcdNumber_65->display(V.avg2);
                        ui->lcdNumber_66->display(V.avg);
                        ui->lcdNumber_67->display(V.avggap);
                        ui->lcdNumber_68->display(V.avggap2);
                        ui->lcdNumber_69->display(V.avggap3);
                        ui->lcdNumber_70->display(V.kecvint);
                        */
                ui->lcdNumber->display(aa);
                ui->lcdNumber_2->display(bb);
                ui->lcdNumber_3->display(hh);
                ui->lcdNumber_4->display(seconds_since_start);
                ui->lcdNumber_5->display(gg);
                ui->lcdNumber_6->display(cc);
                ui->lcdNumber_9->display(ff);
                ui->lcdNumber_10->display(V.gap);
                ui->lcdNumber_11->display(V.areaX);
                ui->lcdNumber_12->display(V.areaB);
                ui->lcdNumber_13->display(V.gap2);
                ui->lcdNumber_14->display(V.areaA);
                ui->lcdNumber_15->display(V.areaC);
                ui->lcdNumber_16->display(70);
                ui->lcdNumber_17->display(ii);
                ui->lcdNumber_18->display(jj);
                ui->lcdNumber_19->display(kk);
                ui->lcdNumber_20->display(ll);
                ui->lcdNumber_21->display(0);
                ui->lcdNumber_22->display(qq);
                ui->lcdNumber_23->display(rr);
                ui->lcdNumber_24->display(0);
                ui->lcdNumber_25->display(nn);
                ui->lcdNumber_26->display(oo);
                ui->lcdNumber_27->display(0);
                ui->lcdNumber_28->display(V.kec2);
                ui->lcdNumber_29->display(V.jarak2);
                ui->lcdNumber_30->display(V.waktu2);
                ui->lcdNumber_31->display(V.jarak);
                ui->lcdNumber_32->display(V.waktu);
                ui->lcdNumber_33->display(V.kec);
                ui->lcdNumber_34->display(0);
                ui->lcdNumber_35->display(0);
                ui->lcdNumber_36->display(0);
                ui->lcdNumber_37->display(tt);
                ui->lcdNumber_38->display(xx);
                ui->lcdNumber_39->display(V.kecv2);
                ui->lcdNumber_40->display(0);
                ui->lcdNumber_41->display(0);
                ui->lcdNumber_42->display(V.jarakv);
                ui->lcdNumber_43->display(V.gapv);
                ui->lcdNumber_44->display(V.waktuv);
                ui->lcdNumber_45->display(dd);
                ui->lcdNumber_46->display(V.kecv);
                ui->lcdNumber_47->display(yy);
                ui->lcdNumber_48->display(0);
                ui->lcdNumber_49->display(0);
                ui->lcdNumber_50->display(vv);
                ui->lcdNumber_51->display(0);
                ui->lcdNumber_52->display(ee);
                ui->lcdNumber_53->display(0);
                ui->lcdNumber_54->display(V.jarakv);
                ui->lcdNumber_55->display(0);
                ui->lcdNumber_56->display(V.areaE);
                ui->lcdNumber_57->display(V.gapv2);
                ui->lcdNumber_58->display(V.waktuv2);
                ui->lcdNumber_59->display(V.areaG);
                ui->lcdNumber_60->display(uu);
                ui->lcdNumber_61->display(V.areaP);
                ui->lcdNumber_62->display(V.areaF);
                ui->lcdNumber_63->display(0);
                ui->lcdNumber_64->display(0);
                ui->lcdNumber_65->display(V.avg2);
                ui->lcdNumber_66->display(V.avg);
                ui->lcdNumber_67->display(V.avggap);
                ui->lcdNumber_68->display(V.avggap2);
                ui->lcdNumber_69->display(0);
                ui->lcdNumber_70->display(V.kecvint);
                ui->lcdNumber_71->display(kecila);
                ui->lcdNumber_72->display(kecilb);
                ui->lcdNumber_73->display(0);
                ui->lcdNumber_74->display(kecilc);
                ui->lcdNumber_75->display(kecild);
                ui->lcdNumber_76->display(0);
                ui->lcdNumber_77->display(kecile);
                ui->lcdNumber_78->display(kecilf);
                        ui->label_42->setText(QString::fromStdString(waktu));
                }

                //cvReleaseCapture(&capture);


}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

