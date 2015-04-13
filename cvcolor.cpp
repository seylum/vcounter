#include <cmath>
#include <iostream>
#include <cv.h>
#include "cvblob.h"

using namespace std;

namespace cvb {

CvScalar cvBlobMeanColor(CvBlob const *blob, IplImage const *imgLabel, IplImage const *img)
{
  CV_FUNCNAME("cvBlobMeanColor");
  __CV_BEGIN__;
  {
    CV_ASSERT(imgLabel&&(imgLabel->depth==IPL_DEPTH_LABEL)&&(imgLabel->nChannels==1));
    CV_ASSERT(img&&(img->depth==IPL_DEPTH_8U)&&(img->nChannels==3));

    int stepLbl = imgLabel->widthStep/(imgLabel->depth/8);
    int stepImg = img->widthStep/(img->depth/8);
    int imgLabel_width = imgLabel->width;
    int imgLabel_height = imgLabel->height;
    int imgLabel_offset = 0;
    int img_width = img->width;
    int img_height = img->height;
    int img_offset = 0;
    if(imgLabel->roi)
    {
  imgLabel_width = imgLabel->roi->width;
  imgLabel_height = imgLabel->roi->height;
  imgLabel_offset = (imgLabel->nChannels * imgLabel->roi->xOffset) + (imgLabel->roi->yOffset * stepLbl);
    }
    if(img->roi)
    {
  img_width = img->roi->width;
  img_height = img->roi->height;
  img_offset = (img->nChannels * img->roi->xOffset) + (img->roi->yOffset * stepImg);
    }

    CvLabel *labels = (CvLabel *)imgLabel->imageData + imgLabel_offset;
    unsigned char *imgData = (unsigned char *)img->imageData + img_offset;

    double mb = 0;
    double mg = 0;
    double mr = 0;
    double pixels = (double)blob->area;

    for (unsigned int r=0; r<(unsigned int)imgLabel_height; r++, labels+=stepLbl, imgData+=stepImg)
  for (unsigned int c=0; c<(unsigned int)imgLabel_width; c++)
  {
    if (labels[c]==blob->label)
    {
      mb += ((double)imgData[img->nChannels*c+0])/pixels; // B
      mg += ((double)imgData[img->nChannels*c+1])/pixels; // G
      mr += ((double)imgData[img->nChannels*c+2])/pixels; // R
    }
  }

    /*double mb = 0;
    double mg = 0;
    double mr = 0;
    double pixels = (double)blob->area;
    for (unsigned int y=0; y<imgLabel->height; y++)
  for (unsigned int x=0; x<imgLabel->width; x++)
  {
    if (cvGetLabel(imgLabel, x, y)==blob->label)
    {
      CvScalar color = cvGet2D(img, y, x);
      mb += color.val[0]/pixels;
      mg += color.val[1]/pixels;
      mr += color.val[2]/pixels;
    }
  }*/

    return cvScalar(mr, mg, mb);
  }
  __CV_END__;
}

}

