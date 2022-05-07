
#include <iostream>
#include "create_gif.h"
//#include "libavformat/avformat.h"
//#include "libavcodec/avcodec.h"
//extern "C" {
//#include "separate_av.h"
//}
//
//static AVFormatContext *fmt_ctx;
//static std::string mp4_file_path = "/Users/a1234/Desktop/workspace/littleGif/assets/testVideo.mp4";
//void convert_mp4_to_h264(){
//    const AVCodec *dec;
//    int ret;
//
//    if ((ret = avformat_open_input(&fmt_ctx, mp4_file_path.c_str(), NULL, NULL)) < 0) {
//        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
//        return ret;
//    }
//
//    AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
//
//    AVPacket packet;
//    while( av_read_frame(format_ctx_, &packet) >= 0 ) {
//        if( packet.stream_index == video_stream_index_ ) {
//            uint8_t* outbuf = nullptr;
//            int outlen = 0;
//            av_bitstream_filter_filter(h264bsfc, codec_ctx_, NULL,
//                &outbuf, &outlen, packet.data, packet.size, 0);
//            fwrite(packet.data, packet.size, 1, fp);
//            if(outbuf){
//                av_free(outbuf);
//            }
//        }
//
//        av_free_packet(&packet);
//    }
//
//    av_bitstream_filter_close(h264bsfc);
//}



//void read_h264(){
////    av_register_all();
////    av_init_packet(<#AVPacket *pkt#>)
//
//    //查找编码器
//    AVCodecID codec_id = AV_CODEC_ID_H264;
//    const AVCodec *pCodec = avcodec_find_decoder(codec_id);
//    if (!pCodec)
//    {
//        printf("Codec not found\n");
//        return ;
//    }
//
//    //分配编码器空间
//    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
//    if (!pCodecCtx)
//    {
//        printf("Could not allocate video codec context\n");
//        return ;
//    }
//
//    //设置解码器属性
//    pCodecCtx->codec_type  = AVMEDIA_TYPE_VIDEO;
//    pCodecCtx->pix_fmt     = AV_PIX_FMT_YUV420P; //解码器的输出格式
//
//    //打开解码器
//    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
//        {
//            std::cout<<"Could not open codec"<<std::endl;
//            return ;
//        }
//   //分配pFrame结构体
//    AVFrame *pFrame = av_frame_alloc();
//   if (!pFrame)
//   {
//       printf("Could not allocate video frame\n");
//       return ;
//   }
////    getNextNal()
////    //打开264文件
////    File *fp_in = fopen(filename_in, "rb");
////    if (!fp_in)
////    {
////        printf("Could not open %s\n", filename_in);
////        return ;
////    }
////    int inLen=0;
////    unsigned char* inBuf=NULL;
////    unsigned char* outBuf=NULL;
////    inBuf  = (unsigned char*)calloc ( 500000, sizeof(char));
////    outBuf = (unsigned char*)calloc ( 1000000, sizeof(char));
//}




int main(){
    std::cout<<"HelloWorld"<<std::endl;
//    separate();
    GifGenerator* gif_generator = new GifGenerator("/Users/a1234/Desktop/gif",1280,720);
    gif_generator->CreateGif();
    return 0;
}
