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

#ifndef QVCT_CDATAPOSITIONVALIDITY_HPP
#define QVCT_CDATAPOSITIONVALIDITY_HPP

// QVCT
#include "data/CDataValidity.hpp"


/// Position data validity
/**
 *  This class gathers all parameters and methods that allow to manage the
 *  validity of position data.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataPositionValidity: public CDataValidity
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Position last-set time, in seconds from Unix epoch
  /** @see setValidityPosition(), getTimeLastPosition() */
  double fdTimeLastPosition;
  /// Position error, in meters
  /** @see setValidityPosition(), getErrorPosition() */
  double fdErrorPosition;
  /// Position (forced) invalidity status
  /** @see setValidityPosition(), isInvalidPosition() */
  bool bInvalidPosition;

  /// Elevation last-set time, in seconds from Unix epoch
  /** @see setValidityElevation(), getTimeLastElevation() */
  double fdTimeLastElevation;
  /// Elevation error, in meters
  /** @see setValidityElevation(), getErrorElevation() */
  double fdErrorElevation;
  /// Elevation (forced) invalidity status
  /** @see setValidityElevation(), isInvalidElevation() */
  bool bInvalidElevation;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataPositionValidity();
  virtual ~CDataPositionValidity() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the position's validity parameters
  void setValidityPosition( double _fdTimeLastPosition, double _fdErrorPosition, bool _bInvalidPosition = false )
  { fdTimeLastPosition = _fdTimeLastPosition; fdErrorPosition = _fdErrorPosition; bInvalidPosition = _bInvalidPosition; };
  /// Sets the elevation's validity parameters
  void setValidityElevation( double _fdTimeLastElevation, double _fdErrorElevation, bool _bInvalidElevation = false )
  { fdTimeLastElevation = _fdTimeLastElevation; fdErrorElevation = _fdErrorElevation; bInvalidElevation = _bInvalidElevation; };

  // GETTERS
public:
  /// Returns the position's last-set time, in seconds from Unix epoch
  double getTimeLastPosition() const { return fdTimeLastPosition; };
  /// Returns the position's error, in seconds
  double getErrorPosition() const { return fdErrorPosition; };
  /// Returns the position's (forced) invalidity status
  bool isInvalidPosition() const { return bInvalidPosition; };
  /// Returns the position's (global) validity status
  /** NOTE: this take into account the position's last-set time and error, based on application settings */
  bool isValidPosition() const;

  /// Returns the elevation's last-set time, in seconds from Unix epoch
  double getTimeLastElevation() const { return fdTimeLastElevation; };
  /// Returns the elevation's error, in seconds
  double getErrorElevation() const { return fdErrorElevation; };
  /// Returns the elevation's (forced) invalidity status
  bool isInvalidElevation() const { return bInvalidElevation; };
  /// Returns the elevation's (global) validity status
  /** NOTE: this take into account the elevation's last-set time and error, based on application settings */
  bool isValidElevation() const;

};

#endif // QVCT_CDATAPOSITIONVALIDITY_HPP
