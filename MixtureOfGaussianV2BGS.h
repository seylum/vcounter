#ifndef MIXTUREOFGAUSSIANV2BGS_H
#define MIXTUREOFGAUSSIANV2BGS_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/video/background_segm.hpp>

class MixtureOfGaussianV2BGS
{
private:
  cv::BackgroundSubtractorMOG2 mog;
  cv::Mat img_foreground;
  double alpha;
  bool enableThreshold;
  int threshold;
  bool showOutput;

public:
    MixtureOfGaussianV2BGS();
    ~MixtureOfGaussianV2BGS();

    void process(const cv::Mat &img_input, cv::Mat &img_output);
};

#endif // MIXTUREOFGAUSSIANV2BGS_H
