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

    * Neither the name of University of Campinas (Unicamp) nor the names of its
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

#include "visualrhythm.h"
#include <algorithm>
#include <cmath>
using namespace cv;
using namespace std;

VisualRhythm::VisualRhythm() {
    this->visual_rhythm_type = 0;
    this->current_frame = 0;
    this->filter = 0;
    this->color_space = 0;
    this->kernel_size = 7;
    this->variance = 2.0;
    this->height = 1;
    this->width = 30;
}

VisualRhythm::~VisualRhythm() {}

void VisualRhythm::set_visual_rhythm(Mat ritmoVisual) {
    this->visual_rhythm = ritmoVisual;
}

void VisualRhythm::set_visual_rhythm_type(int visual_rhythm_type) {
    this->visual_rhythm_type = visual_rhythm_type;
}

void VisualRhythm::set_color_space(int color_space) {
    this->color_space = color_space;
}

void VisualRhythm::set_filter(int filter) {
    this->filter = filter;
}

void VisualRhythm::set_kernel_size(int kernel_size) {
    this->kernel_size = kernel_size;
}

void VisualRhythm::set_variance(float variance) {
    this->variance = variance;
}

void VisualRhythm::set_height(int height) {
    this->height = height;
}

void VisualRhythm::set_width(int width) {
    this->width = width;
}

void VisualRhythm::set_output_filename(string output_filename) {
    this->output_filename = output_filename;
}

int VisualRhythm::compute_dimensions_visual_rhythm(int rows, int cols){
    int j = 0, k = 0, i = 0, n = rows, m = cols, row = 0, step = this->width * 2;

        // upper triangular matrix
        for (i = 0; i < n; i = i + step) {

            j = 0;
            k = i;

            while (k >= 0 && j < m) {
                row++;
                k--;
                j++;
            }

        }

        i -= step;
        int d = ((n - 1) - i);

        // lower triangular matrix
        for (i = step - d; i < m; i = i + step) {

            j = i;
            k = n - 1;

            while (k >= 0 && j < m) {
                row++;
                k--;
                j++;
            }

        }

        return row;
}

void VisualRhythm::save_visual_rhythm() {
    imwrite(this->output_filename.c_str(), this->visual_rhythm);
}

void VisualRhythm::process(cv::Mat &frame, cv::Mat &output) {
    Mat image;
    Mat noise;
    Mat espectrum;
    Mat colorSpace;
    vector<Mat> bandas;

    if (this->color_space == 0) {

        cv::cvtColor(frame, image, CV_BGR2GRAY);

    } else if (this->color_space == 1){

        cv::cvtColor(frame, colorSpace, CV_BGR2Lab);
        cv::split(colorSpace, bandas);
        bandas[0].copyTo(image);

    } else{

        cout << "Erro:VisualRhythm::process():Invalid color space" << endl;
        exit(EXIT_FAILURE);

    }

    if (this->visual_rhythm_type == 0) {

        compute_noise_image(image, noise);
        compute_fourier_spectrum(noise, espectrum);
        compute_vertical_visual_rhythm(espectrum, output);
        this->current_frame++;

    } else if (this->visual_rhythm_type == 1) {

        compute_noise_image(image, noise);
        compute_fourier_spectrum(noise, espectrum);
        compute_horizontal_visual_rhythm(espectrum, output);

        this->current_frame++;
    } else if (this->visual_rhythm_type == 2) {

        compute_noise_image(image, noise);
        compute_fourier_spectrum(noise, espectrum);
        compute_zigzag_visual_rhythm(espectrum, output);
        this->current_frame++;

    } else {

        frame.copyTo(output);
        cout << "Error:VisualRhythm::process():Invalid visual rhyhtm type" << endl;

    }


}

void VisualRhythm::compute_noise_image(Mat &image, Mat &output) {
    Mat filtered;

    if (this->filter == 0) {

        cv::medianBlur(image, filtered, this->kernel_size);
        cv::subtract(image, filtered, output);

    } else if (this->filter == 1) {

        cv::GaussianBlur(image, filtered, cv::Size(this->kernel_size, this->kernel_size),
          this->variance);
        cv::subtract(image, filtered, output);

    } else {

        cout << "Error:VisualRhythm::compute_noise_image():Invalid filter type" << endl;

    }
}

void VisualRhythm::compute_fourier_spectrum(Mat &frame, Mat &output) {
    Mat padded;

    frame.copyTo(padded);

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexFrame;
    merge(planes, 2, complexFrame);

    dft(complexFrame, complexFrame);

    // planes[0] = Re(DFT(complexFrame), planes[1] = Im(DFT(complexFrame))
    split(complexFrame, planes);

    magnitude(planes[0], planes[1], planes[0]);
    Mat magFrame = planes[0];

    magFrame += Scalar::all(1);
    log(magFrame, magFrame);

    magFrame = magFrame(Rect(0, 0, magFrame.cols & -2, magFrame.rows & -2));
    int cx = magFrame.cols / 2;
    int cy = magFrame.rows / 2;

    Mat q0(magFrame, Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
    Mat q1(magFrame, Rect(cx, 0, cx, cy)); // Top-Right
    Mat q2(magFrame, Rect(0, cy, cx, cy)); // Bottom-Left
    Mat q3(magFrame, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magFrame, magFrame, 0, 255, CV_MINMAX);

    magFrame.copyTo(output);

    output = Mat::zeros(magFrame.rows, magFrame.cols, CV_8U);
    magFrame.convertTo(output, CV_8U);
}

void VisualRhythm::compute_vertical_visual_rhythm(Mat &frame, Mat &output) {
    Mat roi;

    roi = frame(
            Rect((frame.cols / 2) - (this->width / 2), 0, this->width, this->height));

    output = Mat::zeros(roi.rows, roi.cols, CV_8U);
    roi.convertTo(output, CV_8U);

    int y = 0, x = 0, x_dst = 0;

    x_dst = this->current_frame * this->width;

    for (y = 0; y < output.rows; y++) {
        for (x = 0; x < output.cols; x++) {
            this->visual_rhythm.at<uchar>(y, x_dst + x) = output.at<uchar>(y, x);
        }
    }
}

void VisualRhythm::compute_horizontal_visual_rhythm(Mat &frame, Mat &output) {
    Mat rot_mat, roi;
    int top, bottom, left, right;
    int borderType;

    int aux = (frame.cols - frame.rows) / 2;
    top = aux;
    bottom = aux;
    left = 0;
    right = 0;

    borderType = BORDER_CONSTANT;

    copyMakeBorder(frame, frame, top, bottom, left, right, borderType, Scalar(0));

    Point center = Point(frame.cols / 2, frame.rows / 2);
    double angle = 90.0;
    double scale = 1.;

    rot_mat = getRotationMatrix2D(center, angle, scale);

    warpAffine(frame, output, rot_mat, frame.size(), CV_INTER_LANCZOS4);

    roi = output(Rect((output.cols / 2) - (this->width / 2), 0, this->width, this->height));
    output = Mat::zeros(roi.rows, roi.cols, CV_8U);
    roi.convertTo(output, CV_8U);

    int y = 0, x = 0, x_dst = 0;

    x_dst = this->current_frame * this->width;

    for (y = 0; y < output.rows; y++) {
        for (x = 0; x < output.cols; x++) {
            this->visual_rhythm.at<uchar>(y, x_dst + x) = output.at<uchar>(y, x);
        }
    }
}

void VisualRhythm::compute_zigzag_visual_rhythm(Mat &frame, Mat &output) {

    Mat roi = Mat::zeros(this->height, this->width, CV_8U);

    int j = 0, k = 0, i = 0, n = frame.rows, m = frame.cols, row = 0, step = this->width * 2;

    // upper triangular matrix
    for (i = 0; i < n; i = i + step) {

        j = 0;
        k = i;

        while (k >= 0 && j < m) {
            for (int l = 0; l < this->width; l++) {
                if ((j + l) < (m + this->width)) {
                    roi.at<uchar>(row, l) = frame.at<uchar>(k, j + l);
                }
            }
            row++;
            k--;
            j++;
        }

    }

    i -= step;
    int d = ((n - 1) - i);

    // lower triangular matrix
    for (i = step - d; i < m; i = i + step) {

        j = i;
        k = n - 1;

        while (k >= 0 && j < m) {
            for (int l = 0; l < this->width; l++) {
                if ((j + l) < (m + this->width)) {
                    roi.at<uchar>(row, l) = frame.at<uchar>(k, j + l);
                }
            }
            row++;
            k--;
            j++;
        }

    }

    output = Mat::zeros(roi.rows, roi.cols, CV_8U);
    roi.convertTo(output, CV_8U);

    int y = 0, x = 0, x_dst = 0;

    x_dst = this->current_frame * this->width;

    for (y = 0; y < output.rows; y++) {
        for (x = 0; x < output.cols; x++) {
            this->visual_rhythm.at<uchar>(y, x_dst + x) = output.at<uchar>(y, x);
        }
    }

}

