#include "SuBSENSE.h"
#include "BackgroundSubtractorSuBSENSE.h"

SuBSENSE::SuBSENSE() :
    pSubsense(0), firstTime(true), showOutput(true),
    fRelLBSPThreshold 			(BGSSUBSENSE_DEFAULT_LBSP_REL_SIMILARITY_THRESHOLD),
    nDescDistThresholdOffset 	(BGSSUBSENSE_DEFAULT_DESC_DIST_THRESHOLD_OFFSET),
    nMinColorDistThreshold 		(BGSSUBSENSE_DEFAULT_MIN_COLOR_DIST_THRESHOLD),
    nBGSamples 					(BGSSUBSENSE_DEFAULT_NB_BG_SAMPLES),
    nRequiredBGSamples 			(BGSSUBSENSE_DEFAULT_REQUIRED_NB_BG_SAMPLES),
    nSamplesForMovingAvgs 		(BGSSUBSENSE_DEFAULT_N_SAMPLES_FOR_MV_AVGS)
    {
    }

SuBSENSE::~SuBSENSE(){
    if (pSubsense)
        delete pSubsense;
}

void SuBSENSE::process(const cv::Mat &img_input, cv::Mat &img_output, cv::Mat &img_bgmodel)
{
  if(img_input.empty())
    return;



  if (firstTime) {

    pSubsense = new BackgroundSubtractorSuBSENSE(
            fRelLBSPThreshold, nDescDistThresholdOffset, nMinColorDistThreshold,
            nBGSamples, nRequiredBGSamples, nSamplesForMovingAvgs);

    pSubsense->initialize(img_input, cv::Mat (img_input.size(), CV_8UC1, cv::Scalar_<uchar>(255)));
    firstTime = false;
  }

  (*pSubsense)(img_input, img_output);
  pSubsense->getBackgroundImage(img_bgmodel);

  if(showOutput) {
      imshow("SuBSENSE FG", img_output);
      imshow("SuBSENSE BG", img_bgmodel);
  }
}
