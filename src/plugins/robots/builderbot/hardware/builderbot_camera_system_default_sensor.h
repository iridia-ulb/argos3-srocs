/*
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @co-author Weixu Zhu (Harry) - <zhuweixu_harry@126.com>
 */

#ifndef BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotCameraSystemDefaultSensor;
}

/* forward declarations for the apriltag library */
struct apriltag_family;
struct apriltag_detector;
struct v4l2_buffer;

#include <memory>

#include <apriltag/image_u8.h>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>

#include <linux/videodev2.h>  // for v4l2

namespace argos {

   class CBuilderBotCameraSystemDefaultSensor : public CPhysicalSensor,
                                                public CCI_BuilderBotCameraSystemSensor {

   public:

      CBuilderBotCameraSystemDefaultSensor();

      virtual ~CBuilderBotCameraSystemDefaultSensor();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void GetPixels(const CVector2& c_center,
                             const CVector2& c_size,
                             std::vector<SPixel>& vec_pixels);

   private:
      const static UInt32 m_unBytesPerPixel = 2;

      /* */
      struct SPixelData {
         unsigned char U0;
         unsigned char Y0;
         unsigned char V0;
         unsigned char Y1;
      };


      /* apriltag components */
      apriltag_family* m_psTagFamily;
      apriltag_detector* m_psTagDetector;

      void Write(const image_u8_t* s_image, const std::string& str_file);

      /**************** about camera ***************/
      void LinkCamera(const char* pch_media_dev);
      void OpenCamera(const char* pch_dev);
      void GetFrame(image_u8_t* pt_y_channel);
      void CloseCamera();
      UInt32 m_unCameraHandle;
      UInt32 m_unBytesPerPixel;  // default 2 (UYVY per 2 frames), set in OpenCamera()

      struct SFrameBuffer {
         unsigned char* Start;
         unsigned int Length;
         unsigned int Offset;
      };

      std::array<SFrameBuffer, 1> m_arrBuffers;

      unsigned char* m_pchCurrentBuffer;  // for holding a buffer, in case of getpixel
      struct v4l2_buffer m_sCaptureBuf;
      /**************** end of camera ***************/



      image_u8_t* m_sCurrentFrame;

      UInt32 m_unImageHeight;
      UInt32 m_unImageWidth;

      std::vector<std::string> m_vecInputFrames;
      std::vector<std::string>::iterator m_itInputFrameIterator;
   };
}

#endif
