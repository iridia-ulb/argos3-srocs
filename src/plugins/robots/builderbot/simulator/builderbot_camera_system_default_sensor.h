/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_camera_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CAMERAS_DEFAULT_SENSOR_H
#define CAMERAS_DEFAULT_SENSOR_H

namespace argos {
   class CEmbodiedEntity;
   class CControllableEntity;
   struct SAnchor;
}



#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/plane.h>

#include <argos3/plugins/simulator/entities/tag_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_entity.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>

#include <vector>
#include <array>

namespace argos {

   class CBuilderBotCameraSystemDefaultSensor : public CSimulatedSensor,
                                                public CCI_BuilderBotCameraSystemSensor,
                                                public CPositionalIndex<CTagEntity>::COperation,
                                                public CPositionalIndex<CDirectionalLEDEntity>::COperation {

   public:

      CBuilderBotCameraSystemDefaultSensor();

      virtual ~CBuilderBotCameraSystemDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual bool operator()(CTagEntity& c_tag);

      virtual bool operator()(CDirectionalLEDEntity& c_led);

      virtual bool DetectLed(const CVector2& c_center,
                             const CVector2& c_size);

      virtual CVector2 GetResolution() const;

   private:

      CRadians GetAngleWithCamera(const CPositionalEntity& c_entity) const;

      CVector2 ProjectOntoSensor(const CVector3& c_point) const;

      bool IsInsideFrustum(const CVector3& c_point) const;

   private:
      CControllableEntity* m_pcControllableEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;

      SAnchor* m_psEndEffectorAnchor;

      CPositionalIndex<CDirectionalLEDEntity>* m_pcDirectionalLEDIndex;
      CPositionalIndex<CTagEntity>* m_pcTagIndex;

      bool m_bShowFrustum, m_bShowTagRays, m_bShowLEDRays;

      /* required for operator() updates */
      CTransformationMatrix3 m_cOffset;
      CVector3 m_cCameraLocation;
      CTransformationMatrix3 m_cCameraToWorldTransform;
      CSquareMatrix<3> m_cProjectionMatrix;
      std::array<CPlane, 6> m_arrFrustumPlanes;

      /* for generating the fustrum */
      Real m_fNearPlaneWidth;
      Real m_fNearPlaneHeight;
      Real m_fFarPlaneWidth;
      Real m_fFarPlaneHeight;
      
      /* buffers */
      std::array<CVector3, 4> m_arrTagCorners;
      std::array<CVector2, 4> m_arrTagCornerPixels;
      CRay3 m_cOcclusionCheckRay;
      SEmbodiedEntityIntersectionItem m_sIntersectionItem;
      
      /* AprilTag corner offsets / ordering */
      const std::array<CVector3, 4> m_arrTagCornerOffsets = {{
         {-0.5, -0.5, 0},
         {-0.5,  0.5, 0},
         { 0.5,  0.5, 0},
         { 0.5, -0.5, 0},
      }};

      /* For LEDs found inside the frustum */
      SLed::TVector m_tLedCache;


   };
}

#endif
