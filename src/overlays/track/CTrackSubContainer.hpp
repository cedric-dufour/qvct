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

#ifndef QVCT_CTRACKSUBCONTAINER_HPP
#define QVCT_CTRACKSUBCONTAINER_HPP

// QT
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "overlays/COverlayContainer.hpp"
#include "overlays/COverlayVisibility.hpp"
class CChart;
class CTrackOverlay;


/// [UI] Track overlay sub-container
/**
 *  This class implements the overlay sub-container for the track overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CTrackSubContainer: public COverlayContainer, public COverlayVisibility
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CTrackSubContainer( const QString& _rqsName );
  virtual ~CTrackSubContainer();


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void unserialize( QDataStream& _rqDataStream );

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent()->parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter );
  virtual void showDetail();
  virtual void showEdit() {};


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
  virtual void setVisibleRouting( bool _bVisibleRouting );
  virtual void toggleVisible();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Returns the last track (item/point) from this sub-container (0 if none is found)
  CTrackPoint* getLastPoint();
  /// Add a new track (item/point) to this sub-container
  CTrackPoint* addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition = CDataPosition::UNDEFINED );
  /// Returns the track's (rhumb-line) length
  double getLengthRL();
  /// Returns the track's elapsed time
  double getTimeElapsed();

public:
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement );
  /// Retrieves this object's content from the given GPX source (file)
  int parseGPX( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected = false ) const;
  /// Stores this object's content to the given GPX destination (file)
  void dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected = false ) const;
  /// Deletes selected items within this sub-container
  int deleteSelection();
  
};

#endif // QVCT_CTRACKSUBCONTAINER_HPP
