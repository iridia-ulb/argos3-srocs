/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_CAMERA_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotCameraSystemDefaultSensor;
}

/* forward declarations for the apriltag library */
struct apriltag_family;
struct apriltag_detector;

#include <memory>

#include <apriltag/image_u8.h>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/hardware/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>

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
      /* apriltag components */
      apriltag_family* m_psTagFamily;
      apriltag_detector* m_psTagDetector;

      /* buffers for the YUV pixel data */
      struct SFrame {
         /* pointers to pixel data */
         std::unique_ptr<image_u8_t, void (*)(image_u8_t*)> Y, U, V;
         /* default constructor */
         SFrame() :
            Y(image_u8_create(0, 0), image_u8_destroy),
            U(image_u8_create(0, 0), image_u8_destroy),
            V(image_u8_create(0, 0), image_u8_destroy) {}
         /* set size of frame */
         void SetSize(size_t un_width, size_t un_height) {
            Y.reset(image_u8_create(un_width, un_height));
            U.reset(image_u8_create(un_width, un_height));
            V.reset(image_u8_create(un_width, un_height));
         }
      };

      void Write(const SFrame& s_frame, const std::string& str_file);

      SFrame m_sCurrentFrame;

      UInt32 m_unImageHeight;
      UInt32 m_unImageWidth;

      std::vector<std::string> m_vecInputFrames;
      std::vector<std::string>::iterator m_itInputFrameIterator;
   };
}

#endif
