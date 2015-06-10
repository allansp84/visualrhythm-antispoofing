# Anti-Spoofing Descriptor based on Visual Rhythm

### Brief Description

This program compute the noise information contained in a input video and their respective spectrum fourier, generating a fourier spectrum video. After, the program compute the visual rhythm from fourier spectrum video, summarizing the most important information for face spoofing detection in a single image, named as visual rhythm.

### Requirements

The OpenCV Package 2.4.10 or later is required for use this software. This software runs on Linux Operating systems, and we tested it by using Ubuntu 14.04 LTS and Debian 8.

### Compiling and Running this Software

To compile this software the use needs set the variables defined in [opencv.inc](https://github.com/allansp84/visualrhythm-antispoofing/blob/master/Release/opencv.inc):

    OPENCVFLAGS = -I/path/to/include
    OPENCVLIBS = -L/path/to/lib

For example, suppose you have installed Opencv Package in the **/home/user/local/opencv-2.4.10**, then the OPENCVFLAGS and OPENCVLIBS variables should be set as:

    OPENCVFLAGS = -I/home/user/local/opencv-2.4.10/include
    OPENCVLIBS = -L/home/user/local/opencv-2.4.10/lib

After set the variables into ./Release/opencv.inc, the user must be run the follows commands to compile the source code:

    make -C Release clean
    make -C Release

First command line remove old binaries, and the second command builds a new binary named as *./Release/VisualRhythmAntiSpoofing*.

### How to Use this Software?

This software run only by command line interfaces (CLIs) such as the shell program (e.g., sh, bash, ksh). We provide the following parameters to the users that can be setted by the command line:

* color_space: Integer between 0 and 1 that indicates the color space used to load the video frames (default=0). Use:
    + 0: To load the frames in grayscale;
    + 1: To load the frames in the *L*ab color space;

* filter: Integer between 0 and 1 that indicates the type of filter used to compute the residual noise video (default=0). Use:
    + 0: To use a median filter;
    + 1: To use a gaussian filter.

* frame_number: Positive integer that indicates the number of consecutive frames used during computation of the visual rhythm (default=50).

* input_video: Filename of the input video to be computed the visual rhythm  **\<required\>**.

* kernel_size: Positive odd integer that indicates the size of the kernel used during filtering of the input video (default=7).

* output_image: Filename of the computed visual rhythm. Visual rhythm is saved as PNG image file **\<required\>**.

* roi_width: Positive integer that indicates the width of the region of interesting extracted of each frames (default=30).

* variance: Float that indicates the variance of the Gaussian filter (default=2).

* visual_rhythm_type: Integer between 0 and 2 that indicates the type of visual rhythm to be computed from input video **\<required\>**. Use:
    + 0: To compute a vertical visual rhythm;
    + 1: To compute a horizontal visual rhythm;
    + 2: To compute a zig-zag visual rhythm.

> P.S.: The parameters must be setted using a hyphen (-) before the name of the parameter followed by blanck space and their value (e.g., -visual_rhythm_type 0, -frame_number 50).

### Examples

For run the sofware the user must be the follow parameters:

1. Compute the *__vertical__ visual rhythm* of an input video (EXAMPLE/data/testcase1.avi) using default value parameters:
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -input_video EXAMPLE/data/testcase1.avi -output_image EXAMPLE/output/visualrhythm/vertical/testcase1.png
>     

2. Compute the *__vertical__ visual rhythm* (-visual_rhythm_type 0) from first 2 seconds (-frame_number 50) of an input video (EXAMPLE/data/testcase2.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 0 -frame_number 50 -color_space 0 -roi_width 30 -filter 0 -kernel_size 7 -variance 2 -input_video EXAMPLE/data/testcase2.avi -output_image EXAMPLE/output/visualrhythm/vertical/testcase2.png
>     

3. Compute the *__horizontal__ visual rhythm* (-visual_rhythm_type 1) from first 2 seconds (-frame_number 50) of an input video (EXAMPLE/data/testcase3.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 1 -frame_number 50 -color_space 0 -roi_width 30 -filter 0 -kernel_size 7 -variance 2 -input_video EXAMPLE/data/testcase3.avi -output_image EXAMPLE/output/visualrhythm/vertical/testcase3.png
>     

4. Compute the *__zig-zag__ visual rhythm* (-visual_rhythm_type 2) from first 2 seconds (-frame_number 50) of an input video (EXAMPLE/data/testcase1.avi):
>     
>     ./Release/VisualRhythmAntiSpoofing -visual_rhythm_type 2 -frame_number 50 -color_space 0 -roi_width 30 -filter 0 -kernel_size 7 -variance 2 -input_video EXAMPLE/data/testcase1.avi -output_image EXAMPLE/output/visualrhythm/vertical/testcase1.png
>     

### Please, Cite our Work!

If you use this software, please cite our paper published in *IEEE Transactions on Information Forensics and Security*:

> **Reference**
>
>     Pinto, A.; Schwartz, W.R.; Pedrini, H.; Rocha, A.d.R., "Using Visual Rhythms for Detecting Video-Based Facial Spoof Attacks," Information Forensics and Security, IEEE Transactions on , vol.10, no.5, pp.1025,1038, May 2015
>     doi: 10.1109/TIFS.2015.2395139
>     keywords: {Authentication;Biometrics (access control);Databases;Face;Feature extraction;Histograms;Noise;Unicamp Video-Attack Database;Video-based Face Spoofing;Video-based face spoofing;Visual Rhythm, Video-based Attacks;impersonation detection in facial biometric systems;unicamp video-attack database;video-based attacks;visual rhythm},
>     URL: http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7017526&isnumber=7073680


> **Bibtex**
>
>     @ARTICLE{7017526,
>     author={Pinto, A. and Schwartz, W.R. and Pedrini, H. and Rocha, A.},
>     journal={Information Forensics and Security, IEEE Transactions on},
>     title={Using Visual Rhythms for Detecting Video-Based Facial Spoof Attacks},
>     year={2015},
>     month={May},
>     volume={10},
>     number={5},
>     pages={1025-1038},
>     keywords={Authentication;Biometrics (access control);Databases;Face;Feature extraction;Histograms;Noise;Unicamp Video-Attack Database;Video-based Face Spoofing;Video-based face spoofing;Visual Rhythm, Video-based Attacks;impersonation detection in facial biometric systems;unicamp video-attack database;video-based attacks;visual rhythm},
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
