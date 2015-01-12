#ifndef VIDEOSOURCE_NET_OPENNI_HPP
#define VIDEOSOURCE_NET_OPENNI_HPP

#include <Image/Camera.hh>
#include <vector>
#include <string>
#include <VideoSource/VideoSource_Net.hh>

class VideoSource_Net_OpenNi : public BIAS::VideoSource_Net {
public:
  VideoSource_Net_OpenNi(bool threaded = true)
      :VideoSource_Net(threaded) {}

  virtual ~VideoSource_Net_OpenNi(){}

  virtual int OpenDevice(const std::string &serverName, int port);
  virtual int InitDepthImage(BIAS::ImageBase &image);
  virtual int GrabSingleDepth(BIAS::Camera <float> &image);
protected:
  virtual int GrabSingleDepth(BIAS::ImageBase &image);
  BIAS::Camera<unsigned char> depth_send_;

};

#endif