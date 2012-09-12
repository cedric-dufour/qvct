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
#include <QMap>
#include <QRegExp>
#include <QString>

// QVCT
#include "QVCTRuntime.hpp"
#include "units/CUnitTimeDelta.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitTimeDeltaSymbols CUnitTimeDelta::oUnitTimeDeltaSymbols;
const CUnitTimeDeltaCodes CUnitTimeDelta::oUnitTimeDeltaCodes;

const QMap<CUnitTimeDelta::EUnit,QString> &CUnitTimeDelta::symbols()
{
  return oUnitTimeDeltaSymbols.qMapSymbols;
}

QString CUnitTimeDelta::toSymbol( CUnitTimeDelta::EUnit _eUnit )
{
  return oUnitTimeDeltaSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitTimeDelta::EUnit CUnitTimeDelta::fromSymbol( const QString& _rqString )
{
  return oUnitTimeDeltaSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitTimeDelta::EUnit,QString> &CUnitTimeDelta::codes()
{
  return oUnitTimeDeltaCodes.qMapCodes;
}

QString CUnitTimeDelta::toCode( CUnitTimeDelta::EUnit _eUnit )
{
  return oUnitTimeDeltaCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitTimeDelta::EUnit CUnitTimeDelta::fromCode( const QString& _rqString )
{
  return oUnitTimeDeltaCodes.qMapCodes.key( _rqString, UNDEFINED );
}

QString CUnitTimeDelta::toString( double _fdValue, CUnitTimeDelta::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {

  case HMS:
    {
      double __fdSec = fabs( _fdValue );
      double __fdMin;
      modf( __fdSec / 60.0, &__fdMin );
      __fdSec -= __fdMin * 60.0;
      double __fdHour;
      modf( __fdMin / 60.0, &__fdHour );
      __fdMin -= __fdHour * 60.0;
      QString __qsSec = QString::number( __fdSec, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsSec[0] == '1' ) { __fdMin += 1; __qsSec[0] = '0'; } // fix rounding-up
      QString __qsMin = QString::number( __fdMin, 'f', 0 ).prepend( "0" ).right( 2 );
      if( __qsMin[0] == '6' ) { __fdHour += 1; __qsMin[0] = '0'; } // fix rounding-up
      return QString::number( _fdValue < 0 ? -__fdHour : __fdHour, 'f', 0 )+":"+__qsMin+":"+__qsSec;
    }

  case HM:
    {
      double __fdMin = fabs( _fdValue ) / 60.0;
      double __fdHour;
      modf( __fdMin / 60.0, &__fdHour );
      __fdMin -= __fdHour * 60.0;
      QString __qsMin = QString::number( __fdMin, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsMin[0] == '6' ) { __fdHour += 1; __qsMin[0] = '0'; } // fix rounding-up
      return QString::number( _fdValue < 0 ? -__fdHour : __fdHour, 'f', 0 )+":"+__qsMin;
    }

  case M:
    return QString::number( _fdValue / 60.0, 'f', _iPrecision )+"'";

  case S:
    return QString::number( _fdValue, 'f', _iPrecision )+"\"";

  default: return "?";
  }
}

QString CUnitTimeDelta::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitTimeDelta(), __poSettings->getPrecisionTimeDelta() );
}

double CUnitTimeDelta::fromString( const QString& _rqString, bool* _pbOK )
{
  static const QRegExp __qRegExpHMS( "^\\s*(-?)(0?\\d|1\\d|2[0-3]):(0?\\d|[1-5]\\d):(0?\\d|[1-5]\\d(\\.\\d+)?)\\s*$" );
  static const QRegExp __qRegExpHM( "^\\s*(-?)(0?\\d|1\\d|2[0-3]):((0?\\d|[1-5]\\d)(\\.\\d+)?)\\s*$" );
  static const QRegExp __qRegExpM( "^\\s*(-?)(\\d+(\\.\\d+)?)\\s*'\\s*$" );
  static const QRegExp __qRegExpS( "^\\s*(-?)(\\d+(\\.\\d+)?)\\s*\"\\s*$" );

  if( _pbOK ) *_pbOK = false;
  bool __bOK;
  double __fdValue;
  QString __qsSign;
  if( __qRegExpHMS.exactMatch( _rqString ) )
  {
    __qsSign = __qRegExpHMS.cap(1);
    __fdValue = __qRegExpHMS.cap(2).toDouble( &__bOK ) * 3600.0;
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpHMS.cap(3).toDouble( &__bOK ) * 60.0;
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpHMS.cap(4).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
  }
  else if( __qRegExpHM.exactMatch( _rqString ) )
  {
    __qsSign = __qRegExpHM.cap(1);
    __fdValue = __qRegExpHM.cap(2).toDouble( &__bOK ) * 3600.0;
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpHM.cap(3).toDouble( &__bOK ) * 60.0;
    if( !__bOK ) return 0.0;
  }
  else if( __qRegExpM.exactMatch( _rqString ) )
  {
    __qsSign = __qRegExpM.cap(1);
    __fdValue = __qRegExpM.cap(2).toDouble( &__bOK ) * 60.0;
    if( !__bOK ) return 0.0;
  }
  else if( __qRegExpS.exactMatch( _rqString ) )
  {
    __qsSign = __qRegExpS.cap(1);
    __fdValue = __qRegExpS.cap(2).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
  }
  else return 0.0;
  if( !__qsSign.isEmpty() ) __fdValue = -__fdValue;
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}
