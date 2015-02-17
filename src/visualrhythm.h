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

// It contains algorithms of basic image processing and algorithms of high-level computer vision
#include <opencv2/opencv.hpp>

// It contains the basic data structures, drawing functions and XML support
#include <opencv2/core/core.hpp>

// It contains functions of I/O and functions of video and image manipulation
#include <opencv2/highgui/highgui.hpp>

// It contains functions related to processing and image analysis tasks.
#include <opencv2/imgproc/imgproc_c.h>

// It contains functions related to pattern recognition task.
#include <opencv2/ml/ml.hpp>

// It contains functions related to video processing task.
#include <opencv2/video/video.hpp>

// It contains functions to control input and output stream
#include <iostream>
#include <fstream>

// Interface whose one method is used as callback function for process the frames
#include "frameprocessor.h"

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
    int color_space;

    // Number of the current frame
    int current_frame;

    // Filter used in the filtering of the frame
    int filter;

    // Kernel size used in the convolution operation to filtering of the frame
    int kernel_size;

    // Variance value used during gaussian filtering
    float variance;

    // Output file name of the visual rhythm computed
    string output_filename;

    // Process the video frames
    void process(cv::Mat &frame, cv::Mat &output);

    // To compute the noise image of a frame
    void compute_noise_image(Mat &gray, Mat &output);

    // To compute the fourier spectrum of a noise image
    void compute_fourier_spectrum(Mat &frame, Mat &output);

    // To compute the vertical visual rhythm
    void compute_vertical_visual_rhythm(Mat &frame, Mat &output);

    // To compute the horizontal visual rhythm
    void compute_horizontal_visual_rhythm(Mat &frame, Mat &output);

    // To compute the zigzag visual rhythm
    void compute_zigzag_visual_rhythm(Mat &frame, Mat &output);

public:

    // Constructor
    VisualRhythm();

    // Destructor
    ~VisualRhythm();

    // To create a matrix used to store the computed visual rhythm
    void set_visual_rhythm(Mat visual_rhythm);

    // To set type of the visual rhythm to be computed
    void set_visual_rhythm_type(int visual_rhythm_type);

    // To set color space of the frames before to extract the noise
    void set_color_space(int color_space);

    // To set the type of the filter used to compute the residual noise images
    void set_filter(int filter);

    // To set the variance value used in the gaussian filter
    void set_variance(float variance);

    // To set the height of the visual rhythm
    void set_height(int height);

    // To set the width of the visual rhythm
    void set_width(int width);

    // To set output file name where the visual rhythm computed will be saved.
    void set_output_filename(string output_filename);

    // To calculate the dimensions of the visual rhythm to be computed.
    int compute_dimensions_visual_rhythm(int rows, int cols);

    // To save the computed visual rhythm
    void save_visual_rhythm();

};

#endif /* FEATURES_H_ */
