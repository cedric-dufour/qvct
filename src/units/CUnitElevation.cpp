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
#include "units/CUnitElevation.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitElevationSymbols CUnitElevation::oUnitElevationSymbols;

const QMap<CUnitElevation::EUnit,QString> &CUnitElevation::symbols()
{
  return oUnitElevationSymbols.qMapSymbols;
}

QString CUnitElevation::toSymbol( CUnitElevation::EUnit _eUnit )
{
  return oUnitElevationSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitElevation::EUnit CUnitElevation::fromSymbol( const QString& _rqString )
{
  return oUnitElevationSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitElevation::EUnit,QString> &CUnitElevation::codes()
{
  return oUnitElevationSymbols.qMapSymbols;
}

QString CUnitElevation::toCode( CUnitElevation::EUnit _eUnit )
{
  return oUnitElevationSymbols.qMapSymbols.value( _eUnit, "undef" );
}

CUnitElevation::EUnit CUnitElevation::fromCode( const QString& _rqString )
{
  return oUnitElevationSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

QString CUnitElevation::toString( double _fdValue, CUnitElevation::EUnit _eUnit, int _iPrecision )
{
  switch( _eUnit )
  {
  case M: return QString::number( _fdValue, 'f', _iPrecision )+oUnitElevationSymbols.qMapSymbols.value( _eUnit );
  case FT: return QString::number( _fdValue / 0.3048, 'f', _iPrecision )+oUnitElevationSymbols.qMapSymbols.value( _eUnit );
  default: return "?";
  }
}

QString CUnitElevation::toString( double _fdValue )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, __poSettings->getUnitElevation(), __poSettings->getPrecisionElevation() );
}

double CUnitElevation::fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK )
{
  static const QRegExp __qRegExp( "^\\s*(-?\\d+(\\.\\d+)?)\\s*([^\\d\\s]*)?\\s*$" );

  if( _pbOK ) *_pbOK = false;
  if( !__qRegExp.exactMatch( _rqString ) ) return 0.0;
  bool __bOK;
  double __fdValue = __qRegExp.cap(1).toDouble( &__bOK );
  if( !__bOK ) return 0.0;
  QString __qsUnit = __qRegExp.cap(3);
  switch( __qsUnit.isEmpty() ? _eUnit : oUnitElevationSymbols.qMapSymbols.key( __qsUnit, UNDEFINED ) )
  {
  case M: break;
  case FT: __fdValue = __fdValue * 0.3048; break;
  default: return 0.0;
  }
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}

double CUnitElevation::fromString( const QString& _rqString, bool* _pbOK )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return fromString( _rqString, __poSettings->getUnitElevation(), _pbOK );
}
