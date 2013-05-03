/*
  CloudCfg.h
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
#ifndef __CLOUDCFG_H__
#define __CLOUDCFG_H__

#include "CloudGlobalDfs.h"

class CloudCfg{
    
    public:
    
    unsigned char ifSetDevice;          // if config device
    unsigned char idDevice;             // id of device
    
    unsigned char ifSetSensor;          // if config sensor
    unsigned char idSensor;             // id of sensor

    unsigned char ifCloud;              // if there is a cloud in the net
    
    private:
    
    unsigned char dtaLight[10];

    public:
    
    void init();
    bool cloudConfig();                 // config!
    
};

extern CloudCfg CONFIG;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/