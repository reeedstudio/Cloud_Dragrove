/*
  BeaconApplicationDfs.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2012-12-3
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __BEACONAPPLICATIONDFS_H__
#define __BEACONAPPLICATIONDFS_H__

#define __DebugApp                  0                   // debug i2c
#define __DebugAppSerial            0                   // debug Serial

#if __DebugAppSerial                                    // debug use Serial
#define __printAppS(x)              Serial.print(x)
#define __printlnAppS(x)            Serial.println(x)
#else
#define __printAppS(x)
#define __printlnAppS(x)
#endif

#if __DebugApp
#define __printApp(x)               __print(x)
#define __printlnApp(x)             __println(x)
#else
#define __printApp(x)
#define __printlnApp(x)
#endif

#define SendByteToRfbee             Serial.write       // send a byte to Serial

#define TURNOFFDELAYTIME            80                  // when press the button for xx*10 ms, turn off the system

// broadcast freq
#define BDF100MS                    0x80                // 100ms
#define BDF1S                       0x81                // 1s
#define BDF1MIN                     0x82                // 1min
#define BDF1HOUR                    0x83                // 1hour

// data frame
#define FRAMESTART1                 0x53                // data frame start1
#define FRAMESTART2                 0x01                // data frame start2
#define FRAMEEND1                   0x2f                // data frame end1
#define FRAMEEND2                   0x45                // data frame end2

//bit of frame
#define FRAMEBITSTART1              0                   // frame data bit0
#define FRAMEBITSTART2              1
#define FRAMEBITSRCID               2
#define FRAMEBITDESTID              3
#define FRAMEBITFRAME               4
#define FRAMEBITDATALEN             5
#define FRAMEBITDATA                6

// frame type
#define FRAMETYPEBC                 1                   // broadcast
#define FRAMETYPECTRL               2                   // control
#define FRAMETYPESET                3                   // setting

// led Mode
#define LEDEANBLE                   1
#define LEDDISABLE                  0

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/