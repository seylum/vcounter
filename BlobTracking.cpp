#include "BlobTracking.h"

//konstruktor BlobTracking dengan 7 member initialization
BlobTracking::BlobTracking() : minArea(1), maxArea(2000), debugTrack(false), debugBlob(false), showBlobMask(true), showOutput(true)
{
    std::cout << "BlobTracking()" << std::endl;
}

//destruktor BlobTracking
BlobTracking::~BlobTracking()
{
    std::cout << "~BlobTracking()" << std::endl;
}

//untuk mengakses tracks (accessor for tracks)
const cvb::CvTracks BlobTracking::getTracks()
{
    return tracks;
}

//fungsi process

void BlobTracking::process(const cv::Mat &img_input, const cv::Mat &img_mask, cv::Mat &img_output)
{
    if(img_input.empty() || img_mask.empty())
        return;

    //mengisi frame dengan image original yang diambil dari video (menggunakan IPL library dari intel & dikonversi dari cv::mat)
    IplImage* frame = new IplImage(img_input);

    //mentransformasikan image yang sudah diambil secara linear berdasarkan nilai default-nya (scale = 1 dan shift = 0)
    cvConvertScale(frame, frame, 1, 0);

    //mengisi segmentated dengan image hasil background subtraction, nantinya image hasil background subtraction akan di"filter"/di"masking"
    IplImage* segmentated = new IplImage(img_mask);

    //membuat kernel sendiri (kernel 5x5 & berbentuk persegi) dengan cara membuat IplConvKernel dan mengopernya ke fungsi cvCreateStructuringElements
    //kernel ini berguna agar cvMorphologyEx dapat mengetahui ukuran dan bentuk dari structuring element yang digunakan
    IplConvKernel* morphKernel = cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,NULL);

    //setelah mendapatkan ukuran dan bentuk structuring element kita akan mentransformasikan image dengan menggunakan morphological operations
    //kita memilih morphological operations tipe open karena hendak memisahkan suatu objek dari objek lainnya (dilate --> erode)
    //kita memilih 5x5 agar terlihat lebih jelas perbedaannya
    cvMorphologyEx(segmentated,segmentated,NULL,morphKernel,CV_MOP_OPEN,1);

    if(showBlobMask)
        cvShowImage("Blob Mask",segmentated);

    //----------tahap untuk mendapatkan blobs----------//

    //mengalokasikan image dengan menggunakan cvCreateImage( size, depth, channels )
    IplImage* labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);
    cvb::CvBlobs blobs;
    unsigned int result = cvb::cvLabel(segmentated,labelImg,blobs);

    //memfilter blob agar tidak terlalu besar atau terlalu kecil
    //cvb::cvFilterByArea(blobs,500,1000000);
    cvb::cvFilterByArea(blobs,minArea,maxArea);

    //merender blob pada image original
    //cvb::cvRenderBlobs(labelImg,blobs,frame,frame,CV_BLOB_RENDER_BOUNDING_BOX);
    if(debugBlob)
        cvb::cvRenderBlobs(labelImg,blobs,frame,frame,CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_CENTROID|CV_BLOB_RENDER_ANGLE|CV_BLOB_RENDER_TO_STD);
    else
        cvb::cvRenderBlobs(labelImg,blobs,frame,frame,CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_CENTROID|CV_BLOB_RENDER_ANGLE);

    //----------tahap melakukan tracking-----------//

    //mengatur jarak minimal antar blob
    //buat jarak minimal sekecil mungkin agar blob yg berdekatan tetap dapat dibedakan
    cvb::cvUpdateTracks(blobs,tracks,1.0,5);

    if(debugTrack)
        cvb::cvRenderTracks(tracks,frame,frame,CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX|CV_TRACK_RENDER_TO_STD);
    else
        cvb::cvRenderTracks(tracks,frame,frame,CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);

    //mengcopy frame ke img_output
    cv::Mat img_result(frame);
    img_result.copyTo(img_output);

    if(showOutput)
        cvShowImage("Blob Tracking",frame);

    //membebaskan memori
    //cvReleaseImage(&frame);
    //cvReleaseImage(&segmentated);
    cvReleaseImage(&labelImg);
    delete frame;
    delete segmentated;
    cvReleaseBlobs(blobs);
    cvReleaseStructuringElement(&morphKernel);

}



