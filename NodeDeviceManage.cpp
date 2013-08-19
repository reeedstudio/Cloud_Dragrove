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
#include <Arduino.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <Streaming.h>
#include <BeaconDrive.h>
#include <EEPROM.h>

#include <BeaconSensor.h>
#include <Wire.h>
#include <Ultrasonic.h>

#include "NodeDeviceManage.h"
#include "CloudGlobalDfs.h"

SoftwareSerial mySerial(6, 7);  

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void NodeManage::init()
{

    mySerial.begin(9600);
    atomNum     = 0;
    postNumNow  = 0;
    memset(atomId, 0, MAXDEVICE*3);
    memset(atomValue, 0, MAXDEVICE);
    memset(getAtomValue, 0, MAXDEVICE);
    
    yeelinkFree = 1;                                                // if yeelink free

#if 0
    ifSetSensor = EEPROM.read(0);
    if(ifSetSensor)
    {
#if __Debug
        Serial.println("cloud sensor configed");
#endif

        freqCloud   = EEPROM.read(2);
        sensorIdCloud = EEPROM.read(1);

        unsigned char dta[3] = {0, sensorIdCloud, 0};
        addDevice(dta);
    }
    else
    {
#if __Debug
        Serial.println("cloud sensor had not been configed");
#endif
    }
#else



    ifSetSensor = 1;
    freqCloud = 1;
    sensorIdCloud = 48;

    unsigned char dta[3] = {0, sensorIdCloud, 0};
    addDevice(dta);
#endif

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
        Serial.println("Device exist already");
        
#endif
        return -1;
    }
    
    int tmp = checkId(id[0]);
    
    if(tmp == -1)
    {
        DBG("Brand new device!");

        for(int i=0; i<3; i++)
        {
            atomId[atomNum][i] = id[i];
        }

        for(int i=0; i<10; i++)
        yeelinkAdd(atomId[atomNum][0], atomId[atomNum][1], atomId[atomNum][2]);
        return atomNum++;  
        
    }
    else
    {
        DBG("Devide ID exist");
        for(int i=0; i<3; i++)
        {
            atomId[tmp][i] = id[i]; 
        }
        
        for(int i=0; i<10; i++)
        yeelinkAdd(atomId[tmp][0], atomId[tmp][1], atomId[tmp][2]);
        return tmp;
    }

}


/*********************************************************************************************************
** Function name:           checkId
** Descriptions:            if certain id in net, 
** Return:                  -1, no such device, 0-9:Num
*********************************************************************************************************/
int NodeManage::checkId(unsigned char *id)
{

    for(int i=0; i<atomNum; i++)
    {
    
        for(int j=0; j<3; j++)
        {
            if(id[j] == atomId[i][j])
            {
                if(j == 2)
                {
                    return i;
                }
            }
            else
            {
                break;
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
** Function name:           cloudDta
** Descriptions:            get cloud sensor data
*********************************************************************************************************/
void NodeManage::cloudDta()
{
    
    if(!ifSetSensor)return ;

    static long t1 = millis();

    if(millis() - t1 > 10000)
    {
        t1 = millis();
        int dtaSensor = 0;
        SENSOR.init(sensorIdCloud);

        unsigned char dta[10];
        SENSOR.getSensor(dta);

        if(dta[0] == 1)
        {
            dtaSensor = dta[1];
        }
        else if(dta[0] == 2)
        {
            dtaSensor = dta[1];
            dtaSensor = dtaSensor<<8;
            dtaSensor += dta[2];
        }
#if __Debug

        Serial.print("push Cloud Dta: ");
        Serial.println(dtaSensor);
#endif
        pushDta(0, dtaSensor);
    }
}
/*********************************************************************************************************
** Function name:           postDta to yeelink
** Descriptions:            return 1: ok 0: nok
*********************************************************************************************************/
int NodeManage::postDta()
{   

    if(atomNum <= 0)return 0;
    
    cloudDta();

    static long t1 = millis();

    if(millis()-t1 > 12000)
    {
        yeelinkFree = 1;
        t1 = millis();
    }
    else
    {
        return 0;
    }

    if(yeelinkFree)
    {

        yeelinkFree = 0;
        
        int cntTimeOut = 0;
        while(1)
        {
            if(getAtomValue[postNumNow])                    // have data
            {
            
                LEDON();
                delay(5);
                LEDOFF();
           

                int dtaVal = popDta(postNumNow);
                // post data here ?
                yeelinkPost(atomId[postNumNow][0], dtaVal);
                postNumNow++;
                
                if(postNumNow >= atomNum)
                {
                    postNumNow = 0;
                }
#if __Debug
                Serial.print("post Dta: ");
                Serial.println(postNumNow);

#endif
                return 1;
            }
        
            postNumNow++;
            
            if(postNumNow >= atomNum)
            {
                postNumNow = 0;
            }
            

            cntTimeOut++;
            if(cntTimeOut >= atomNum)
            {
                return 0;
            }
        
        }
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
    Serial.println(tmp);
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
    Serial.println(tmp);
#endif
    mySerial.println(tmp);
}

/*********************************************************************************************************
** Function name:           addCloudSensor 
** Descriptions:            add a cloud sensor , write to eeprom: 
                            1: if set
                            2: freq
                            3: sensor id
*********************************************************************************************************/
void NodeManage::addCloudSensor(unsigned char id, unsigned char freq)
{

    ifSetSensor = 1;
    freqCloud   = freq;
    sensorIdCloud = id;

    EEPROM.write(1, ifSetSensor);
    EEPROM.write(2, freqCloud);
    EEPROM.write(3, sensorIdCloud);
}

NodeManage NODE;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
