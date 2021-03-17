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

#ifndef QVCT_CDATACOURSE_HPP
#define QVCT_CDATACOURSE_HPP

// QT
class QDataStream;


/// (Geographical) Course data [bearing, horizontal/vertical speeds]
/**
 *  This class gathers all components of a fully-qualified geographical course
 *  (bearing, horizontal/vertical speeds) in SI units (respectively degrees and
 *  meters per second).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataCourse
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Specific value for an undefined bearing
  static constexpr double UNDEFINED_BEARING = -999;
  /// Specific value for an undefined speed
  static constexpr double UNDEFINED_SPEED = -9999999;
  /// Specific value for an undefined course
  static const CDataCourse UNDEFINED;


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  ///  Bearing, in degrees
  /** @see setCourse(), setBearing(), getBearing() */
  double fdBearing;
  ///  Horizontal speed, in meters per second
  /** @see setCourse(), setSpeed(), getSpeed() */
  double fdSpeed;
  ///  Vertical speed, in meters per second
  /** @see setCourse(), setSpeedVertical(), getSpeedVertical() */
  double fdSpeedVertical;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataCourse();
  /** @param[in] _fdBearing Bearing, in degrees
   *  @param[in] _fdSpeed Horizontal speed, in meters per second
   *  @param[in] _fdSpeedVertical Vertical speed, in meters per second */
  CDataCourse( double _fdBearing, double _fdSpeed, double _fdSpeedVertical = UNDEFINED_SPEED );
  CDataCourse( const CDataCourse& _roDataCourse );
  virtual ~CDataCourse() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets course values
  /** @param[in] _fdBearing Bearing, in degrees
   *  @param[in] _fdSpeed Horizontal speed, in meters per second
   *  @param[in] _fdSpeedVertical Vertical speed, in meters per second */
  void setCourse( double _fdBearing, double _fdSpeed, double _fdSpeedVertical = UNDEFINED_SPEED );
  /// Copy values from another course
  void setCourse( const CDataCourse& _roDataCourse );
  /// Resets all values (to an undefined course)
  /** @see UNDEFINED */
  void resetCourse();
  /// Sets this course's bearing, in degrees
  void setBearing( double _fdBearing ) { fdBearing = _fdBearing; };
  /// Resets this course's bearing
  /** @see UNDEFINED_BEARING */
  void resetBearing() { fdBearing = UNDEFINED_BEARING; };
  /// Sets this course's horizontal speed, in meters per second
  void setSpeed( double _fdSpeed, double _fdSpeedVertical = UNDEFINED_SPEED ) { fdSpeed = _fdSpeed; fdSpeedVertical = _fdSpeedVertical; };
  /// Resets this course's horizontal speed
  /** @see UNDEFINED_SPEED */
  void resetSpeed() { fdSpeed = UNDEFINED_SPEED; };
  /// Sets this course's vertical speed, in meters per second
  void setSpeedVertical( double _fdSpeedVertical ) { fdSpeedVertical = _fdSpeedVertical; };
  /// Resets this course's vertical speed
  /** @see UNDEFINED_SPEED */
  void resetSpeedVertical() { fdSpeedVertical = UNDEFINED_SPEED; };

  // GETTERS
public:
  /// Returns this course's bearing, in degrees
  double getBearing() const { return fdBearing; };
  /// Returns this course's horizontal speed, in meters per second
  double getSpeed() const { return fdSpeed; };
  /// Returns this course's vertical speed, in meters per second
  double getSpeedVertical() const { return fdSpeedVertical; };

  // OTHER
public:
  /// Serializes (store) this object's data to binary format
  void serialize( QDataStream& _rqDataStream ) const;
  /// Unserializes (restore) this object's data from binary format
  void unserialize( QDataStream& _rqDataStream );

  // OPERATORS
public:
  /// Equality operator
  /** This operator returns true if all components - bearing, horizontal speed
   *  and vertical speed - match. */
  bool operator==( const CDataCourse& _roCourse ) const;
  /// Inequality operator
  /** This operator returns true if any components - bearing, horizontal speed
   *  and vertical speed - mismatch. */
  bool operator!=( const CDataCourse& _roCourse ) const;

  // COMPARATORS
public:
  /// Bearing (ascending sort) comparison operator
  static bool compareBearingAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );
  /// Bearing (descending sort) comparison operator
  static bool compareBearingDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );
  /// Horizontal speed (ascending sort) comparison operator
  static bool compareSpeedAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );
  /// Horizontal speed (descending sort) comparison operator
  static bool compareSpeedDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );
  /// Vertical speed (ascending sort) comparison operator
  static bool compareSpeedVerticalAscending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );
  /// Vertical speed (descending sort) comparison operator
  static bool compareSpeedVerticalDescending( const CDataCourse& _roCourse1, const CDataCourse& _roCourse2 );

};


/// Ground and apparent geographical courses container
/**
 *  This class gathers both ground and apparent geographical course
 *  in a single container.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDataCourseGA
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

public:
  /// Ground course
  CDataCourse GroundCourse;
  /// Apparent course
  CDataCourse ApparentCourse;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDataCourseGA();
  CDataCourseGA( const CDataCourse& _roDataCourseGround, const CDataCourse& _roDataCourseApparent );
  virtual ~CDataCourseGA() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Serializes (store) this object's data to binary format
  void serialize( QDataStream& _rqDataStream ) const;
  /// Unserializes (restore) this object's data from binary format
  void unserialize( QDataStream& _rqDataStream );

};

#endif // QVCT_CDATACOURSE_HPP
