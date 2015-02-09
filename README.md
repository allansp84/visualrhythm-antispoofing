# Anti-Spoofing Descriptor based on Visual Rhythm

### Brief Description
This program compute the noise information contained in a input video and their respective spectrum fourier, generating a fourier spectrum video. After, the program compute the visual rhythm from fourier spectrum video, summarizing the most important information for face spoofing detection in a single image, named as visual rhythm.

### Requirements
The OpenCV Package 2.4.8 or later is required for use this software. This software runs on Linux Operating systems, and we tested it by using Ubuntu 14.04 LTS.

### Compiling and Running this Software
To compile this software the use needs set the variables defined in [opencv.inc](https://github.com/allansp84/visualrhythm-antispoofing/blob/master/Release/opencv.inc):

    OPENCVFLAGS = -I/path/to/include/opencv
    OPENCVLIBS = -L/path/to/lib

For example, suppose you have installed Opencv Package in the **/home/user/local**, then the OPENCVFLAGS and OPENCVLIBS variables should be set as:

    OPENCVFLAGS = -I/home/user/local/include/opencv
    OPENCVLIBS = -L/home/user/local/lib

After set the variables into ./Release/opencv.inc, the user must be run the follows commands to compile the source code:

    make -C Release clean
    make -C Release

First command line remove old binaries, and the second command builds a new binary named as *./Release/VisualRhythmAntiSpoofing*.

### How to Use this Software?

This software run only by command line interfaces (CLIs) such as the shell program (e.g., sh, bash, ksh). We provide the following parameters to the users that can be setted by the command line:

* visual_rhythm_type: Integer between 0 and 2 that indicates the type of visual rhythm to be computed from input video (default=0) **\<required\>**. Use:
    + 0: To compute a vertical visual rhythm;
    + 1: To compute a horizontal visual rhythm;
    + 2: To compute a zig-zag visual rhythm.
* frame_number: Integer that indicates the number of frames used during computation of the visual rhythm, and that the frames are taken in sequence (default=50).
* color_space: String that indicates the color space used to load the video frames (default=gray). Use:
    + gray: To load the frames in grayscale;
    + lab: To load the frames in the *L*ab color space;
* roi_width: Integer that indicates the width of the region of interesting extracted of each frames (default=30).
* filter: String that indicates the type of filter used to compute the residual noise video from the input video (default=gauss). Use:
    + gauss to use a gaussian filter;
    + median to use a median filter.
* kernel_size: Integer that indicates the size of the kernel used during filtering from the input video (default=3).
* variance: Float that indicates the variance of the kernel used during gaussian filtering from the input video (default=2).
* input_video: String that indicates the file name of the input video to be computed the visual rhythm **\<required\>**.
* output_image: String that indicates the file name where the extracted visual rhythm will be saved. The visual rhythms are saved as PNG image file **\<required\>**.

> P.S.: The parameters must be pass using a - before the name parameter followed by blanck space and the value to be setted (e.g., -visual_rhythm_type 0, -frame_number 50).

### Examples
For run the sofware the user must be the follow parameters:

1. Compute the *__vertical__ visual rhythm* of an input video (data/testcase1.avi) using default value parameters:
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -input_video data/testcase1.avi -output_image output/visualrhythm/vertical/testcase1.avi
>     

2. Compute the *__vertical__ visual rhythm* (-visual_rhythm_type 0) from first 2 seconds (-frame_number 50) of an input video (data/testcase2.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase1.avi -output_image output/visualrhythm/vertical/testcase1.avi
>     

3. Compute the *__horizontal__ visual rhythm* (-visual_rhythm_type 1) from first 2 seconds (-frame_number 50) of an input video (data/testcase3.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 1 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase2.avi -output_image output/visualrhythm/vertical/testcase2.avi
>     

4. Compute the *__zig-zag__ visual rhythm* (-visual_rhythm_type 2) from first 2 seconds (-frame_number 50) of an input video (data/testcase1.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 2 -frame_number 50 -color_space gray -roi_width 30 -filter gauss -kernel_size 3 -variance 2 -input_video data/testcase1.avi -output_image output/visualrhythm/vertical/testcase1.avi
>     

### Please, Cite our Work!
If you use this software, please cite our paper published in *IEEE Transactions on Information Forensics and Security*:

> **Reference**
>
>     Pinto, A.; Schwartz, W.; Pedrini, H.; Rocha, A., "Using Visual Rhythms for Detecting Video-based Facial Spoof Attacks," Information Forensics and Security, IEEE Transactions on , vol.PP, no.99, pp.1,1
>     doi: 10.1109/TIFS.2015.2395139
>     keywords: {Authentication;Biometrics (access control);Databases;Face;Feature extraction;Histograms;Noise;Unicamp Video-Attack Database;Video-based Face Spoofing;Visual Rhythm, Video-based Attacks},
>     URL: http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7017526&isnumber=4358835


> **Bibtex**
>
>     @ARTICLE{7017526,
>     author={Pinto, A. and Schwartz, W. and Pedrini, H. and Rocha, A.},
>     journal={Information Forensics and Security, IEEE Transactions on},
>     title={Using Visual Rhythms for Detecting Video-based Facial Spoof Attacks},
>     year={2015},
>     month={},
>     volume={PP},
>     number={99},
>     pages={1-1},
>     keywords={Authentication;Biometrics (access control);Databases;Face;Feature extraction;Histograms;Noise;Unicamp Video-Attack Database;Video-based Face Spoofing;Visual Rhythm, Video-based Attacks},
>     doi={10.1109/TIFS.2015.2395139},
>     ISSN={1556-6013},}

### License

This software is available under condition of the New BSD Licence, whose terms are:

    Unless otherwise specified by LICENSE in individual files, all code is

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
