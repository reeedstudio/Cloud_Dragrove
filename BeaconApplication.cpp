/*
  BeaconApplication.cpp
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
#include <Arduino.h>
#include <BeaconSensor.h>
#include <BeaconActuator.h>
#include <BeaconActuatorDfs.h>
#include <I2C_Uart.h>
#include <BeaconDrive.h>
#include <EEPROM.h>

#include "BeaconApplication.h"
#include "BeaconConfig.h"
#include "BeaconApplicationDfs.h"
#include "BeaconConfigDfs.h"

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void BeaconApplication::init()
{
    bdFreq          = CONFIG.freqSensor;
    BcnDrive.init();
}

/*********************************************************************************************************
** Function name:           appTimerIsr
** Descriptions:            appTimerIsr
*********************************************************************************************************/
void BeaconApplication::appTimerIsr()
{
    BcnDrive.ledIsr();
}

/*********************************************************************************************************
** Function name:           sendDtaRfbee
** Descriptions:            send buf to rfbee
*********************************************************************************************************/
void BeaconApplication::sendDtaRfbee(unsigned char len, unsigned char *dta)
{
    SendByteToRfbee(FRAMESTART1);
    SendByteToRfbee(FRAMESTART2);
    for(unsigned int i = 0; i<len; i++)
    {
        SendByteToRfbee(dta[i]);
    }
    SendByteToRfbee(FRAMEEND1);
    SendByteToRfbee(FRAMEEND2);

}

/*********************************************************************************************************
** Function name:           sensorBroadCast
** Descriptions:            BroadCast sensor value now
*********************************************************************************************************/
void BeaconApplication::sensorBroadCast()
{
    if(CONFIG.ifSetSensor != 0x55)
    {
        return ;
    }
    
    unsigned char dtaSe[10];
    SENSOR.getSensor(dtaSe);

    dtaSendRf[0] = CONFIG.idDevice;
    dtaSendRf[1] = 0;
    dtaSendRf[2] = FRAMETYPEBC;
    dtaSendRf[3] = dtaSe[0];

    for(int i = 0; i<dtaSe[0]; i++)
    {
        dtaSendRf[i+4] = dtaSe[i+1];
    }
    dtaSendRf[4+dtaSe[0]] = 0;

    sendDtaRfbee(5+dtaSe[0], dtaSendRf);
    sendDtaRfbee(5+dtaSe[0], dtaSendRf);

    for(int i = 4+dtaSe[0]+4; i>=2; i--)
    {
        dtaSendRf[i] = dtaSendRf[i-2];
    }
    dtaSendRf[0] = FRAMESTART1;
    dtaSendRf[1] = FRAMESTART2;
    if(isTrigger(dtaSendRf))                        // trigger device itself
    {
        Trigger(dtaSendRf);
    }
}

/*********************************************************************************************************
** Function name:           isTrigger
** Descriptions:            if trigger
*********************************************************************************************************/
bool BeaconApplication::isTrigger(unsigned char *dta)
{
    __printlnApp("GET IN ISTRIGGER!");

    //BcnDrive.setLedShine(LEDCOLORRED, 5);
    if(CONFIG.ifSetActuator != 0x55)
    {
        __printlnApp("ACTUATOR NO CONFIG!");
        return 0;
    }
    // it is a broadcast frame!
    if(dta[FRAMEBITDESTID] == 0 && dta[FRAMEBITFRAME] == FRAMETYPEBC)
    {
        for(int i = 0; i<CONFIG.nTC; i++)
        {
            if(CONFIG.TC[i][1] == dta[FRAMEBITSRCID])
            {
                __printApp("GET TRIGGER!!\t");
                __printlnApp(i+1);
                tcNum = i+1;
                return tcNum;
            }
            else
            {
                __printlnApp("SRCID ERR!!\t");
                __printApp("tc[i][1] = ");
                __printlnApp(CONFIG.TC[i][1]);
                __printApp("dta[FRA..] = ");
                __printlnApp(dta[FRAMEBITSRCID]); //CONFIG.idDevice
                __printApp("CONFIG.idDevice = ");
                __printlnApp(CONFIG.idDevice);
            }
        }
    }
    else
    {
        __printlnApp("NO TRIGGER!");
    }
    return 0;
}

/*********************************************************************************************************
** Function name:           Trigger
** Descriptions:            do something
*********************************************************************************************************/
void BeaconApplication::Trigger(unsigned char *dta)
{

    //BcnDrive.setLedShine(LEDCOLORRED, 5);
    unsigned char nTmp[3];
    /*
     *      IO Actuator
     */

    __printAppS("trigger");
    if(CONFIG.idActuator <= 200)        // IO
    {
        TriggerAnalog(dta);
    }
    else
    {
        switch(CONFIG.idActuator)
        {
            case ACTUATOROLED12864:

            nTmp[0] = dta[5];
            for(int i = 0; i<dta[5]; i++)
            {
                nTmp[i+1] = dta[6+i];
            }
            ACTUATOR.driveActuator(nTmp);
            break;

            default:
            ;
        }
    }
}

/*********************************************************************************************************
** Function name:           TriggerAnalog
** Descriptions:            trigger when analog
*********************************************************************************************************/
void BeaconApplication::TriggerAnalog(unsigned char *dta)
{

    unsigned int cmpDtaSensor   = 0;
    unsigned int cmpDtaSet      = 0;
    unsigned char dtaAc[2];

    unsigned int cmpLarge      = 0;             // lower limit
    unsigned int cmpSmall      = 0;             // higer limit

    __printAppS("\r\ndta[LEN] = ");
    __printlnAppS(dta[FRAMEBITDATALEN]);

    if(dta[FRAMEBITDATALEN] == 1)
    {
        cmpDtaSensor = dta[EEPOFFSETACDATA];
        for(int i = 0; i<2; i++)
        {
            cmpDtaSet       = cmpDtaSet<<8;
            cmpDtaSet      += CONFIG.TC[tcNum-1][EEPOFFSETACDATA+i];
            __printlnAppS("\r\ncmpDtaSet = :");
            __printlnAppS(cmpDtaSet);
        }
    }
    else
    for(int i = 0; i<dta[FRAMEBITDATALEN]; i++)
    {
        cmpDtaSensor    = cmpDtaSensor<<8;
        cmpDtaSet       = cmpDtaSet<<8;
        cmpDtaSet      += CONFIG.TC[tcNum-1][EEPOFFSETACDATA+i];
        __printlnAppS("\r\ncmpDtaSet = :");
        __printlnAppS(cmpDtaSet);
        cmpDtaSensor   += dta[FRAMEBITDATA+i];
    }

    /*
     *      to give value to cmpLarge and cmpSmall
     */

    switch(CONFIG.TC[tcNum-1][EEPOFFSETACOMTYPE])
    {
        case COMPTYPEACGREAT:
        cmpLarge = 1023;
        cmpSmall = cmpDtaSet;

        break;

        case COMPTYPEACLESS:
        cmpLarge = cmpDtaSet;
        cmpSmall = 0;

        break;

        default:
        ;
    }

    __printlnAppS("TRIGGER:");
    __printAppS("cmpLarge = ");
    __printlnAppS(cmpLarge);
    __printAppS("cmpSmall = ");
    __printlnAppS(cmpSmall);

    unsigned char dtaCmp = (cmpDtaSensor >= cmpSmall && cmpDtaSensor <= cmpLarge);

    dtaAc[0] = 1;
    dtaAc[1] = (CONFIG.TC[tcNum-1][EEPOFFSETACACTIONTYPE] == ACTIONTYPEON) ? dtaCmp : 1-dtaCmp;
    ACTUATOR.driveActuator(dtaAc);
}

/*********************************************************************************************************
** Function name:           sendSync
** Descriptions:            tell other devices to sync
*********************************************************************************************************/
void BeaconApplication::sendSync()
{
    dtaSendRf[0] = CONFIG.idDevice;
    dtaSendRf[1] = 0;
    dtaSendRf[2] = 5;
    dtaSendRf[3] = 0;
    dtaSendRf[4] = 0;
    sendDtaRfbee(5, dtaSendRf);
}

/*********************************************************************************************************
** Function name:           carryState
** Descriptions:            carryState
*********************************************************************************************************/
void BeaconApplication::carryState()
{
  
#if 0
    if(bdFreq == BDF1S)                             // this device is the min id
    {
        if(workStateCnt % 1000 == 10)               // send sync
        {
            workStateCnt++;
            sendSync();
        }
        else if(workStateCnt % 1000 == 40)          // broadcast sensor value
        {
            workStateCnt++;
            sensorBroadCast();                      // broadcast
        }
        else if(workStateCnt % 1000 == 100)         // begin to sleep
        {
            sendRfSleep();                          // tell rfbee to sleep 900ms
            for(int i = 0; i<9; i++)
            {
                BcnDrive.pwrDown(105);              // sleep 100 ms
                BcnDrive.wakeUp();
                if(!digitalRead(PINSYSBUTT))        // button ?
                {
                    workStateBuf = WORKSTATECARRY;
                    delay(10);
                    stateChange(WORKSTATEBUTTON);
                    BcnDrive.rLedCnt = BcnDrive.rLedCnt>100 ? BcnDrive.rLedCnt-100 : 0;
                    BcnDrive.gLedCnt = BcnDrive.gLedCnt>100 ? BcnDrive.gLedCnt-100 : 0;
                    break;
                }
                BcnDrive.rLedCnt = BcnDrive.rLedCnt>100 ? BcnDrive.rLedCnt-100 : 0;
                BcnDrive.gLedCnt = BcnDrive.gLedCnt>100 ? BcnDrive.gLedCnt-100 : 0;
            }
            workStateCnt  = 0;
        }
    }

    else if(bdFreq == BDF100MS)
    {
        if(workStateCnt % 100 == 5 || workStateCnt % 100 == 10)                // send sync
        {
            workStateCnt++;
            sendSync();
        }
        else if(workStateCnt % 100 == 45)
        {
            workStateCnt++;
            sensorBroadCast();                      // broadcast
        }
        else if(workStateCnt % 100 >= 99)
        {
            workStateCnt = 0;
        }
    }
    
#endif
}

BeaconApplication BeaconApp;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
