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

#ifndef QVCT_CDATAPOSITION_HPP
#define QVCT_CDATAPOSITION_HPP

// QT
class QDataStream;


/// (Geographical) Position data [long,lat,elev]
/**
 *  This class gathers all components of a fully-qualified geographical position
 *  (longitude, latitude and elevation) in SI units (respectively degrees, degrees
 *  and meters).
 *  It also provides useful geographical position processing functions, such as
 *  comparison (sorting), distance and bearing between two positions.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataPosition
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Specific value for an undefined longitude
  static const double UNDEFINED_LONGITUDE = -999;
  /// Specific value for an undefined latitude
  static const double UNDEFINED_LATITUDE = -99;
  /// Specific value for an undefined elevation
  static const double UNDEFINED_ELEVATION = -9999999;
  /// Specific value for an undefined position
  static const CDataPosition UNDEFINED;

public:
  /// Returns the WGS84-corrected great-circle distance between two points, in meters
  static double distanceGC( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Returns the great-circle (initial) bearing between two points, in degrees
  static double bearingGC( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Returns the rhumb-line distance between two points, in meters
  static double distanceRL( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Returns the rhumb-line (constant) bearing between two points, in degrees
  static double bearingRL( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Longitude, in degrees
  /** @see setPosition(), getLongitude() */
  double fdLongitude;
  /// Latitude, in degrees
  /** @see setPosition(), getLatitude() */
  double fdLatitude;
  /// Elevation, in meters
  /** @see setPosition(), setElevation(), getElevation() */
  double fdElevation;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataPosition();
  /** @param[in] _fdLongitude Longitude, in degrees
   *  @param[in] _fdLatitude Latitude, in degrees
   *  @param[in] _fdElevation Elevation, in meters */
  CDataPosition( double _fdLongitude, double _fdLatitude,
                double _fdElevation = UNDEFINED_ELEVATION );
  CDataPosition( const CDataPosition& _roDataPosition );
  virtual ~CDataPosition() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets new coordinates
  /** @param[in] _fdLongitude Longitude, in degrees
   *  @param[in] _fdLatitude Latitude, in degrees
   *  @param[in] _fdElevation Elevation, in meters */
  void setPosition( double _fdLongitude, double _fdLatitude,
                    double _fdElevation = UNDEFINED_ELEVATION );
  /// Copy coordinates from another position
  void setPosition( const CDataPosition& _roDataPosition );
  /// Resets all coordinates (to an undefined position)
  /** @see UNDEFINED */
  void resetPosition();
  /// Sets the elevation, in meters
  void setElevation( double _fdElevation ) { fdElevation = _fdElevation; };
  /// Resets the elevation (to an undefined elevation)
  /** @see UNDEFINED_ELEVATION */
  void resetElevation() { fdElevation = UNDEFINED_ELEVATION; };

  // GETTERS
public:
  /// Returns this position's longitude, in degrees
  double getLongitude() const { return fdLongitude; };
  /// Returns this position's latitude, in degrees
  double getLatitude() const { return fdLatitude; };
  /// Returns this position's elevation, in meters
  double getElevation() const { return fdElevation; };

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
  bool operator==( const CDataPosition& _roPosition ) const;
  /// Inequality operator
  /** This operator returns true if any components - longitude, latitude and
   *  elevation - mismatch. */
  bool operator!=( const CDataPosition& _roPosition ) const;

  // COMPARATORS
public:
  /// Longitude (ascending sort) comparison operator
  static bool compareLongitudeAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Longitude (descending sort) comparison operator
  static bool compareLongitudeDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Latitude (ascending sort) comparison operator
  static bool compareLatitudeAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Latitude (descending sort) comparison operator
  static bool compareLatitudeDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Elevation (ascending sort) comparison operator
  static bool compareElevationAscending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );
  /// Elevation (descending sort) comparison operator
  static bool compareElevationDescending( const CDataPosition& _roPosition1, const CDataPosition& _roPosition2 );

};

#endif // QVCT_CDATAPOSITION_HPP
