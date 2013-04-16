/*
  NodeDeviceManage.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-4-16
  
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
#ifndef __NODEDEVICEMANAGE_H__
#define __NODEDEVICEMANAGE_H__

#define MAXDEVICE       10

class NodeManage
{
public:

    unsigned char atomNum;                  // number of AtomDevice
    
    unsigned char atomId[MAXDEVICE];     
    unsigned int  atomValue[MAXDEVICE];
    bool getAtomValue[MAXDEVICE];
    
public:

    void init();
    unsigned char addDevice(unsigned char id);      // add a device
    unsigned char delDevice(unsigned char id);      // delete a device
    unsigned char checkId(unsigned char id);        // if certain id in net

    unsigned char pushDta(unsigned char id, int dta);                 // push data to certain device
    unsigned char popDta(unsigned char id, int *dta);         // po


};

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
