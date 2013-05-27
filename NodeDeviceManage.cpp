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
#include <SoftwareSerial.h>
#include <Streaming.h>

#include "NodeDeviceManage.h"
#include "CloudGlobalDfs.h"s

SoftwareSerial mySerial(6, 7);  

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void NodeManage::init()
{

    atomNum     = 0;
    postNumNow  = 0;
    memset(atomId, 0, MAXDEVICE*3);
    memset(atomValue, 0, MAXDEVICE);
    memset(getAtomValue, 0, MAXDEVICE);
    
    yeelinkFree = 1;                                                // if yeelink free
    cntNodeM    = 0;                                                // count of node manage
    
    mySerial.begin(9600);
}

/*********************************************************************************************************
** Function name:           timerIsr
** Descriptions:            timerIsr
*********************************************************************************************************/
void NodeManage::timerIsr()
{
    
    cntNodeM++;
    
    if(!yeelinkFree && cntNodeM > 12000)
    {
        cntNodeM    = 0;
        yeelinkFree = 1;
    }

}

/*********************************************************************************************************
** Function name:           getDeviceNum
** Descriptions:            getDeviceNum
*********************************************************************************************************/
int NodeManage::getDeviceNum()
{
    return atomNum;
}

/*********************************************************************************************************
** Function name:           addDevice
** Descriptions:            add a device
*********************************************************************************************************/
int NodeManage::addDevice(unsigned char *id)
{

    if(checkId(id)>-1)
    {
#if __Debug
        cout << "Device exist already!" << endl;
#endif
        return -1;
    }
    
    int tmp = checkId(id[0]);
    
    if(tmp == -1)
    {
#if __Debug
        cout << "Brand new device!" << endl;
#endif
        for(int i=0; i<3; i++)
        {
            atomId[atomNum][i] = id[i];
        }

        yeelinkAdd(atomId[atomNum][0], atomId[atomNum][1], atomId[atomNum][2]);
        
        return atomNum++;  
        
    }
    else
    {
#if __Debug
        cout << "Device ID exist!" << endl;
        for(int i=0; i<3; i++)
        {
            atomId[tmp][i] = id[i]; 
        }
        yeelinkAdd(atomId[tmp][0], atomId[tmp][1], atomId[tmp][2]);
        return tmp;
#endif
    }

}


/*********************************************************************************************************
** Function name:           checkId
** Descriptions:            if certain id in net, 
** Return:                  -1, no such device, 0-9:Num
*********************************************************************************************************/
int NodeManage::checkId(unsigned char *id)
{

#if __Debug
    cout << "CheckID:"<< endl;
    cout << "input:" << id[0] << ' ' << id[1] << ' ' << id[2] << endl;
    cout << "atomNum = " << atomNum <<endl;
#endif
    for(int i=0; i<atomNum; i++)
    {
    
#if __Debug
        cout << "atomId[" << i << "] = " << atomId[i][0] << atomId[i][1] << atomId[i][2] << endl;
#endif
        for(int j=0; j<3; j++)
        {
            if(id[j] == atomId[i][j])
            {
                if(j == 2)
                {
                    cout << "id exist: " << i << endl;
                    return i;
                }
            }
            else
            {
                continue;
            }
        }
    }
    
    return -1;
}

/*********************************************************************************************************
** Function name:           checkId
** Descriptions:            if certain id in net, 
** Return:                  -1, no such device, 0-9:Num
*********************************************************************************************************/
int NodeManage::checkId(unsigned char idDevice)
{
    
    for(int i=0; i<atomNum; i++)
    {
        if(idDevice == atomId[i][0])
        {
            return i;
        }
    }
    
    return -1;
}
/*********************************************************************************************************
** Function name:           pushDta
** Descriptions:            push data to certain device
*********************************************************************************************************/
unsigned char NodeManage::pushDta(unsigned char id, int dta)
{
    int nId = checkId(id);
    if(nId<0)return 0;
    
    getAtomValue[nId] = 1;
    atomValue[nId]    = dta;
    return 1;
}

/*********************************************************************************************************
** Function name:           popDta
** Descriptions:            return value
*********************************************************************************************************/
unsigned int NodeManage::popDta(unsigned char id)
{

    getAtomValue[id] = 0;
    return atomValue[id];

}

/*********************************************************************************************************
** Function name:           postDta to yeelink
** Descriptions:            return 1: ok 0: nok
*********************************************************************************************************/
int NodeManage::postDta()
{   

    if(atomNum <= 0)return 0;
    
    if(yeelinkFree)
    {

        yeelinkFree = 0;
        
        if(getAtomValue[postNumNow])                    // have data
        {
            int dtaVal = popDta(postNumNow);
            // post data here ?
            yeelinkPost(atomId[postNumNow][0], dtaVal);
#if __Debug||1
            cout << "postDta: " << postNumNow << endl;
#endif
        }
        
        postNumNow++;
        if(postNumNow >= atomNum)
        {
            postNumNow = 0;
        }
        
        return 1;
    }
    
    return 0;
}

/*********************************************************************************************************
** Function name:           postDta to yeelink
** Descriptions:            return 1: ok 0: nok
*********************************************************************************************************/
void NodeManage::yeelinkAdd(unsigned char idNode, unsigned char idSensor, unsigned char idActuator)
{
    char tmp[20];
    sprintf(tmp, "ss1 %d,%d,%dgg", idNode, idSensor, idActuator);
#if __Debug
    cout << tmp << endl;
#endif
    mySerial.println(tmp);
}

/*********************************************************************************************************
** Function name:           postDta to yeelink
** Descriptions:            return 1: ok 0: nok
*********************************************************************************************************/
void NodeManage::yeelinkPost(unsigned char idNode, unsigned int psDta)
{
    char tmp[20];
    sprintf(tmp, "ss3 %d,%d.00gg", idNode, psDta);
#if __Debug
    cout << tmp << endl;
#endif
    mySerial.println(tmp);
}

NodeManage NODE;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/