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

#ifndef QVCT_CVESSELCONTAINERDEVICE_HPP
#define QVCT_CVESSELCONTAINERDEVICE_HPP

// QT
#include <QDataStream>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "devices/data/CDeviceDataFix.hpp"
#include "overlays/COverlayItem.hpp"
class CChart;
class CDevice;


/// [UI] Flotilla device
/**
 *  This class implements the link between a (dynamic) flotilla and a device,
 *  used to update the flotilla with automatically created vessels (based on
 *  what the device sends).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselContainerDevice: public COverlayItem
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Corresponding device's pointer
  CDevice* poDevice;

  /// (Vessels) Time-to-live (seconds)
  /** @see setTTL(), getTTL() */
  int iTTL;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselContainerDevice( const QString& _rqsName );
  virtual ~CVesselContainerDevice() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const {};
  virtual void unserialize( QDataStream& _rqDataStream ) {};

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent()->parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter ) {};
  virtual void showDetail();
  virtual void showEdit();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SIGNALS
signals:
  /// Signal emitted when views should refresh this object's corresponding content
  void signalRefreshContent();

  // SLOTS
private slots:
  /// Slot to handle object destruction
  void slotDestroyed( QObject* _pqObject );

private slots:
  /// Slot to handle fix data (sent by device)
  void slotDataFix( const CDeviceDataFix& _roDeviceDataFix );

  // SETTERS
public:
  /// Sets the (vessels) time-to-live (seconds)
  void setTTL( int _iTTL ) { iTTL = _iTTL; };

  // GETTERS
public:
  /// Returns whether the actual device is connected
  bool isDeviceConnected() const;
  /// Returns the (vessels) time-to-live
  int getTTL() const { return iTTL; };

  // OTHER
public:
  /// Connects to the actual device
  bool connectDevice();
  /// Dicconnects from the actual device
  void disconnectDevice();

public:
  /// Retrieves this object's content from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;

};

#endif // QVCT_CVESSELCONTAINERDEVICE_HPP
