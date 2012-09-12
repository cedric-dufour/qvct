// INDENTING (emacs/vi): -*- mode:c++; tab-width:2; c-basic-offset:2; intent-tabs-mode:nil; -*- ex: set tabstop=2 expandtab:

/*
 * Qt Virtual Chart Table (QVCT)
 * Copyright (C) 2012 Cedric Dufour <http://cedric.dufour.name>
 * Author: Cedric Dufour <http://cedric.dufour.name>
 *
 * The Qt Virtual Chart Table (QVCT) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, Version 3.
 *
 * The Qt Virtual Chart Table (QVCT) is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 */

// QT
#include <QMap>
#include <QString>

// QVCT
#include "devices/drivers/CDeviceDriver.hpp"
#include "devices/drivers/GpsdAis/CDeviceGpsdAis.hpp"
#include "devices/drivers/GpsdGps/CDeviceGpsdGps.hpp"
// #include "devices/drivers/Sample/CDeviceSample.hpp" // SAMPLE_DRIVER


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CDeviceDriverNames CDeviceDriver::oDeviceDriverNames;
const CDeviceDriverCodes CDeviceDriver::oDeviceDriverCodes;

const QMap<CDeviceDriver::EDriver,QString> &CDeviceDriver::names()
{
  return oDeviceDriverNames.qMapNames;
}

QString CDeviceDriver::toName( CDeviceDriver::EDriver _eDriver )
{
  return oDeviceDriverNames.qMapNames.value( _eDriver, "?" );
}

CDeviceDriver::EDriver CDeviceDriver::fromName( const QString& _rqString )
{
  return oDeviceDriverNames.qMapNames.key( _rqString, UNDEFINED );
}

const QMap<CDeviceDriver::EDriver,QString> &CDeviceDriver::codes()
{
  return oDeviceDriverCodes.qMapCodes;
}

QString CDeviceDriver::toCode( CDeviceDriver::EDriver _eDriver )
{
  return oDeviceDriverCodes.qMapCodes.value( _eDriver, "undef" );
}

CDeviceDriver::EDriver CDeviceDriver::fromCode( const QString& _rqString )
{
  return oDeviceDriverCodes.qMapCodes.key( _rqString, UNDEFINED );
}

CDevice* CDeviceDriver::createDevice( const QString& _rqsDeviceName, CDeviceDriver::EDriver _eDriver )
{
  switch( _eDriver )
  {

  case GPSD_GPS:
    return new CDeviceGpsdGps( _rqsDeviceName );

  case GPSD_AIS:
    return new CDeviceGpsdAis( _rqsDeviceName );

  // case SAMPLE:                                  // SAMPLE_DRIVER
  //   return new CDeviceSample( _rqsDeviceName ); // SAMPLE_DRIVER

  case UNDEFINED:
  default:; // WHAT THE F*** !?!

  }
  return 0;
}
