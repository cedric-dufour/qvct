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
#include "units/CUnitSpeed.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitSpeedSymbols CUnitSpeed::oUnitSpeedSymbols;
const CUnitSpeedCodes CUnitSpeed::oUnitSpeedCodes;

const QMap<CUnitSpeed::EUnit,QString> &CUnitSpeed::symbols()
{
  return oUnitSpeedSymbols.qMapSymbols;
}

QString CUnitSpeed::toSymbol( CUnitSpeed::EUnit _eUnit )
{
  return oUnitSpeedSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitSpeed::EUnit CUnitSpeed::fromSymbol( const QString& _rqString )
{
  return oUnitSpeedSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitSpeed::EUnit,QString> &CUnitSpeed::codes()
{
  return oUnitSpeedCodes.qMapCodes;
}

QString CUnitSpeed::toCode( CUnitSpeed::EUnit _eUnit )
{
  return oUnitSpeedCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitSpeed::EUnit CUnitSpeed::fromCode( const QString& _rqString )
{
  return oUnitSpeedCodes.qMapCodes.key( _rqString, UNDEFINED );
}

QString CUnitSpeed::toString( double _fdValue, CUnitSpeed::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {
  case M_S: return QString::number( _fdValue, 'f', _iPrecision )+oUnitSpeedSymbols.qMapSymbols.value( _eUnit );
  case KM_H: return QString::number( _fdValue * 3.6, 'f', _iPrecision )+oUnitSpeedSymbols.qMapSymbols.value( _eUnit );
  case MI_H: return QString::number( _fdValue * 2.23693629205, 'f', _iPrecision )+oUnitSpeedSymbols.qMapSymbols.value( _eUnit );
  case KT: return QString::number( _fdValue * 1.94384449244, 'f', _iPrecision )+oUnitSpeedSymbols.qMapSymbols.value( _eUnit );
  default: return "?";
  }
}

QString CUnitSpeed::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitSpeed(), __poSettings->getPrecisionSpeed() );
}

double CUnitSpeed::fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK )
{
  static const QRegExp __qRegExp( "^\\s*(-?\\d+(\\.\\d+)?)\\s*([^\\d\\s]*)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  if( !__qRegExp.exactMatch( _rqString ) ) return 0.0;
  bool __bOK;
  double __fdValue = __qRegExp.cap(1).toDouble( &__bOK );
  if( !__bOK ) return 0.0;
  QString __qsUnit = __qRegExp.cap(3);
  switch( __qsUnit.isEmpty() ? _eUnit : oUnitSpeedSymbols.qMapSymbols.key( __qsUnit, UNDEFINED ) )
  {
  case M_S: break;
  case KM_H: __fdValue = __fdValue / 3.6; break;
  case MI_H: __fdValue = __fdValue / 2.23693629205; break;
  case KT: __fdValue = __fdValue / 1.94384449244; break;
  default: return 0.0;
  }
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}

double CUnitSpeed::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitSpeed(), _pbOK );
}
