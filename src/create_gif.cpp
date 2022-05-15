//
//  create_gif.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/4.
//

#include "create_gif.h"
int tick = 0;
int frame_length = 0;
FILE *rgb_file;
bool is_tree_build = false;
GifGenerator::GifGenerator(GeneratorBuilder *builder) {
  this->gif_file = fopen(builder->create_path.c_str(), "w");
  this->yuv_2_rgb = new YUV2RGB();
  this->width = builder->width;
  this->height = builder->height;
  this->frame_gap = builder->frame_gap;
  this->rgb = (u_int8_t *)malloc(this->width * this->height * 3);
  this->color_mode = builder->color_mode;
  this->delay_time = builder->delay_time;
  if (this->color_mode == GLOBAL_COLOR_TABLE) {
    this->global_octree = new Octree();
    this->global_color_table = new u_int32_t[256];
  }

  /*test*/
  rgb_file = fopen("/Users/a1234/Desktop/testRGB.rgb", "w");
  /*test*/
}

void GifGenerator::onDecodeFrame(const BaseDecoderTest::Frame &frame) {
  //        std::cout<<"onDecodeFrame"<<std::endl;
  if ((tick++) % this->frame_gap != 0)
    return;

  //    std::cout<<"width  height"<<std::endl;
  //    std::cout<<frame.y.width<<"  "<<frame.y.height<<std::endl;
  //    std::cout<<frame.u.width<<"  "<<frame.u.height<<std::endl;
  //    std::cout<<frame.v.width<<"  "<<frame.v.height<<std::endl;
  //    std::cout<<frame.y.stride<<std::endl;
  //    std::cout<<frame.u.stride<<std::endl;
  //    std::cout<<frame.v.stride<<std::endl;
  if (this->color_mode == GLOBAL_COLOR_TABLE) {
    if (is_tree_build) {
      this->yuv_2_rgb->YUV420P2RGB(width, height, frame.y.stride, frame.y.data,
                                   frame.u.data, frame.v.data, rgb);
      this->global_octree->RGB2Index(
          this->global_octree->global_root_node, rgb, width, height,
          this->global_color_table,
          this->global_rgb_index[(tick - 1) / this->frame_gap]);

    } else {
      frame_length++;
      this->yuv_2_rgb->YUV420P2RGB(width, height, frame.y.stride, frame.y.data,
                                   frame.u.data, frame.v.data, rgb);
      this->global_octree->PutColors(this->global_octree->global_root_node, rgb,
                                     width, height);
    }
  } else {
    WriteGraphicControlExtension(this->delay_time);
    WriteImageDescriptor();
    int width2 = frame.y.width;
    this->yuv_2_rgb->YUV420P2RGB(width, height, frame.y.stride, frame.y.data,
                                 frame.u.data, frame.v.data, rgb);
    Octree *octree = new Octree();
    u_int32_t color_table[256] = {0};
    u_int8_t *rgb_index = (u_int8_t *)malloc(width * height);
    if (NULL == rgb_index) {
      std::cout << "内存分配失败" << std::endl;
    }
    //    std::cout<<sizeof(rgb_index)<<std::endl;
    octree->Quantizise(rgb, width, height, color_table, rgb_index);
    //  if (tick == 1)
    //    fwrite(rgb, width * height * 3, 1, rgb_file);
    delete octree;
    WriteLocalColorTable(color_table);
    WriteImgData(rgb_index);
    std::cout << "writeData over" << std::endl;
    free(rgb_index);
  }
}

void GifGenerator::CreateGif() {
  WriteHeader(GIF89A);
  WriteLogicalScreenDescriptor();
  if (this->color_mode == GLOBAL_COLOR_TABLE) {
    Decode(this); // build tree
    is_tree_build = true;
    this->global_rgb_index = new u_int8_t *[(tick - 1) / this->frame_gap + 1];
    for (int i = 0; i < (tick - 1) / this->frame_gap + 1; i++) {
      this->global_rgb_index[i] = new u_int8_t[width * height];
    }
    tick = 0;
    Decode(this); // get index and table
    WriteLocalColorTable(this->global_color_table);
    WriteApplicationExtension();
    WriteCommentExtension();
    for (int i = 0; i < frame_length; i++) {
      WriteGraphicControlExtension(this->delay_time);
      WriteImageDescriptor();
      WriteImgData(this->global_rgb_index[i]);
      std::cout << "writeData over" << std::endl;
      //            free(rgb_index);
    }
  } else {
    WriteApplicationExtension();
    WriteCommentExtension();
    Decode(this);
  }

  WriteTail();
}

void GifGenerator::WriteHeader(enum GifEditon editon) {
  u_int8_t gif_header[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
  if (editon == GIF87A)
    gif_header[4] = 0x37;
  fwrite(gif_header, 6, 1, this->gif_file);
}

void GifGenerator::WriteLocalColorTable(u_int32_t *color_table) {
  for (int i = 0; i < 256; i++) {
    // 根据颜色索引取出颜色表中的颜色
    u_int32_t color_rgb = color_table[i];
    // 当前颜色 R 分量
    u_int8_t R = (color_rgb & 0xFF0000) >> 16;
    // 当前颜色 G 分量
    u_int8_t G = (color_rgb & 0x00FF00) >> 8;
    // 当前颜色 B 分量
    u_int8_t B = color_rgb & 0x0000FF;
    int result = fputc(R, gif_file);
    result = fputc(G, gif_file);
    result = fputc(B, gif_file);
  }
}

void GifGenerator::WriteImgData(u_int8_t *rgb_index) {
  //  GIF 一帧图像的数据压缩后大小
  unsigned long compressed_size = 0;
  u_int8_t *encoded_data = nullptr;

  // GIF 一帧图像的数据解压后的数据
  unsigned char *img;
  lzw_compress_gif(8, width * height, rgb_index, &compressed_size,
                   &encoded_data);
  //    u_int8_t *encoded_data = (u_int8_t *)malloc(width * height * 2);
  //  int compressed_size = encoding(rgb_index, encoded_data, width * height);
  printf("GIF 一帧图像压缩后大小：%ld\n", compressed_size);
  fputc(0x08, gif_file);
  unsigned long current_index = 0;
  while (current_index < compressed_size) {
    if ((current_index + 0xFF) >= compressed_size) {
      unsigned long diff = compressed_size - current_index;
      fputc(diff, gif_file);
      fwrite(encoded_data + current_index, diff, 1, gif_file);
      fputc(0x00, gif_file);
      current_index += diff;
    } else {
      fputc(0xFF, gif_file);
      fwrite(encoded_data + current_index, 0xFF, 1, gif_file);
      current_index += 0xFF;
    }
  }
  free(encoded_data);
}

void GifGenerator::WriteApplicationExtension() {
  uint8_t gif_application_extension[] = {
      0x21, 0xFF, 0x0B, 0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50,
      0x45, 0x32, 0x2E, 0x30, 0x03, 0x01, 0x00, 0x00, 0x00};
  fwrite(gif_application_extension, 19, 1, this->gif_file);
}

void GifGenerator::WriteCommentExtension() {
  uint8_t gif_comment_extension[] = {
      0x21, 0xFE, 0x20, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77,
      0x69, 0x74, 0x68, 0x20, 0x65, 0x7A, 0x67, 0x69, 0x66, 0x2E, 0x63, 0x6F,
      0x6D, 0x20, 0x47, 0x49, 0x46, 0x20, 0x6D, 0x61, 0x6B, 0x65, 0x72, 0x00};
  fwrite(gif_comment_extension, 36, 1, this->gif_file);
}

void GifGenerator::WriteGraphicControlExtension(int delay_time) {
  // 图形控制扩展
  u_int8_t d1 = delay_time >> 0;
  u_int8_t d2 = delay_time >> 8;
  std::cout << (int)d1 << std::endl;
  std::cout << (int)d2 << std::endl;
  uint8_t gif_graphic_control_extension[] = {0x21, 0xF9, 0x04, 0x00,
                                             d1,   d2,   0xFF, 0x00};
  fwrite(gif_graphic_control_extension, 8, 1, this->gif_file);
}
void GifGenerator::WriteImageDescriptor() {
  // 图像标识符
  u_int8_t w1 = width >> 0;
  u_int8_t w2 = (width >> 8);
  u_int8_t h1 = height;
  u_int8_t h2 = (height >> 8);

  // 1 0 1 0 0 111 = 0xA7  local
  // 0 0 0 0 0 000 = 0x00 global
  u_int8_t field;
  if (this->color_mode == GLOBAL_COLOR_TABLE)
    field = 0x00;
  else
    field = 0xA7;
  uint8_t gif_image_descriptor[] = {0x2C, 0x00, 0x00, 0x00, 0x00,
                                    w1,   w2,   h1,   h2,   field};
  std::cout << (int)w1 << (int)w2 << std::endl;
  fwrite(gif_image_descriptor, 10, 1, gif_file);
}

void GifGenerator::WriteTail() {
  fputc(0x3B, gif_file);
  fflush(gif_file);
  fclose(gif_file);
}

void GifGenerator::WriteLogicalScreenDescriptor() {
  // 0x72 = 0   1 1 1   0   0 1 0
  // 0xF7 = 1   1 1 1   0   111
  uint8_t gif_logical_screen_pack_byte;
  if (this->color_mode == LOCAL_COLOR_TABLE)
    gif_logical_screen_pack_byte = 0x72;
  else
    gif_logical_screen_pack_byte = 0xF7;
  //    uint8_t gif_logical_screen_pack_byte = 0xF7;
  uint8_t gif_bg_color_index = 0;
  uint8_t gif_pixel_aspect = 0;
  fputc(width >> 0, gif_file);  // width low 8
  fputc(width >> 8, gif_file);  // width high 8
  fputc(height >> 0, gif_file); // height low 8
  fputc(height >> 8, gif_file); // height high 8
  fputc(gif_logical_screen_pack_byte, gif_file);
  fputc(gif_bg_color_index, gif_file);
  fputc(gif_pixel_aspect, gif_file);
}

GeneratorBuilder::GeneratorBuilder(std::string _create_path, int _height,
                                   int _width) {
  this->create_path = _create_path;
  this->width = _width;
  this->height = _height;
  this->color_mode = GLOBAL_COLOR_TABLE;
  this->frame_gap = 5;
  this->delay_time = 15;
}

void GeneratorBuilder::SetColorMode(enum GifColorTableMode mode) {
  this->color_mode = mode;
}

void GeneratorBuilder::SetComment(std::string _comment) {
  this->comment = _comment;
}

void GeneratorBuilder::SetDelayTime(int time) { this->delay_time = time; }

void GeneratorBuilder::SetFrameGap(int gap) { this->frame_gap = gap; }
