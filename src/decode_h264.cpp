//
//  decode_h264.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/1.
//

#include "decode_h264.h"




void Decode(BaseDecoderTest::Callback* callback){
    std::string filename = "/Users/a1234/Desktop/workspace/littleGif/assets/testVideo.h264";
    
    BaseDecoderTest* test = new BaseDecoderTest();
    test->SetUp();
    test->DecodeFile (filename.c_str(), callback);
}


