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
private:
    
    unsigned char atomNum;                                      // number of AtomDevice
    unsigned char atomId[MAXDEVICE][3];                         // device Id, sensor id, actuator id     
    unsigned int  atomValue[MAXDEVICE];
    bool getAtomValue[MAXDEVICE];
    
    bool yeelinkFree;                                           // if yeelink free
    
    unsigned char postNumNow;

    unsigned char ifSetSensor;                                  // if set sensor of cloud
    unsigned char freqCloud;                                    // cloud sensor broadcase freq
    unsigned char sensorIdCloud;                                // cloud sensor id
    
private:

    void yeelinkPost(unsigned char idNode, unsigned int psDta);
    void yeelinkAdd(unsigned char idNode, unsigned char idSensor, unsigned char idActuator);
    unsigned int popDta(unsigned char id);                      // pop data from a certain device
    void cloudDta();                                            // cloud data proc
    
public:
    
    void init();
    
    int getDeviceNum();
    int addDevice(unsigned char *id);                           // add a device
    int checkId(unsigned char *id);                             // if certain id in net
    int checkId(unsigned char idDevice);

    unsigned char pushDta(unsigned char id, int dta);           // push data to certain device
    
    int postDta();

    void addCloudSensor(unsigned char id, unsigned char freq);                      // add cloud sensor

};

extern NodeManage NODE;

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
