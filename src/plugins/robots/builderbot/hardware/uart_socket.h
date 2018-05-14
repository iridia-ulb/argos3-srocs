#ifndef UART_SOCKET_H
#define UART_SOCKET_H

#include <cstdint>
#include <cstddef>

#include <termios.h>
#include <string>

class CUARTSocket {
   /* target port */
   std::string m_strDevice;
   uint32_t m_unBaud;
   
   /* file descriptor for the serial port */
   int m_nPort;
   /* previous settings */
   struct termios sPrevPortSettings;
public:

   CUARTSocket(const std::string& str_device, uint32_t un_baud = 115200) : 
      m_strDevice(str_device),
      m_unBaud(un_baud),
      m_nPort(-1) {}

   ~CUARTSocket() {
      if(m_nPort != -1) {
         Close();
      }
   }

   /*  open a serial port connection */
   int Open();

   /* close the connection */
   int Close();

   /* read into buffer */ 
   int16_t Read(uint8_t* p_buffer, size_t un_length) const;

   /* write from buffer */
   int16_t Write(const uint8_t* p_buffer, size_t un_length) const;

};

#endif
