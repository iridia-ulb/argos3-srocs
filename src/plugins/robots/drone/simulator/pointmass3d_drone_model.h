/**
 * @file <argos3/plugins/robots/drone/simulator/pointmass3d_drone_model.h>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef POINTMASS3D_DRONE_MODEL_H
#define POINTMASS3D_DRONE_MODEL_H

namespace argos {
   class CPointMass3DEngine;
   class CDroneEntity;
   class CDroneFlightSystemEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>

namespace argos {

   class CPointMass3DDroneModel : public CPointMass3DModel {

   public:
      
      CPointMass3DDroneModel(CPointMass3DEngine& c_engine,
                             CDroneEntity& c_drone);

      virtual ~CPointMass3DDroneModel() {}
      
      virtual void Reset();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void UpdatePhysics();

      virtual void Step();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual void CalculateBoundingBox();

      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

   private:

      const static Real m_fBodyHeight;
      const static Real m_fArmLength;
      const static Real m_fMass;

   private:

      /* reference to the drone entity */
      CDroneFlightSystemEntity& m_cFlightSystemEntity;

      /* position and orientation of the drone in the GCS */
      CVector3 m_cDronePosition;
      CQuaternion m_cDroneOrientation;

   };

}

#endif
