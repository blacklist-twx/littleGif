//
//  create_gif.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/4.
//

#include "create_gif.h"
int tick = 0;
GifGenerator::GifGenerator(std::string create_path,int _height,int _width){
    this->gif_file = fopen(create_path.c_str(), "w");
    this->yuv_2_rgb = new YUV2RGB();
    this->width = _width;
    this->height = _height;
    this->rgb = (u_int8_t*)malloc(_width*_height*3);
}

void GifGenerator::onDecodeFrame (const BaseDecoderTest::Frame& frame) {
//        std::cout<<"onDecodeFrame"<<std::endl;
    std::cout<<"width  height"<<std::endl;
    std::cout<<frame.y.width<<"  "<<frame.y.height<<std::endl;
    std::cout<<frame.u.width<<"  "<<frame.u.height<<std::endl;
    std::cout<<frame.v.width<<"  "<<frame.v.height<<std::endl;
    std::cout<<frame.y.stride<<std::endl;
    std::cout<<frame.u.stride<<std::endl;
    std::cout<<frame.v.stride<<std::endl;
    
    if(tick==0){
        this->yuv_2_rgb->YUV420P2RGB(width, height,frame.y.stride, frame.y.data, frame.u.data, frame.v.data, rgb);
        fwrite(rgb,width*height*3,1,gif_file);
//        fwrite(frame.y.data,800*height,1,gif_file);
//        fwrite(frame.u.data,800*height/4,1,gif_file);
//        fwrite(frame.v.data,800*height/4,1,gif_file);
        fclose(gif_file);
    }
    
    
}
void GifGenerator::CreateGif(){
    
    Decode(this);
}

void GifGenerator::WriteHeader(enum GifEditon editon){
    u_int8_t gif_header[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
    if(editon==GIF87A)
        gif_header[4] = 0x37;
    fwrite(gif_header, 6, 1, this->gif_file);
}
