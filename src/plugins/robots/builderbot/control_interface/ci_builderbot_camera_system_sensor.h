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

#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/control_interface/ci_sensor.h>

#include <string>

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

   public:

      CCI_BuilderBotCameraSystemSensor() :
         m_bEnabled(false) {}

      virtual ~CCI_BuilderBotCameraSystemSensor() {}

      virtual void Reset() {
         /* clear the readings */
         m_tTags.clear();
         /* zero the timestamp */
         m_fTimestamp = 0.0f;
      }

      virtual CColor DetectLed(const CVector2& c_center,
                               const CVector2& c_size) = 0;

      virtual CVector2 GetResolution() const = 0;

      Real GetTimestamp() const {
         return m_fTimestamp;
      }

      const STag::TVector& GetTags() const {
         return m_tTags;
      }

      void SetEnabled(bool b_enabled) {
         m_bEnabled = b_enabled;
      }

      void Enable() {
         SetEnabled(true);
      }

      void Disable() {
         SetEnabled(false);
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   protected:
      /* whether or not the camera sensor is enabled */
      bool m_bEnabled;    
      /* the detected tags in the current frame */
      STag::TVector m_tTags;
      /* the timestamp of the current frame */
      Real m_fTimestamp;

   };

}

#endif
