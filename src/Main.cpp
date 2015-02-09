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

#include "Video.h"
#include "VisualRhythm.h"

// This function makes a parsing of the command line and sets parameter values
void checkParameters(int argc, char **argv, int &visual_rhythm_type, int &frame_number,
        int &roi_width, string &filter, int &kernel_size, float &variance, string &color_space,
        string &video_filename, string &image_fname);

// This function instantiate the video controller and visual rhythm objects 
// using the values provided by the user by command line
void computeVisualRhythm(int kernel_size, int visual_rhythm_type, int frame_number, int roi_width,
        string filter, float variance, string color_space, string video_filename, string image_fname);

int main(int argc, char** argv) {
    int visual_rhythm_type = 0;
    int frame_number = 50;
    int roi_width = 30;
    string filter;
    int kernel_size = 3;
    float variance = 2;
    string color_space;
    string video_filename;
    string image_fname;

    cout << "Checking OpenCV Version ... ";
    if (CV_MAJOR_VERSION >= 2) {
        if (CV_MINOR_VERSION >=4) {
            if (CV_SUBMINOR_VERSION >= 8) {
                cout << "Ok (" << CV_VERSION << ")" << endl;
            }
            else{
                cout << "OpenCV version required >= 2.4.8" << endl;
                cout << "Current OpenCV version (" << CV_VERSION << ")" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            cout << "OpenCV version required >= 2.4.8" << endl;
            cout << "Current OpenCV version (" << CV_VERSION << ")" << endl;
            exit(EXIT_FAILURE);
        }
    }else{
        cout << "OpenCV version required >= 2.4.8" << endl;
        cout << "Current OpenCV version (" << CV_VERSION << ")" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Checking parameters ... ";
    checkParameters(argc, argv, visual_rhythm_type, frame_number, roi_width, filter, kernel_size,
            variance, color_space, video_filename, image_fname);
    cout << "Ok!" << endl;

    computeVisualRhythm(kernel_size, visual_rhythm_type, frame_number, roi_width, filter, variance, color_space, 
            video_filename, image_fname);

    return 0;
}

void computeVisualRhythm(int kernel_size, int visual_rhythm_type, int frame_number, int roi_width,
        string filter, float variance, string color_space, string video_filename, string image_fname) {

    //Object liable for control of the video
    Video processor;

    //Object liable for processing of each frame
    VisualRhythm visualrhythm;

    processor.setInput(video_filename.c_str());
    processor.setFrameProcessor(&visualrhythm);
    processor.setFrameToStop(frame_number);
    processor.setDelay(1000. / processor.getFrameRate());

    visualrhythm.setVisualRhythmType(visual_rhythm_type);
    visualrhythm.setColorSpace(color_space);
    visualrhythm.setFilter(filter);
    visualrhythm.setVariance(variance);
    visualrhythm.setWidth(roi_width);
    visualrhythm.setOutputFileName(image_fname.c_str());

    if (visual_rhythm_type == 0) {
        cout << "Extracting vertical visual rhythm ... ";

        int height_for_vertical = processor.getFrameSize().height;
        visualrhythm.setHeight(height_for_vertical);
        visualrhythm.createVisualRhythm(Mat(height_for_vertical, roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visualrhythm.saveVisualRhythm();
        cout << "Ok!" << endl;

    } else if (visual_rhythm_type == 1) {
        cout << "Extracting horizontal visual rhythm ... ";

        int height_for_horizontal = processor.getFrameSize().width;
        visualrhythm.setHeight(height_for_horizontal);
        visualrhythm.createVisualRhythm(Mat(height_for_horizontal, roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visualrhythm.saveVisualRhythm();
        cout << "Ok!" << endl;

    } else if (visual_rhythm_type == 2) {
        cout << "Extracting zig-zag visual rhythm ... ";

        int height = processor.getFrameSize().height;
        int width = processor.getFrameSize().width;
        int height_for_zigzag = visualrhythm.estimateDimensionsOfVisualRhythm(height, width);

        visualrhythm.setHeight(height_for_zigzag);
        visualrhythm.createVisualRhythm(Mat(height_for_zigzag, roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visualrhythm.saveVisualRhythm();
        cout << "Ok!" << endl;

    } else {
        cout << "Invalid type for visual rhythm!";
        exit(EXIT_FAILURE);
    }

    cout << "Done!\n" << endl;
}

void checkParameters(int argc, char **argv, int &visual_rhythm_type, int &frame_number,
        int &roi_width, string &filter, int &kernel_size, float &variance, string &color_space, string &video_filename,
        string &image_fname) {

    string helpString = "--help";
    string visualRhythmTypePattern = "-visual_rhythm_type";
    string frameNumberPattern = "-frame_number";
    string roiWidthPattern = "-roi_width";
    string filterPattern = "-filter";
    string kernelSizePattern = "-kernel_size";
    string variancePattern = "-variance";
    string colorSpacePattern = "-color_space";
    string inputFnamePattern = "-input_video";
    string imageFnamePattern = "-output_image";

    int i = 1;

    while (i < argc) {

        if (imageFnamePattern.compare(0, imageFnamePattern.length(), argv[i],
                imageFnamePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << imageFnamePattern << endl;
                exit(EXIT_FAILURE);
            }
            image_fname = string(argv[i]);

        } else if (roiWidthPattern.compare(0, roiWidthPattern.length(), argv[i], roiWidthPattern.length())
                == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << roiWidthPattern << endl;
                exit(EXIT_FAILURE);
            }
            roi_width = atoi(argv[i]);

        } else if (kernelSizePattern.compare(0, kernelSizePattern.length(), argv[i],
                kernelSizePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << kernelSizePattern << endl;
                exit(EXIT_FAILURE);
            }
            kernel_size = atoi(argv[i]);

        } else if (variancePattern.compare(0, variancePattern.length(), argv[i],
                variancePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << variancePattern << endl;
                exit(EXIT_FAILURE);
            }
            variance = atof(argv[i]);

        } else if (colorSpacePattern.compare(0, colorSpacePattern.length(), argv[i],
                colorSpacePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << colorSpacePattern << endl;
                exit(EXIT_FAILURE);
            }
            color_space = string(argv[i]);

        } else if (visualRhythmTypePattern.compare(0, visualRhythmTypePattern.length(), argv[i],
                visualRhythmTypePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << visualRhythmTypePattern << endl;
                exit(EXIT_FAILURE);
            }
            visual_rhythm_type = atoi(argv[i]);

        } else if (frameNumberPattern.compare(0, frameNumberPattern.length(), argv[i],
                frameNumberPattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << frameNumberPattern << endl;
                exit(EXIT_FAILURE);
            }
            frame_number = atoi(argv[i]);

        } else if (filterPattern.compare(0, filterPattern.length(), argv[i],
                filterPattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << filterPattern << endl;
                exit(EXIT_FAILURE);
            }
            filter = string(argv[i]);

        } else if (inputFnamePattern.compare(0, inputFnamePattern.length(), argv[i],
                inputFnamePattern.length()) == 0) {
            i++;
            if ((argv[i]) == NULL) {
                cout << "Error:checkParameters():missing parameter value " << inputFnamePattern << endl;
                exit(EXIT_FAILURE);
            }
            video_filename = string(argv[i]);

        } else {
            cout << "Warning:checkParameters():unknown parameter " << argv[i] << endl;
        }
        i++;
    }

    if (video_filename.empty() || image_fname.empty()) {
        cout << "Error:checkParameters():missing one or more parameter values" << endl;
        exit(EXIT_FAILURE);
    }
}
