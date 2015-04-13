#ifndef SIGMADELTABGS_H
#define SIGMADELTABGS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "sdLaMa091.h"

class SigmaDeltaBGS
{
private:

  bool firstTime;
  unsigned int ampFactor;
  unsigned int minVar;
  unsigned int maxVar;
  sdLaMa091_t* algorithm;
  bool showOutput;

public:

  SigmaDeltaBGS();

  ~SigmaDeltaBGS();

  void process(
    const cv::Mat &img_input,
    cv::Mat &img_output,
    cv::Mat &img_bgmodel
    );

private:
  void applyParams();
};

#endif // SIGMADELTABGS_H
