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

#include "video.h"
#include "visualrhythm.h"
#include <sys/stat.h>
#include <errno.h>


bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void compute_visual_rhythm(int color_space, int filter, int frame_number, string input_video,
  int kernel_size, string output_image, int roi_width, float variance, int visual_rhythm_type);

int create_path(std::string s, mode_t mode);

void help(string filename);

void parse_command_line(int argc, char **argv, int &color_space, int &filter, int &frame_number,
  string &input_video, int &kernel_size, string &output_image, int &roi_width, float &variance,
  int &visual_rhythm_type);

void split_filename(const string& str, string& path, string& file, string& extension);

bool verify_command_line(int color_space, int filter, int frame_number, string input_video,
  int kernel_size, string &output_image, int roi_width, float variance, int visual_rhythm_type);

int main(int argc, char** argv) {

    int color_space = 0;
    int filter = 0;
    int frame_number = 50;
    string input_video = "";
    int kernel_size = 7;
    string output_image = "";
    int roi_width = 30;
    float variance = 2;
    int visual_rhythm_type = -99;

    bool is_opencv_version = false;

    if (CV_MAJOR_VERSION == SUPPORTED_CV_MAJOR_VERSION) {
        if (CV_MINOR_VERSION == SUPPORTED_CV_MINOR_VERSION) {
            if (CV_SUBMINOR_VERSION >= SUPPORTED_CV_SUBMINOR_VERSION){
                is_opencv_version = true;
            } else {
                is_opencv_version = false;
            }
        } else if (CV_MINOR_VERSION > SUPPORTED_CV_MINOR_VERSION) {
            is_opencv_version = true;
        } else {
            is_opencv_version = false;
        }
    } else if (CV_MAJOR_VERSION > SUPPORTED_CV_MAJOR_VERSION) {
        is_opencv_version = true;
    }else {
        is_opencv_version = false;
    }

    if (!is_opencv_version) {
        cout << "OpenCV version required >= 2.4.8" << endl;
        cout << "Current OpenCV version (" << CV_VERSION << ")" << endl;
        exit(EXIT_FAILURE);
    }

    parse_command_line(argc, argv, color_space, filter, frame_number, input_video, kernel_size, 
      output_image, roi_width, variance, visual_rhythm_type);

    compute_visual_rhythm(color_space, filter, frame_number, input_video, kernel_size, 
      output_image, roi_width, variance, visual_rhythm_type);

    return 0;
}

void compute_visual_rhythm(int color_space, int filter, int frame_number, string input_video,
  int kernel_size, string output_image, int roi_width, float variance, int visual_rhythm_type) {

    //Object liable for control of the video
    Video processor;

    //Object liable for processing of each frame
    VisualRhythm visual_rhythm;

    processor.set_input_video(input_video.c_str());
    processor.set_frame_processor(&visual_rhythm);
    processor.set_frame_to_stop(frame_number);

    visual_rhythm.set_visual_rhythm_type(visual_rhythm_type);
    visual_rhythm.set_color_space(color_space);
    visual_rhythm.set_filter(filter);
    visual_rhythm.set_variance(variance);
    visual_rhythm.set_width(roi_width);
    visual_rhythm.set_output_filename(output_image.c_str());

    if (visual_rhythm_type == 0) {
        cout << "Extracting vertical visual rhythm ... ";

        int height_vertical = processor.get_frame_height();
        visual_rhythm.set_height(height_vertical);
        visual_rhythm.set_visual_rhythm(Mat(height_vertical, roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visual_rhythm.save_visual_rhythm();
        cout << "Ok!" << endl;

    } else if (visual_rhythm_type == 1) {
        cout << "Extracting horizontal visual rhythm ... ";

        int height_horizontal = processor.get_frame_width();
        visual_rhythm.set_height(height_horizontal);
        visual_rhythm.set_visual_rhythm(Mat(height_horizontal,roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visual_rhythm.save_visual_rhythm();
        cout << "Ok!" << endl;

    } else if (visual_rhythm_type == 2) {
        cout << "Extracting zig-zag visual rhythm ... ";

        int height = processor.get_frame_height();
        int width = processor.get_frame_width();
        int height_zigzag = visual_rhythm.compute_dimensions_visual_rhythm(height, width);

        visual_rhythm.set_height(height_zigzag);
        visual_rhythm.set_visual_rhythm(Mat(height_zigzag, roi_width * frame_number, CV_8U));
        processor.run();
        cout << "Ok!" << endl;

        cout << "Saving the generated visual rhythm ... ";
        visual_rhythm.save_visual_rhythm();
        cout << "Ok!" << endl;

    } else {
        cout << "Invalid type for visual rhythm!";
        exit(EXIT_FAILURE);
    }

    cout << "Done!\n" << endl;
}

int create_path(std::string s,mode_t mode){
    size_t pre=0,pos;
    std::string dir;
    int mdret;

    if (s[s.size()-1]!='/') {
        s+='/';
    }

    while ((pos=s.find_first_of('/',pre))!=std::string::npos) {
        dir=s.substr(0,pos++);
        pre=pos;

        if (dir.size() == 0) {
            continue;
        }

        if ((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST) {
            return mdret;
        }
    }

    return mdret;
}

void help(string filename){
    string path = "";
    string program_name = "";
    string extension = "";

    split_filename(filename, path, program_name, extension);

    cout << program_name << " (Version 1.0.0)" << endl;

    cout << "" << endl;

    cout << "New BSD License." << endl;
    cout << "Copyright (c) 2015, Allan Pinto, William Robson Schwartz, Helio Pedrini, " << endl;
    cout << "and Anderson Rocha" << endl;
    cout << "All rights reserved." << endl;

    cout << "" << endl;

    cout << "Usage: " << program_name << " [-option [value]] " << endl;

    cout << "" << endl;

    cout << "Options:" << endl;

    cout << "  -color_space\t\t Integer between 0 and 1 that indicates the color space used ";
    cout << "to load the video frames (default=0). Use:" << endl;
    cout << "   \t\t\t   0: To load the frames in grayscale" << endl;
    cout << "   \t\t\t   1: To load the frames in the Lab color space" << endl;

    cout << "  -filter\t\t Integer between 0 and 1 that indicates the type of filter used ";
    cout << "to compute the residual noise video (default=0). Use:" << endl;
    cout << "   \t\t\t   0: To use a Median filter" << endl;
    cout << "   \t\t\t   1: To use a Gaussian filter" << endl;

    cout << "  -frame_number\t\t Positive integer that indicates the number of consecutive frames ";
    cout << "used during computation of the visual rhythm (default=50)." << endl;

    cout << "  -input_video\t\t Filename of the input video to be computed the ";
    cout << "visual rhythm <required>." << endl;

    cout << "  -kernel_size\t\t Positive odd integer that indicates the size of the ";
    cout << "kernel used during filtering of the input video (default=7)." << endl;

    cout << "  -output_image\t\t Filename of the computed visual rhythm. Visual rhythm is saved ";
    cout << "as PNG image file <required>." << endl;

    cout << "  -roi_width\t\t Positive integer that indicates the width of the ";
    cout << "region of interesting extracted of each frames (default=30)." << endl;

    cout << "  -variance\t\t Float that indicates the variance of the ";
    cout << "Gaussian filter (default=2)." << endl;

    cout << "  -visual_rhythm_type\t Integer between 0 and 2 that indicates the ";
    cout << "type of visual rhythm to be computed from input video <required>. Use:" << endl;
    cout << "   \t\t\t   0: To compute a vertical visual rhythm" << endl;
    cout << "   \t\t\t   1: To compute a horizontal visual rhythm" << endl;
    cout << "   \t\t\t   2: To compute a zig-zag visual rhythm" << endl;

    cout << "" << endl;

    cout << "Examples: See README." << endl;

    cout << "" << endl;

    cout << "News, support and information: ";
    cout << "https://github.com/allansp84/visualrhythm-antispoofing" << endl;

}

void parse_command_line(int argc, char **argv, int &color_space, int &filter, int &frame_number,
  string &input_video, int &kernel_size, string &output_image, int &roi_width, float &variance,
  int &visual_rhythm_type) {

    int i = 1;
    bool is_missing_parameter = false;

    string help_pattern = "--help";
    string visual_rhythm_type_pattern = "-visual_rhythm_type";
    string frame_number_pattern = "-frame_number";
    string roi_width_pattern = "-roi_width";
    string filter_pattern = "-filter";
    string kernel_size_pattern = "-kernel_size";
    string variance_pattern = "-variance";
    string color_space_pattern = "-color_space";
    string input_video_pattern = "-input_video";
    string output_image_pattern = "-output_image";

    if (help_pattern.compare(0, help_pattern.length(), argv[i], help_pattern.length()) == 0) {
        help(string(argv[0]));
        exit(EXIT_FAILURE);
    }

    while ((i < argc) && (is_missing_parameter == false)) {

        if (output_image_pattern.compare(0, output_image_pattern.length(), argv[i],
              output_image_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << output_image_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else {
                output_image = string(argv[i]);
            }

        } else if (roi_width_pattern.compare(0, roi_width_pattern.length(), argv[i],
              roi_width_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << roi_width_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                roi_width = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << kernel_size_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (kernel_size_pattern.compare(0, kernel_size_pattern.length(), argv[i],
              kernel_size_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << kernel_size_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                kernel_size = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << kernel_size_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (variance_pattern.compare(0, variance_pattern.length(), argv[i],
              variance_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << variance_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                variance = atof(argv[i]);
            } else {
                cout << "Missing value for parameter " << kernel_size_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (color_space_pattern.compare(0, color_space_pattern.length(), argv[i],
              color_space_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << color_space_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                color_space = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << color_space_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (visual_rhythm_type_pattern.compare(0, visual_rhythm_type_pattern.length(),
              argv[i], visual_rhythm_type_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << visual_rhythm_type_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                visual_rhythm_type = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << visual_rhythm_type_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (frame_number_pattern.compare(0, frame_number_pattern.length(), argv[i],
              frame_number_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << frame_number_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                frame_number = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << frame_number_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (filter_pattern.compare(0, filter_pattern.length(), argv[i],
              filter_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << filter_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else if (is_number(argv[i])) {
                filter = atoi(argv[i]);
            } else {
                cout << "Missing value for parameter " << filter_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            }

        } else if (input_video_pattern.compare(0, input_video_pattern.length(), argv[i],
              input_video_pattern.length()) == 0) {

            i++;
            if ((argv[i]) == NULL) {
                cout << "Missing value for parameter " << input_video_pattern;
                cout << ". See --help." << endl;
                is_missing_parameter = true;
            } else {
                input_video = string(argv[i]);
            }

        } else {

            cout << "Warning:parse_command_line():unknown parameter " << argv[i];
            cout << ". See --help." << endl;
            is_missing_parameter = true;

        }

        i++;

    }

    if (!is_missing_parameter){
        is_missing_parameter = verify_command_line(color_space, filter, frame_number, input_video,
          kernel_size, output_image, roi_width, variance, visual_rhythm_type);

    }

    if (is_missing_parameter) {
        exit(EXIT_FAILURE);
    }

}

void split_filename(const string& str, string& path, string& file, string& extension) {

    size_t found = str.find_last_of("/\\");
    size_t last_dot = str.find_last_of(".");

    if (found != string::npos) {
        path = str.substr(0,found);
        file = str.substr(found+1);

    } else {
        file = str;
    }

    if (last_dot != string::npos) {
        extension = str.substr(last_dot+1);
    }

}

bool verify_command_line(int color_space, int filter, int frame_number, string input_video,
  int kernel_size, string &output_image, int roi_width, float variance, int visual_rhythm_type){

    bool is_missing_parameter = false;
    struct stat file_stat;

    string path = "";
    string file = "";
    string extension = "";

    if ((visual_rhythm_type < 0) || (visual_rhythm_type > 2)) {
        cout << "Invalid value used in visual_rhythm_type. See --help" << endl;
        is_missing_parameter = true;
    }

    if (frame_number < 1) {
        cout << "Invalid value used in frame_number. See --help" << endl;
        is_missing_parameter = true;
    }

    if (roi_width < 1) {
        cout << "Invalid value used in roi_width. See --help" << endl;
        is_missing_parameter = true;
    }

    if ((filter < 0) || (filter > 1)) {
        cout << "Invalid value used in filter. See --help" << endl;
        is_missing_parameter = true;
    }

    if ((kernel_size < 3) || (kernel_size % 2 == 0)) {
        cout << "Invalid value used in kernel_size. See --help" << endl;
        is_missing_parameter = true;
    }

    if (variance < 0) {
        cout << "Invalid value used in variance. See --help" << endl;
        is_missing_parameter = true;
    }

    if ((color_space < 0) || (color_space > 1)) {
        cout << "Invalid value used in color_space. See --help" << endl;
        is_missing_parameter = true;
    }

    if (lstat(input_video.c_str(), &file_stat) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        cout << "Invalid value used in input_video. See --help" << endl;
        is_missing_parameter = true;
    }

    split_filename(output_image, path, file, extension);

    if (file.empty()) {
        cout << "Invalid file name used in output_image. See --help" << endl;
        is_missing_parameter = true;
    }

    if (extension.empty() || extension.compare("png")) {
        output_image += ".png";
    }

    if (!path.empty()) {
        create_path(path, 0755);
    }

    return is_missing_parameter;

}
