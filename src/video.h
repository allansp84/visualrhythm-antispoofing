/*------------------------------------------------------------------------------------------------*\
    This file contains material supporting chapter 10 of the cookbook:  
    Computer Vision Programming using the OpenCV Library. 
    by Robert Laganiere, Packt Publishing, 2011.

    This program is free software; permission is hereby granted to use, copy, modify, 
    and distribute this source code, or portions thereof, for any purpose, without fee, 
    subject to the restriction that the copyright notice may not be removed 
    or altered from any source or altered source distribution. 
    The software is released on an as-is basis and without any warranties of any kind. 
    In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
    The author disclaims all warranties with regard to this software, any use, 
    and any consequent failure, is purely the responsibility of the user.
 
    Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------------*/


#ifndef VIDEO_H_
#define VIDEO_H_

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

// Interface whose one method is used as callback function for process the frames
#include "frameprocessor.h"

#define SUPPORTED_CV_MAJOR_VERSION 2
#define SUPPORTED_CV_MINOR_VERSION 4
#define SUPPORTED_CV_SUBMINOR_VERSION 8


using namespace std;
using namespace cv;

// Class liable for controlling the video processing, and that contain functions of reading, 
// processing and writing a video
class Video {

private:

    // The OpenCV video capture object
    cv::VideoCapture input_video;

    // The OpenCV video writer object
    cv::VideoWriter output_video;

    // The pointer to the class implementing the FrameProcessor interface
    FrameProcessor *frame_processor;

    // To stop the processing
    bool stop;

    // Delay between each frame processing
    int delay;

    // To stop at this frame number
    long frame_to_stop;

    // Input display window name
    std::string window_name_input;

    // Output display window name
    std::string window_name_output;

    // Output filename
    std::string output_filename;

    // To stop the processing
    void stop_it();

    // Is the process stopped?
    bool is_stopped() const;

    // Is a capture device opened?
    bool is_opened();

    // To get the next frame from input video
    bool read_next_frame(Mat &frame);

    // To write the output frame into output video
    void write_next_frame(Mat &frame);

public:

    // Constructor
    Video();

    // Destructor
    ~Video();

    // To set the name of the video file
    bool set_input_video(string filename);

    // To set the output video file by default the same parameters than input video will be used
    bool set_output_video(const std::string &output_filename, int codec, double frame_rate,
      bool is_color);

    // To set the instance of the class that implements the FrameProcessor interface
    void set_frame_processor(FrameProcessor* frame_processor_ptr);

    // To set the last frame number to be processed
    void set_frame_to_stop(long frame_to_stop);

    // To set a delay between each frame
    // 0 means wait at each frame and negative means no delay
    void set_delay(int delay);

    // To go to this frame number
    bool set_position_frame_number(long frame_number);

    // To go to this position
    bool set_position_millisecond(double ms);

    // To get the last frame number to be processed
    long get_frame_to_stop() const;

    // To get the current position in frame number of the video
    long get_position_frame_number();

    // To get the current position in milliseconds of the video
    long get_position_millisecond();

    // To get the frame rate
    double get_frame_rate();

    // To get the size of the video frame
    Size get_frame_size();

    // To get the height of the video frame
    int get_frame_height();

    // To get the width of the video frame
    int get_frame_width();

    // To get the number of frames in video
    long get_total_frame_count();

    // To get the codec of input video
    int get_codec();

    // To display the processed frames
    void display_input_video(std::string window_name_input);

    // To display the processed frames
    void display_output_video(std::string window_name_output);

    // Do not display the frames from input video
    void dont_display_input_video();

    // Do not display the processed frames
    void dont_display_output_video();

    // To grab (and process) the frames of the input video
    void run();

};

#endif /* VIDEO_H_ */
