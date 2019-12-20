/**
 * @file <argos3/plugins/robots/drone/simulator/pointmass3d_drone_model.cpp>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pointmass3d_drone_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>

namespace argos
{

   /****************************************/
   /****************************************/

   CPointMass3DDroneModel::CPointMass3DDroneModel(CPointMass3DEngine &c_engine,
                                                  CDroneEntity &c_drone) :
      CPointMass3DModel(c_engine, c_drone.GetEmbodiedEntity()),
      m_cFlightSystemEntity(c_drone.GetFlightSystemEntity()) {
      /* register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DDroneModel::UpdateOriginAnchor);
      /* reset the model */
      Reset();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::Reset() {
      /* reset the drone's position */
      m_cDronePosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      /* reset the home position (may have been changed by MoveTo or by added noise) */
      m_cHomePosition = m_cDronePosition;
      /* reset the drone's orientation */
      CRadians cYaw, cPitch, cRoll;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cYaw, cPitch, cRoll);
      m_cDroneOrientation.SetX(cRoll.GetValue());
      m_cDroneOrientation.SetY(cPitch.GetValue());
      m_cDroneOrientation.SetZ(cYaw.GetValue());
      /* reset the home yaw angle (may have been changed by MoveTo or by added noise) */
      m_fHomeYawAngle = cYaw.GetValue();
      /* reset the drone's velocity */
      m_cDroneVelocity.Set(0.0, 0.0, 0.0);
      m_cDroneVelocityPrev.Set(0.0, 0.0, 0.0);
      /* reset the drone's angular velocity */
      m_cDroneAngularVelocity.Set(0.0, 0.0, 0.0);
      m_cDroneAngularVelocityPrev.Set(0.0, 0.0, 0.0);
      /* reset the drone's previous acceleration */
      m_cDroneAccelerationPrev.Set(0.0, 0.0, 0.0);
      /* reset the drone's previous angular acceleration */
      m_cDroneAngularAccelerationPrev.Set(0.0, 0.0, 0.0);
      /* TODO verify if the following variables are necessary */
      m_cDroneOrientationTargetPrev.Set(0.0, 0.0, 0.0);
      m_cDroneAngularVelocityCumulativeError.Set(0.0, 0.0, 0.0);
      m_fAltitudeCumulativeError = 0.0;
      m_fTargetPositionZPrev = 0.0;
      /* update the axis-aligned bounding box, anchors, and entities */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateEntityStatus() {
      /* push sensor data to the control interface */
      /* TODO: model the actual sensor behavior and take modeled sensor data */
      m_cFlightSystemEntity.SetPositionReading(m_cDronePosition - m_cHomePosition);
      m_cFlightSystemEntity.SetOrientationReading(m_cDroneOrientation - (CVector3::Z * m_fHomeYawAngle));
      m_cFlightSystemEntity.SetVelocityReading(m_cDroneVelocity);
      m_cFlightSystemEntity.SetAngularVelocityReading(m_cDroneAngularVelocity);
      /* update the axis-aligned bounding box, anchors, and entities */
      CPointMass3DModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateFromEntityStatus() {
      /* pull actuator data from the control interface */
      m_cInputPosition = m_cFlightSystemEntity.GetTargetPosition();
      m_cInputYawAngle = m_cFlightSystemEntity.GetTargetYawAngle();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdatePhysics() {
      /* update the position (XY) and altitude (Z) controller */
      /* calculate position error */
      CVector3 cPositionalError((m_cHomePosition + m_cInputPosition) - m_cDronePosition);
      /* accumulate the altitude error */
      m_fAltitudeCumulativeError += cPositionalError.GetZ() * GetPM3DEngine().GetPhysicsClockTick();
      /* calculate the azimuth contribution to the navigation of drone on XY plane */
      Real fAzimuth = std::atan2(std::abs(cPositionalError.GetY()),
                                 std::abs(cPositionalError.GetX()));
      /* calculate velocity limits */
      CRange<Real> cVelocityLimitX(-XY_VEL_MAX * std::cos(fAzimuth), XY_VEL_MAX * std::cos(fAzimuth));
      CRange<Real> cVelocityLimitY(-XY_VEL_MAX * std::sin(fAzimuth), XY_VEL_MAX * std::sin(fAzimuth));
      CRange<Real> cVelocityLimitZ(-Z_VEL_MAX, Z_VEL_MAX);
      /* calculate desired XYZ velocities */
      Real fTargetTransVelX = 
         cPositionalError.GetX() * XY_POS_KP;
      Real fTargetTransVelY =
         cPositionalError.GetY() * XY_POS_KP;
      Real fTargetTransVelZ =
         (m_cInputPosition.GetZ() - m_fTargetPositionZPrev) / GetPM3DEngine().GetPhysicsClockTick();
      /* saturate velocities */
      cVelocityLimitX.TruncValue(fTargetTransVelX);
      cVelocityLimitY.TruncValue(fTargetTransVelY);
      cVelocityLimitZ.TruncValue(fTargetTransVelZ);
      /* store the previous desired altitude for the altitude PID calculation */
      m_fTargetPositionZPrev = m_cInputPosition.GetZ();
      /* store XYZ velocity error */
      CVector3 cTransVelocityError(fTargetTransVelX, fTargetTransVelY, fTargetTransVelZ);
      cTransVelocityError -= m_cDroneVelocity;
      /* calculate XY desired accelerations */
      CVector3 cTargetTransAcc = cTransVelocityError * XY_VEL_KP;
      /* yaw angle correction to get downward Z axis in the right handed coordinate system */
      Real fYawAngleCorrected(m_cDroneOrientation.GetZ() + M_PI);
      /* outputs of the position controller */
      Real fDesiredRollAngle = std::cos(m_cDroneOrientation.GetY()) * std::cos(m_cDroneOrientation.GetX()) * 
         (std::sin(fYawAngleCorrected) * cTargetTransAcc.GetX() - std::cos(fYawAngleCorrected) * cTargetTransAcc.GetY()) / m_cPM3DEngine.GetGravity();
      Real fDesiredPitchAngle = std::cos(m_cDroneOrientation.GetY()) * std::cos(m_cDroneOrientation.GetX()) *
         (std::cos(fYawAngleCorrected) * cTargetTransAcc.GetX() + std::sin(fYawAngleCorrected) * cTargetTransAcc.GetY()) / m_cPM3DEngine.GetGravity();
      Real fDesiredYawAngle = m_fHomeYawAngle + m_cInputYawAngle.GetValue();
      /* saturate the outputs of the position controller */
      ROLL_PITCH_LIMIT.TruncValue(fDesiredRollAngle);
      ROLL_PITCH_LIMIT.TruncValue(fDesiredPitchAngle);
      /* store the desired orientation values in a  vector */
      CVector3 cDroneOrientationTarget(fDesiredRollAngle, fDesiredPitchAngle, fDesiredYawAngle);
      /* output of the altitude controller */
      Real fAltitudeControlSignal = MASS * GetPM3DEngine().GetGravity() + 
         CalculatePIDResponse(cPositionalError.GetZ(),
                              m_fAltitudeCumulativeError,
                              cTransVelocityError.GetZ(),
                              ALTITUDE_KP,
                              ALTITUDE_KI,
                              ALTITUDE_KD) / (std::cos(m_cDroneOrientation.GetX()) * std::cos(m_cDroneOrientation.GetY()));
      /*** attitude (roll, pitch, yaw) control ***/
      /* roll, pitch, yaw errors */
      CVector3 cDroneOrientationError(cDroneOrientationTarget - m_cDroneOrientation);
      /* desired  roll, pitch, yaw rates */
      CVector3 cDroneAngularVelocityTarget = 
         (cDroneOrientationTarget - m_cDroneOrientationTargetPrev) / GetPM3DEngine().GetPhysicsClockTick();
      /* previous desired roll, pitch, yaw values for the controllers */
      m_cDroneOrientationTargetPrev = cDroneOrientationTarget;
      /* rotational rate errors */
      CVector3 cDroneAngularVelocityError(cDroneAngularVelocityTarget - m_cDroneAngularVelocity);
      /* accumulated roll, pitch, yaw errors for the controllers */
      m_cDroneAngularVelocityCumulativeError += 
         cDroneAngularVelocityError * GetPM3DEngine().GetPhysicsClockTick();
      /* roll controller output signal */
      Real fAttitudeControlSignalX = INERTIA.GetX() *
         CalculatePIDResponse(cDroneOrientationError.GetX(),
                              m_cDroneAngularVelocityCumulativeError.GetX(),
                              cDroneAngularVelocityError.GetX(),
                              ROLL_PITCH_KP,
                              ROLL_PITCH_KI,
                              ROLL_PITCH_KD);
      /* pitch controller output signal */
      Real fAttitudeControlSignalY = INERTIA.GetY() * 
         CalculatePIDResponse(cDroneOrientationError.GetY(),
                              m_cDroneAngularVelocityCumulativeError.GetY(),
                              cDroneAngularVelocityError.GetY(),
                              ROLL_PITCH_KP,
                              ROLL_PITCH_KI,
                              ROLL_PITCH_KD);
      /* yaw controller output signal */
      Real fAttitudeControlSignalZ = INERTIA.GetZ() * 
         CalculatePIDResponse(cDroneOrientationError.GetZ(),
                              m_cDroneAngularVelocityCumulativeError.GetZ(),
                              cDroneAngularVelocityError.GetZ(),
                              YAW_KP,
                              YAW_KI,
                              YAW_KD);
      /* saturate controller outputs */
      TORQUE_LIMIT.TruncValue(fAttitudeControlSignalX);
      TORQUE_LIMIT.TruncValue(fAttitudeControlSignalY);
      TORQUE_LIMIT.TruncValue(fAttitudeControlSignalZ);
      /* calculate the rotor speeds from the control signals */
      std::array<Real, 4> arrSquaredRotorSpeeds = {
         fAltitudeControlSignal / (4 * B) - 
            fAttitudeControlSignalX * (ROOT_TWO / (4 * B * ARM_LENGTH)) -
            fAttitudeControlSignalY * (ROOT_TWO / (4 * B * ARM_LENGTH)) -
            fAttitudeControlSignalZ / (4 * D),
         fAltitudeControlSignal / (4 * B) -
            fAttitudeControlSignalX * (ROOT_TWO / (4 * B * ARM_LENGTH)) +
            fAttitudeControlSignalY * (ROOT_TWO / (4 * B * ARM_LENGTH)) +
            fAttitudeControlSignalZ / (4 * D),
         fAltitudeControlSignal / (4 * B) +
            fAttitudeControlSignalX * (ROOT_TWO / (4 * B * ARM_LENGTH)) +
            fAttitudeControlSignalY * (ROOT_TWO / (4 * B * ARM_LENGTH)) -
            fAttitudeControlSignalZ / (4 * D),
         fAltitudeControlSignal / (4 * B) +
            fAttitudeControlSignalX * (ROOT_TWO / (4 * B * ARM_LENGTH)) -
            fAttitudeControlSignalY * (ROOT_TWO / (4 * B * ARM_LENGTH)) +
            fAttitudeControlSignalZ / (4 * D),
      };
      /* calculate the effect of the rotor speed on the drone dynamics*/
      Real fOmegaR = -std::sqrt(std::abs(arrSquaredRotorSpeeds[0])) + 
                      std::sqrt(std::abs(arrSquaredRotorSpeeds[1])) +
                     -std::sqrt(std::abs(arrSquaredRotorSpeeds[2])) +
                      std::sqrt(std::abs(arrSquaredRotorSpeeds[3]));
      /* calculate the drone thrusts and torques */
      Real fThrust = B *
         ( arrSquaredRotorSpeeds[0] + arrSquaredRotorSpeeds[1] + arrSquaredRotorSpeeds[2] + arrSquaredRotorSpeeds[3]);
      Real fTorqueX = (ARM_LENGTH / ROOT_TWO) * B *
         (-arrSquaredRotorSpeeds[0] - arrSquaredRotorSpeeds[1] + arrSquaredRotorSpeeds[2] + arrSquaredRotorSpeeds[3]);
      Real fTorqueY = (ARM_LENGTH / ROOT_TWO) * B *
         (-arrSquaredRotorSpeeds[0] + arrSquaredRotorSpeeds[1] + arrSquaredRotorSpeeds[2] - arrSquaredRotorSpeeds[3]);
      Real fTorqueZ = D *
         (-arrSquaredRotorSpeeds[0] + arrSquaredRotorSpeeds[1] - arrSquaredRotorSpeeds[2] + arrSquaredRotorSpeeds[3]);
      /* saturate the thrust */
      THRUST_LIMIT.TruncValue(fThrust);
      /* calculate the system response: acceleration */
      CVector3 cDroneAcceleration {
         (std::cos(fYawAngleCorrected) * std::sin(m_cDroneOrientation.GetY()) * std::cos(m_cDroneOrientation.GetX()) +
          std::sin(fYawAngleCorrected) * std::sin(m_cDroneOrientation.GetX())) * fThrust / MASS,
         (std::sin(fYawAngleCorrected) * std::sin(m_cDroneOrientation.GetY()) * std::cos(m_cDroneOrientation.GetX()) -
          std::cos(fYawAngleCorrected) * std::sin(m_cDroneOrientation.GetX())) * fThrust / MASS,
         -m_cPM3DEngine.GetGravity() + (std::cos(m_cDroneOrientation.GetY()) * std::cos(m_cDroneOrientation.GetX()) * fThrust / MASS)
      };
      /* calculate the system response: angular acceleration */
      CVector3 cDroneAngularAcceleration {
         (fTorqueX / INERTIA.GetX()) -
         (JR * m_cDroneAngularVelocity.GetY() * fOmegaR / INERTIA.GetX()) +
         (m_cDroneAngularVelocity.GetZ() * m_cDroneAngularVelocity.GetY() * (INERTIA.GetY() - INERTIA.GetZ()) / INERTIA.GetX()),
         (fTorqueY / INERTIA.GetY()) +
         (JR * m_cDroneAngularVelocity.GetX() * fOmegaR / INERTIA.GetY()) +
         (m_cDroneAngularVelocity.GetZ() * m_cDroneAngularVelocity.GetX() * (INERTIA.GetZ() - INERTIA.GetX()) / INERTIA.GetY()),
         (fTorqueZ / INERTIA.GetZ()) +
         (m_cDroneAngularVelocity.GetY() * m_cDroneAngularVelocity.GetX() * (INERTIA.GetX() - INERTIA.GetY()) / INERTIA.GetZ()),
      };
      /* update the velocity using trapezoid integration */
      cDroneAcceleration = 0.5 * (m_cDroneAccelerationPrev + cDroneAcceleration);
      m_cDroneVelocity += cDroneAcceleration * GetPM3DEngine().GetPhysicsClockTick();
      m_cDroneAccelerationPrev = cDroneAcceleration;
      /* update the angular velocity using trapezoid integration */
      cDroneAngularAcceleration = 0.5 * (m_cDroneAngularAccelerationPrev + cDroneAngularAcceleration);
      m_cDroneAngularVelocity += cDroneAngularAcceleration * GetPM3DEngine().GetPhysicsClockTick();
      m_cDroneAngularAccelerationPrev = cDroneAngularAcceleration;
      /* update the position using trapezoid integration */
      m_cDroneVelocity = 0.5 * (m_cDroneVelocityPrev + m_cDroneVelocity);
      m_cDronePosition += m_cDroneVelocity * GetPM3DEngine().GetPhysicsClockTick();
      m_cDroneVelocityPrev = m_cDroneVelocity;
      /* update the orientation using trapezoid integration */
      m_cDroneAngularVelocity = 0.5 * (m_cDroneAngularVelocityPrev + m_cDroneAngularVelocity);
      m_cDroneOrientation += m_cDroneAngularVelocity * GetPM3DEngine().GetPhysicsClockTick();
      m_cDroneAngularVelocityPrev = m_cDroneAngularVelocity;
   }

   /****************************************/
   /****************************************/

   Real CPointMass3DDroneModel::CalculatePIDResponse(Real f_cur_error, Real f_sum_error, Real f_vel_error,
                                                     Real f_k_p, Real f_k_i, Real f_k_d) {
      return (f_k_p * f_cur_error + f_k_i * f_sum_error + f_k_d * f_vel_error);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::MoveTo(const CVector3 &c_position,
                                       const CQuaternion &c_orientation) {
      /* convert to Euler angles */
      CRadians cYaw, cPitch, cRoll;
      c_orientation.ToEulerAngles(cYaw, cPitch, cRoll);
      /* update the home position and home yaw */
      m_cHomePosition += c_position - m_cDronePosition;
      m_fHomeYawAngle += cYaw.GetValue() - m_cDronePosition.GetZ();
      /* update the position and orientation */
      m_cDronePosition = c_position;
      /* the drone orientation should include roll, pitch, yaw angles*/
      m_cDroneOrientation.Set(cRoll.GetValue(),
                              cPitch.GetValue(),
                              cYaw.GetValue());
      /* update the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::CalculateBoundingBox() {
      /* TODO update this method to compensate for drone orientation */
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - ARM_LENGTH,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - ARM_LENGTH,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + ARM_LENGTH,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + ARM_LENGTH,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + HEIGHT);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DDroneModel::CheckIntersectionWithRay(Real &f_t_on_ray,
                                                         const CRay3 &c_ray) const {
      const CQuaternion& cDroneOrientation = 
         GetEmbodiedEntity().GetOriginAnchor().Orientation;
      CVector3 cDroneAxis(CVector3::Z);
      cDroneAxis.Rotate(cDroneOrientation);
      /* create a cylinder shape for testing the intersection */
      CCylinder m_cShape(ARM_LENGTH,
                         HEIGHT,
                         m_cDronePosition,
                         cDroneAxis);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DDroneModel::UpdateOriginAnchor(SAnchor &s_anchor) {
      s_anchor.Position = m_cDronePosition;
      s_anchor.Orientation.FromEulerAngles(CRadians(m_cDroneOrientation.GetZ()),
                                           CRadians(m_cDroneOrientation.GetY()),
                                           CRadians(m_cDroneOrientation.GetX()));
   }

   /****************************************/
   /****************************************/

   /* drone parameters */
   const Real CPointMass3DDroneModel::HEIGHT = 0.25;
   const Real CPointMass3DDroneModel::ARM_LENGTH = 0.22;
   const Real CPointMass3DDroneModel::MASS = 0.812;
   const CVector3 CPointMass3DDroneModel::INERTIA = {0.01085, 0.01092, 0.02121};
   const Real CPointMass3DDroneModel::B = 1.1236e-5;
   const Real CPointMass3DDroneModel::D = 1.4088e-7;
   const Real CPointMass3DDroneModel::JR = 5.225e-5; 
   /* limits */
   const CRange<Real> CPointMass3DDroneModel::TORQUE_LIMIT = CRange<Real>(-0.5721, 0.5721);
   const CRange<Real> CPointMass3DDroneModel::THRUST_LIMIT = CRange<Real>(-15, 15);
   const CRange<Real> CPointMass3DDroneModel::ROLL_PITCH_LIMIT = CRange<Real>(-0.5, 0.5);
   const Real CPointMass3DDroneModel::XY_VEL_MAX = 1;
   const Real CPointMass3DDroneModel::Z_VEL_MAX = 1;
   /* PID coefficients */
   const Real CPointMass3DDroneModel::XY_POS_KP = 1;
   const Real CPointMass3DDroneModel::XY_VEL_KP = 3;
   const Real CPointMass3DDroneModel::YAW_KP = 13;
   const Real CPointMass3DDroneModel::YAW_KI = 0;
   const Real CPointMass3DDroneModel::YAW_KD = 8;
   const Real CPointMass3DDroneModel::ALTITUDE_KP = 10;
   const Real CPointMass3DDroneModel::ALTITUDE_KI = 0;
   const Real CPointMass3DDroneModel::ALTITUDE_KD = 6;   
   const Real CPointMass3DDroneModel::ROLL_PITCH_KP = 12;
   const Real CPointMass3DDroneModel::ROLL_PITCH_KI = 0;
   const Real CPointMass3DDroneModel::ROLL_PITCH_KD = 6;
   const Real CPointMass3DDroneModel::ROOT_TWO = std::sqrt(2.0);

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CDroneEntity, CPointMass3DDroneModel);

   /****************************************/
   /****************************************/

}
