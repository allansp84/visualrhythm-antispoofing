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

// Interface whose one method is used as callback function for process the frames
#include "FrameProcessor.h"

using namespace std;
using namespace cv;

// Class liable for controlling the video processing, and that contain functions of reading, 
// processing and writing a video
class Video {

private:

    // the OpenCV video capture object
    cv::VideoCapture capture;

    // the callback function to be called for the processing of each frame
    void (*processo)(cv::Mat&, cv::Mat&);

    // the pointer to the class implementing the FrameProcessor interface
    FrameProcessor *frameProcessor;

    // a bool to determine if the process callback will be called
    bool callIt;

    // Input display window name
    std::string windowNameInput;

    // Output display window name
    std::string windowNameOutput;

    // delay between each frame processing
    int delay;

    // number of processed frames 
    long fnumber;

    // stop at this frame number
    long frameToStop;

    // to stop the processing
    bool stop;

    // the OpenCV video writer object
    cv::VideoWriter writer;

    // output filename
    std::string outputFile;

public:

    // constructor
    Video();

    // destructor
    ~Video();

    // set the name of the video file
    bool setInput(string filename);

    // set the output video file
    // by default the same parameters than input video will be used
    bool setOutput(const std::string &filename, int codec, double framerate, 
        cv::Size size, bool isColor);

    // set the callback function that will be called for each frame
    void setFrameProcessor(void(*frameProcessingCallback)(cv::Mat&, cv::Mat&));

    // set the instance of the class that implements the FrameProcessor interface
    void setFrameProcessor(FrameProcessor* frameProcessorPtr);

    // stop streaming at this frame number
    void stopAtFrameNo(long frame);

    // process callback to be called
    void callProcess();

    // do not call process callback
    void dontCallProcess();

    // to display the processed frames
    void displayInput(std::string wn);

    // to display the processed frames
    void displayOutput(std::string wn);

    // do not display the processed frames
    void dontDisplay();

    // set a delay between each frame
    // 0 means wait at each frame
    // negative means no delay
    void setDelay(int delay);

    // a count is kept of the processed frames
    long getNumberOfProcessedFrames();

    // return the size of the video frame
    Size getFrameSize();

    // return the frame number of the next frame
    long getFrameNumber();

    // return the frame rate
    double getFrameRate();

    // return the number of frames in video
    long getTotalFrameCount();

    // get the codec of input video
    int getCodec(char codec[4]);

    // Stop the processing
    void stopIt();

    // Is the process stopped?
    bool isStopped() const;

    // Is a capture device opened?
    bool isOpened();

    // Sets the last frame number to be processed
    void setFrameToStop(long frameToStop);

    // Gets the last frame number to be processed
    long getFrameToStop() const;

    // Save the first frame
    void saveFirstFrame(string fileImage);

    // to get the next frame could be: video file; camera; vector of images
    bool readNextFrame(Mat &frame);

    // to write the output frame could be: video file or images
    void writeNextFrame(Mat &frame);

    // to grab (and process) the frames of the sequence
    void run();

};

#endif /* VIDEO_H_ */
