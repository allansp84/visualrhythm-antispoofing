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

#include "VisualRhythm.h"
#include <algorithm>
#include <cmath>
using namespace cv;
using namespace std;

VisualRhythm::VisualRhythm() {
    this->visual_rhythm_type = 0;
    this->current_frame = 0;
    this->filter = "gauss";
    this->kernel_size = 3;
    this->variance = 1.0;
    this->height = 1;
    this->width = 1;
}

VisualRhythm::~VisualRhythm() {
}

void VisualRhythm::setVisualRhythmType(int visual_rhythm_type) {
    this->visual_rhythm_type = visual_rhythm_type;
}

void VisualRhythm::setFilter(string filter) {
    if (!filter.empty()) {
        std::transform(filter.begin(), filter.end(), filter.begin(), ::tolower);
        this->filter = filter;
    } else {
        cout << "Erro:VisualRhythm::setFilter():-" << endl;
        exit(EXIT_FAILURE);
    }
}

void VisualRhythm::setVariance(float variance) {
    this->variance = variance;
}

void VisualRhythm::setColorSpace(string color_space) {
    this->color_space = color_space;
}

void VisualRhythm::setHeight(int height) {
    this->height = height;
}

void VisualRhythm::setWidth(int width) {
    this->width = width;
}

void VisualRhythm::setOutputFileName(string output_filename) {
    this->output_filename = output_filename;
}

void VisualRhythm::createVisualRhythm(Mat ritmoVisual) {
    this->visual_rhythm = ritmoVisual;
}

int VisualRhythm::estimateDimensionsOfVisualRhythm(int rows, int cols){
    int j = 0, k = 0, i = 0, n = rows, m = cols, step = 60,
                row = 0;

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

void VisualRhythm::process(cv::Mat &frame, cv::Mat &output) {
    Mat image;
    Mat noise;
    Mat espectrum;
    Mat colorSpace;
    vector<Mat> bandas;

    if (this->color_space.compare("gray") == 0) {
        cv::cvtColor(frame, image, CV_BGR2GRAY);
    } else if (this->color_space.compare("lab") == 0){
        cv::cvtColor(frame, colorSpace, CV_BGR2Lab);
        cv::split(colorSpace, bandas);
        bandas[0].copyTo(image);
    } else{
        cout << "Erro:VisualRhythm::process():Invalid color space" << endl;
        exit(EXIT_FAILURE);
    }

    if (this->visual_rhythm_type == 0) {
        computeNoiseImage(image, noise);
        computeFourierSpectrum(noise, espectrum);
        computeVerticalVisualRhythm(espectrum, output);
        this->current_frame++;

    } else if (this->visual_rhythm_type == 1) {
        computeNoiseImage(image, noise);
        computeFourierSpectrum(noise, espectrum);
        computeHorizontalVisualRhythm(espectrum, output);

        this->current_frame++;
    } else if (this->visual_rhythm_type == 2) {
        computeNoiseImage(image, noise);
        computeFourierSpectrum(noise, espectrum);
        computeZigZagVisualRhythm(espectrum, output);
        this->current_frame++;

    } else {
        frame.copyTo(output);
        cout << "Error:VisualRhythm::process():Invalid visual rhyhtm type" << endl;
    }

}

void VisualRhythm::computeNoiseImage(Mat &image, Mat &output) {
    Mat filtered;
    int dimension = kernel_size * 2 + 1;

    if (this->filter.compare("median") == 0) {
        cv::medianBlur(image, filtered, dimension);
        cv::subtract(image, filtered, output);

    } else if (this->filter.compare("gauss") == 0) {
        cv::GaussianBlur(image, filtered, cv::Size(dimension, dimension), this->variance);
        cv::subtract(image, filtered, output);

    } else {
        cout << "Error:VisualRhythm::computeNoiseImage():Invalid filter type" << endl;
    }
}

void VisualRhythm::showNoiseImage(Mat &image, Mat &output) {

    computeNoiseImage(image, output);

    for (int row = 0; row < output.rows; row++) {
        for (int col = 0; col < output.cols; col++) {
            output.at<uchar>(row, col) = 255 - (int) output.at<uchar>(row, col);
        }
    }
}

void VisualRhythm::saveNoiseImage(string filenameInput, string filenameOutput) {

    Mat src = imread(filenameInput.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    Mat dst;

    computeNoiseImage(src, dst);
    for (int row = 0; row < dst.rows; row++) {
        for (int col = 0; col < dst.cols; col++) {
            dst.at<uchar>(row, col) = 255 - (int) dst.at<uchar>(row, col);
        }
    }

    imwrite(filenameOutput.c_str(), dst);
}


void VisualRhythm::computeFourierSpectrum(Mat &frame, Mat &output) {
    Mat padded;

    frame.copyTo(padded);

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexFrame;
    merge(planes, 2, complexFrame);

    dft(complexFrame, complexFrame);

    /* planes[0] = Re(DFT(complexFrame), planes[1] = Im(DFT(complexFrame)) */
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

void VisualRhythm::saveFourierSpectrum(string filenameInput,
        string filenameOutput) {

    Mat src = imread(filenameInput.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    Mat noise, dst;

    computeNoiseImage(src, noise);
    computeFourierSpectrum(noise, dst);

    imwrite(filenameOutput.c_str(), dst);
}

void VisualRhythm::computeVerticalVisualRhythm(Mat &frame, Mat &output) {
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

void VisualRhythm::computeHorizontalVisualRhythm(Mat &frame, Mat &output) {
    Mat rot_mat, roi;
    int top, bottom, left, right;
    int borderType;

    int aux = (frame.cols - frame.rows) / 2;
    top = aux;
    bottom = aux;
    left = 0;
    right = 0;

    borderType = BORDER_CONSTANT;

    copyMakeBorder(frame, frame, top, bottom, left, right, borderType,
            Scalar(0));

    Point center = Point(frame.cols / 2, frame.rows / 2);
    double angle = 90.0;
    double scale = 1;

    rot_mat = getRotationMatrix2D(center, angle, scale);

    warpAffine(frame, output, rot_mat, frame.size(), CV_INTER_LANCZOS4);

    roi = output(
            Rect((output.cols / 2) - (this->width / 2), 0, this->width, this->height));
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

void VisualRhythm::computeZigZagVisualRhythm(Mat &frame, Mat &output) {

    Mat roi = Mat::zeros(this->height, this->width, CV_8U);

    int j = 0, k = 0, i = 0, n = frame.rows, m = frame.cols, step = 60,
            row = -1;

    // upper triangular matrix
    for (i = 0; i < n; i = i + step) {

        j = 0;
        k = i;

        while (k >= 0 && j < m) {
            row++;
            for (int l = 0; l < this->width; l++) {
                if ((j + l) < (m + this->width)) {
                    roi.at<uchar>(row, l) = frame.at<uchar>(k, j + l);
                }
            }
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
            for (int l = 0; l < this->width; l++) {
                if ((j + l) < (m + this->width)) {
                    roi.at<uchar>(row, l) = frame.at<uchar>(k, j + l);
                }
            }
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

void VisualRhythm::saveVisualRhythm() {
    imwrite(this->output_filename.c_str(), this->visual_rhythm);
}
