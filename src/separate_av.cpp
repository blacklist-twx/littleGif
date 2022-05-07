////
////  separate_av.cpp
////  littleGif
////
////  Created by 1234 on 2022/4/30.
////
//
//#include "separate_av.h"
//
//void separate(){
//    char            *mp4_filename     = "/Users/a1234/Desktop/workspace/littleGif/assets/testVideo.mp4";               // mp4视频，200kbps.768x320
//        char            *h264_filename    = "out.h264";                 // 视频流输出文件
////        char            *aac_filename     = "out.aac";                  // 音频流输出文件
////        FILE            *aac_fd           = fopen(aac_filename, "wb");  // 打开aac音频输出文件
//        FILE            *h264_fd          = fopen(h264_filename, "wb"); // 打开h264视频输出文件
//        AVFormatContext *fmt_ctx          = avformat_alloc_context();   // 解封装上下文
//        AVPacket        *pkt              = av_packet_alloc();          // 为packet分配空间，用于存放音视频帧数据
//        AVBSFContext    *bsf_ctx          = NULL;                       // bit流过滤器上下文
//        int             video_index       = -1;                         // 视频流索引
//        int             audio_index       = -1;                         // 音频流索引
//        const AVBitStreamFilter *bsfilter = av_bsf_get_by_name("h264_mp4toannexb");  // 查找h264比特流过滤器
//
//        if (!h264_fd)  { return -1; }  // h264视频输出文件打开失败
////        if (!aac_fd)   { return -1; }  // aac音频输出文件打开失败
//        if (!fmt_ctx)  { return -1; }  // 解封装上下文创建失败
//        if (!pkt)      { return -1; }  // AVPacket创建失败
//        if (!bsfilter) { return -1; }  // bit流过滤器创建失败
//
//
//        int ret = avformat_open_input(&fmt_ctx, mp4_filename, NULL, NULL); // 打开音视频流
//        if(ret < 0) {
//            avformat_close_input(&fmt_ctx); // 关闭音视频流
//            return -1;
//        }
//
//        // 获取视频和音频索引
//        video_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
//        audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
//        if(video_index == -1 || audio_index == -1) {
//            avformat_close_input(&fmt_ctx); // 关闭音视频流
//            return -1; // 音视频索引获取失败
//        }
//
//        ret = av_bsf_alloc(bsfilter, &bsf_ctx); // 为h264比特流过滤器分配空间
//        if(ret < 0) {
//            avformat_close_input(&fmt_ctx); // 关闭音视频流
//            return -1; // 空间分配失败
//        }
//
//        // 将编码器的参数拷贝给bit流过滤器
//        ret = avcodec_parameters_copy(bsf_ctx->par_in, fmt_ctx->streams[video_index]->codecpar);
//        if(ret < 0) {
//            avformat_close_input(&fmt_ctx); // 关闭音视频流
//            av_bsf_free(&bsf_ctx);
//            return -1; // 参数拷贝失败
//        }
//
//        ret = av_bsf_init(bsf_ctx); // 初始化bit流过滤器
//        if(ret < 0) {
//            avformat_close_input(&fmt_ctx); // 关闭音视频流
//            av_bsf_free(&bsf_ctx);
//            return -1;
//        }
//
//        av_init_packet(pkt);
//        while (1)
//        {
//            ret = av_read_frame(fmt_ctx, pkt);
//            if (ret < 0 ) { break; }              // 音视频流读取结束
//
//            if (pkt->stream_index == video_index) // 视频流
//            {
//                ret = av_bsf_send_packet(bsf_ctx, pkt);
//                while (1) {
//                    ret = av_bsf_receive_packet(bsf_ctx, pkt); // 检索已过滤的数据包
//                    if(ret != 0) { break; }
//                    fwrite(pkt->data, 1, pkt->size, h264_fd);  // 写入h264数据
//                    av_packet_unref(pkt); // 释放buffer
//                }
//            }
////            else if (pkt->stream_index == audio_index)  // 音频流
////            {
////                char adts_header_buf[7] = {0};  // 每个packet都要添加一个7字节的音频头
////                adts_header(adts_header_buf, pkt->size,
////                            fmt_ctx->streams[audio_index]->codecpar->profile,     // 编码器bit流限制
////                            fmt_ctx->streams[audio_index]->codecpar->sample_rate, // 采样率
////                            fmt_ctx->streams[audio_index]->codecpar->channels);   // 通道数
////                fwrite(adts_header_buf, 1, 7, aac_fd);   // 写adts header，7字节音频头
////                fwrite(pkt->data, 1, pkt->size, aac_fd); // 写adts data，音频数据
////                av_packet_unref(pkt); // 释放buffer
////            }
//            else // 其他流
//            {
//                av_packet_unref(pkt); // 释放buffer
//            }
//        }
//
//        printf("Split OK!\n");
//
//        if(h264_fd)
//            fclose(h264_fd);
////        if(aac_fd)
////            fclose(aac_fd);
//        if(pkt)
//            av_packet_free(&pkt);
//        if(fmt_ctx)
//            avformat_close_input(&fmt_ctx);
//
//}
