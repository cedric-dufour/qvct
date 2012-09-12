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

#ifndef QVCT_CDEVICE_HPP
#define QVCT_CDEVICE_HPP

// QT
#include <QDomElement> // QtXml module
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "QVCT.hpp"
#include "devices/drivers/CDeviceDriver.hpp"
#include "overlays/COverlayItem.hpp"
class CDeviceDataFix;
class CDeviceDataSkyView;


/// Generic navigation device (GPS, speedometer, compass, etc.)
/**
 *  This class defines the generic representation of a navigation device, able
 *  to emit full or partial geographical position or course data, along other
 *  useful information.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDevice: public COverlayItem
{
  Q_OBJECT;

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Device capabilities
  //  NOTE: use only power of 2! (so capabalities can be aggregated/differentiated)
  enum ECapability {
    FIX = 1,    ///< Device emits 'newDataFix' signal
    SKYVIEW = 2 ///< Device emits 'newDataSkyView' signal
  };
  /// Device operating mode (stop, start, pause)
  enum EOperatingMode { UNDEFINED, STOP, PAUSE, START };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Device's operating mode
  /** @see setOperatingMode(), getOperatingMode() */
  EOperatingMode eOperatingMode;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CDevice( const QString& _rqsName );

public:
  virtual ~CDevice() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

private:
  virtual void serialize( QDataStream& _rqDataStream ) const {};
  virtual void unserialize( QDataStream& _rqDataStream ) {};

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent(); };

private:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter ) {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SIGNALS
signals:
  /// Signal emitted by the device when its operating mode changed
  void signalOperatingMode( EOperatingMode _eOperatingMode );
  /// Signal emitted by the device when activity occures
  void signalActivity();
  /// Signal emitted by the device when an updated fix is available
  void signalDataFix( const CDeviceDataFix& _roDeviceDataFix );
  /// Signal emitted by the device when an updated sky view is available
  void signalDataSkyView( const CDeviceDataSkyView& _roDeviceDataSkyView );
  /// Signal emitted by the device when an error occured
  void signalError( const QString& _rqsErrorMessage );

  // SETTERS
public:
  /// Sets the device's operating mode
  virtual QVCT::EStatus setOperatingMode( EOperatingMode _eOperatingMode ) = 0;

  // GETTERS
public:
  /// Returns the device's driver ID
  virtual CDeviceDriver::EDriver getDriver() const = 0;
  /// Returns the device's capabilities
  /** @see ECapability */
  virtual int getCapabilities() const = 0;
  /// Returns the device's operating mode
  virtual EOperatingMode getOperatingMode() { return eOperatingMode; };

  // OTHER
public:
  /// Displays the device's details (in the appropriate widget/view)
  virtual void showDetail() = 0;
  /// Displays the device's edit (configuration) widget/view
  virtual void showEdit() = 0;

public:
  /// Retrieves the device's configuration from the given QVCT source (file)
  virtual void parseQVCT( const QDomElement& _rqDomElement ) = 0;
  /// Stores the device's configuration to the given QVCT destination (file)
  virtual void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const = 0;

};

#endif // QVCT_CDEVICE_HPP
