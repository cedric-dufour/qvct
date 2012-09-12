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

#ifndef QVCT_CUNITTIMEZONE_HPP
#define QVCT_CUNITTIMEZONE_HPP

// QT
#include <QMap>
#include <QString>

// QVCT
class CUnitTimeZoneSymbols; // see below
class CUnitTimeZoneCodes; // see below


/// Time zone (UTC, local) selection class
/**
 *  This class allows to select UTC or local times for rendering/parsing dates
 *  and times.
 *  @see CUnitDate, CUnitTime
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CUnitTimeZone
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Format/unit ID
  enum EUnit {
    UTC,      ///< Universal Time Coordinates
    LOCAL,    ///< Local Time
    UNDEFINED ///< undefined format/unit
  };

private:
  /// Container for supported human-readable format/unit symbols
  static const CUnitTimeZoneSymbols oUnitTimeZoneSymbols;
  /// Container for supported machine-friendly format/unit codes
  static const CUnitTimeZoneCodes oUnitTimeZoneCodes;

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

};

/// Container class for supported human-readable format/unit symbols
class CUnitTimeZoneSymbols
{
  friend class CUnitTimeZone;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitTimeZoneSymbols()
  {
    qMapSymbols.insert( CUnitTimeZone::UTC, "UTC[Z]" );
    qMapSymbols.insert( CUnitTimeZone::LOCAL, "Local" );
  };
  QMap<CUnitTimeZone::EUnit,QString> qMapSymbols;
};

/// Container class for supported machine-friendly format/unit codes
class CUnitTimeZoneCodes
{
  friend class CUnitTimeZone;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CUnitTimeZoneCodes()
  {
    qMapCodes.insert( CUnitTimeZone::UTC, "utc" );
    qMapCodes.insert( CUnitTimeZone::LOCAL, "local" );
  };
  QMap<CUnitTimeZone::EUnit,QString> qMapCodes;
};

#endif // QVCT_CUNITTIMEZONE_HPP
