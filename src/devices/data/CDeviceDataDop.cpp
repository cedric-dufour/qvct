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

// QT
#include <QString>

// QVCT
#include "devices/data/CDeviceDataDop.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const double CDeviceDataDop::UNDEFINED_VALUE = -9999999;
const CDeviceDataDop CDeviceDataDop::UNDEFINED( CDeviceDataDop::UNDEFINED_VALUE, CDeviceDataDop::UNDEFINED_VALUE, CDeviceDataDop::UNDEFINED_VALUE );

//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceDataDop::CDeviceDataDop( double _fdDopHorizontal, double _fdDopVertical, double _fdDopTime )
  : fdDopHorizontal( _fdDopHorizontal )
  , fdDopVertical( _fdDopVertical )
  , fdDopTime( _fdDopTime )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

void CDeviceDataDop::setDop( const CDeviceDataDop& _roDeviceDataDop )
{
  fdDopHorizontal = _roDeviceDataDop.fdDopHorizontal;
  fdDopVertical = _roDeviceDataDop.fdDopVertical;
  fdDopTime = _roDeviceDataDop.fdDopTime;
}

double CDeviceDataDop::getDopPosition() const
{
  if( fdDopHorizontal == UNDEFINED_VALUE || fdDopVertical == UNDEFINED_VALUE )
    return UNDEFINED_VALUE;
  return sqrt( fdDopHorizontal*fdDopHorizontal + fdDopVertical*fdDopVertical );
}

double CDeviceDataDop::getDopGlobal() const
{
  if( fdDopTime == UNDEFINED_VALUE || fdDopHorizontal == UNDEFINED_VALUE || fdDopVertical == UNDEFINED_VALUE )
    return UNDEFINED_VALUE;
  return sqrt( fdDopTime*fdDopTime + fdDopHorizontal*fdDopHorizontal + fdDopVertical*fdDopVertical );
}

//
// OPERATORS
//

bool CDeviceDataDop::operator==( const CDeviceDataDop& _roDeviceDataDop ) const
{
  return( fdDopHorizontal == _roDeviceDataDop.fdDopHorizontal
          && fdDopVertical == _roDeviceDataDop.fdDopVertical
          && fdDopTime == _roDeviceDataDop.fdDopTime );
}

bool CDeviceDataDop::operator!=( const CDeviceDataDop& _roDeviceDataDop ) const
{
  return( fdDopHorizontal != _roDeviceDataDop.fdDopHorizontal
          || fdDopVertical != _roDeviceDataDop.fdDopVertical
          || fdDopTime != _roDeviceDataDop.fdDopTime );
}

