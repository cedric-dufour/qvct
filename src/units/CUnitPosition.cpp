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

// GPS
#include <gps.h>

// QVCT
#include "QVCTRuntime.hpp"
#include "units/CUnitPosition.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitPositionSymbols CUnitPosition::oUnitPositionSymbols;
const CUnitPositionCodes CUnitPosition::oUnitPositionCodes;

const QMap<CUnitPosition::EUnit,QString> &CUnitPosition::symbols()
{
  return oUnitPositionSymbols.qMapSymbols;
}

QString CUnitPosition::toSymbol( CUnitPosition::EUnit _eUnit )
{
  return oUnitPositionSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitPosition::EUnit CUnitPosition::fromSymbol( const QString& _rqString )
{
  return oUnitPositionSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitPosition::EUnit,QString> &CUnitPosition::codes()
{
  return oUnitPositionCodes.qMapCodes;
}

QString CUnitPosition::toCode( CUnitPosition::EUnit _eUnit )
{
  return oUnitPositionCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitPosition::EUnit CUnitPosition::fromCode( const QString& _rqString )
{
  return oUnitPositionCodes.qMapCodes.key( _rqString, UNDEFINED );
}

QString CUnitPosition::toString( double _fdValue, EType _eType, CUnitPosition::EUnit _eUnit, int _iPrecision )
{
  QString __qString;
  switch( _eUnit )
  {

  case DMS:
    {
      double __fdDeg;
      double __fdMin = modf( fabs( _fdValue ), &__fdDeg ) * 60;
      double __fdSec = modf( __fdMin, &__fdMin ) * 60;
      QString __qsSec = QString::number( __fdSec, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsSec[0] == '6' ) { __fdMin += 1; __qsSec[0] = '0'; } // fix rounding-up
      QString __qsMin = QString::number( __fdMin, 'f', 0 ).prepend( "0" ).right( 2 );
      if( __qsMin[0] == '6' ) { __fdDeg += 1; __qsMin[0] = '0'; } // fix rounding-up
      __qString = QString::number( __fdDeg, 'f', 0 )+QString::fromUtf8("°")+__qsMin+"'"+__qsSec+"\"";
    }
    break;

  case DM:
    {
      double __fdDeg;
      double __fdMin = modf( fabs( _fdValue ), &__fdDeg ) * 60;
      QString __qsMin = QString::number( __fdMin, 'f', _iPrecision ).prepend( "0" ).right( 2+( _iPrecision>0 ? _iPrecision+1 : 0 ) ); // WARNING: rounding-up may occur!
      if( __qsMin[0] == '6' ) { __fdDeg += 1; __qsMin[0] = '0'; } // fix rounding-up
      __qString = QString::number( __fdDeg, 'f', 0 )+QString::fromUtf8("°")+__qsMin+"'";
    }
    break;

  case DEG:
    __qString = QString::number( fabs( _fdValue ), 'f', _iPrecision )+QString::fromUtf8("°");
    break;

  case RAD:
    __qString = QString::number( fabs( _fdValue ) * GPS_PI / 180.0, 'f', _iPrecision )+QString::fromUtf8("°r");
    break;

  case GRAD:
    __qString = QString::number( fabs( _fdValue ) / 0.9, 'f', _iPrecision )+QString::fromUtf8("°g");
    break;

  default:
    return "?";

  }
  __qString += _fdValue >= 0 ? ( _eType == LONGITUDE ? "E" : "N" ) : ( _eType == LONGITUDE ? "W" : "S" );
  return __qString;
}

QString CUnitPosition::toString( double _fdValue, EType _eType )
{
  CSettings* __poSettings = QVCTRuntime::useSettings();
  return toString( _fdValue, _eType, __poSettings->getUnitPosition(), __poSettings->getPrecisionPosition() );
}

double CUnitPosition::fromString( const QString& _rqString, bool* _pbOK )
{
  static const QRegExp __qRegExpDMS( QString::fromUtf8( "^\\s*(-?\\d+)\\s*°([0-5]?[0-9])\\s*'([0-5]?[0-9](\\.\\d+)?)\\s*\"?\\s*([NnEeSsWw])?\\s*$" ) );
  static const QRegExp __qRegExpDM( QString::fromUtf8( "^\\s*(-?\\d+)\\s*°([0-5]?[0-9](\\.\\d+)?)\\s*'?\\s*([NnEeSsWw])?\\s*$" ) );
  static const QRegExp __qRegExpDEG( QString::fromUtf8( "^\\s*(-?\\d+(\\.\\d+)?)\\s*°?\\s*([NnEeSsWw])?\\s*$" ) );
  static const QRegExp __qRegExpRAD( QString::fromUtf8( "^\\s*(-?\\d+(\\.\\d+)?)\\s*°?r(ad)?\\s*([NnEeSsWw])?\\s*$" ) );
  static const QRegExp __qRegExpGRAD( QString::fromUtf8( "^\\s*(-?\\d+(\\.\\d+)?)\\s*°?g(rad)?\\s*([NnEeSsWw])?\\s*$" ) );

  if( _pbOK ) *_pbOK = false;
  bool __bOK;
  double __fdValue;
  QString __qsDirection;
  if( __qRegExpDMS.exactMatch( _rqString ) )
  {
    __fdValue = __qRegExpDMS.cap(1).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpDMS.cap(2).toDouble( &__bOK ) / 60.0;
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpDMS.cap(3).toDouble( &__bOK ) / 3600.0;
    if( !__bOK ) return 0.0;
    __qsDirection = __qRegExpDMS.cap(5).toUpper();
  }
  else if( __qRegExpDM.exactMatch( _rqString ) )
  {
    __fdValue = __qRegExpDM.cap(1).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    __fdValue += __qRegExpDM.cap(2).toDouble( &__bOK ) / 60.0;
    if( !__bOK ) return 0.0;
    __qsDirection = __qRegExpDM.cap(4).toUpper();
  }
  else if( __qRegExpDEG.exactMatch( _rqString ) )
  {
    __fdValue = __qRegExpDEG.cap(1).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    __qsDirection = __qRegExpDEG.cap(3).toUpper();
  }
  else if( __qRegExpRAD.exactMatch( _rqString ) )
  {
    __fdValue = __qRegExpDEG.cap(1).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    __fdValue = __fdValue * 0.9;
    __qsDirection = __qRegExpDEG.cap(4).toUpper();
  }
  else if( __qRegExpGRAD.exactMatch( _rqString ) )
  {
    __fdValue = __qRegExpDEG.cap(1).toDouble( &__bOK );
    if( !__bOK ) return 0.0;
    __fdValue = __fdValue * 180.0 / GPS_PI;
    __qsDirection = __qRegExpDEG.cap(4).toUpper();
  }
  else return 0.0;
  if( !__qsDirection.isEmpty() && __fdValue < 0 ) return 0.0;
  if( __qsDirection == "W" || __qsDirection == "S" ) __fdValue = -__fdValue;
  if( _pbOK ) *_pbOK = true;
  return __fdValue;
}
