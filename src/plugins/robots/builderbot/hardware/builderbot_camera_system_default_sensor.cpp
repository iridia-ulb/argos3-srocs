/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_camera_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_camera_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/image_u8.h>
#include <apriltag/zarray.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::CBuilderBotCameraSystemDefaultSensor() {
      /* initialize the apriltag components */
      m_psTagFamily = tag36h11_create();
      m_psTagFamily->black_border = 1;
      /* create the tag detector */
      m_psTagDetector = apriltag_detector_create();
      /* add the tag family to the tag detector */
      apriltag_detector_add_family(m_psTagDetector, m_psTagFamily);
      /* configure the tag detector */
      m_psTagDetector->quad_decimate = 1.0f;
      m_psTagDetector->quad_sigma = 0.0f;
      m_psTagDetector->nthreads = 1;
      m_psTagDetector->debug = 0;
      m_psTagDetector->refine_edges = 1;
      m_psTagDetector->refine_decode = 0;
      m_psTagDetector->refine_pose = 0;
   }

   /****************************************/
   /****************************************/

   CBuilderBotCameraSystemDefaultSensor::~CBuilderBotCameraSystemDefaultSensor() {
      /* uninitialize the apriltag components */
      apriltag_detector_remove_family(m_psTagDetector, m_psTagFamily);
      /* destroy the tag detector */
      apriltag_detector_destroy(m_psTagDetector);
      /* destroy the tag family */
      tag36h11_destroy(m_psTagFamily);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotCameraSystemSensor::Init(t_tree);
         /* parse and set the resolution */
         CVector2 cResolution;
         GetNodeAttribute(t_tree, "resolution", cResolution);
         m_unImageHeight = std::round(cResolution.GetY());
         m_unImageWidth = std::round(cResolution.GetX());
         /* allocate storage */
         m_sCurrentFrame.SetSize(m_unImageWidth, m_unImageHeight);
         /* parse input frame paths */
         TConfigurationNodeIterator itFrame("frame");
         std::string strFramePath;
         for(itFrame = itFrame.begin(&t_tree);
             itFrame != itFrame.end();
             ++itFrame) {
            /* get the path to the file */
            GetNodeAttribute(*itFrame, "src", strFramePath);
            m_vecInputFrames.emplace_back(strFramePath);
         }
         m_itInputFrameIterator = std::begin(m_vecInputFrames);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Update() {
      if(m_itInputFrameIterator != std::end(m_vecInputFrames)) {
         image_u8_t* ptFrame = 
            image_u8_create_from_pnm(m_itInputFrameIterator->c_str());
         if(ptFrame == nullptr) {
            THROW_ARGOSEXCEPTION("Can not open " << *m_itInputFrameIterator
                                 << " as an 8-bit PNM image");
         }
         else {
            m_sCurrentFrame.Y.reset(ptFrame);
            /* other channels not defined */
            m_sCurrentFrame.U.release();
            m_sCurrentFrame.V.release();
            m_itInputFrameIterator++;
         }
      }
      else {
         THROW_ARGOSEXCEPTION("Implement camera capture here");
      }

      /* detect tags */
      CVector2 c_center_pixel;
      std::array<CVector2, 4> arr_corner_pixels;
      /* run the apriltags algorithm */
      zarray_t* psDetectionArray =
         apriltag_detector_detect(m_psTagDetector, m_sCurrentFrame.Y.get());
      /* get the detected tags count */
      size_t unTagCount = static_cast<size_t>(zarray_size(psDetectionArray));
      /* clear out previous readings */
      m_tTags.clear();
      /* reserve space for the tags */
      m_tTags.reserve(unTagCount);
      /* process detections */
      for(size_t un_index = 0; un_index < unTagCount; un_index++) {
         apriltag_detection_t *psDetection;
         zarray_get(psDetectionArray, un_index, &psDetection);
         /* copy the tag corner coordinates */
         arr_corner_pixels[0].Set(psDetection->p[0][0], psDetection->p[0][1]),
         arr_corner_pixels[1].Set(psDetection->p[1][0], psDetection->p[1][1]),
         arr_corner_pixels[2].Set(psDetection->p[2][0], psDetection->p[2][1]),
         arr_corner_pixels[3].Set(psDetection->p[3][0], psDetection->p[3][1]),
         /* copy the tag center coordinate */
         c_center_pixel.Set(psDetection->c[0], psDetection->c[1]);
         /* copy reading to the control interface */
         m_tTags.emplace_back(std::to_string(psDetection->id), c_center_pixel, arr_corner_pixels);
      }
      /* destroy the readings array */
      apriltag_detections_destroy(psDetectionArray);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::GetPixels(const CVector2& c_offset,
                                                  const CVector2& c_size,
                                                  std::vector<SPixel>& vec_pixels) {
      /* round the given coordinates to look up the pixels */     
      UInt32 unOffsetX = std::round(c_offset.GetX());
      UInt32 unOffsetY = std::round(c_offset.GetY());
      UInt32 unWidth = std::round(c_size.GetX());
      UInt32 unHeight = std::round(c_size.GetY());
      UInt32 unFrameStride = m_sCurrentFrame.Y->stride;
      /* copy the requested pixels */
      for(UInt32 un_y = unOffsetY; un_y < unOffsetY + unHeight; un_y++) {
         for(UInt32 un_x = unOffsetX; un_x < unOffsetX + unWidth; un_x++) {
            vec_pixels.emplace_back(
               m_sCurrentFrame.Y->buf[un_y * unFrameStride + un_x],
               m_sCurrentFrame.U ? m_sCurrentFrame.U->buf[un_y * unFrameStride + un_x] : 0,
               m_sCurrentFrame.V ? m_sCurrentFrame.V->buf[un_y * unFrameStride + un_x] : 0);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotCameraSystemDefaultSensor::Write(const CBuilderBotCameraSystemDefaultSensor::SFrame& s_frame,
                                                    const std::string& str_file) {
      /* write to PNM file */
      static int frame = 0;
      image_u8_write_pnm(s_frame.Y.get(), (str_file + std::to_string(frame)).c_str());
      frame++;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotCameraSystemDefaultSensor,
                   "builderbot_camera_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Camera sensor for the BuilderBot Robot",
                   "Camera sensor for the BuilderBot Robot\n",
                   "Under development");
}
