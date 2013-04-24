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

#include "CloudApplication.h"
#include "CloudConfig.h"
#include "CloudGlobalDfs.h"

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void BeaconApplication::init()
{
    workStateCnt    = 0;

    BcnDrive.init();
}

/*********************************************************************************************************
** Function name:           appTimerIsr
** Descriptions:            appTimerIsr
*********************************************************************************************************/
void BeaconApplication::appTimerIsr()
{
    BcnDrive.ledIsr();
    workStateCnt++;
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
    BcnDrive.setLedShine(LEDCOLORGREEN, 5);

    unsigned char dtaSe[10];
    SENSOR.getSensor(dtaSe);

    dtaSendRf[0] = CONFIG.idDevice;
    dtaSendRf[1] = CONFIG.idSensor;
    dtaSendRf[2] = 0;
    dtaSendRf[3] = FRAMETYPEBC;
    dtaSendRf[4] = dtaSe[0];

    for(int i = 0; i<dtaSe[0]; i++)
    {
        dtaSendRf[i+5] = dtaSe[i+1];
    }
    dtaSendRf[5+dtaSe[0]] = 0;

    sendDtaRfbee(6+dtaSe[0], dtaSendRf);

}

/*********************************************************************************************************
** Function name:           sendJoin
** Descriptions:            sendJoin
*********************************************************************************************************/
void BeaconApplication::sendJoin()
{
    // add code here
}

/*********************************************************************************************************
** Function name:           carryState
** Descriptions:            carryState
*********************************************************************************************************/
void BeaconApplication::cloudWorking()
{

        if(workStateCnt % 1000 == 10)               // send sync
        {
            workStateCnt++;
        }
        else if(workStateCnt % 1000 == 50)          // broadcast sensor value
        {
            workStateCnt++;
            sensorBroadCast();                      // broadcast
        }
}

BeaconApplication BeaconApp;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
