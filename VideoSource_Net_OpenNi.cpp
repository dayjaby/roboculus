#include <Base/Common/W32Compat.hh> // sprintf wrn
#include <Base/Debug/TimeMeasure.hh>
#include "VideoSource_Net_OpenNi.hpp"

int VideoSource_Net_OpenNi::OpenDevice(const std::string &serverName, int port) {
  client_.RegisterMsg("BIAS_IMAGE_DEPTH",BIAS::CS_BINARY);
  int ret = VideoSource_Net::OpenDevice(serverName,port);
  return ret;
}
  
int VideoSource_Net_OpenNi::InitDepthImage(BIAS::ImageBase &image) {
  if (!image.IsEmpty()) {
    BIASWARN("OpenNI Error: image already inited");
    return 1;
  }
  image.Init(640,480,1,BIAS::ImageBase::ST_float);
  image.SetColorModel(BIAS::ImageBase::CM_Depth);
  depth_send_.Init(640,480,2);

  return 0;
}

int VideoSource_Net_OpenNi::GrabSingleDepth(BIAS::Camera <float> &image){
  BIAS::ImageBase imgBase;
  int res=GrabSingleDepth(imgBase);
  if (imgBase.GetStorageType()==BIAS::ImageBase::ST_unsignedchar) {
    depth_send_=BIAS::Camera <unsigned char>(imgBase);
    for(int x=0;x<640;x++) {
      for(int y=0;y<480;y++) {
	unsigned char c0 = depth_send_.PixelValue(x,y,0);
	unsigned char c1 = depth_send_.PixelValue(x,y,1);
	image.SetPixel((c0+c1*255)/255.0,x,y,0);
      }
    }
  } else {
    BIASERR("received image is not unsigned char");
    return -2;
  }
  image.ParseMetaData();
  return res;
}
int VideoSource_Net_OpenNi::GrabSingleDepth(BIAS::ImageBase &image) {
  if (!UseTCP_) {
      BIASERR("UDP not yet");
      return -1;
  }
  static std::vector<char> data;
  int res=-1;
  while (res!=0) {
    res=client_.GetData("BIAS_IMAGE_DEPTH",data);

    #ifdef WIN32
    BIAS::Sleep(1);
    #else
    biasusleep(10);
    #endif
  }
  pImgBuffer_->rdbuf()->sputn(&data[0],data.size());

  // this does the trick
  *pImgBuffer_>>image;

  count_++;
  if (count_==20) {
      count_=0;
      ////// free some memory
      // save unprocessed data
      std::string dummy(pImgBuffer_->rdbuf()->in_avail(),0);
      pImgBuffer_->rdbuf()->sgetn(&dummy[0],pImgBuffer_->rdbuf()->in_avail());
      // delete and reallocate
      delete pImgBuffer_;
      pImgBuffer_ = new std::stringstream;
      pImgBuffer_->rdbuf()->sputn(&dummy[0],dummy.length());
  }
  return 0;
}