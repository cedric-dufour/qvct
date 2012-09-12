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

#ifndef QVCT_CDEVICESAMPLE_HPP
#define QVCT_CDEVICESAMPLE_HPP


// QT
#include <QDomElement> // QtXml module
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "devices/CDevice.hpp"
#include "devices/drivers/CDeviceDriver.hpp"


/// SAMPLE_DRIVER
class CDeviceSample: public CDevice
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

  //
  // Driver configuration parameters
  //

private:
  /// Network host
  /** @see setHost(), getHost() */
  QString qsHost;
  /// Network port
  /** @see setPort(), getPort() */
  int iPort;



  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceSample( const QString& _rqsName );
  virtual ~CDeviceSample();


  //------------------------------------------------------------------------------
  // METHODS: CDevice (implement/override)
  //------------------------------------------------------------------------------

public:
  QVCT::EStatus setOperatingMode( CDevice::EOperatingMode _eOperatingMode );

public:
  virtual CDeviceDriver::EDriver getDriver() const { return CDeviceDriver::SAMPLE; };
  virtual int getCapabilities() const { return CDevice::FIX & CDevice::SKYVIEW; };

public:
  virtual void showDetail();
  virtual void showEdit();

public:
  void parseQVCT( const QDomElement& _rqDomElement );
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  //
  // Methods to process the device data and send corresponding CDeviceDataFix
  // and/or CDeviceDataSkyView
  //

  // SLOTS
private slots:
  /// Slots to process device data
  void slotProcessData( int );

  //
  // Methods to set/retrieve driver configuration parameters
  //

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

};

#endif // QVCT_CDEVICESAMPLE_HPP
