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

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_

#include <iostream>

// Interface of the FrameProcessor class
class FrameProcessor {

public:

    // To process the input frame and return the result in output frame
    virtual void process(cv::Mat &input, cv::Mat &output){}

    // Destructor
    virtual ~FrameProcessor() {}

};

#endif /* FRAMEPROCESSOR_H_ */
