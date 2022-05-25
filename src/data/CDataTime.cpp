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

// C/C++
#include <cmath>
#include <time.h>

// QT
#include <QDataStream>

// QVCT
#include "data/CDataTime.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CDataTime CDataTime::UNDEFINED( CDataTime::UNDEFINED_TIME );


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDataTime::CDataTime( double _fdTime )
{
  setTime( _fdTime );
}

CDataTime::CDataTime( const CDataTime& _roDataTime )
{
  setTime( _roDataTime );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CDataTime::setTime( double _fdTime )
{
  if( _fdTime < 0.0 ) _fdTime = UNDEFINED_TIME;
  fdTime = _fdTime;
}

void CDataTime::setTime( time_t _tSeconds, long _lNanoSeconds )
{
  if( _tSeconds < 0 ) {
    fdTime = UNDEFINED_TIME;
  }
  else {
    fdTime = (double)_tSeconds + (double)_lNanoSeconds/1000000000.0;
  }
}

void CDataTime::setTime( const CDataTime& _roDataTime )
{
  fdTime = _roDataTime.fdTime;
}

//
// OTHER
//

void CDataTime::serialize( QDataStream& _rqDataStream ) const
{
  _rqDataStream << fdTime;
}

void CDataTime::unserialize( QDataStream& _rqDataStream )
{
  _rqDataStream >> fdTime;
}

//
// OPERATORS
//
bool CDataTime::operator==( const CDataTime& _roPC ) const
{
  return( fdTime == _roPC.fdTime );
}

bool CDataTime::operator!=( const CDataTime& _roPC ) const
{
  return( fdTime != _roPC.fdTime );
}

//
// COMPARATORS
//

bool CDataTime::compareTimeAscending( const CDataTime& _roTime1, const CDataTime& _roTime2 )
{
  return( _roTime1.fdTime < _roTime2.fdTime );
}

bool CDataTime::compareTimeDescending( const CDataTime& _roTime1, const CDataTime& _roTime2 )
{
  return( _roTime1.fdTime > _roTime2.fdTime );
}

