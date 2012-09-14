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
#include "data/CDataCourseValidity.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDataCourseValidity::CDataCourseValidity()
  : fdTimeLastBearing( UNDEFINED_VALUE )
  , fdErrorBearing( UNDEFINED_VALUE )
  , bInvalidBearing( false )
  , fdTimeLastSpeed( UNDEFINED_VALUE )
  , fdErrorSpeed( UNDEFINED_VALUE )
  , bInvalidSpeed( false )
  , fdTimeLastSpeedVertical( UNDEFINED_VALUE )
  , fdErrorSpeedVertical( UNDEFINED_VALUE )
  , bInvalidSpeedVertical( false )
{}

CDataCourseValidityGA::CDataCourseValidityGA()
  : GroundCourseValidity()
  , ApparentCourseValidity()
{}

CDataCourseValidityGA::CDataCourseValidityGA( const CDataCourseValidity& _roDataCourseValidityGround,
                                            const CDataCourseValidity& _roDataCourseValidityApparent )
  : GroundCourseValidity( _roDataCourseValidityGround )
  , ApparentCourseValidity( _roDataCourseValidityApparent )
{}

//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTERS
//

bool CDataCourseValidity::isValidBearing() const
{
  if( bInvalidBearing ) return false;
  if( fdErrorBearing != UNDEFINED_VALUE && fdErrorBearing > QVCTRuntime::useSettings()->getMaxErrorBearing() ) return false;
  if( fdTimeLastBearing != UNDEFINED_VALUE && microtime() - fdTimeLastBearing > QVCTRuntime::useSettings()->getMaxAgeBearing() ) return false;
  return true;
}

bool CDataCourseValidity::isValidSpeed() const
{
  if( bInvalidSpeed ) return false;
  if( fdErrorSpeed != UNDEFINED_VALUE && fdErrorSpeed > QVCTRuntime::useSettings()->getMaxErrorSpeed() ) return false;
  if( fdTimeLastSpeed != UNDEFINED_VALUE && microtime() - fdTimeLastSpeed > QVCTRuntime::useSettings()->getMaxAgeSpeed() ) return false;
  return true;
}

bool CDataCourseValidity::isValidSpeedVertical() const
{
  if( bInvalidSpeedVertical ) return false;
  if( fdErrorSpeedVertical != UNDEFINED_VALUE && fdErrorSpeedVertical > QVCTRuntime::useSettings()->getMaxErrorSpeedVertical() ) return false;
  if( fdTimeLastSpeedVertical != UNDEFINED_VALUE && microtime() - fdTimeLastSpeedVertical > QVCTRuntime::useSettings()->getMaxAgeSpeedVertical() ) return false;
  return true;
}

