#ifndef BLOBTRACKING_H
#define BLOBTRACKING_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "cvblob.h"

//membuat kelas BlobTracking

class BlobTracking
{
private:
    int minArea;
    int maxArea;

    bool debugTrack;
    bool debugBlob;
    bool showBlobMask;
    bool showOutput;

    //instansiasi dari kelas tracks yang ada pada CvBlob
    cvb::CvTracks tracks;

public:

    //pendeklarasian konstruktor BlobTracking()
    BlobTracking();

    //pedeklarasian destruktor BlobTracking()
    ~BlobTracking();

    //pendeklarasian fungsi process()
    void process(const cv::Mat &img_input, const cv::Mat &img_mask, cv::Mat &img_output);

    //pendeklarasian fungsi getTracks()
    const cvb::CvTracks getTracks();
};

#endif // BLOBTRACKING_H
