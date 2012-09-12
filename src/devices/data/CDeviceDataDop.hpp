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

#ifndef QVCT_CDEVICEDATADOP_HPP
#define QVCT_CDEVICEDATADOP_HPP

// QVCT
#include "devices/data/CDeviceDataSource.hpp"
#include "data/CDataPosition.hpp"
#include "data/CDataCourse.hpp"


/// Dilution-of-Precision (DOP) data
/**
 *  This class gathers the various Dilution-of-Precision (DOP) values that may be
 *  associated with other device data.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDataDop
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Specific value for an undefined component
  static const double UNDEFINED_VALUE;
  /// Specific value for undefined DOP data
  static const CDeviceDataDop UNDEFINED;


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Horizontal position Dilution-of-Precision (HDOP)
  /** @see setDopPosition(), getDopHorizontal() */
  double fdDopHorizontal;
  /// Vertical position Dilution-of-Precision (VDOP)
  /** @see setDopPosition(), getDopVertical() */
  double fdDopVertical;
  /// Time Dilution-of-Precision (TDOP)
  /** @see setDopTime(), getDopTime() */
  double fdDopTime;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceDataDop( double _fdDopHorizontal = UNDEFINED_VALUE,
                  double _fdDopVertical = UNDEFINED_VALUE,
                  double _fdDopTime = UNDEFINED_VALUE );
  virtual ~CDeviceDataDop() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets (copy) the Dilution-of-Precision (HDOP, VDOP, TDOP) values from other DOP data
  void setDop( const CDeviceDataDop& _roDeviceDataDop );
  /// Sets the position Dilution-of-Precision (HDOP, VDOP)
  void setDopPosition( double _fdDopHorizontal, double _fdDopVertical = UNDEFINED_VALUE ) { fdDopHorizontal = _fdDopHorizontal; fdDopVertical = _fdDopVertical; };
  /// Sets the time Dilution-of-Precision (TDOP)
  void setDopTime( double _fdDopTime ) { fdDopTime = _fdDopTime; };

  // GETTERS
public:
  /// Returns the time Dilution-of-Precision (TDOP)
  double getDopTime() const { return fdDopTime; };
  /// Returns the horizontal position Dilution-of-Precision (HDOP)
  double getDopHorizontal() const { return fdDopHorizontal; };
  /// Returns the vertical position Dilution-of-Precision (VDOP)
  double getDopVertical() const { return fdDopVertical; };
  /// Returns the position Dilution-of-Precision (PDOP)
  double getDopPosition() const;
  /// Returns the global Dilution-of-Precision (GDOP)
  double getDopGlobal() const;

  // OPERATORS
public:
  /// Equality operator
  /** This operator returns true if all components match */
  bool operator==( const CDeviceDataDop& _roDeviceDataDop ) const;
  /// Inequality operator
  /** This operator returns true if any components mismatch */
  bool operator!=( const CDeviceDataDop& _roDeviceDataDop ) const;

};

#endif // QVCT_CDEVICEDATADOP_HPP
