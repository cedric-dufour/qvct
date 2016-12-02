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
#include <QDataStream>

// GPS
#include <gps.h>

// QVCT
#include "data/CDataPosition.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CDataPosition CDataPosition::UNDEFINED( CDataPosition::UNDEFINED_LONGITUDE,
                                            CDataPosition::UNDEFINED_LATITUDE,
                                            CDataPosition::UNDEFINED_ELEVATION );

double CDataPosition::distanceGC( const CDataPosition& _roGP1, const CDataPosition& _roGP2 )
{
  // NOTE: DEG_2_RAD and RAD_2_DEG come from "gps.h"
  double __fdLon1 = _roGP1.fdLongitude * DEG_2_RAD, __fdLat1 = _roGP1.fdLatitude * DEG_2_RAD;
  double __fdLon2 = _roGP2.fdLongitude * DEG_2_RAD, __fdLat2 = _roGP2.fdLatitude * DEG_2_RAD;
  double __fdLonD = __fdLon2 - __fdLon1, __fdLatD = __fdLat2 - __fdLat1;

  // Formula shamelessly copied from http://www.movable-type.co.uk/scripts/latlong.html
  double __fdLonS = sin( __fdLonD/2.0 ), __fdLatS = sin( __fdLatD/2.0 );
  double __fdA = __fdLatS*__fdLatS + cos( __fdLat1 )*cos( __fdLat2 )*__fdLonS*__fdLonS;
  double __fdDistance = ( WGS84A + WGS84B ) * atan2( sqrt( __fdA ), sqrt( 1.0-__fdA ) );
  return __fdDistance;
}

double CDataPosition::bearingGC( const CDataPosition& _roGP1, const CDataPosition& _roGP2 )
{
  // NOTE: DEG_2_RAD and RAD_2_DEG come from "gps.h"
  double __fdLon1 = _roGP1.fdLongitude * DEG_2_RAD, __fdLat1 = _roGP1.fdLatitude * DEG_2_RAD;
  double __fdLon2 = _roGP2.fdLongitude * DEG_2_RAD, __fdLat2 = _roGP2.fdLatitude * DEG_2_RAD;
  double __fdLonD = __fdLon2 - __fdLon1;

  // Formula shamelessly copied from http://www.movable-type.co.uk/scripts/latlong.html
  double __fdBearing = atan2( sin( __fdLonD )*cos( __fdLat2 ),
                              cos( __fdLat1 )*sin( __fdLat2 ) - sin( __fdLat1 )*cos( __fdLat2 )*cos( __fdLonD ) );
  __fdBearing *= RAD_2_DEG;
  if( __fdBearing < 0 ) __fdBearing += 360;
  return __fdBearing;
}

double CDataPosition::distanceRL( const CDataPosition& _roGP1, const CDataPosition& _roGP2 )
{
  // NOTE: GPS_PI, DEG_2_RAD, RAD_2_DEG and WGS84A come from "gps.h"
  double __fdLon1 = _roGP1.fdLongitude * DEG_2_RAD, __fdLat1 = _roGP1.fdLatitude * DEG_2_RAD;
  double __fdLon2 = _roGP2.fdLongitude * DEG_2_RAD, __fdLat2 = _roGP2.fdLatitude * DEG_2_RAD;
  double __fdLonD = __fdLon2 - __fdLon1, __fdLatD = __fdLat2 - __fdLat1;

  // Formula shamelessly copied from http://www.movable-type.co.uk/scripts/latlong.html
  double __fdPhiD = log( tan( __fdLat2/2.0 + GPS_PI/4.0 ) / tan( __fdLat1/2.0 + GPS_PI/4.0 ) );
  double __fdQ = __fdPhiD != 0 ? __fdLatD/__fdPhiD : cos( __fdLat1 );
  if( fabs( __fdLonD ) > GPS_PI ) __fdLonD = __fdLonD > 0 ? __fdLonD - GPS_PI*2.0 : GPS_PI*2.0 + __fdLonD;
  double __fdDistance = ( WGS84A + WGS84B ) / 2.0 * sqrt( __fdLatD*__fdLatD + __fdLonD*__fdLonD * __fdQ*__fdQ );
  return __fdDistance;
}

double CDataPosition::bearingRL( const CDataPosition& _roGP1, const CDataPosition& _roGP2 )
{
  // NOTE: GPS_PI, DEG_2_RAD and RAD_2_DEG come from "gps.h"
  double __fdLon1 = _roGP1.fdLongitude * DEG_2_RAD, __fdLat1 = _roGP1.fdLatitude * DEG_2_RAD;
  double __fdLon2 = _roGP2.fdLongitude * DEG_2_RAD, __fdLat2 = _roGP2.fdLatitude * DEG_2_RAD;
  double __fdLonD = __fdLon2 - __fdLon1;

  // Formula shamelessly copied from http://www.movable-type.co.uk/scripts/latlong.html
  double __fdPhiD = log( tan( __fdLat2/2.0 + GPS_PI/4.0 ) / tan( __fdLat1/2.0 + GPS_PI/4.0 ) );
  if( fabs( __fdLonD ) > GPS_PI ) __fdLonD = __fdLonD > 0 ? __fdLonD - GPS_PI*2.0 : GPS_PI*2.0 + __fdLonD;
  double __fdBearing = atan2( __fdLonD, __fdPhiD );
  __fdBearing *= RAD_2_DEG;
  if( __fdBearing < 0 ) __fdBearing += 360;
  return __fdBearing;
}

double CDataPosition::length( const CDataPosition& _roGP1, const CDataPosition& _roGP2 )
{
  double __fdDistance = CDataPosition::distanceRL( _roGP1, _roGP2 );
  if( _roGP1.fdElevation != UNDEFINED_ELEVATION && _roGP2.fdElevation != UNDEFINED_ELEVATION )
  {
    double __fdClimb = _roGP2.fdElevation - _roGP1.fdElevation;
    __fdDistance = sqrt( __fdDistance*__fdDistance + __fdClimb*__fdClimb );
  }
  return __fdDistance;
}


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDataPosition::CDataPosition()
  : fdLongitude( CDataPosition::UNDEFINED_LONGITUDE )
  , fdLatitude( CDataPosition::UNDEFINED_LATITUDE )
  , fdElevation( CDataPosition::UNDEFINED_ELEVATION )
{}

CDataPosition::CDataPosition( double _fdLongitude, double _fdLatitude, double _fdElevation )
{
  setPosition( _fdLongitude, _fdLatitude, _fdElevation );
}

CDataPosition::CDataPosition( const CDataPosition& _roDataPosition )
{
  setPosition( _roDataPosition );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CDataPosition::setPosition( double _fdLongitude, double _fdLatitude, double _fdElevation )
{
  if( _fdLongitude != UNDEFINED_LONGITUDE && _fdLatitude != UNDEFINED_LATITUDE )
  {
    // Normalize latitude => [-90;90]
    while( _fdLatitude >= 270 ) _fdLatitude -= 360;
    while( _fdLatitude <= -270 ) _fdLatitude += 360;
    if( _fdLatitude > 90 ) { _fdLatitude = 180 - _fdLatitude; _fdLongitude += 180; }
    if( _fdLatitude < -90 ) { _fdLatitude = -180 - _fdLatitude; _fdLongitude += 180; }

    // Normalize longitude => [-180;180]
    while( _fdLongitude > 180 ) _fdLongitude -= 360;
    while( _fdLongitude < -180 ) _fdLongitude += 360;
  }
  else
  {
    _fdLongitude = UNDEFINED_LONGITUDE;
    _fdLatitude = UNDEFINED_LATITUDE;
  }

  // Save position
  fdLongitude = _fdLongitude;
  fdLatitude = _fdLatitude;
  fdElevation = _fdElevation;
}

void CDataPosition::setPosition( const CDataPosition& _roDataPosition )
{
  fdLongitude = _roDataPosition.fdLongitude;
  fdLatitude = _roDataPosition.fdLatitude;
  fdElevation = _roDataPosition.fdElevation;
}

void CDataPosition::resetPosition()
{
  fdLongitude = UNDEFINED_LONGITUDE;
  fdLatitude = UNDEFINED_LATITUDE;
  fdElevation = UNDEFINED_ELEVATION;
}

//
// OTHER
//

void CDataPosition::serialize( QDataStream& _rqDataStream ) const
{
  _rqDataStream << fdLongitude << fdLatitude << fdElevation;
}

void CDataPosition::unserialize( QDataStream& _rqDataStream )
{
  _rqDataStream >> fdLongitude >> fdLatitude >> fdElevation;
}

//
// OPERATORS
//
bool CDataPosition::operator==( const CDataPosition& _roPosition ) const
{
  return( fdLongitude == _roPosition.fdLongitude
          && fdLatitude == _roPosition.fdLatitude
          && fdElevation == _roPosition.fdElevation );
}

bool CDataPosition::operator!=( const CDataPosition& _roPosition ) const
{
  return( fdLongitude != _roPosition.fdLongitude
          || fdLatitude != _roPosition.fdLatitude
          || fdElevation != _roPosition.fdElevation );
}

//
// COMPARATORS
//

bool CDataPosition::compareLongitudeAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdLongitude < _roPosition2.fdLongitude );
}

bool CDataPosition::compareLongitudeDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdLongitude > _roPosition2.fdLongitude );
}

bool CDataPosition::compareLatitudeAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdLatitude < _roPosition2.fdLatitude );
}

bool CDataPosition::compareLatitudeDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdLatitude > _roPosition2.fdLatitude );
}

bool CDataPosition::compareElevationAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdElevation < _roPosition2.fdElevation );
}

bool CDataPosition::compareElevationDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 )
{
  return( _roPosition1.fdElevation > _roPosition2.fdElevation );
}
