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
#include <QString>

// QVCT
#include "devices/data/CDeviceDataFix.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

QString CDeviceDataFix::getTypeString( int _eType )
{
  if( _eType == FIX_NONE ) return "none";
  if( _eType & FIX_DGPS )
  {
    if( _eType & FIX_3D ) return "3D / DGPS";
    if( _eType & FIX_2D ) return "2D / DGPS";
  }
  if( _eType & FIX_3D ) return "3D";
  if( _eType & FIX_2D ) return "2D";
  return "";
}


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceDataFix::CDeviceDataFix( const QString& _rqsSourceName, int _eType, double _fdTime )
  : CDeviceDataSource( _rqsSourceName )
  , CDataTime()
  , CDataPosition()
  , CDataCourse()
  , CDeviceDataDop()
  , eType( _eType )
  , iSourcesSeen( 0 )
  , iSourcesUsed( 0 )
  , fdErrorTime( CDataValidity::UNDEFINED_VALUE )
  , fdErrorHorizontal( CDataValidity::UNDEFINED_VALUE )
  , fdErrorVertical( CDataValidity::UNDEFINED_VALUE )
  , fdErrorBearing( CDataValidity::UNDEFINED_VALUE )
  , fdErrorSpeed( CDataValidity::UNDEFINED_VALUE )
  , fdErrorSpeedVertical( CDataValidity::UNDEFINED_VALUE )
  , qsText()
  , bCourseFromPosition( true )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

void CDeviceDataFix::setFix( const CDeviceDataFix& _roDeviceDataFix, bool _bCopyTime, bool _bCopyPosition, bool _bCopyCourse, bool _bCopyDop )
{
  if( _bCopyTime ) CDataTime::setTime( _roDeviceDataFix );
  if( _bCopyPosition ) CDataPosition::setPosition( _roDeviceDataFix );
  if( _bCopyCourse ) CDataCourse::setCourse( _roDeviceDataFix );
  if( _bCopyDop ) CDeviceDataDop::setDop( _roDeviceDataFix );
  eType = _roDeviceDataFix.eType;
  iSourcesSeen = _roDeviceDataFix.iSourcesSeen;
  iSourcesUsed = _roDeviceDataFix.iSourcesUsed;
  fdErrorHorizontal = _roDeviceDataFix.fdErrorHorizontal;
  fdErrorVertical = _roDeviceDataFix.fdErrorVertical;
  fdErrorTime = _roDeviceDataFix.fdErrorTime;
  fdErrorBearing = _roDeviceDataFix.fdErrorBearing;
  fdErrorSpeed = _roDeviceDataFix.fdErrorSpeed;
  fdErrorSpeedVertical = _roDeviceDataFix.fdErrorSpeedVertical;
  qsText = _roDeviceDataFix.qsText;
  bCourseFromPosition = _roDeviceDataFix.bCourseFromPosition;
}

QString CDeviceDataFix::getTypeString() const
{
  return getTypeString( eType );
}
