/*
  BeaconDemoCodeV0.2.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.
  
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
#include <EEPROM.h>
#include <BeaconSensor.h>
//#include <BeaconActuator.h>
#include <BeaconDrive.h>
#include <TimerOne.h>
#include <Wire.h>
#include <SeeedOLED.h>

#include "CloudApplication.h"
#include "CloudConfig.h"
#include "CloudGlobalDfs.h"

#define __DebugIno      0

#if __DebugIno
#define __printIno(x)   __print(x)
#define __printlnIno(x) __println(x)
#else
#define __printIno(x)
#define __printlnIno(x)
#endif

unsigned char __GdtaUart[50];                   // uart data buff
unsigned char __GdtaUartLen        = 0;         // uart data len
unsigned char __GstringComplete    = 0;         // if get data

/*********************************************************************************************************
** Function name:           timer1ISR
** Descriptions:            timer interrupt service 
*********************************************************************************************************/
void timer1ISR()
{
    BeaconApp.appTimerIsr();                    // application isr
}

void just_for_fun(unsigned char *dta)
{
    for(int i = 0; i<5; i++)
    {
        if(dta[i+2] != 0x55)
        {
            return ;
        }
    }
    BcnDrive.beepOn();
    
    while(1);               // get 10 continus 0x55, down!!
}

/*********************************************************************************************************
** Function name:           checkGoodDta
** Descriptions:            if uart get good data
*********************************************************************************************************/
unsigned char checkGoodDta(unsigned char *dta)
{
    unsigned char ioffset = 0;
    for(ioffset = 0; ioffset < __GdtaUartLen; ioffset++)
    {
        if(dta[ioffset] == FRAMESTART1 && dta[ioffset+1] == FRAMESTART2)
        {
            break;
        }
    }

    if(ioffset > 0)
    {
        for(int i = 0; i<7; i++)
        {
            dta[i] = dta[i+ioffset];
        }
    }

    if((__GdtaUartLen - ioffset)<6 \
        || !(   dta[0] == FRAMESTART1 \
             && dta[1] == FRAMESTART2 \
             && dta[8+dta[6]] == FRAMEEND1 \
             && dta[9+dta[6]] == FRAMEEND2 ))
    {
        __GdtaUartLen     = 0;
        __GstringComplete = 0;
        return 0;
    }

    return 1;
}

/*********************************************************************************************************
** Function name:           rfDtaProc
** Descriptions:            get data from rfBee and process
*********************************************************************************************************/
void rfDtaProc()
{

    if(__GstringComplete == 1 && checkGoodDta(__GdtaUart))                      // if serial get data
    {
        if(__GdtaUart[FRAMEBITFRAME] == 4)                                      // other device join
        {
            // add code here
        }
        else        // add code here
        {
            // add code here
        }
        __GdtaUartLen      = 0;
        __GstringComplete  = 0;
        
        
    }
}

/*********************************************************************************************************
** Function name:           setup
** Descriptions:            setup
*********************************************************************************************************/
void setup()
{
    
    BcnDrive.init();

    Serial.begin(57600);                     // Serial, to send/rev data from RFBee
    
    Serial.println("Serial init over");
    
    CONFIG.init();                              // init config
    SENSOR.init(CONFIG.idSensor);               // init sensor
    BeaconApp.init();                           // init application
    
    Timer1.initialize(1000);                    // set a timer of length 1ms
    Timer1.attachInterrupt(timer1ISR);          // attach the service routine here
    
    BeaconApp.sendJoin();
}

/*********************************************************************************************************
** Function name:           loop
** Descriptions:            loop
*********************************************************************************************************/
void loop()
{
    rfDtaProc();                                // data process
    mySerialEvent();                            // check serial data
}

/*********************************************************************************************************
** Function name:           serialEvent1
** Descriptions:            Serial event
*********************************************************************************************************/
void mySerialEvent() 
{
    while (Serial.available())
    {
        __GdtaUart[__GdtaUartLen++] = (unsigned char)Serial.read();
        __GstringComplete =   (__GdtaUart[__GdtaUartLen-1] == FRAMEEND2) ? 1 : __GstringComplete;
        __GdtaUartLen = (__GdtaUartLen > 45) ? 0 :  __GdtaUartLen;
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/