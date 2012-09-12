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
#include <QDateTime>
#include <QMap>
#include <QRegExp>
#include <QString>
#include <QTime>

// QVCT
#include "QVCTRuntime.hpp"
#include "units/CUnitTime.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitTimeSymbols CUnitTime::oUnitTimeSymbols;
const CUnitTimeCodes CUnitTime::oUnitTimeCodes;

const QMap<CUnitTime::EUnit,QString> &CUnitTime::symbols()
{
  return oUnitTimeSymbols.qMapSymbols;
}

QString CUnitTime::toSymbol( CUnitTime::EUnit _eUnit )
{
  return oUnitTimeSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitTime::EUnit CUnitTime::fromSymbol( const QString& _rqString )
{
  return oUnitTimeSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitTime::EUnit,QString> &CUnitTime::codes()
{
  return oUnitTimeCodes.qMapCodes;
}

QString CUnitTime::toCode( CUnitTime::EUnit _eUnit )
{
  return oUnitTimeCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitTime::EUnit CUnitTime::fromCode( const QString& _rqString )
{
  return oUnitTimeCodes.qMapCodes.key( _rqString, UNDEFINED );
}

QString CUnitTime::toString( double _fdValue, CUnitTimeZone::EUnit _eUnitTimeZone, CUnitTime::EUnit _eUnit, int _iPrecision )
{
  QDateTime __qDateTime = QDateTime::fromTime_t( _fdValue );
  double __fdDummy;
  __qDateTime.addMSecs( 1000.0*modf( _fdValue, &__fdDummy ) + 0.5 );
  QTime __qTime = _eUnitTimeZone == CUnitTimeZone::UTC ? __qDateTime.toUTC().time() : __qDateTime.toLocalTime().time();
  switch( _eUnit )
  {

  case HMS:
    {
      int __iHour = __qTime.hour();
      int __iMin = __qTime.minute();
      double __fdSec = __qTime.second() + (double)__qTime.msec()/1000.0;
      QString __qsSec = QString::number( __fdSec, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsSec[0] == '1' ) { __iMin += 1; __qsSec[0] = '0'; } // fix rounding-up
      QString __qsMin = QString::number( __iMin ).prepend( "0" ).right( 2 );
      if( __qsMin[0] == '6' ) { __iHour += 1; __qsMin[0] = '0'; } // fix rounding-up
      QString __qsHour = QString::number( __iHour );
      return __qsHour+":"+__qsMin+":"+__qsSec+( _eUnitTimeZone == CUnitTimeZone::UTC ? "Z" : "" );
    }

  case HM:
    {
      int __iHour = __qTime.hour();
      double __fdMin = __qTime.minute() + ( (double)__qTime.second() + (double)__qTime.msec()/1000.0 )/60.0;
      QString __qsMin = QString::number( __fdMin, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsMin[0] == '6' ) { __iHour += 1; __qsMin[0] = '0'; } // fix rounding-up
      QString __qsHour = QString::number( __iHour );
      return __qsHour+":"+__qsMin+( _eUnitTimeZone == CUnitTimeZone::UTC ? "Z" : "" );
    }

  default: return "?";
  }
}

QString CUnitTime::toString( double _fdValue, bool _bAlternateTimeZone )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  CUnitTimeZone::EUnit __eUnitTimeZone = __poSettings->getUnitTimeZone();
  if( _bAlternateTimeZone ) __eUnitTimeZone = ( __eUnitTimeZone == CUnitTimeZone::UTC ) ? CUnitTimeZone::LOCAL : CUnitTimeZone::UTC;
  return toString( _fdValue, __eUnitTimeZone, __poSettings->getUnitTime(), __poSettings->getPrecisionTime() );
}

double CUnitTime::fromString( const QString& _rqString, double _fdDate, CUnitTimeZone::EUnit _eUnitTimeZone, bool* _pbOK )
{
  static const QRegExp __qRegExpHMS( "^\\s*(0?\\d|1\\d|2[0-3]):(0?\\d|[1-5]\\d):(0?\\d|[1-5]\\d)(\\.\\d+)?\\s*([ZzLl])?\\s*$" );
  static const QRegExp __qRegExpHM( "^\\s*(0?\\d|1\\d|2[0-3]):((0?\\d|[1-5]\\d)(\\.\\d+)?)\\s*(ZzLl)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  bool __bOK;
  QDateTime __qDateTime;
  __qDateTime.setTime_t( _fdDate );
  if( __qRegExpHMS.exactMatch( _rqString ) )
  {
    CUnitTimeZone::EUnit __eUnitTimeZone = _eUnitTimeZone;
    QString __qsTimeZone = __qRegExpHMS.cap(5).toUpper();
    if( __qsTimeZone == "Z" ) __eUnitTimeZone = CUnitTimeZone::UTC;
    else if( __qsTimeZone == "L" ) __eUnitTimeZone = CUnitTimeZone::LOCAL;
    __qDateTime.setTimeSpec( __eUnitTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
    int __iHour = __qRegExpHMS.cap(1).toInt( &__bOK );
    if( !__bOK ) return 0.0;
    int __iMin = __qRegExpHMS.cap(2).toInt( &__bOK );
    if( !__bOK ) return 0.0;
    int __iSec = __qRegExpHMS.cap(3).toInt( &__bOK );
    if( !__bOK ) return 0.0;
    int __iMilli = 0;
    if( !__qRegExpHMS.cap(4).isEmpty() )
    {
      __iMilli = 1000*__qRegExpHMS.cap(4).toDouble( &__bOK );
      if( !__bOK ) return 0.0;
    }
    __qDateTime.setTime( QTime( __iHour, __iMin, __iSec, __iMilli ) );
  }
  else if( __qRegExpHM.exactMatch( _rqString ) )
  {
    CUnitTimeZone::EUnit __eUnitTimeZone = _eUnitTimeZone;
    QString __qsTimeZone = __qRegExpHM.cap(5).toUpper();
    if( __qsTimeZone == "Z" ) __eUnitTimeZone = CUnitTimeZone::UTC;
    else if( __qsTimeZone == "L" ) __eUnitTimeZone = CUnitTimeZone::LOCAL;
    __qDateTime.setTimeSpec( __eUnitTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
    int __iHour = __qRegExpHM.cap(1).toInt( &__bOK );
    if( !__bOK ) return 0.0;
    double __fdMin = __qRegExpHM.cap(2).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    double __fdSec = modf( __fdMin, &__fdMin ) * 60.0;
    int __iMin = (int)__fdMin;
    int __iMilli = (int)( modf( __fdSec, &__fdSec )*1000.0 ); // WARNING: rounding-up may occur!
    int __iSec = (int)__fdSec;
    if( __iMilli >= 1000 ) { __iMilli -= 1000; __iSec += 1; } // fix rounding-up
    if( __iSec >= 60 ) { __iSec -= 60; __iMin += 1; } // fix rounding-up
    if( __iMin >= 60 ) { __iMin -= 60; __iHour += 1; } // fix rounding-up
    __qDateTime.setTime( QTime( __iHour, __iMin, __iSec, __iMilli ) );
  }
  else return 0.0;
  if( _pbOK ) *_pbOK = true;
  return( (double)__qDateTime.toTime_t() + (double)__qDateTime.time().msec()/1000.0 );
}

double CUnitTime::fromString( const QString& _rqString, double _fdDate, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, _fdDate, __poSettings->getUnitTimeZone(), _pbOK );
}
