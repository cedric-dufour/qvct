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

#ifndef QVCT_CDEVICEGPSDAIS_HPP
#define QVCT_CDEVICEGPSDAIS_HPP

// GPSD
#include "gps.h"
#if GPSD_API_MAJOR_VERSION < 4
#error "Unsupported GPSD API version (<4)"
#endif
// #include "gpsdclient.h"


// QT
#include <QDomElement> // QtXml module
#include <QSocketNotifier>
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "devices/CDevice.hpp"
#include "devices/drivers/CDeviceDriver.hpp"


/// GPSD daemon (GPS mode) navigation device
/**
 *  This class allows to interface with the GPSD daemon (via the network) and
 *  use the data corresponding to GPS activity.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceGpsdAis: public CDevice
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Network host
  /** @see setHost(), getHost() */
  QString qsHost;
  /// Network port
  /** @see setPort(), getPort() */
  int iPort;
  /// GPSD data
  /** @see gps.h */
  struct gps_data_t sGpsData;
  /// GPSD data pointer
  struct gps_data_t* psGpsData;
  /// Socket notifier
  QSocketNotifier* pqSocketNotifier;
  /// Pause status
  bool bPaused;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceGpsdAis( const QString& _rqsName );
  virtual ~CDeviceGpsdAis();


  //------------------------------------------------------------------------------
  // METHODS: CDevice (implement/override)
  //------------------------------------------------------------------------------

public:
  QVCT::EStatus setOperatingMode( CDevice::EOperatingMode _eOperatingMode );

public:
  virtual CDeviceDriver::EDriver getDriver() const { return CDeviceDriver::GPSD_AIS; };
  virtual int getCapabilities() const { return CDevice::FIX; };

public:
  virtual void showDetail();
  virtual void showEdit();

public:
  void parseQVCT( const QDomElement& _rqDomElement );
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slots to process device data
  void slotProcessData( int );

  // SETTERS
public:
  // Sets GPSD's network host
  void setHost( const QString& _rqsHost ) { qsHost = _rqsHost; };
  // Sets GPSD's network port
  void setPort( int _iPort ) { iPort = _iPort; };

  // GETTERS
public:
  // Returns GPSD's network host
  QString getHost() { return qsHost; };
  // Returns GPSD's network port
  int getPort() { return iPort; };

  // OTHER
private:
  /// Start the device
  QVCT::EStatus stop();
  /// Pause the device
  QVCT::EStatus pause();
  /// Stop the device
  QVCT::EStatus start();
  /// Returns the device's status (operating mode)
  CDevice::EOperatingMode status();

};

#endif // QVCT_CDEVICEGPSDAIS_HPP
