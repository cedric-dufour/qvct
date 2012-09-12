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

#ifndef QVCT_CUNITPOSITION_HPP
#define QVCT_CUNITPOSITION_HPP

// QT
#include <QMap>
#include <QString>

// QVCT
class CUnitPositionSymbols; // see below
class CUnitPositionCodes; // see below


/// Geographical position unit (D,DM,DMS,RAD,GRAD) rendering/parsing class
/**
 *  This class allows to render/parse geographical positions according to the
 *  various supported (and user specified) formats/units.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CUnitPosition
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Format/unit ID
  enum EUnit {
    DMS,      ///< degrees, minutes, seconds
    DM,       ///< degrees, minutes
    DEG,      ///< degrees
    RAD,      ///< radians
    GRAD,     ///< grads
    UNDEFINED ///< undefined format/unit
  };
  /// Position coordinate type ID
  enum EType {
    LONGITUDE,
    LATITUDE
  };

private:
  /// Container for supported human-readable format/unit symbols
  static const CUnitPositionSymbols oUnitPositionSymbols;
  /// Container for supported machine-friendly format/unit codes
  static const CUnitPositionCodes oUnitPositionCodes;

public:
  /// Returns the list of supported human-readable format/unit symbols
  static const QMap<EUnit,QString> &symbols();
  /// Returns the human-readable symbol corresponding to the given format/unit ID
  static QString toSymbol( EUnit _eUnit );
  /// Returns the format/unit ID corresponding to the given human-readable symbol
  static EUnit fromSymbol( const QString& _rqsSymbol );
  /// Returns the list of supported machine-friendly format/unit codes
  static const QMap<EUnit,QString> &codes();
  /// Returns the machine-friendly code corresponding to the given format/unit ID
  static QString toCode( EUnit _eUnit );
  /// Returns the format/unit ID corresponding to the given machine-friendly code
  static EUnit fromCode( const QString& _rqsCode );
  /// Returns the formatted represention of the given value, using the specified format/unit and decimal precision
  static QString toString( double _fdValue, EType _eType, EUnit _eUnit, int _iPrecision = 0 );
  /// Returns the formatted represention of the given value, using the application settings
  static QString toString( double _fdValue, EType _eType );
  /// Returns the numeric value corresponding (parsed) from the string
  /** This methods will attempt to automatically detect the used format/unit.
   *  If the format/unit can not be determined, it will default to the (optionally)
   *  specified format/unit).
   *  The success of the parsing may be retrieved using the (optionally) supplied
   *  boolean flag.
   *  @param[in] _rqString Formatted represention of the value
   *  @param[out] _pbOK Parsing success status
   */
  static double fromString( const QString& _rqString, bool* _pbOK = 0 );

};

/// Container class for supported human-readable format/unit symbols
class CUnitPositionSymbols
{
  friend class CUnitPosition;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitPositionSymbols()
  {
    qMapSymbols.insert( CUnitPosition::DMS, QString::fromUtf8( "ddd°mm'ss\"" ) );
    qMapSymbols.insert( CUnitPosition::DM, QString::fromUtf8( "ddd°mm'" ) );
    qMapSymbols.insert( CUnitPosition::DEG, QString::fromUtf8( "ddd°" ) );
    qMapSymbols.insert( CUnitPosition::RAD, QString::fromUtf8( "rad°r" ) );
    qMapSymbols.insert( CUnitPosition::GRAD, QString::fromUtf8( "grad°g" ) );
  };
  QMap<CUnitPosition::EUnit,QString> qMapSymbols;
};

/// Container class for supported machine-friendly format/unit codes
class CUnitPositionCodes
{
  friend class CUnitPosition;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitPositionCodes()
  {
    qMapCodes.insert( CUnitPosition::DMS, "dms" );
    qMapCodes.insert( CUnitPosition::DM, "dm" );
    qMapCodes.insert( CUnitPosition::DEG, "deg" );
    qMapCodes.insert( CUnitPosition::RAD, "rad" );
    qMapCodes.insert( CUnitPosition::GRAD, "grad" );
  };
  QMap<CUnitPosition::EUnit,QString> qMapCodes;
};

#endif // QVCT_CUNITPOSITION_HPP
