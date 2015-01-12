#include <bias_config.h>
#include <Geometry/Projection.hh>
#include <Base/Geometry/HomgPoint2D.hh>
#include <Base/Math/Matrix.hh>
#include <Utils/TriangleMesh.hh>

#include "VideoSource_Net_OpenNi.hpp"


int xyz(int argc, char**argv) {
  int port = atoi(argv[1]);
  std::cout << "blablabla" << std::endl;
  VideoSource_Net_OpenNi cam;
  BIAS::Camera<unsigned char> img;
  BIAS::Camera<float> greyimg;
  cam.OpenDevice("powerbot",port);
  cam.InitImage(img);

  cam.InitDepthImage(greyimg);
  cam.PreGrab();
  cam.GrabSingle(img);
  cam.GrabSingleDepth(greyimg);
  cam.PostGrab();
  cam.CloseDevice();
  // convert all pixels to 3d points
  BIAS::Projection projection;
  
  projection.XMLRead("depthproj.xml");
  
  BIAS::ProjectionParametersPerspectiveDepth *ppp = dynamic_cast<BIAS::ProjectionParametersPerspectiveDepth *>(projection.GetParameters());
  BIAS::TriangleMesh mesh;
  mesh.GenerateDenseMesh(greyimg,projection,img);
  BIAS::ImageIO::ExportLibJPEG("test.jpg",greyimg);
  BIAS::ImageIO::ExportLibJPEG("test2.jpg",img);
  return 0;
}