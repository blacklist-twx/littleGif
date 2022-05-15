//
//  create_gif.h
//  littleGif
//
//  Created by 1234 on 2022/5/4.
//

#ifndef create_gif_h
#define create_gif_h

#include "BaseDecoderTest.h"
#include "decode_h264.h"
#include "gif_util.h"
#include "lzw/mlzw.h"
#include "quantization/octree.h"
#include <iostream>
extern "C" {
#include "lzw/lzw.h"
}

enum GifEditon { GIF89A = 1, GIF87A };
class GifGenerator : public BaseDecoderTest::Callback {
private:
  FILE *gif_file;
  YUV2RGB *yuv_2_rgb;
  u_int8_t *rgb;
  int height;
  int width;

public:
  GifGenerator(std::string create_path, int _height, int _width);
  void onDecodeFrame(const BaseDecoderTest::Frame &frame);
  void WriteHeader(enum GifEditon editon);
  void CreateGif();
  void WriteApplicationExtension();
  void WriteCommentExtension();
  void WriteGraphicControlExtension();
  void WriteImageDescriptor();
  void WriteImgData(u_int8_t *rgb_index);
  void WriteLocalColorTable(u_int32_t *color_table);
  void WriteTail();
  void WriteLogicalScreenDescriptor();
};

#endif /* create_gif_h*/
