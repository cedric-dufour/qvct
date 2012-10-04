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

#ifndef QVCT_CUNITSPEEDVERTICAL_HPP
#define QVCT_CUNITSPEEDVERTICAL_HPP

// QT
#include <QMap>
#include <QString>

// QVCT
class CUnitSpeedVerticalSymbols; // see below
class CUnitSpeedVerticalCodes; // see below


/// Vertical speed unit (m/s,ft/min) rendering/parsing class
/**
 *  This class allows to render/parse vertical speed values according to the
 *  various supported (and user specified) formats/units.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CUnitSpeedVertical
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Format/unit ID
  enum EUnit {
    M_S,      ///< meters per second [m/s]
    FT_MIN,   ///< feet per minut [ft/min]
    UNDEFINED ///< undefined format/unit
  };

private:
  /// Container for supported human-readable format/unit symbols
  static const CUnitSpeedVerticalSymbols oUnitSpeedVerticalSymbols;
  /// Container for supported machine-friendly format/unit codes
  static const CUnitSpeedVerticalCodes oUnitSpeedVerticalCodes;

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
  /// Returns the converted value, using the specified format/unit
  static double toValue( double _fdValue, EUnit _eUnit );
  /// Returns the formatted represention of the given value, using the specified format/unit and decimal precision
  static QString toString( double _fdValue, EUnit _eUnit, int _iPrecision = 0 );
  /// Returns the formatted represention of the given value, using the application settings
  static QString toString( double _fdValue );
  /// Returns the numeric value corresponding (parsed) from the string
  /** This methods will attempt to automatically detect the used format/unit.
   *  If the format/unit can not be determined, it will default to the (optionally)
   *  specified format/unit).
   *  The success of the parsing may be retrieved using the (optionally) supplied
   *  boolean flag.
   *  @param[in] _rqString Formatted represention of the value
   *  @param[in] _eUnit Default format/unit (if none can be automatically determined)
   *  @param[out] _pbOK Parsing success status
   */
  static double fromString( const QString& _rqString, EUnit _eUnit, bool* _pbOK = 0 );
  /// Returns the numeric value corresponding (parsed) from the string, using the application settings
  static double fromString( const QString& _rqString, bool* _pbOK = 0 );

};

/// Container class for supported human-readable format/unit symbols
class CUnitSpeedVerticalSymbols
{
  friend class CUnitSpeedVertical;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitSpeedVerticalSymbols()
  {
    qMapSymbols.insert( CUnitSpeedVertical::M_S, "m/s" );
    qMapSymbols.insert( CUnitSpeedVertical::FT_MIN, "ft/min" );
  };
  QMap<CUnitSpeedVertical::EUnit,QString> qMapSymbols;
};

/// Container class for supported machine-friendly format/unit codes
class CUnitSpeedVerticalCodes
{
  friend class CUnitSpeedVertical;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitSpeedVerticalCodes()
  {
    qMapCodes.insert( CUnitSpeedVertical::M_S, "m_s" );
    qMapCodes.insert( CUnitSpeedVertical::FT_MIN, "ft_min" );
  };
  QMap<CUnitSpeedVertical::EUnit,QString> qMapCodes;
};

#endif // QVCT_CUNITSPEEDVERTICAL_HPP
