/*
  CloudApp.cpp
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

#include "CloudApp.h"
#include "CloudGlobalDfs.h"

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void CloudApp::init()
{
    workStateCnt    = 0;
    BcnDrive.init();
}

/*********************************************************************************************************
** Function name:           appTimerIsr
** Descriptions:            appTimerIsr
*********************************************************************************************************/
void CloudApp::appTimerIsr()
{
    workStateCnt++;
}

/*********************************************************************************************************
** Function name:           carryState
** Descriptions:            carryState
*********************************************************************************************************/
void CloudApp::cloudWorking()
{

        if(workStateCnt % 1000 == 10)               // send sync
        {
            workStateCnt++;
        }

}

CloudApp APP;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/