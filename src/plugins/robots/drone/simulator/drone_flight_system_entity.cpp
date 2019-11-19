/**
 * @file <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_flight_system_entity.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneFlightSystemEntity::CDroneFlightSystemEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_cAccelerometerReading(CVector3::ZERO),
      m_cGyroscopeReading(CVector3::ZERO),
      m_cTargetVelocity(CVector3::ZERO),
      m_fTargetYawRate(0.0) {}

   /****************************************/
   /****************************************/

   CDroneFlightSystemEntity::CDroneFlightSystemEntity(CComposableEntity* pc_parent,
                                                      const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_cAccelerometerReading(CVector3::ZERO),
      m_cGyroscopeReading(CVector3::ZERO),
      m_cTargetVelocity(CVector3::ZERO),
      m_fTargetYawRate(0.0) {}
  
   /****************************************/
   /****************************************/
    
   void CDroneFlightSystemEntity::Reset() {
      m_cAccelerometerReading = CVector3::ZERO;
      m_cGyroscopeReading = CVector3::ZERO;
      m_cTargetVelocity = CVector3::ZERO;
      m_fTargetYawRate = 0.0;
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CDroneFlightSystemEntity);

   /****************************************/
   /****************************************/

}
   
