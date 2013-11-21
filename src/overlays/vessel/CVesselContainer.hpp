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

#ifndef QVCT_CVESSELCONTAINER_HPP
#define QVCT_CVESSELCONTAINER_HPP

// QT
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QXmlStreamWriter>

// QVCT
#include "data/CDataPosition.hpp"
#include "overlays/COverlayContainer.hpp"
#include "overlays/COverlayVisibility.hpp"
class CChart;
class CDevice;
class CVesselContainerDevice;
class CVesselOverlay;
class CVesselPoint;


/// [UI] Vessel overlay container / flotilla
/**
 *  This class implements the overlay container for the vessel overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselContainer: public COverlayContainer, public COverlayVisibility
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Dynamic status
  /** NOTE: A dynamic flotilla is one for which a device has been set and
   *  stored as its first child (index 0) and used to dynamically generate
   *  vessels (at indices > 0 ) based on what the device sends, with each
   *  vessel's name corresponding to the device source's name.
   *  @see setDevice(), clearDevice() */
  bool bDynamic;

  /// Vessel's type
  QString qsType;
  /// Vessel's description
  QString qsDescription;
  /// Vessel's comment
  QString qsComment;
  /// Vessel's URL
  QString qsUrl;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselContainer( const QString& _rqsName );
  virtual ~CVesselContainer();


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const;
  virtual void unserialize( QDataStream& _rqDataStream );

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter );
  virtual void showDetail();
  virtual void showEdit();


  //------------------------------------------------------------------------------
  // METHODS: COverlayContainer (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const;


  //------------------------------------------------------------------------------
  // METHODS: COverlayVisibility (override)
  //------------------------------------------------------------------------------

public:
  virtual void setVisibleName( bool _bVisibleName );
  virtual void setVisiblePosition( bool _bVisiblePosition );
  virtual void setVisibleCourse( bool _bVisibleCourse );
  virtual void toggleVisible();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets this flotilla's type
  void setType( const QString& _rqsType ) { qsType = _rqsType; };
  /// Sets this flotilla's description
  void setDescription( const QString& _rqsDescription ) { qsDescription = _rqsDescription; };
  /// Sets this flotilla's comment
  void setComment( const QString& _rqsComment ) { qsComment = _rqsComment; };
  /// Sets this flotilla's URL
  void setUrl( const QString& _rqsUrl ) { qsUrl = _rqsUrl; };

  // GETTERS
public:
  /// Returns this flotilla's dynamic status
  bool isDynamic() { return bDynamic; };
  /// Returns whether this flotilla's vessels devices are all connected
  bool isDeviceConnected() const;

  /// Returns this flotilla's type
  QString getType() const { return qsType; };
  /// Returns this flotilla's description
  QString getDescription() const { return qsDescription; };
  /// Returns this flotilla's comment
  QString getComment() const { return qsComment; };
  /// Returns this flotilla's URL
  QString getUrl() const { return qsUrl; };

  // OTHER
public:
  /// Sets a device to dynamically generate this flotilla
  void setDevice( CVesselContainerDevice* _poVesselContainerDevice );
  /// Connects all children vessels devices
  void connectDevice();
  /// Disconnects all children vessels devices
  void disconnectDevice();
  /// Clears the device set to dynamically generate this flotilla
  void clearDevice();

public:
  /// Add a new vessel (item/point) to this container
  CVesselPoint* addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition = CDataPosition::UNDEFINED );
  /// Add a new dynamic vessel (item/point) with the given name to this container
  /** NOTE: If a vessel with a matching name already exists, none is created */
  void addPointDynamic( const QString& _rqsName, const QString& _rqsDeviceName );
  /// Deletes dynamic vessels (items/points) that have not been updated for the given "time-to-live" seconds
  int cleanPointDynamic( int _iTTL );

public:
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected = false ) const;
  /// Deletes selected items within this container
  int deleteSelection();

private:
  /// Clear the entire content of this container
  void clear();

};

#endif // QVCT_CVESSELCONTAINER_HPP
