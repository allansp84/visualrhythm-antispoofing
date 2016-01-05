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

#include "video.h"
using namespace std;
using namespace cv;

Video::Video() {
    this->input_video.release();
    this->stop = false;
    this->delay = -1;
    this->frame_to_stop = -1;
    this->frame_processor = NULL;
    this->window_name_input = "";
    this->window_name_output = "";
}

Video::~Video() {}

bool Video::set_input_video(string filename) {
    input_video.release();
    return input_video.open(filename);
}

bool Video::set_output_video(const std::string &output_filename, int codec=0, double frame_rate=0.0,
  bool is_color=false) {

    this->output_filename = output_filename;

    if (codec == 0) {
        codec = get_codec();
    }

    if (frame_rate == 0.0) {
        frame_rate = get_frame_rate();
    }

    return output_video.open(this->output_filename, codec, frame_rate, this->get_frame_size(),
      is_color);
}

void Video::set_frame_processor(FrameProcessor* frame_processor_ptr) {
    frame_processor = frame_processor_ptr;
}

void Video::set_frame_to_stop(long frame_to_stop) {
    this->frame_to_stop = frame_to_stop;
}

void Video::set_delay(int delay) {
    this->delay = delay;
}

bool Video::set_position_frame_number(long frame_number) {
    return input_video.set(CV_CAP_PROP_POS_FRAMES, frame_number);
}

bool Video::set_position_millisecond(double ms) {
    return input_video.set(CV_CAP_PROP_POS_MSEC, ms);
}

long Video::get_frame_to_stop() const {
    return frame_to_stop;
}

long Video::get_position_frame_number() {
    return input_video.get(CV_CAP_PROP_POS_FRAMES);
}

long Video::get_position_millisecond() {
    return input_video.get(CV_CAP_PROP_POS_MSEC);
}

double Video::get_frame_rate() {
    return input_video.get(CV_CAP_PROP_FPS);
}

Size Video::get_frame_size() {

    int width = static_cast<int>(this->input_video.get(CV_CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(this->input_video.get(CV_CAP_PROP_FRAME_HEIGHT));

    return Size(width, height);
}

int Video::get_frame_height() {
    return input_video.get(CV_CAP_PROP_FRAME_HEIGHT);
}

int Video::get_frame_width() {
    return input_video.get(CV_CAP_PROP_FRAME_WIDTH);
}

long Video::get_total_frame_count() {
    return input_video.get(CV_CAP_PROP_FRAME_COUNT);
}

int Video::get_codec() {
    return static_cast<int>(input_video.get(CV_CAP_PROP_FOURCC));
}

void Video::display_input_video(string window_name_input) {
    this->window_name_input = window_name_input;
    namedWindow(this->window_name_input.c_str(), CV_WINDOW_AUTOSIZE);
}

void Video::display_output_video(string window_name_output) {
    this->window_name_output = window_name_output;
    namedWindow(this->window_name_output.c_str(), CV_WINDOW_AUTOSIZE);
}

void Video::dont_display_input_video() {
    destroyWindow(window_name_input.c_str());
    window_name_input.clear();
}

void Video::dont_display_output_video() {
    destroyWindow(window_name_output.c_str());
    window_name_output.clear();
}

void Video::run() {

    cv::Mat frame;
    cv::Mat output;

    if (!is_opened())
        return;

    stop = false;

    while (!is_stopped()) {

        if (!read_next_frame(frame))
            break;

        if (window_name_input.length() != 0)
            cv::imshow(window_name_input.c_str(), frame);

        frame_processor->process(frame, output);

        if (output_filename.length() != 0)
            write_next_frame(output);

        if (window_name_output.length() != 0)
            cv::imshow(window_name_output.c_str(), output);

        if (delay >= 0 && cv::waitKey(delay) >= 0)
            stop_it();

        if (frame_to_stop >= 0 && get_position_frame_number() == frame_to_stop)
            stop_it();
    }
}

void Video::stop_it() {
    stop = true;
}

bool Video::is_stopped() const {
    return stop;
}

bool Video::is_opened() {
    return input_video.isOpened();
}

bool Video::read_next_frame(cv::Mat& frame) {
    return input_video.read(frame);
}

void Video::write_next_frame(cv::Mat& frame) {
    output_video.write(frame);
}
