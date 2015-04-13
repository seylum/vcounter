#include "MixtureOfGaussianV2BGS.h"

MixtureOfGaussianV2BGS::MixtureOfGaussianV2BGS() : alpha(0.05), enableThreshold(true), threshold(15), showOutput(true)
{
    std::cout << "MixtureOfGaussianV2BGS()" << std::endl;
}

MixtureOfGaussianV2BGS::~MixtureOfGaussianV2BGS()
{
  std::cout << "~MixtureOfGaussianV2BGS()" << std::endl;
}

void MixtureOfGaussianV2BGS::process(const cv::Mat &img_input, cv::Mat &img_output)
{
  if(img_input.empty())
    return;

  mog(img_input, img_foreground, alpha);

  if(enableThreshold)
    cv::threshold(img_foreground, img_foreground, threshold, 255, cv::THRESH_BINARY);

  //if(showOutput)
    //cv::imshow("Gaussian Mixture Model (Zivkovic&Heijden)", img_foreground);

  img_foreground.copyTo(img_output);
}

