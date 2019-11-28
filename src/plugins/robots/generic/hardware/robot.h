/**
 * @file <argos3/plugins/robots/generic/hardware/robot.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef ROBOT_H
#define ROBOT_H

#include <argos3/core/utility/networking/tcp_socket.h>

namespace argos {

   class CRobot {

   public:

      /**
       * Class destructor.
       */
      virtual ~CRobot() {}

      CTCPSocket& GetSocket() {
         return m_cSocket;
      }

   protected:

      /* the TCP socket for WiFi communication */
      CTCPSocket m_cSocket;

   };

   /****************************************/
   /****************************************/

}

#endif
