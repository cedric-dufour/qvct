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

#ifndef QVCT_CDEVICEDRIVER_HPP
#define QVCT_CDEVICEDRIVER_HPP

// QT
#include <QMap>
#include <QString>

// QVCT
class CDevice;
class CDeviceDriverNames; // see below
class CDeviceDriverCodes; // see below


/// Device driver inventory and instantiation class
/**
 *  This class lists existing device drivers and allows to create corresponding
 *  devices.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDriver
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Driver ID
  enum EDriver {
    GPSD_GPS,  ///< GPSD daemon (GPS mode)
    GPSD_AIS,  ///< GPSD daemon (AIS mode)
    // SAMPLE, ///< SAMPLE_DRIVER
    UNDEFINED  ///< Undefined driver
  };

private:
  /// Container for supported human-readable device driver names
  static const CDeviceDriverNames oDeviceDriverNames;
  /// Container for supported machine-friendly device driver codes
  static const CDeviceDriverCodes oDeviceDriverCodes;

public:
  /// Returns the list of supported human-readable device driver names
  static const QMap<EDriver,QString> &names();
  /// Returns the human-readable name corresponding to the given device driver ID
  static QString toName( EDriver _eDriver );
  /// Returns the device driver ID corresponding to the given human-readable name
  static EDriver fromName( const QString& _rqsName );
  /// Returns the list of supported machine-friendly device driver codes
  static const QMap<EDriver,QString> &codes();
  /// Returns the machine-friendly code corresponding to the given device driver ID
  static QString toCode( EDriver _eDriver );
  /// Returns the device driver ID corresponding to the given machine-friendly code
  static EDriver fromCode( const QString& _rqsCode );
  /// Create a new device based on the specified driver
  static CDevice* createDevice( const QString& _rqsDeviceName, EDriver _eDriver );

};

/// Container class for supported human-readable device driver names
class CDeviceDriverNames
{
  friend class CDeviceDriver;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CDeviceDriverNames()
  {
    qMapNames.insert( CDeviceDriver::GPSD_GPS, "GPSD (GPS)" );
    qMapNames.insert( CDeviceDriver::GPSD_AIS, "GPSD (AIS)" );
    // qMapNames.insert( CDeviceDriver::SAMPLE, "Sample Driver" ); // SAMPLE_DRIVER
  };
  QMap<CDeviceDriver::EDriver,QString> qMapNames;
};

/// Container class for supported machine-friendly device driver codes
class CDeviceDriverCodes
{
  friend class CDeviceDriver;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

private:
  CDeviceDriverCodes()
  {
    qMapCodes.insert( CDeviceDriver::GPSD_GPS, "gpsd_gps" );
    qMapCodes.insert( CDeviceDriver::GPSD_AIS, "gpsd_ais" );
    // qMapCodes.insert( CDeviceDriver::SAMPLE, "sample" ); // SAMPLE_DRIVER
  };
  QMap<CDeviceDriver::EDriver,QString> qMapCodes;
};

#endif // QVCT_CDEVICEDRIVER_HPP
