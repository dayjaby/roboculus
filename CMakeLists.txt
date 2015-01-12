PROJECT(roboculus)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH})
FIND_PACKAGE(BIAS)
IF(BIAS_FOUND)
  INCLUDE(${BIAS_USE_FILE})
ENDIF(BIAS_FOUND)

SET(ROBOCULUS_EXTERN_INCLUDE_DIR ${BIAS_INCLUDE_DIR} "${BIAS_INCLUDE_DIR}/BIAS")
SET(ROBOCULUS_EXTERN_LINK_DIR ${ROBOCULUS_EXTERN_LINK_DIR} ${BIAS_LINK_DIRECTORIES} )


# start the find script with the above settings
FIND_PACKAGE(wxWidgets REQUIRED)

IF(wxWidgets_FOUND)

  SET(BIAS_HAVE_WXWIDGETS TRUE) # for header bias_config.h
  SET(ROBOCULUS_EXTERN_INCLUDE_DIR  ${ROBOCULUS_EXTERN_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS})
  IF    (wxWidgets_INCLUDE_DIRS AND NOT WIN32)
    FOREACH(DIR ${wxWidgets_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${DIR}")
    ENDFOREACH(DIR)
  ENDIF (wxWidgets_INCLUDE_DIRS AND NOT WIN32)
  IF   (wxWidgets_LIBRARY_DIRS)
    SET(ROBOCULUS_EXTERN_LINK_DIR   ${ROBOCULUS_EXTERN_LINK_DIR}    ${wxWidgets_LIBRARY_DIRS})
    LINK_DIRECTORIES(${wxWidgets_LIBRARY_DIRS})
  ENDIF(wxWidgets_LIBRARY_DIRS)
  # this is not required with new FindwxWidgets.cmake - but does no harm.
  IF   (wxWidgets_CXX_FLAGS)
    STRING(REGEX REPLACE "-I" "-isystem" wxWidgets_CXX_FLAGS "${wxWidgets_CXX_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")
  ENDIF(wxWidgets_CXX_FLAGS)

  IF   (wxWidgets_DEFINITIONS)
    IF(COMMAND cmake_policy)
      # this should be fixed sometime to allow for NEW behaviour of cmake 2.6
      cmake_policy(SET CMP0005 OLD)
    ENDIF(COMMAND cmake_policy)
    ADD_DEFINITIONS(${wxWidgets_DEFINITIONS})
  ENDIF(wxWidgets_DEFINITIONS)

  #avoid double definition of ssize_t
  IF ( CMAKE_SIZEOF_VOID_P EQUAL 8)
    IF (WIN32)
  ADD_DEFINITIONS(-DHAVE_SSIZE_T)
    ENDIF (WIN32)
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8)

ELSE(wxWidgets_FOUND)
  MSG_FATAL("wxWidgets requested but not found.")
  # SET(USE_WXWIDGETS OFF)
ENDIF(wxWidgets_FOUND)
SET(OCULUSWORLDPATH ovr)
FIND_PACKAGE(OculusSDK REQUIRED)
SET(ROBOCULUS_EXTERN_INCLUDE_DIR  ${ROBOCULUS_EXTERN_INCLUDE_DIR} ${OCULUS_SDK_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${ROBOCULUS_EXTERN_INCLUDE_DIR} ${OCULUSWORLDPATH}/TinyXml)
LINK_DIRECTORIES(${ROBOCULUS_EXTERN_LINK_DIR})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BIAS_CXX_FLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BIAS_C_FLAGS}" )
SET(CMAKE_BUILD_TYPE ${BIAS_BUILD_TYPE} CACHE STRING "build type for roboculus" FORCE)
SET(LIBS ${LIBS} ${OCULUS_SDK_LIBRARIES})

SET(LIBS ${LIBS} OpenNI BIASVideoSourceD BIASImageBaseD BIASCommonD BIASDebugD BIASFeatureDetectorD BIASFilterD BIASGeometryBaseD BIASGeometryD BIASGuiD BIASImageD 
BIASImageUtilsBaseD BIASMatcher2DD BIASMathAlgoD BIASMathBaseD BIASNetworkCommD BIASOpenGLFrameworkD BIASStateEstimatorD BIASUtilsD GL X11 Xxf86vm rt Xrandr udev ${wxWidgets_LIBRARIES})
MESSAGE(TestVar "Libs: ${LIBS} Flags: ${CMAKE_CXX_FLAGS}")
MESSAGE(TestVar "${OCULUS_SDK_LIBRARIES}")
ADD_EXECUTABLE(roboculus main.cpp VideoSource_Net_OpenNi.cpp ovr/OculusWorldDemo/OculusWorldDemo_Scene.cpp ovr/OculusWorldDemo/OculusWorldDemo.cpp ovr/OculusWorldDemo/Player.cpp 
${OCULUSWORLDPATH}/CommonSrc/Util/RenderProfiler.cpp 
${OCULUSWORLDPATH}/CommonSrc/Util/OptionMenu.cpp
${OCULUSWORLDPATH}/CommonSrc/Platform/Linux_Gamepad.cpp
${OCULUSWORLDPATH}/CommonSrc/Platform/Linux_Platform.cpp
${OCULUSWORLDPATH}/CommonSrc/Platform/Platform.cpp
${OCULUSWORLDPATH}/CommonSrc/Render/Render_Device.cpp
${OCULUSWORLDPATH}/CommonSrc/Render/Render_GL_Device.cpp
${OCULUSWORLDPATH}/CommonSrc/Render/Render_LoadTextureDDS.cpp
${OCULUSWORLDPATH}/CommonSrc/Render/Render_LoadTextureTGA.cpp
${OCULUSWORLDPATH}/CommonSrc/Render/Render_XmlSceneLoader.cpp
${OCULUSWORLDPATH}/TinyXml/tinyxml2.cpp
)

TARGET_LINK_LIBRARIES(roboculus ${LIBS})
	 

INSTALL(TARGETS roboculus RUNTIME DESTINATION bin)
