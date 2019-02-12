/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_CAMERA_SYSTEM_SENSOR_H
#define CCI_BUILDERBOT_CAMERA_SYSTEM_SENSOR_H

namespace argos {
   class CCI_BuilderBotCameraSystemSensor;
}

#include <chrono>
#include <string>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotCameraSystemSensor : public CCI_Sensor {

   public:

      struct STag {
         /* constructor */
         STag(const std::string& str_payload,
              const CVector2& c_center,
              const std::array<CVector2, 4>& arr_corners) :
            Payload(str_payload),
            Center(c_center),
            Corners(arr_corners) {}
         /* members */
         std::string Payload;
         CVector2 Center;
         std::array<CVector2, 4> Corners;
         using TVector = std::vector<STag>;
      };

      struct SLed {
         /* constructor */
         SLed(const CColor& c_color,
              const CVector2& c_center) :
            Color(c_color),
            Center(c_center) {}
         /* members */
         CColor Color;
         CVector2 Center;
         using TVector = std::vector<SLed>;
      };

      struct SPixel {
         /* constructor */
         SPixel(UInt8 un_y, UInt8 un_u, UInt8 un_v) :
            Y(un_y), U(un_u), V(un_v) {}
         /* members */
         UInt8 Y, U, V;
      };

   public:

      CCI_BuilderBotCameraSystemSensor() :
         m_bEnable(true) {}

      virtual ~CCI_BuilderBotCameraSystemSensor() {}

      virtual bool DetectLed(const CVector2& c_center,
                             const CVector2& c_size) = 0;

      virtual CVector2 GetResolution() const = 0;

      const std::chrono::time_point<std::chrono::steady_clock>& GetTimestamp() const {
         return m_tpTimestamp;
      }

      const STag::TVector& GetTags() const {
         return m_tTags;
      }

      const SLed::TVector& GetLeds() const {
         return m_tLeds;
      }

      bool m_bEnable;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   protected:
      /* the detected leds in the current frame */
      SLed::TVector m_tLeds;
      /* the detected tags in the current frame */
      STag::TVector m_tTags;
      /* the timestamp of the current frame */
      std::chrono::time_point<std::chrono::steady_clock> m_tpTimestamp;

   };

}

#endif
