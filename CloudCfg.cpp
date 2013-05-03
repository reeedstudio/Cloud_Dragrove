/*
  CloudCfg.cpp
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

#include <EEPROM.h>
#include <BeaconSensor.h>
#include <BeaconActuator.h>
#include <BeaconActuatorDfs.h>
#include <BeaconSensorDfs.h>
#include <I2C_Uart.h>
#include <BeaconDrive.h>

#include "CloudApp.h"
#include "CloudCfg.h"
#include "CloudGlobalDfs.h"

/*********************************************************************************************************
** Function name:           init
** Descriptions:            init
*********************************************************************************************************/
void CloudCfg::init()
{

    ifCloud     = 0;
    ifSetDevice = EEPROM.read(EEPADDISSET);
    
    if(ifSetDevice != 0x55)
    {
        return;
    }
    
    idDevice = 0x80;
    ifSetSensor = EEPROM.read(EEPADDIFSETSE);
    if(ifSetSensor == 0x55)
    {
        idSensor    = EEPROM.read(EEPADDSENSORID);                      // id of sensor
    }

}

/*********************************************************************************************************
** Function name:           cloudConfig
** Descriptions:            config
*********************************************************************************************************/
bool CloudCfg::cloudConfig()
{
    return 1;
    // add code here

}

CloudCfg CONFIG;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
