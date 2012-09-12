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

#ifndef QVCT_CDEVICEDATAFIX_HPP
#define QVCT_CDEVICEDATAFIX_HPP

// QVCT
#include "data/CDataTime.hpp"
#include "data/CDataPosition.hpp"
#include "data/CDataCourse.hpp"
#include "data/CDataValidity.hpp"
#include "devices/data/CDeviceDataDop.hpp"
#include "devices/data/CDeviceDataSource.hpp"


/// Fix data [source,time,position,course,DOPs,...]
/**
 *  This class gathers all components of a fully-qualified geographical
 *  position and course fix, along values that allow to estimate the quality
 *  of that fix.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDataFix: public CDeviceDataSource, public CDataTime, public CDataPosition, public CDataCourse, public CDeviceDataDop
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Fix type (2D, 3D, no fix)
  enum EType {
    FIX_NONE = 0,
    FIX_2D = 1,
    FIX_3D = 2,
    FIX_DGPS = 4,
    FIX_UNDEFINED = 128
  };

public:
  static QString getTypeString( int _eType );


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Fix type
  /** @see setType(), getType() */
  int eType;
  /// Seen sources count (most likely satellites)
  /** @see setSources(), getSourcesSeen() */
  int iSourcesSeen;
  /// Used sources count (most likely satellites)
  /** @see setSources(), getSourcesUsed() */
  int iSourcesUsed;
  /// Time error, in seconds
  /** @see setErrorTime(), getErrorTime() */
  double fdErrorTime;
  /// Horizontal position error, in meters
  /** @see setErrorPosition(), getErrorHorizontal() */
  double fdErrorHorizontal;
  /// Vertical position error, in meters
  /** @see setErrorPosition(), getErrorVertical() */
  double fdErrorVertical;
  /// Bearing error, in degrees
  /** @see setErrorBearing(), getErrorBearing() */
  double fdErrorBearing;
  /// Horizontal speed error, in meters per second
  /** @see setErrorSpeed(), getErrorSpeed() */
  double fdErrorSpeed;
  /// Vertical speed error, in meters per second
  /** @see setErrorSpeed(), getErrorSpeedVertical() */
  double fdErrorSpeedVertical;
  /// Additional textual data string
  /** @see setText(), getText() */
  QString qsText;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceDataFix( const QString& _rqsSourceName, int _eType = FIX_UNDEFINED, double _fdTime = CDataValidity::UNDEFINED_VALUE );
  virtual ~CDeviceDataFix() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets (copy) the fix data from another instance
  void setFix( const CDeviceDataFix& _roDeviceDataFix, bool _bCopyTime = true, bool _bCopyPosition = true, bool _bCopyCourse = true, bool _bCopyDop = true );
  /// Sets the fix type
  void setType( int _eType ) { eType = _eType; };
  /// Sets the sources count (most likely satellites)
  void setSources( int _iSourcesSeen, int _iSourcesUsed )
  { iSourcesSeen = _iSourcesSeen; iSourcesUsed = _iSourcesUsed; };
  /// Sets the time error, in seconds
  void setErrorTime( double _fdErrorTime ) { fdErrorTime = _fdErrorTime; };
  /// Sets the (horizontal and vertical) position error, in meters
  void setErrorPosition( double _fdErrorHorizontal, double _fdErrorVertical = CDataValidity::UNDEFINED_VALUE )
  { fdErrorHorizontal = _fdErrorHorizontal; fdErrorVertical = _fdErrorVertical; };
  /// Sets the bearing error, in degrees
  void setErrorBearing( double _fdErrorBearing ) { fdErrorBearing = _fdErrorBearing; };
  /// Sets the (horizontal and vertical) speed error, in meters per second
  void setErrorSpeed( double _fdErrorSpeed, double _fdErrorSpeedVertical = CDataValidity::UNDEFINED_VALUE )
  { fdErrorSpeed = _fdErrorSpeed; fdErrorSpeedVertical = _fdErrorSpeedVertical; };
  /// Sets the additional textual data string
  void setText( const QString& _rqsText ) { qsText = _rqsText; };

  // GETTERS
public:
  /// Returns the fix type
  int getType() const { return eType; };
  /// Returns the fix type as a human-friendly string
  QString getTypeString() const;
  /// Returns the seen sources count (most likely satellites)
  int getSourcesSeen() const { return iSourcesSeen; };
  /// Returns the used sources count (most likely satellites)
  int getSourcesUsed() const { return iSourcesUsed; };
  /// Returns the time error, in seconds
  double getErrorTime() const { return fdErrorTime; };
  /// Returns the horizontal position error, in meters
  double getErrorHorizontal() const { return fdErrorHorizontal; };
  /// Returns the vertical position error, in meters
  double getErrorVertical() const { return fdErrorVertical; };
  /// Returns the bearing error, in degrees
  double getErrorBearing() const { return fdErrorBearing; };
  /// Returns the horizontal speed error, in meters per second
  double getErrorSpeed() const { return fdErrorSpeed; };
  /// Returns the vertical speed error, in meters per second
  double getErrorSpeedVertical() const { return fdErrorSpeedVertical; };
  /// Returns the additional textual data string
  QString getText() const { return qsText; };

};

#endif // QVCT_CDEVICEDATAFIX_HPP
