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

#include "Video.h"
using namespace std;
using namespace cv;

Video::Video() {
    this->stop = false;
    this->callIt = false;
    this->delay = 0;
    this->fnumber = 0;
    this->frameToStop = -1;
    this->frameProcessor = 0;
    this->processo = 0;
}

Video::~Video() {
}

bool Video::setInput(string filename) {
    fnumber = 0;
    capture.release();
    return capture.open(filename);
}

bool Video::setOutput(const std::string &filename, int codec, double framerate,
        cv::Size size, bool isColor) {

    outputFile = filename;

    char c[4];

    // use same codec as input
    if (codec == -1) {
        codec = getCodec(c);
    }

    // Abre o vídeo de saída
    return writer.open(outputFile, codec, framerate, size, isColor);
}

void Video::setFrameProcessor(
        void (*frameProcessingCallback)(cv::Mat&, cv::Mat&)) {

    frameProcessor = 0;

    processo = frameProcessingCallback;
    callProcess();
}

void Video::setFrameProcessor(FrameProcessor* frameProcessorPtr) {

    processo = 0;

    frameProcessor = frameProcessorPtr;
    callProcess();
}

void Video::stopAtFrameNo(long frame) {
    this->frameToStop = frame;
}

void Video::callProcess() {
    this->callIt = true;
}

void Video::dontCallProcess() {
    this->callIt = false;
}

void Video::displayInput(string wn) {
    windowNameInput = wn;
    namedWindow(windowNameInput, CV_WINDOW_AUTOSIZE);
}

void Video::displayOutput(string wn) {
    windowNameOutput = wn;
    namedWindow(windowNameOutput, CV_WINDOW_AUTOSIZE);
}

void Video::dontDisplay() {
    destroyWindow(windowNameInput);
    destroyWindow(windowNameOutput);
    windowNameInput.clear();
    windowNameOutput.clear();
}

void Video::setDelay(int delay) {
    this->delay = delay;
}

long Video::getNumberOfProcessedFrames() {
    return fnumber;
}

Size Video::getFrameSize() {

    return cv::Size((int) capture.get(CV_CAP_PROP_FRAME_WIDTH),
            (int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));
}

long Video::getFrameNumber() {
    long f = capture.get(CV_CAP_PROP_POS_FRAMES);
    return f;
}

double Video::getFrameRate() {
    double r = capture.get(CV_CAP_PROP_FPS);
    return r;
}

long Video::getTotalFrameCount() {
    long t = capture.get(CV_CAP_PROP_FRAME_COUNT);
    return t;
}

int Video::getCodec(char codec[4]) {

    union {
        int value;
        char code[4];
    } returned;

    returned.value = static_cast<int>(capture.get(CV_CAP_PROP_FOURCC));

    codec[0] = returned.code[0];
    codec[1] = returned.code[1];
    codec[2] = returned.code[2];
    codec[3] = returned.code[3];

    return returned.value;
}

void Video::stopIt() {
    this->stop = true;
}

bool Video::isStopped() const {
    return stop;
}

bool Video::isOpened() {
    return capture.isOpened();
}

long Video::getFrameToStop() const {
    return frameToStop;
}

void Video::setFrameToStop(long frameToStop) {
    this->frameToStop = frameToStop;
}

void Video::saveFirstFrame(std::string fileImage) {
    Mat frame;
    if (!isOpened()) {
        cout << "Não foi possível abrir o arquivo de vídeo" << endl;
        return;
    }
    if (!readNextFrame(frame)) {
        cout << "Erro: readNextFrame" << endl;
        return;
    }

    imwrite(fileImage, frame);

    capture.release();
    frame.release();
}

bool Video::readNextFrame(cv::Mat& frame) {
    return capture.read(frame);
}

void Video::writeNextFrame(cv::Mat& frame) {
    writer.write(frame);
}

void Video::run() {

    cv::Mat frame;
    cv::Mat output;

    if (!isOpened())
        return;

    stop = false;

    while (!isStopped()) {

        if (!readNextFrame(frame))
            break;

        if (windowNameInput.length() != 0)
            cv::imshow(windowNameInput, frame);

        if (callIt) {
            if (processo)
                processo(frame, output);
            else if (frameProcessor)
                frameProcessor->process(frame, output);

        } else {
            output = frame;
        }

        if (outputFile.length() != 0)
            writeNextFrame(output);

        if (windowNameOutput.length() != 0)
            cv::imshow(windowNameOutput, output);

        if (delay >= 0 && cv::waitKey(delay) >= 0)
            stopIt();

        if (frameToStop >= 0 && getFrameNumber() == frameToStop)
            stopIt();
    }
}
