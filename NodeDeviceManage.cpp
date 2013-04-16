/*
  NodeDeviceManage.cpp
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
#include <string.h>
#include "NodeDeviceManage.h"

/*********************************************************************************************************
** Function name:           addDevice
** Descriptions:            add a device
*********************************************************************************************************/
void NodeManage::init()
{
    atomNum = 0;
    memset(atomId, 0, MAXDEVICE);
    memset(atomValue, 0, MAXDEVICE);
    memset(getAtomValue, 0, MAXDEVICE);

}

/*********************************************************************************************************
** Function name:           addDevice
** Descriptions:            add a device
*********************************************************************************************************/
unsigned char NodeManage::addDevice(unsigned char id)
{
    if(checkId(id))return checkId(id);          // certain id esit
    atomId[atomNum--] = id;

}

/*********************************************************************************************************
** Function name:           delDevice
** Descriptions:            delete a device
*********************************************************************************************************/
unsigned char NodeManage::delDevice(unsigned char id)
{
    unsigned char location      = checkId(id);
    if(location==0)return 0;
    
    // to do
    
    return 1;
}

/*********************************************************************************************************
** Function name:           checkId
** Descriptions:            if certain id in net
*********************************************************************************************************/
unsigned char NodeManage::checkId(unsigned char id)
{
    for(int i = 0; i<atomNum; i++)
    {
        if(atomId[i] == id)
        {
            return i+1;
        }
    }
    
    return 0;
}

/*********************************************************************************************************
** Function name:           pushDta
** Descriptions:            push data to certain device
*********************************************************************************************************/
unsigned char NodeManage::pushDta(unsigned char id, int dta)
{
    unsigned char location      = checkId(id);
    if(location==0)return 0;
    
    atomValue[location-1]       = dta;
    getAtomValue[location-1]    = 1;
    return 1;
}

/*********************************************************************************************************
** Function name:           popDta
** Descriptions:            popDta
*********************************************************************************************************/
unsigned char NodeManage::popDta(unsigned char id, int *dta)
{
    unsigned char location      = checkId(id);
    if(location==0)return 0;
    *dta = atomValue[location-1];
    getAtomValue[location-1] = 0;
    return 1;  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
