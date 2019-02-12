/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_camera_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_camera_system_default_sensor.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

#include <chrono>

#define CAMERA_RESOLUTION_X 640.0f
#define CAMERA_RESOLUTION_Y 360.0f
#define CAMERA_FOCAL_LENGTH_X 883.961f
#define CAMERA_FOCAL_LENGTH_Y 883.961f
#define CAMERA_PRINCIPAL_POINT_X 319.5f
#define CAMERA_PRINCIPAL_POINT_Y 179.5f
#define CAMERA_RANGE_MIN 0.05f
#define CAMERA_RANGE_MAX 0.25f

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::CBuilderBotCameraSystemDefaultSensor() :
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_psEndEffectorAnchor(nullptr),
      m_pcDirectionalLEDIndex(nullptr),
      m_pcTagIndex(nullptr),
      m_bShowFrustum(false),
      m_bShowTagRays(false),
      m_bShowLEDRays(false) {
      /* set up the project matrix */
      m_cProjectionMatrix.SetIdentityMatrix();
      m_cProjectionMatrix(0,0) = CAMERA_FOCAL_LENGTH_X;
      m_cProjectionMatrix(1,1) = CAMERA_FOCAL_LENGTH_Y;
      m_cProjectionMatrix(0,2) = CAMERA_PRINCIPAL_POINT_X;
      m_cProjectionMatrix(1,2) = CAMERA_PRINCIPAL_POINT_Y;
      /* calculate fustrum constants */
      Real fWidthToDepthRatio = (0.5 * CAMERA_RESOLUTION_X) / CAMERA_FOCAL_LENGTH_X;
      Real fHeightToDepthRatio = (0.5 * CAMERA_RESOLUTION_Y) / CAMERA_FOCAL_LENGTH_Y;
      m_fNearPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MIN;
      m_fNearPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MIN;
      m_fFarPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MAX;
      m_fFarPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MAX;
      /* offset from end effector anchor */
      CVector3 cOffsetPosition(0.0f, 0.0f, 0.075f);
      CQuaternion cOffsetOrientation;
      cOffsetOrientation.FromEulerAngles(-0.50f * CRadians::PI,
                                          0.75f * CRadians::PI,
                                          0.00f * CRadians::PI);
      /* transformation matrix */
      m_cOffset.SetFromComponents(cOffsetOrientation, cOffsetPosition);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         /* get required components */
         m_pcControllableEntity =
            &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcEmbodiedEntity =
            &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         /* get end effector anchor */
         m_psEndEffectorAnchor =
            &(m_pcEmbodiedEntity->GetAnchor("end_effector"));
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting robot for the builderbot camera system sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the parent class */
         CCI_BuilderBotCameraSystemSensor::Init(t_tree);
         /* show the frustum? */
         GetNodeAttributeOrDefault(t_tree, "show_frustum", m_bShowFrustum, m_bShowFrustum);
         GetNodeAttributeOrDefault(t_tree, "show_tag_rays", m_bShowTagRays, m_bShowTagRays);
         GetNodeAttributeOrDefault(t_tree, "show_led_rays", m_bShowLEDRays, m_bShowLEDRays);
         /* get indices */
         m_pcTagIndex = 
            &(CSimulator::GetInstance().GetMedium<CTagMedium>("tags").GetIndex());
         m_pcDirectionalLEDIndex =
            &(CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>("directional_leds").GetIndex());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the builderbot camera system sensor", ex);
      }
      Update();
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Update() {
      using std::chrono::time_point;
      using std::chrono::duration_cast;
      using std::chrono::duration;
      using std::chrono::steady_clock;
      /* increment the timestamp */
      Real fExperimentRunTime = CPhysicsEngine::GetSimulationClockTick() *
         CSimulator::GetInstance().GetSpace().GetSimulationClock();
      m_tpTimestamp = time_point<steady_clock>() + 
         duration_cast<steady_clock::duration>(duration<Real>(fExperimentRunTime));
      /* clear out the readings from the last update */
      m_tTags.clear();
      m_tLeds.clear();
      m_tLedCache.clear();
      /* get a reference to the checked rays for the controller */
      std::vector<std::pair<bool, CRay3> >& vecCheckedRays =
       m_pcControllableEntity->GetCheckedRays();
      /* sensor parameters */
      CTransformationMatrix3 cWorldToAnchorTransform;
      CTransformationMatrix3 cWorldToCameraTransform;
      CVector3 cLookAt, cUp;
      CVector3 cX, cY, cZ;
      CVector3 cNearCenter, cNearTopLeft, cNearTopRight, cNearBottomLeft, cNearBottomRight;
      CVector3 cFarCenter, cFarTopLeft, cFarTopRight, cFarBottomLeft, cFarBottomRight;
      CVector3 cBoundingBoxMinCorner, cBoundingBoxMaxCorner;
      CVector3 cBoundingBoxPosition, cBoundingBoxHalfExtents;
      /* calculate transform matrices */
      cWorldToAnchorTransform.SetFromComponents(m_psEndEffectorAnchor->Orientation, m_psEndEffectorAnchor->Position);
      cWorldToCameraTransform = cWorldToAnchorTransform * m_cOffset;
      m_cCameraToWorldTransform = cWorldToCameraTransform.GetInverse();
      /* calculate camera direction vectors */
      m_cCameraLocation = cWorldToCameraTransform.GetTranslationVector();
      cLookAt = cWorldToCameraTransform * CVector3::Z;
      cUp = -CVector3::Y;
      cUp.Rotate(cWorldToCameraTransform.GetRotationMatrix());
      /* calculate direction vectors */
      cZ = m_cCameraLocation - cLookAt;
      cZ.Normalize();
      cX = cUp;
      cX.CrossProduct(cZ);
      cX.Normalize();
      cY = cZ;
      cY.CrossProduct(cX);
      /* calculate frustum coordinates */
      cNearCenter = m_cCameraLocation - cZ * CAMERA_RANGE_MIN;
      cFarCenter = m_cCameraLocation - cZ * CAMERA_RANGE_MAX;
      cNearTopLeft = cNearCenter + (cY * m_fNearPlaneHeight) - (cX * m_fNearPlaneWidth);
      cNearTopRight = cNearCenter + (cY * m_fNearPlaneHeight) + (cX * m_fNearPlaneWidth);
      cNearBottomLeft = cNearCenter - (cY * m_fNearPlaneHeight) - (cX * m_fNearPlaneWidth);
      cNearBottomRight = cNearCenter - (cY * m_fNearPlaneHeight) + (cX * m_fNearPlaneWidth);
      cFarTopLeft = cFarCenter + (cY * m_fFarPlaneHeight) - (cX * m_fFarPlaneWidth);
      cFarTopRight = cFarCenter + (cY * m_fFarPlaneHeight) + (cX * m_fFarPlaneWidth);
      cFarBottomLeft = cFarCenter - (cY * m_fFarPlaneHeight) - (cX * m_fFarPlaneWidth);
      cFarBottomRight = cFarCenter - (cY * m_fFarPlaneHeight) + (cX * m_fFarPlaneWidth);
      /* show frustum if enabled by adding outline to the checked rays vector */
      if(m_bShowFrustum) {
         vecCheckedRays.emplace_back(false, CRay3(cNearTopLeft, cNearTopRight));
         vecCheckedRays.emplace_back(false, CRay3(cNearTopRight, cNearBottomRight));
         vecCheckedRays.emplace_back(false, CRay3(cNearBottomRight, cNearBottomLeft));
         vecCheckedRays.emplace_back(false, CRay3(cNearBottomLeft, cNearTopLeft));
         vecCheckedRays.emplace_back(false, CRay3(cFarTopLeft, cFarTopRight));
         vecCheckedRays.emplace_back(false, CRay3(cFarTopRight, cFarBottomRight));
         vecCheckedRays.emplace_back(false, CRay3(cFarBottomRight, cFarBottomLeft));
         vecCheckedRays.emplace_back(false, CRay3(cFarBottomLeft, cFarTopLeft));
         vecCheckedRays.emplace_back(false, CRay3(cNearTopLeft, cFarTopLeft));
         vecCheckedRays.emplace_back(false, CRay3(cNearTopRight, cFarTopRight));
         vecCheckedRays.emplace_back(false, CRay3(cNearBottomRight, cFarBottomRight));
         vecCheckedRays.emplace_back(false, CRay3(cNearBottomLeft, cFarBottomLeft));
      }
      /* generate a bounding box for the frustum */
      cBoundingBoxMinCorner = cNearCenter;
      cBoundingBoxMaxCorner = cNearCenter;
      for(const CVector3& c_point : {
            cNearTopLeft, cNearTopRight, cNearBottomLeft, cNearBottomRight, 
            cFarTopLeft, cFarTopRight, cFarBottomLeft, cFarBottomRight
         }) {
         if(c_point.GetX() > cBoundingBoxMaxCorner.GetX()) {
            cBoundingBoxMaxCorner.SetX(c_point.GetX());
         }
         if(c_point.GetX() < cBoundingBoxMinCorner.GetX()) {
            cBoundingBoxMinCorner.SetX(c_point.GetX());
         }
         if(c_point.GetY() > cBoundingBoxMaxCorner.GetY()) {
            cBoundingBoxMaxCorner.SetY(c_point.GetY());
         }
         if(c_point.GetY() < cBoundingBoxMinCorner.GetY()) {
            cBoundingBoxMinCorner.SetY(c_point.GetY());
         }
         if(c_point.GetZ() > cBoundingBoxMaxCorner.GetZ()) {
            cBoundingBoxMaxCorner.SetZ(c_point.GetZ());
         }
         if(c_point.GetZ() < cBoundingBoxMinCorner.GetZ()) {
            cBoundingBoxMinCorner.SetZ(c_point.GetZ());
         }
      }
      cBoundingBoxMaxCorner *= 0.5f;
      cBoundingBoxMinCorner *= 0.5f;
      cBoundingBoxPosition = (cBoundingBoxMaxCorner + cBoundingBoxMinCorner);
      cBoundingBoxHalfExtents = (cBoundingBoxMaxCorner - cBoundingBoxMinCorner);
      /* generate frustum planes */
      m_arrFrustumPlanes[0].SetFromThreePoints(cNearTopRight, cNearTopLeft, cFarTopLeft);
      m_arrFrustumPlanes[1].SetFromThreePoints(cNearBottomLeft, cNearBottomRight, cFarBottomRight);
      m_arrFrustumPlanes[2].SetFromThreePoints(cNearTopLeft, cNearBottomLeft, cFarBottomLeft);
      m_arrFrustumPlanes[3].SetFromThreePoints(cNearBottomRight, cNearTopRight, cFarBottomRight);
      m_arrFrustumPlanes[4].SetFromThreePoints(cNearTopLeft, cNearTopRight, cNearBottomRight);
      m_arrFrustumPlanes[5].SetFromThreePoints(cFarTopRight, cFarTopLeft, cFarBottomLeft);
      /* set the start of the ray for checking occlusions to the camera's location */
      m_cOcclusionCheckRay.SetStart(m_cCameraLocation);
      /* detect tags */
      m_pcTagIndex->ForEntitiesInBoxRange(cBoundingBoxPosition,
                                          cBoundingBoxHalfExtents,
                                          *this);
      /* detect directional LEDs */
      m_pcDirectionalLEDIndex->ForEntitiesInBoxRange(cBoundingBoxPosition,
                                                     cBoundingBoxHalfExtents,
                                                     *this);
   }

   /****************************************/
   /****************************************/

   bool CBuilderBotCameraSystemDefaultSensor::operator()(CTagEntity& c_tag) {
      if(GetAngleWithCamera(c_tag) > c_tag.GetObservableAngle()) {
         return true;
      }
      std::transform(std::begin(m_arrTagCornerOffsets),
                     std::end(m_arrTagCornerOffsets),
                     std::begin(m_arrTagCorners),
                     [&c_tag] (const CVector3& c_tag_corner_offset) {
         CVector3 cCorner(c_tag_corner_offset * c_tag.GetSideLength());
         cCorner.Rotate(c_tag.GetOrientation());
         return (cCorner + c_tag.GetPosition());
      });
      for(const CVector3& c_corner : m_arrTagCorners) {
         if(IsInsideFrustum(c_corner) == false) {
            /* no more checks necessary, move on to the next tag */
            return true;
         }
      }
      for(const CVector3& c_corner : m_arrTagCorners) {
         m_cOcclusionCheckRay.SetEnd(c_corner);
         if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
            /* corner is occluded */
            if(m_bShowTagRays)
               m_pcControllableEntity->GetCheckedRays().emplace_back(true, m_cOcclusionCheckRay);
            /* no more checks necessary, move on to the next tag */
            return true;
         }
         else {
            /* corner not occluded */
            if(m_bShowTagRays)
               m_pcControllableEntity->GetCheckedRays().emplace_back(false, m_cOcclusionCheckRay);
         }
      }
      std::transform(std::begin(m_arrTagCorners),
                     std::end(m_arrTagCorners),
                     std::begin(m_arrTagCornerPixels),
                     [this] (const CVector3& c_tag_corner) {
         return ProjectOntoSensor(c_tag_corner);
      });
      const CVector2& cCenterPixel = ProjectOntoSensor(c_tag.GetPosition());
      const std::string& strPayload = c_tag.GetPayload();
      /* transfer readings to the control interface */
      m_tTags.emplace_back(strPayload, cCenterPixel, m_arrTagCornerPixels);
      return true;
   }

   /****************************************/
   /****************************************/

   bool CBuilderBotCameraSystemDefaultSensor::operator()(CDirectionalLEDEntity& c_led) {
      if(c_led.GetColor() == CColor::BLACK) {
         return true;
      }
      if(GetAngleWithCamera(c_led) > c_led.GetObservableAngle()) {
         return true;
      }
      const CVector3& cLedPosition = c_led.GetPosition();
      if(IsInsideFrustum(cLedPosition) == false) {
         return true;
      }
      m_cOcclusionCheckRay.SetEnd(cLedPosition);
      if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
         if(m_bShowLEDRays)
            m_pcControllableEntity->GetCheckedRays().emplace_back(true, m_cOcclusionCheckRay);
         return true;
      }
      if(m_bShowLEDRays)
         m_pcControllableEntity->GetCheckedRays().emplace_back(false, m_cOcclusionCheckRay);
      m_tLedCache.emplace_back(c_led.GetColor(), ProjectOntoSensor(cLedPosition));
      return true;
   }

   /****************************************/
   /****************************************/

   bool CBuilderBotCameraSystemDefaultSensor::DetectLed(const CVector2& c_center,
                                                        const CVector2& c_size) {
      CVector2 cMinCorner(c_center - 0.5f * c_size);
      CVector2 cMaxCorner(c_center + 0.5f * c_size);
      for(const SLed& s_led : m_tLedCache) {
         if(s_led.Center.GetX() > cMinCorner.GetX() &&
            s_led.Center.GetY() > cMinCorner.GetY() &&
            s_led.Center.GetX() < cMaxCorner.GetX() &&
            s_led.Center.GetY() < cMaxCorner.GetY()) {
            /* LED detected */
            m_tLeds.emplace_back(s_led);
            /* return true to indicate that an LED has been detected */
            return true;
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

   CVector2 CBuilderBotCameraSystemDefaultSensor::GetResolution() const {
      return CVector2(CAMERA_RESOLUTION_X, CAMERA_RESOLUTION_Y);
   }

   /****************************************/
   /****************************************/

   CRadians CBuilderBotCameraSystemDefaultSensor::GetAngleWithCamera(const CPositionalEntity& c_entity) const {
      CVector3 cEntityToCamera(m_cCameraLocation - c_entity.GetPosition());
      CVector3 cEntityDirection(CVector3::Z);
      cEntityDirection.Rotate(c_entity.GetOrientation());
      Real fDotProduct = cEntityDirection.DotProduct(cEntityToCamera);
      return ACos(fDotProduct / (cEntityDirection.Length() * cEntityToCamera.Length()));
   }

   /****************************************/
   /****************************************/

   CVector2 CBuilderBotCameraSystemDefaultSensor::ProjectOntoSensor(const CVector3& c_point) const {
      CVector3 cCameraToEntityTranslation(m_cCameraToWorldTransform * c_point);
      /* this could be avoided if CVector3 inherited from CMatrix<3,1> */
      CMatrix<3,1> cCameraToEntityTranslationMatrix;
      cCameraToEntityTranslationMatrix(0,0) = 
         cCameraToEntityTranslation.GetX() / cCameraToEntityTranslation.GetZ();
      cCameraToEntityTranslationMatrix(1,0) =
         cCameraToEntityTranslation.GetY() / cCameraToEntityTranslation.GetZ();
      cCameraToEntityTranslationMatrix(2,0) = 1.0f;
      /* get image coordinates */              
      CMatrix<3,1> cImageCoordinates(m_cProjectionMatrix * cCameraToEntityTranslationMatrix);
      /* return as vector2 */
      return CVector2(cImageCoordinates(0,0), cImageCoordinates(1,0));
   }

   /****************************************/
   /****************************************/

   bool CBuilderBotCameraSystemDefaultSensor::IsInsideFrustum(const CVector3& c_point) const {
      for(const CPlane& c_plane : m_arrFrustumPlanes) {
         if(c_plane.GetNormal().DotProduct(c_point - c_plane.GetPosition()) < 0.0) {
            return false;
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotCameraSystemDefaultSensor,
                   "builderbot_camera_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "A generic multi-camera sensor capable of running various algorithms",
                   "Long description\n",
                   "Usable");
}
