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

#ifndef QVCT_CLANDMARKCONTAINER_HPP
#define QVCT_CLANDMARKCONTAINER_HPP

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
class CLandmarkOverlay;
class CLandmarkPoint;


/// [UI] Landmark overlay container
/**
 *  This class implements the overlay container for the landmark overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CLandmarkContainer: public COverlayContainer, public COverlayVisibility
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CLandmarkContainer( const QString& _rqsName );
  virtual ~CLandmarkContainer();


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
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
  virtual void toggleVisibility();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Add a new landmark (item/point) to this container
  CLandmarkPoint* addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition = CDataPosition::UNDEFINED );

public:
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement );
  /// Retrieves this object's content from the given GPX source (file)
  int parseGPX( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected = false ) const;
  /// Stores this object's content to the given GPX destination (file)
  void dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected = false ) const;
  /// Deletes selected items within this container
  int deleteSelection();

protected:
  using COverlayVisibility::toggleVisibility;

};

#endif // QVCT_CLANDMARKCONTAINER_HPP
