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
#include <QDate>

// QVCT
#include "QVCTRuntime.hpp"
#include "units/CUnitDate.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitDateSymbols CUnitDate::oUnitDateSymbols;
const CUnitDateCodes CUnitDate::oUnitDateCodes;

const QMap<CUnitDate::EUnit,QString> &CUnitDate::symbols()
{
  return oUnitDateSymbols.qMapSymbols;
}

QString CUnitDate::toSymbol( CUnitDate::EUnit _eUnit )
{
  return oUnitDateSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitDate::EUnit CUnitDate::fromSymbol( const QString& _rqString )
{
  return oUnitDateSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitDate::EUnit,QString> &CUnitDate::codes()
{
  return oUnitDateCodes.qMapCodes;
}

QString CUnitDate::toCode( CUnitDate::EUnit _eUnit )
{
  return oUnitDateCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitDate::EUnit CUnitDate::fromCode( const QString& _rqString )
{
  return oUnitDateCodes.qMapCodes.key( _rqString, UNDEFINED );
}

QString CUnitDate::toString( double _fdValue, CUnitTimeZone::EUnit _eUnitTimeZone, CUnitDate::EUnit _eUnit )
{
  QDateTime __qDateTime = QDateTime::fromTime_t( _fdValue );
  QDate __qDate = _eUnitTimeZone == CUnitTimeZone::UTC ? __qDateTime.toUTC().date() : __qDateTime.toLocalTime().date();
  switch( _eUnit )
  {

  case YMD:
    return __qDate.toString( "yyyy-MM-dd" );

  case DMY:
    return __qDate.toString( "dd.MM.yyyy" );

  case MDY:
    return __qDate.toString( "MM,dd,yyyy" );

  default: return "?";
  }
}

QString CUnitDate::toString( double _fdValue, bool _bAlternateTimeZone )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  CUnitTimeZone::EUnit __eUnitTimeZone = __poSettings->getUnitTimeZone();
  if( _bAlternateTimeZone ) __eUnitTimeZone = ( __eUnitTimeZone == CUnitTimeZone::UTC ) ? CUnitTimeZone::LOCAL : CUnitTimeZone::UTC;
  return toString( _fdValue, __eUnitTimeZone, __poSettings->getUnitDate() );
}

double CUnitDate::fromString( const QString& _rqString, CUnitTimeZone::EUnit _eUnitTimeZone, bool* _pbOK )
{
  static const QRegExp __qRegExpYMD( "^\\s*(197\\d|19[89]\\d|[2-9]\\d\\d\\d)-(0?[1-9]|1[12])-(0?[1-9]|[12]\\d|3[01])\\s*([ZzLl])?\\s*$" );
  static const QRegExp __qRegExpDMY( "^\\s*(0?[1-9]|[12]\\d|3[01]).(0?[1-9]|1[12]).(197\\d|19[89]\\d|[2-9]\\d\\d\\d)\\s*([ZzLl])?\\s*$" );
  static const QRegExp __qRegExpMDY( "^\\s*(0?[1-9]|1[12]),(0?[1-9]|[12]\\d|3[01]),(197\\d|19[89]\\d|[2-9]\\d\\d\\d)\\s*([ZzLl])?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  QDateTime __qDateTime;
  if( __qRegExpYMD.exactMatch( _rqString ) )
  {
    CUnitTimeZone::EUnit __eUnitTimeZone = _eUnitTimeZone;
    QString __qsTimeZone = __qRegExpYMD.cap(4).toUpper();
    if( __qsTimeZone == "Z" ) __eUnitTimeZone = CUnitTimeZone::UTC;
    else if( __qsTimeZone == "L" ) __eUnitTimeZone = CUnitTimeZone::LOCAL;
    __qDateTime.setTimeSpec( __eUnitTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
    __qDateTime.setDate( QDate( __qRegExpYMD.cap(1).toInt(), __qRegExpYMD.cap(2).toInt(), __qRegExpYMD.cap(3).toInt() ) );
  }
  else if( __qRegExpDMY.exactMatch( _rqString ) )
  {
    CUnitTimeZone::EUnit __eUnitTimeZone = _eUnitTimeZone;
    QString __qsTimeZone = __qRegExpDMY.cap(4).toUpper();
    if( __qsTimeZone == "Z" ) __eUnitTimeZone = CUnitTimeZone::UTC;
    else if( __qsTimeZone == "L" ) __eUnitTimeZone = CUnitTimeZone::LOCAL;
    __qDateTime.setTimeSpec( __eUnitTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
    __qDateTime.setDate( QDate( __qRegExpDMY.cap(3).toInt(), __qRegExpDMY.cap(2).toInt(), __qRegExpDMY.cap(1).toInt() ) );
  }
  else if( __qRegExpMDY.exactMatch( _rqString ) )
  {
    CUnitTimeZone::EUnit __eUnitTimeZone = _eUnitTimeZone;
    QString __qsTimeZone = __qRegExpMDY.cap(4).toUpper();
    if( __qsTimeZone == "Z" ) __eUnitTimeZone = CUnitTimeZone::UTC;
    else if( __qsTimeZone == "L" ) __eUnitTimeZone = CUnitTimeZone::LOCAL;
    __qDateTime.setTimeSpec( __eUnitTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
    __qDateTime.setDate( QDate( __qRegExpMDY.cap(3).toInt(), __qRegExpMDY.cap(1).toInt(), __qRegExpMDY.cap(2).toInt() ) );
  }
  else return 0.0;
  if( _pbOK ) *_pbOK = true;
  return( (double)__qDateTime.toTime_t() );
}

double CUnitDate::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitTimeZone(), _pbOK );
}
