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

#ifndef QVCT_CUNITDATE_HPP
#define QVCT_CUNITDATE_HPP

// QT
#include <QMap>
#include <QString>

// QVCT
#include "units/CUnitTimeZone.hpp"
class CUnitDateSymbols; // see below
class CUnitDateCodes; // see below


/// Date unit (YMD, DMY, MDY) rendering/parsing class
/**
 *  This class allows to render/parse UTC or local dates according to the
 *  various supported (and user specified) formats/units.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CUnitDate
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Format/unit ID
  enum EUnit {
    YMD,      ///< YYYY-MM-DD
    DMY,      ///< DD-MM-YYYY
    MDY,      ///< MM-DD-YYYY
    UNDEFINED ///< undefined format/unit
  };

private:
  /// Container for supported human-readable format/unit symbols
  static const CUnitDateSymbols oUnitDateSymbols;
  /// Container for supported machine-friendly format/unit codes
  static const CUnitDateCodes oUnitDateCodes;

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
  static QString toString( double _fdValue, CUnitTimeZone::EUnit _eUnitTimeZone, EUnit _eUnit );
  /// Returns the formatted represention of the given value, using the application settings
  static QString toString( double _fdValue, bool _bAlternateTimeZone = false );
  /// Returns the numeric value corresponding (parsed) from the string
  /** This methods will attempt to automatically detect the used format/unit.
   *  If the format/unit can not be determined, it will default to the (optionally)
   *  specified format/unit).
   *  The success of the parsing may be retrieved using the (optionally) supplied
   *  boolean flag.
   *  @param[in] _rqString Formatted represention of the value
   *  @param[in] _eUnitTimeZone Timezone format/unit (if none can be automatically determined)
   *  @param[out] _pbOK Parsing success status
   */
  static double fromString( const QString& _rqString, CUnitTimeZone::EUnit _eUnitTimeZone, bool* _pbOK = 0 );
  /// Returns the numeric value corresponding (parsed) from the string, using the application settings
  static double fromString( const QString& _rqString, bool* _pbOK = 0 );

};

/// Container class for supported human-readable format/unit symbols
class CUnitDateSymbols
{
  friend class CUnitDate;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitDateSymbols()
  {
    qMapSymbols.insert( CUnitDate::YMD, "yyyy-mm-dd" );
    qMapSymbols.insert( CUnitDate::DMY, "dd.mm.yyyy" );
    qMapSymbols.insert( CUnitDate::MDY, "mm,dd,yyyy" );
  };
  QMap<CUnitDate::EUnit,QString> qMapSymbols;
};

/// Container class for supported machine-friendly format/unit codes
class CUnitDateCodes
{
  friend class CUnitDate;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitDateCodes()
  {
    qMapCodes.insert( CUnitDate::YMD, "ymd" );
    qMapCodes.insert( CUnitDate::DMY, "dmy" );
    qMapCodes.insert( CUnitDate::MDY, "mdy" );
  };
  QMap<CUnitDate::EUnit,QString> qMapCodes;
};

#endif // QVCT_CUNITDATE_HPP
