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

#ifndef QVCT_CDATATIME_HPP
#define QVCT_CDATATIME_HPP

// QT
class QDataStream;


/// Time data
/**
 *  This class encapsulates time data, using Universal Time Coordinates (UTC)
 *  and milliseconds precision.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataTime
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Specific value for an undefined time
  static const double UNDEFINED_TIME = -9999999;
  /// Specific value for an undefined time object
  static const CDataTime UNDEFINED;


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Time, in seconds from Unix epoch
  /** @see setTime(), getTime() */
  double fdTime;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  /** @param[in] _fdTime Time, in seconds from Unix epoch */
  CDataTime( double _fdTime = UNDEFINED_TIME );
  CDataTime( const CDataTime& _roDataTime );
  virtual ~CDataTime() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the time, in seconds from Unix epoch
  void setTime( double _fdTime );
  /// Copy time from another time object
  void setTime( const CDataTime& _roDataTime );
  /// Resets the time (to an undefined Time)
  /** @see UNDEFINED, UNDEFINED_TIME */
  void resetTime() { fdTime = UNDEFINED_TIME; };

  // GETTERS
public:
  /// Returns this time's time, in seconds from Unix epoch
  double getTime() const { return fdTime; };

  // OTHER
public:
  /// Serializes (store) this object's data to binary format
  void serialize( QDataStream& _rqDataStream ) const;
  /// Unserializes (restore) this object's data from binary format
  void unserialize( QDataStream& _rqDataStream );


  // OPERATORS
public:
  /// Equality operator
  /** This operator returns true if all components - longitude, latitude and
   *  elevation - match. */
  bool operator==( const CDataTime& _roTime ) const;
  /// Inequality operator
  /** This operator returns true if any components - longitude, latitude and
   *  elevation - mismatch. */
  bool operator!=( const CDataTime& _roTime ) const;

  // COMPARATORS
public:
  /// Time (ascending sort) comparison operator
  static bool compareTimeAscending( const CDataTime& _roTime1, const CDataTime& _roTime2 );
  /// Time (descending sort) comparison operator
  static bool compareTimeDescending( const CDataTime& _roTime1, const CDataTime& _roTime2 );

};

#endif // QVCT_CDATATIME_HPP
