/**
 * @file <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_FLIGHT_SYSTEM_ENTITY_H
#define DRONE_FLIGHT_SYSTEM_ENTITY_H

namespace argos {
   class CDroneFlightSystemEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CDroneFlightSystemEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CDroneFlightSystemEntity(CComposableEntity* pc_parent);

      CDroneFlightSystemEntity(CComposableEntity* pc_parent,
                               const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CDroneFlightSystemEntity() {}

      virtual void Update() {}

      virtual void Reset();

      void SetAccelerometerReading(const CVector3& c_reading) {
         m_cAccelerometerReading = c_reading;
      }

      const CVector3& GetAccelerometerReading() const {
         return m_cAccelerometerReading;
      }

      void SetGyroscopeReading(const CVector3& c_reading) {
         m_cGyroscopeReading = c_reading;
      }

      const CVector3& GetGyroscopeReading() const {
         return m_cGyroscopeReading;
      }
     
      void SetTargetVelocity(const CVector3& c_velocity) {
         m_cTargetVelocity = c_velocity;
      }

      const CVector3& GetTargetVelocity() const {
         return m_cTargetVelocity;
      }

      void SetTargetYawRate(Real f_yaw_rate) {
         m_fTargetYawRate = f_yaw_rate;
      }

      Real GetTargetYawRate() const {
         return m_fTargetYawRate;
      }
      
      virtual std::string GetTypeDescription() const {
         return "flight_system";
      }

   private:
      CVector3 m_cAccelerometerReading;
      CVector3 m_cGyroscopeReading;

      CVector3 m_cTargetVelocity;
      Real m_fTargetYawRate;     
      
   };
}

#endif
