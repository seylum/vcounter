#ifndef SUBSENSE_H
#define SUBSENSE_H

#include <opencv2/opencv.hpp>

class BackgroundSubtractorSuBSENSE;

class SuBSENSE
{
private:
    BackgroundSubtractorSuBSENSE* pSubsense;
    bool firstTime;
    bool showOutput;

    float fRelLBSPThreshold;
    size_t nDescDistThresholdOffset;
    size_t nMinColorDistThreshold;
    size_t nBGSamples;
    size_t nRequiredBGSamples;
    size_t nSamplesForMovingAvgs;

public:
    SuBSENSE();
    ~SuBSENSE();

    void process(const cv::Mat &img_input, cv::Mat &img_output,
                cv::Mat &img_bgmodel);

};

#endif // SUBSENSE_H
