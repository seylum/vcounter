#include "SigmaDeltaBGS.h"

SigmaDeltaBGS::SigmaDeltaBGS() :
firstTime(true),
ampFactor(1),
minVar(15),
maxVar(255),
algorithm(sdLaMa091New()),
showOutput(true) {

  applyParams();
  std::cout << "SigmaDeltaBGS()" << std::endl;
}

SigmaDeltaBGS::~SigmaDeltaBGS() {
  sdLaMa091Free(algorithm);
  std::cout << "~SigmaDeltaBGS()" << std::endl;
}

void SigmaDeltaBGS::process(
  const cv::Mat &img_input,
  cv::Mat &img_output,
  cv::Mat &img_bgmodel
  ) {
  if (img_input.empty())
    return;

  if (firstTime) {
    sdLaMa091AllocInit_8u_C3R(algorithm, img_input.data, img_input.cols, img_input.rows, img_input.step);
    firstTime = false;
    return;
  }

  img_output = cv::Mat(img_input.rows, img_input.cols, CV_8UC1);
  cv::Mat img_output_tmp(img_input.rows, img_input.cols, CV_8UC3);

  sdLaMa091Update_8u_C3R(algorithm, img_input.data, img_output_tmp.data);

  unsigned char* tmpBuffer = (unsigned char*)img_output_tmp.data;
  unsigned char* outBuffer = (unsigned char*)img_output.data;

  for (size_t i = 0; i < img_output.total(); ++i) {
    *outBuffer = *tmpBuffer;

    ++outBuffer;
    tmpBuffer += img_output_tmp.channels();
  }

  //memperhalus image (smoothing)
    cv::medianBlur(img_output, img_output, 7);

  if (showOutput)
    cv::imshow("Sigma-Delta", img_output);
}

void SigmaDeltaBGS::applyParams() {
  sdLaMa091SetAmplificationFactor(algorithm, ampFactor);
  sdLaMa091SetMinimalVariance(algorithm, minVar);
  sdLaMa091SetMaximalVariance(algorithm, maxVar);
}
