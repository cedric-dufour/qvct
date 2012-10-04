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
#include "units/CUnitDistance.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitDistanceSymbols CUnitDistance::oUnitDistanceSymbols;

const QMap<CUnitDistance::EUnit,QString> &CUnitDistance::symbols()
{
  return oUnitDistanceSymbols.qMapSymbols;
}

QString CUnitDistance::toSymbol( CUnitDistance::EUnit _eUnit )
{
  return oUnitDistanceSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitDistance::EUnit CUnitDistance::fromSymbol( const QString& _rqString )
{
  return oUnitDistanceSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitDistance::EUnit,QString> &CUnitDistance::codes()
{
  return oUnitDistanceSymbols.qMapSymbols;
}

QString CUnitDistance::toCode( CUnitDistance::EUnit _eUnit )
{
  return oUnitDistanceSymbols.qMapSymbols.value( _eUnit, "undef" );
}

CUnitDistance::EUnit CUnitDistance::fromCode( const QString& _rqString )
{
  return oUnitDistanceSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

double CUnitDistance::toValue( double _fdValue, CUnitDistance::EUnit _eUnit )
{
  switch( _eUnit )
  {
  case M: return _fdValue;
  case KM: return _fdValue / 1000.0;
  case FT: return _fdValue / 0.3048;
  case MI: return _fdValue / 1609.344;
  case NM: return _fdValue / 1852.0;
  default: return 0;
  }
}

QString CUnitDistance::toString( double _fdValue, CUnitDistance::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {
  case M: return QString::number( _fdValue, 'f', _iPrecision )+oUnitDistanceSymbols.qMapSymbols.value( _eUnit );
  case KM: return QString::number( _fdValue / 1000.0, 'f', _iPrecision )+oUnitDistanceSymbols.qMapSymbols.value( _eUnit );
  case FT: return QString::number( _fdValue / 0.3048, 'f', _iPrecision )+oUnitDistanceSymbols.qMapSymbols.value( _eUnit );
  case MI: return QString::number( _fdValue / 1609.344, 'f', _iPrecision )+oUnitDistanceSymbols.qMapSymbols.value( _eUnit );
  case NM: return QString::number( _fdValue / 1852.0, 'f', _iPrecision )+oUnitDistanceSymbols.qMapSymbols.value( _eUnit );
  default: return "?";
  }
}

QString CUnitDistance::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitDistance(), __poSettings->getPrecisionDistance() );
}

double CUnitDistance::fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK )
{
  static const QRegExp __qRegExp( "^\\s*(-?\\d+(\\.\\d+)?)\\s*([^\\d\\s]*)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  if( !__qRegExp.exactMatch( _rqString ) ) return 0.0;
  bool __bOK;
  double __fdValue = __qRegExp.cap(1).toDouble( &__bOK );
  if( !__bOK ) return 0.0;
  QString __qsUnit = __qRegExp.cap(3);
  switch( __qsUnit.isEmpty() ? _eUnit : oUnitDistanceSymbols.qMapSymbols.key( __qsUnit, UNDEFINED ) )
  {
  case M: break;
  case KM: __fdValue = __fdValue * 1000.0; break;
  case FT: __fdValue = __fdValue * 0.3048; break;
  case MI: __fdValue = __fdValue * 1609.344; break;
  case NM: __fdValue = __fdValue * 1852.0; break;
  default: return 0.0;
  }
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}

double CUnitDistance::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitDistance(), _pbOK );
}
