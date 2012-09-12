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
#include <QDateTime>

// QVCT
#include "QVCTRuntime.hpp"
#include "data/CDataPositionValidity.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDataPositionValidity::CDataPositionValidity()
  : fdTimeLastPosition( UNDEFINED_VALUE )
  , fdErrorPosition( UNDEFINED_VALUE )
  , bInvalidPosition( false )
  , fdTimeLastElevation( UNDEFINED_VALUE )
  , fdErrorElevation( UNDEFINED_VALUE )
  , bInvalidElevation( false )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTERS
//

bool CDataPositionValidity::isValidPosition() const
{
  if( bInvalidPosition ) return false;
  if( fdErrorPosition == UNDEFINED_VALUE || fdTimeLastPosition == UNDEFINED_VALUE ) return false;
  if( fdErrorPosition > QVCTRuntime::useSettings()->getMaxErrorPosition() ) return false;
  QDateTime __qDateTime = QDateTime::currentDateTime();
  double __fdCurrentTime = __qDateTime.toUTC().toTime_t()+__qDateTime.time().msec()/1000.0;
  if( __fdCurrentTime - fdTimeLastPosition > QVCTRuntime::useSettings()->getMaxAgePosition() ) return false;
  return true;
}

bool CDataPositionValidity::isValidElevation() const
{
  if( bInvalidElevation ) return false;
  if( fdErrorElevation == UNDEFINED_VALUE || fdTimeLastElevation == UNDEFINED_VALUE ) return false;
  if( fdErrorElevation > QVCTRuntime::useSettings()->getMaxErrorElevation() ) return false;
  QDateTime __qDateTime = QDateTime::currentDateTime();
  double __fdCurrentTime = __qDateTime.toUTC().toTime_t()+__qDateTime.time().msec()/1000.0;
  if( __fdCurrentTime - fdTimeLastElevation > QVCTRuntime::useSettings()->getMaxAgeElevation() ) return false;
  return true;
}
