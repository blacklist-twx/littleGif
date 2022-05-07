//
//  gif_util.hpp
//  littleGif
//
//  Created by 1234 on 2022/5/4.
//

#ifndef gif_util_h
#define gif_util_h

#include <stdio.h>
class YUV2RGB{
private:
    short __s_r_1370705v[256] = {0};
    short __s_b_1732446u[256] = {0};
    short __s_g_337633u[256] = {0};
    short __s_g_698001v[256] = {0};
public:
    YUV2RGB();
    void YUV420P2RGB(int width, int height,int stride, const u_int8_t* Y, const u_int8_t* U,const u_int8_t* V, u_int8_t* rgb);
//    void convert(uint8_t* y_data,uint8_t* u_data)
};

#endif /* gif_util_h */
