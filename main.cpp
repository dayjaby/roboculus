#include <VideoSource/VideoSource_Net.hh>
#include <Base/Image/Image.hh>
#include <Base/Image/ImageBase.hh>
#include <Base/Image/ImageIO.hh>
#include <Image/Camera.hh>
#include <vector>
#include <string>
#include <Base/Image/ImageConvert.hh>


int main(int argc, char**argv) {
  BIAS::VideoSource_Net* cam = new BIAS::VideoSource_Net;
  //BIAS::Image<float> greyimg(640,480,1,BIAS::ImageBase::ST_float);
  BIAS::Camera<unsigned char> img;
  std::cout << img.GetStorageType() << std::endl;
  
  cam->OpenDevice("powerbot",44887);
  cam->CloseDevice();
  cam->InitImage(img);
  cam->PreGrab();
  cam->GrabSingle(img);
  cam->PostGrab();
  cam->CloseDevice();
  //BIAS::ImageConvert::ToGrey(img,greyimg);
  BIAS::ImageIO::ExportLibJPEG("test.jpg",img);
  return 0;
}