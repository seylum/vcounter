#include "PixelBased.h"

PixelBased::PixelBased() : showOutput(true), enableInputBlur(true), enableOutputBlur(true),
alpha(7.0), beta(1.0), N(20), Raute_min(2), R_incdec(0.05), R_lower(18),
R_scale(5), T_dec(0.05), T_inc(1), T_init(18), T_lower(2), T_upper(200)
{
  std::cout << "PixelBased()" << std::endl;
}

PixelBased::~PixelBased()
{
    std::cout << "~PixelBased()" << std::endl;
}

void PixelBased::process(const cv::Mat &img_input, cv::Mat &img_output)
{
  if(img_input.empty())
    return;

  cv::Mat img_input_new;
  if(enableInputBlur)
    cv::GaussianBlur(img_input, img_input_new, cv::Size(5,5), 1.5);
  else
    img_input.copyTo(img_input_new);

  cv::Mat img_foreground;
  pbas.process(&img_input_new, &img_foreground);

  //memperhalus image (smoothing)
  if(enableOutputBlur)
    cv::medianBlur(img_foreground, img_foreground, 5);

  if(showOutput)
    cv::imshow("PBAS", img_foreground);

  img_foreground.copyTo(img_output);

}


