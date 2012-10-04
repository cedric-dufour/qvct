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
#include "units/CUnitBearing.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitBearingSymbols CUnitBearing::oUnitBearingSymbols;
const CUnitBearingCodes CUnitBearing::oUnitBearingCodes;

const QMap<CUnitBearing::EUnit,QString> &CUnitBearing::symbols()
{
  return oUnitBearingSymbols.qMapSymbols;
}

QString CUnitBearing::toSymbol( CUnitBearing::EUnit _eUnit )
{
  return oUnitBearingSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitBearing::EUnit CUnitBearing::fromSymbol( const QString& _rqString )
{
  return oUnitBearingSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitBearing::EUnit,QString> &CUnitBearing::codes()
{
  return oUnitBearingCodes.qMapCodes;
}

QString CUnitBearing::toCode( CUnitBearing::EUnit _eUnit )
{
  return oUnitBearingCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitBearing::EUnit CUnitBearing::fromCode( const QString& _rqString )
{
  return oUnitBearingCodes.qMapCodes.key( _rqString, UNDEFINED );
}

double CUnitBearing::toValue( double _fdValue, CUnitBearing::EUnit _eUnit )
{
  switch( _eUnit )
  {
  case DEG: return _fdValue;
  case RAD: return _fdValue * QVCT::PI / 180.0;
  case GRAD: return _fdValue / 0.9;
  default: return 0;
  }
}

QString CUnitBearing::toString( double _fdValue, CUnitBearing::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {
  case DEG: return QString::number( _fdValue, 'f', _iPrecision )+oUnitBearingSymbols.qMapSymbols.value( _eUnit );
  case RAD: return QString::number( _fdValue * QVCT::PI / 180.0, 'f', _iPrecision )+oUnitBearingSymbols.qMapSymbols.value( _eUnit );
  case GRAD: return QString::number( _fdValue / 0.9, 'f', _iPrecision )+oUnitBearingSymbols.qMapSymbols.value( _eUnit );
  default: return "?";
  }
}

QString CUnitBearing::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitBearing(), __poSettings->getPrecisionBearing() );
}

double CUnitBearing::fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK )
{
  static const QRegExp __qRegExp( "^\\s*(-?\\d+(\\.\\d+)?)\\s*([^\\d\\s]*)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  if( !__qRegExp.exactMatch( _rqString ) ) return 0.0;
  bool __bOK;
  double __fdValue = __qRegExp.cap(1).toDouble( &__bOK );
  if( !__bOK ) return 0.0;
  QString __qsUnit = __qRegExp.cap(3);
  switch( __qsUnit.isEmpty() ? _eUnit : oUnitBearingSymbols.qMapSymbols.key( __qsUnit, UNDEFINED ) )
  {
  case DEG: break;
  case RAD: __fdValue = __fdValue * 180.0 / QVCT::PI; break;
  case GRAD: __fdValue = __fdValue * 0.9; break;
  default: return 0.0;
  }
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}

double CUnitBearing::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitBearing(), _pbOK );
}
