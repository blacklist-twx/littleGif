//
//  separate_av.hpp
//  littleGif
//
//  Created by 1234 on 2022/4/30.
//

#ifndef separate_av_h
#define separate_av_h

#include <stdio.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/bsf.h"
}
void separate();
#endif /* separate_av_h */
