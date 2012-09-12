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
#include <QDataStream>

// QVCT
#include "data/CDataCourse.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CDataCourse CDataCourse::UNDEFINED( CDataCourse::UNDEFINED_BEARING, CDataCourse::UNDEFINED_SPEED, CDataCourse::UNDEFINED_SPEED );


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDataCourse::CDataCourse()
  : fdBearing( CDataCourse::UNDEFINED_BEARING )
  , fdSpeed( CDataCourse::UNDEFINED_SPEED )
  , fdSpeedVertical( CDataCourse::UNDEFINED_SPEED )
{}

CDataCourse::CDataCourse( double _fdBearing, double _fdSpeed, double _fdSpeedVertical )
{
  setCourse( _fdBearing, _fdSpeed, _fdSpeedVertical );
}

CDataCourse::CDataCourse( const CDataCourse& _roDataCourse )
{
  setCourse( _roDataCourse );
}

CDataCourseGA::CDataCourseGA()
  : GroundCourse()
  , ApparentCourse()
{}

CDataCourseGA::CDataCourseGA( const CDataCourse& _roDataCourseGround, const CDataCourse& _roDataCourseApparent )
  : GroundCourse( _roDataCourseGround )
  , ApparentCourse( _roDataCourseApparent )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CDataCourse::setCourse( double _fdBearing, double _fdSpeed, double _fdSpeedVertical )
{
  if( _fdBearing != UNDEFINED_BEARING )
  {
    // Normalize bearing => [0;360[
    while( _fdBearing >= 360 ) _fdBearing -= 360;
    while( _fdBearing < 0 ) _fdBearing += 360;
  }

  // Save course
  fdBearing = _fdBearing;
  fdSpeed = _fdSpeed;
  fdSpeedVertical = _fdSpeedVertical;
}

void CDataCourse::setCourse( const CDataCourse& _roDataCourse )
{
  fdBearing = _roDataCourse.fdBearing;
  fdSpeed = _roDataCourse.fdSpeed;
  fdSpeedVertical = _roDataCourse.fdSpeedVertical;
}

void CDataCourse::resetCourse()
{
  fdBearing = UNDEFINED_BEARING;
  fdSpeed = UNDEFINED_SPEED;
  fdSpeedVertical = UNDEFINED_SPEED;
}

//
// OTHER
//

void CDataCourse::serialize( QDataStream& _rqDataStream ) const
{
  _rqDataStream << fdBearing << fdSpeed << fdSpeedVertical;
}

void CDataCourse::unserialize( QDataStream& _rqDataStream )
{
  _rqDataStream >> fdBearing >> fdSpeed >> fdSpeedVertical;
}

void CDataCourseGA::serialize( QDataStream& _rqDataStream ) const
{
  GroundCourse.serialize( _rqDataStream );
  ApparentCourse.serialize( _rqDataStream );
}

void CDataCourseGA::unserialize( QDataStream& _rqDataStream )
{
  GroundCourse.unserialize( _rqDataStream );
  ApparentCourse.unserialize( _rqDataStream );
}

//
// OPERATORS
//
bool CDataCourse::operator==( const CDataCourse& _roCourse ) const
{
  return( fdBearing == _roCourse.fdBearing
          && fdSpeed == _roCourse.fdSpeed
          && fdSpeedVertical == _roCourse.fdSpeedVertical );
}

bool CDataCourse::operator!=( const CDataCourse& _roCourse ) const
{
  return( fdBearing != _roCourse.fdBearing
          || fdSpeed != _roCourse.fdSpeed
          || fdSpeedVertical != _roCourse.fdSpeedVertical );
}

//
// COMPARATORS
//

bool CDataCourse::compareBearingAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdBearing < _roCourse2.fdBearing );
}

bool CDataCourse::compareBearingDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdBearing > _roCourse2.fdBearing );
}

bool CDataCourse::compareSpeedAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdSpeed < _roCourse2.fdSpeed );
}

bool CDataCourse::compareSpeedDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdSpeed > _roCourse2.fdSpeed );
}

bool CDataCourse::compareSpeedVerticalAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdSpeedVertical < _roCourse2.fdSpeedVertical );
}

bool CDataCourse::compareSpeedVerticalDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 )
{
  return( _roCourse1.fdSpeedVertical > _roCourse2.fdSpeedVertical );
}
