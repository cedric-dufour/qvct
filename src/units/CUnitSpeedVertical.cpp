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
#include <QMap>
#include <QRegExp>
#include <QString>

// QVCT
#include "QVCTRuntime.hpp"
#include "units/CUnitSpeedVertical.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitSpeedVerticalSymbols CUnitSpeedVertical::oUnitSpeedVerticalSymbols;
const CUnitSpeedVerticalCodes CUnitSpeedVertical::oUnitSpeedVerticalCodes;

const QMap<CUnitSpeedVertical::EUnit,QString> &CUnitSpeedVertical::symbols()
{
  return oUnitSpeedVerticalSymbols.qMapSymbols;
}

QString CUnitSpeedVertical::toSymbol( CUnitSpeedVertical::EUnit _eUnit )
{
  return oUnitSpeedVerticalSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitSpeedVertical::EUnit CUnitSpeedVertical::fromSymbol( const QString& _rqString )
{
  return oUnitSpeedVerticalSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitSpeedVertical::EUnit,QString> &CUnitSpeedVertical::codes()
{
  return oUnitSpeedVerticalCodes.qMapCodes;
}

QString CUnitSpeedVertical::toCode( CUnitSpeedVertical::EUnit _eUnit )
{
  return oUnitSpeedVerticalCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitSpeedVertical::EUnit CUnitSpeedVertical::fromCode( const QString& _rqString )
{
  return oUnitSpeedVerticalCodes.qMapCodes.key( _rqString, UNDEFINED );
}

double CUnitSpeedVertical::toValue( double _fdValue, CUnitSpeedVertical::EUnit _eUnit )
{
  switch( _eUnit )
  {
  case M_S: return _fdValue;
  case FT_MIN: return _fdValue * 196.8503937;
  default: return 0;
  }
}

QString CUnitSpeedVertical::toString( double _fdValue, CUnitSpeedVertical::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {
  case M_S: return QString::number( _fdValue, 'f', _iPrecision )+oUnitSpeedVerticalSymbols.qMapSymbols.value( _eUnit );
  case FT_MIN: return QString::number( _fdValue * 196.8503937, 'f', _iPrecision )+oUnitSpeedVerticalSymbols.qMapSymbols.value( _eUnit );
  default: return "?";
  }
}

QString CUnitSpeedVertical::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitSpeedVertical(), __poSettings->getPrecisionSpeedVertical() );
}

double CUnitSpeedVertical::fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK )
{
  static const QRegExp __qRegExp( "^\\s*(-?\\d+(\\.\\d+)?)\\s*([^\\d\\s]*)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  if( !__qRegExp.exactMatch( _rqString ) ) return 0.0;
  bool __bOK;
  double __fdValue = __qRegExp.cap(1).toDouble( &__bOK );
  if( !__bOK ) return 0.0;
  QString __qsUnit = __qRegExp.cap(3);
  switch( __qsUnit.isEmpty() ? _eUnit : oUnitSpeedVerticalSymbols.qMapSymbols.key( __qsUnit, UNDEFINED ) )
  {
  case M_S: break;
  case FT_MIN: __fdValue = __fdValue / 196.8503937; break;
  default: return 0.0;
  }
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}

double CUnitSpeedVertical::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitSpeedVertical(), _pbOK );
}
