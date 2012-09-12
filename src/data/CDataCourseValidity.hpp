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

#ifndef QVCT_CDATACOURSEVALIDITY_HPP
#define QVCT_CDATACOURSEVALIDITY_HPP

// QVCT
#include "data/CDataValidity.hpp"


/// Course data validity
/**
 *  This class gathers all parameters and methods that allow to manage the
 *  validity of course data.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataCourseValidity: public CDataValidity
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Bearing last-set time, in seconds from Unix epoch
  /** @see setValidityBearing(), getTimeLastBearing() */
  double fdTimeLastBearing;
  /// Bearing error, in meters
  /** @see setValidityBearing(), getErrorBearing() */
  double fdErrorBearing;
  /// Bearing (forced) invalidity status
  /** @see setValidityBearing(), isInvalidBearing() */
  bool bInvalidBearing;

  /// Horizontal speed last-set time, in seconds from Unix epoch
  /** @see setValiditySpeed(), getTimeLastSpeed() */
  double fdTimeLastSpeed;
  /// Horizontal speed error, in meters per second
  /** @see setValiditySpeed(), getErrorSpeed() */
  double fdErrorSpeed;
  /// Horizontal speed (forced) invalidity status
  /** @see setValiditySpeed(), isInvalidSpeed() */
  bool bInvalidSpeed;

  /// Vertical speed last-set time, in seconds from Unix epoch
  /** @see setValiditySpeedVertical(), getTimeLastSpeedVertical() */
  double fdTimeLastSpeedVertical;
  /// Vertical speed error, in meters per second
  /** @see setValiditySpeedVertical(), getErrorSpeedVertical() */
  double fdErrorSpeedVertical;
  /// Vertical speed (forced) invalidity status
  /** @see setValiditySpeedVertical(), isInvalidSpeedVertical() */
  bool bInvalidSpeedVertical;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataCourseValidity();
  virtual ~CDataCourseValidity() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the bearing's validity parameters
  void setValidityBearing( double _fdTimeLastBearing, double _fdErrorBearing, bool _bInvalidBearing = false )
  { fdTimeLastBearing = _fdTimeLastBearing; fdErrorBearing = _fdErrorBearing; bInvalidBearing = _bInvalidBearing; };
  /// Sets the horizontal speed's validity parameters
  void setValiditySpeed( double _fdTimeLastSpeed, double _fdErrorSpeed, bool _bInvalidSpeed = false )
  { fdTimeLastSpeed = _fdTimeLastSpeed; fdErrorSpeed = _fdErrorSpeed; bInvalidSpeed = _bInvalidSpeed; };
  /// Sets the vertical speed's validity parameters
  void setValiditySpeedVertical( double _fdTimeLastSpeedVertical, double _fdErrorSpeedVertical, bool _bInvalidSpeedVertical = false )
  { fdTimeLastSpeedVertical = _fdTimeLastSpeedVertical; fdErrorSpeedVertical = _fdErrorSpeedVertical; bInvalidSpeedVertical = _bInvalidSpeedVertical; };

  // GETTERS
public:
  /// Returns the bearing's last-set time, in seconds from Unix epoch
  double getTimeLastBearing() const { return fdTimeLastBearing; };
  /// Returns the bearing's error, in seconds
  double getErrorBearing() const { return fdErrorBearing; };
  /// Returns the bearing's (forced) invalidity status
  bool isInvalidBearing() const { return bInvalidBearing; };
  /// Returns the bearing's (global) validity status
  /** NOTE: this take into account the bearing's last-set time and error, based on application settings */
  bool isValidBearing() const;

  /// Returns the horizontal speed's last-set time, in seconds from Unix epoch
  double getTimeLastSpeed() const { return fdTimeLastSpeed; };
  /// Returns the horizontal speed's error, in seconds
  double getErrorSpeed() const { return fdErrorSpeed; };
  /// Returns the horizontal speed's (forced) invalidity status
  bool isInvalidSpeed() const { return bInvalidSpeed; };
  /// Returns the horizontal speed's (global) validity status
  /** NOTE: this take into account the horizontal speed's last-set time and error, based on application settings */
  bool isValidSpeed() const;

  /// Returns the vertical speed's last-set time, in seconds from Unix epoch
  double getTimeLastSpeedVertical() const { return fdTimeLastSpeedVertical; };
  /// Returns the vertical speed's error, in seconds
  double getErrorSpeedVertical() const { return fdErrorSpeedVertical; };
  /// Returns the vertical speed's (forced) invalidity status
  bool isInvalidSpeedVertical() const { return bInvalidSpeedVertical; };
  /// Returns the vertical speed's (global) validity status
  /** NOTE: this take into account the vertical speed's last-set time and error, based on application settings */
  bool isValidSpeedVertical() const;

};

/// Ground and apparent geographical courses validity container
/**
 *  This class gathers both ground and apparent geographical courses validity
 *  in a single container.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataCourseValidityGA
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

public:
  /// Ground course validity
  CDataCourseValidity GroundCourseValidity;
  /// Apparent course validity
  CDataCourseValidity ApparentCourseValidity;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataCourseValidityGA();
  CDataCourseValidityGA( const CDataCourseValidity& _roGroundGeoCourseValidity, const CDataCourseValidity& _roApparentGeoCourseValidity );
  virtual ~CDataCourseValidityGA() {};

};

#endif // QVCT_CDATACOURSEVALIDITY_HPP
