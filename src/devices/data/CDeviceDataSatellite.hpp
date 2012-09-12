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

#ifndef QVCT_CDEVICEDATASATELLITE_HPP
#define QVCT_CDEVICEDATASATELLITE_HPP


/// Satellite data [PRN,signal,azimuth,elevation,etc.]
/**
 *  This class gathers all details of satellites used to acquire a geographical
 *  fix.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDataSatellite
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Satellite PRN (ID)
  /** @see getPRN() */
  int iPRN;
  /// Satellite azimuth, in degrees
  /** @see setAzimuth(), getAzimuth() */
  double fdAzimuth;
  /// Satellite elevation, in degrees
  /** @see setElevation(), getElevation() */
  double fdElevation;
  /// Satellite signal-to-noise ratio, in decibels
  /** @see setSignal(), getSignal() */
  double fdSignal;
  /// Satellite usage status
  /** @see setUsed(), isUsed() */
  bool bUsed;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceDataSatellite( int _iPRN );
  virtual ~CDeviceDataSatellite() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the satellite azimuth, in degrees
  void setAzimuth( double _fdAzimuth ) { fdAzimuth = _fdAzimuth; };
  /// Sets the satellite elevation, in degrees
  void setElevation( double _fdElevation ) { fdElevation = _fdElevation; };
  /// Sets the satellite signal-to-noise ratio, in decibels
  void setSignal( double _fdSignal ) { fdSignal = _fdSignal; };
  /// Sets the satellite usage status
  void setUsed( bool _bUsed ) { bUsed = _bUsed; };

  // GETTERS
public:
  /// Returns the satellite PRN (ID)
  int getPRN() const { return iPRN; };
  /// Returns the satellite azimuth, in degrees
  double getAzimuth() const { return fdAzimuth; };
  /// Returns the satellite elevation, in degrees
  double getElevation() const { return fdElevation; };
  /// Returns the satellite signal-to-noise ratio, in decibels
  double getSignal() const { return fdSignal; };
  /// Returns the satellite usage status
  bool isUsed() { return bUsed; };

};

#endif // QVCT_CDEVICEDATASATELLITE_HPP
