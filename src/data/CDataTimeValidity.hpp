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

#ifndef QVCT_CDATATIMEVALIDITY_HPP
#define QVCT_CDATATIMEVALIDITY_HPP

// QVCT
#include "data/CDataValidity.hpp"


/// Time data validity
/**
 *  This class gathers all parameters and methods that allow to manage the
 *  validity of a time data.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataTimeValidity: public CDataValidity
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Time last-set time, in seconds from Unix epoch
  /** @see setValidityTime(), getTimeLastTime() */
  double fdTimeLastTime;
  /// Time error, in seconds
  /** @see setValidityTime(), getErrorTime() */
  double fdErrorTime;
  /// Time (forced) invalidity status
  /** @see setValidityTime(), isInvalidTime() */
  bool bInvalidTime;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataTimeValidity();
  virtual ~CDataTimeValidity() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the time's validity parameters
  void setValidityTime( double _fdTimeLastTime, double _fdErrorTime, bool _bInvalidTime = false )
  { fdTimeLastTime = _fdTimeLastTime; fdErrorTime = _fdErrorTime; bInvalidTime = _bInvalidTime; };

  // GETTERS
public:
  /// Returns the time's last-set time, in seconds from Unix epoch
  double getTimeLastTime() const { return fdTimeLastTime; };
  /// Returns the time's error, in seconds
  double getErrorTime() const { return fdErrorTime; };
  /// Returns the time's (forced) invalidity status
  bool isInvalidTime() const { return bInvalidTime; };
  /// Returns the time's (global) validity status
  /** NOTE: this take into account the time's error, based on application settings */
  bool isValidTime() const;

};

#endif // QVCT_CDATATIMEVALIDITY_HPP
