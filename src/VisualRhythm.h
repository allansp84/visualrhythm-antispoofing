/*------------------------------------------------------------------------------------------------*\
    Copyright (c) 2015, Allan Pinto, William Robson Schwartz, Helio Pedrini, and Anderson Rocha
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of visualrhythm-antispoofing nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\*------------------------------------------------------------------------------------------------*/

#ifndef FEATURES_H_
#define FEATURES_H_

// Contains algorithms of basic image processing and algorithms of high-level computer vision
#include <opencv2/opencv.hpp>

// Contains the basic data structures, drawing functions and XML support
#include <opencv2/core/core.hpp>

// Contains functions of I/O and functions of video and image manipulation
#include <opencv2/highgui/highgui.hpp>

// Contains functions related to processing and image analysis tasks.
#include <opencv2/imgproc/imgproc_c.h>

// Contains functions related to pattern recognition task.
#include <opencv2/ml/ml.hpp>

// Contains functions related to video processing task.
#include <opencv2/video/video.hpp>

// Contains functions to control input and output stream
#include <iostream>
#include <fstream>

// Interface whose one method is used as callback function for process the frames
#include "FrameProcessor.h"

// Constants
#define SUCESSO "SUCESSO"
#define FALHA "FALHA"
#define NUMERO_PONTOS 15

using namespace std;
using namespace cv;

// Class liable for compute the visual rhythm of a input video
class VisualRhythm: public FrameProcessor {
private:

    // Visual rhythm calculated
    Mat visual_rhythm;

    // Type of visual rhythm to be computed
    int visual_rhythm_type;

    // Height of the visual rhythm
    int height;

    // Width of the visual rhythm
    int width;

    // Color space of the frame before to extract the noise
    string color_space;

    // Number of the current frame
    int current_frame;

    // Filter used in the filtering of the frame
    string filter;

    // Kernel size used in the convolution operation to filtering of the frame
    int kernel_size;

    // Variance value used during gaussian filtering
    float variance;

    // Output file name of the visual rhythm computed
    string output_filename;

public:

    // Constructor
    VisualRhythm();

    // Destructor
    virtual ~VisualRhythm();

    // Sets type of the visual rhythm to be computed
    void setVisualRhythmType(int visual_rhythm_type);

    // Sets the type of the filter used to compute the residual noise images
    void setFilter(string filter);

    // Sets the variance value used in the gaussian filter
    void setVariance(float variance);

    // Sets color space of the frames before to extract the noise
    void setColorSpace(string color_space);

    // Sets the height of the visual rhythm
    void setHeight(int height);

    // Sets the width of the visual rhythm
    void setWidth(int width);

    // Sets output file name where the visual rhythm computed will be saved.
    void setOutputFileName(string output_filename);

    // Creates a matrix used to store the computed visual rhythm
    void createVisualRhythm(Mat visual_rhythm);

    // Calculates the dimensions of the visual rhythm to be computed.
    int estimateDimensionsOfVisualRhythm(int rows, int cols);

    // Process the video frames
    void process(cv::Mat &frame, cv::Mat &output);

    // Computes the noise image of a frame
    void computeNoiseImage(Mat &gray, Mat &output);

    // Shows the noise image
    void showNoiseImage(Mat &gray, Mat &output);

    // Saves the calculated noise image
    void saveNoiseImage(string filenameInput, string filenameOutput);

    // Computes the fourier spectrum of a noise image
    void computeFourierSpectrum(Mat &frame, Mat &output);

    // Saves the calculated fourier spectrum
    void saveFourierSpectrum(string filenameInput, string filenameOutput);

    // Computes the vertical visual rhythm
    void computeVerticalVisualRhythm(Mat &frame, Mat &output);

    // Computes the horizontal visual rhythm
    void computeHorizontalVisualRhythm(Mat &frame, Mat &output);

    // Computes the zigzag visual rhythm
    void computeZigZagVisualRhythm(Mat &frame, Mat &output);

    // Saves the computed visual rhythm
    void saveVisualRhythm();

};

#endif /* FEATURES_H_ */
