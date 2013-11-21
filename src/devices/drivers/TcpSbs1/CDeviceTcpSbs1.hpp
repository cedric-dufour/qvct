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

#ifndef QVCT_CDEVICETCPSBS1_HPP
#define QVCT_CDEVICETCPSBS1_HPP

// QT
#include <QAbstractSocket> // QtNetwork module
#include <QDomElement> // QtXml module
#include <QString>
#include <QTcpSocket> // QtNetwork module
#include <QXmlStreamWriter>

// QVCT
#include "devices/CDevice.hpp"
#include "devices/drivers/CDeviceDriver.hpp"
#include "units/CUnitTimeZone.hpp"


/// SBS-1 protocol (TCP socket) navigation device
/**
 *  This class allows to interface with a SBS-1 data provider (via the network)
 *  and use the data corresponding to GPS activity.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceTcpSbs1: public CDevice
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
  /// Time zone
  /** @see setTimeZone(), getTimeZone() */
  CUnitTimeZone::EUnit eTimeZone;
  /// Ground traffic (inclusion/track)
  /** @see setGroundTraffic(), isGroundTraffic() */
  bool bGroundTraffic;
  /// TCP socket
  QTcpSocket* pqTcpSocket;
  /// Start status
  bool bStarted;
  /// Pause status
  bool bPaused;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceTcpSbs1( const QString& _rqsName );
  virtual ~CDeviceTcpSbs1();


  //------------------------------------------------------------------------------
  // METHODS: CDevice (implement/override)
  //------------------------------------------------------------------------------

public:
  QVCT::EStatus setOperatingMode( CDevice::EOperatingMode _eOperatingMode );

public:
  virtual CDeviceDriver::EDriver getDriver() const { return CDeviceDriver::TCP_SBS1; };
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
  /// Slots to report TCP connection
  void slotTcpConnected();
  /// Slots to report TCP error
  void slotTcpError( QAbstractSocket::SocketError _qSocketError );
  /// Slots to process device data
  void slotProcessData();

  // SETTERS
public:
  // Sets the SBS-1 device's network host
  void setHost( const QString& _rqsHost ) { qsHost = _rqsHost; };
  // Sets the SBS-1 device's network port
  void setPort( int _iPort ) { iPort = _iPort; };
  // Sets the SBS-1 device's time zone
  void setTimeZone( CUnitTimeZone::EUnit _eTimeZone ) { eTimeZone = _eTimeZone; };
  // Sets ground traffic (inclusion/track) state
  void setGroundTraffic( bool _bGroundTraffic ) { bGroundTraffic = _bGroundTraffic; };

  // GETTERS
public:
  // Returns the SBS-1 device's network host
  QString getHost() { return qsHost; };
  // Returns the SBS-1 device's network port
  int getPort() { return iPort; };
  // Returns the SBS-1 device's time zone
  CUnitTimeZone::EUnit getTimeZone() { return eTimeZone; };
  // Returns ground traffic (inclusion/track) state
  bool isGroundTraffic() { return bGroundTraffic; };

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

#endif // QVCT_CDEVICETCPSBS1_HPP
