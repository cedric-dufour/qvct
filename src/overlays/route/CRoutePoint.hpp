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

#ifndef QVCT_CROUTEPOINT_HPP
#define QVCT_CROUTEPOINT_HPP

// QT
#include <QDataStream>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "overlays/COverlayPoint.hpp"


/// [UI] Route overlay point (item) / waypoint
/**
 *  This class implements the overlay point (item) for the route overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CRoutePoint: public COverlayPoint
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Waypoint's type
  QString qsType;
  /// Waypoint's description
  QString qsDescription;
  /// Waypoint's comment
  QString qsComment;
  /// Waypoint's symbol
  /** @see CMainWindow::symbolExists(), CMainWindow::symbolPixmap() */
  QString qsSymbol;
  /// Waypoint's URL
  QString qsUrl;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CRoutePoint( const QString& _rqsName );
  CRoutePoint( const QString& _rqsName, const CDataPosition& _roDataPosition );
  virtual ~CRoutePoint() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const;
  virtual void unserialize( QDataStream& _rqDataStream );

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent()->parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter );
  virtual void showDetail();
  virtual void showEdit();


  //------------------------------------------------------------------------------
  // METHODS: COverlayPoint (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual bool matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const;


  //------------------------------------------------------------------------------
  // METHODS: COverlayVisibility (override)
  //------------------------------------------------------------------------------

public:
  virtual void toggleVisible() { COverlayVisibility::toggleVisible( false, true ); };


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Toggles this waypoint's selection status
  void toggleMultiSelected();
  /// Sets this waypoint's type
  void setType( const QString& _rqsType ) { qsType = _rqsType; };
  /// Sets this waypoint's description
  void setDescription( const QString& _rqsDescription ) { qsDescription = _rqsDescription; };
  /// Sets this waypoint's comment
  void setComment( const QString& _rqsComment ) { qsComment = _rqsComment; };
  /// Sets this waypoint's symbol
  void setSymbol( const QString& _rqsSymbol ) { qsSymbol = _rqsSymbol; };
  /// Sets this waypoint's URL
  void setUrl( const QString& _rqsUrl ) { qsUrl = _rqsUrl; };

  // GETTERS
public:
  /// Returns this waypoint's type
  QString getType() const { return qsType; };
  /// Returns this waypoint's description
  QString getDescription() const { return qsDescription; };
  /// Returns this waypoint's comment
  QString getComment() const { return qsComment; };
  /// Returns this waypoint's symbol
  QString getSymbol() const { return qsSymbol; };
  /// Returns this waypoint's URL
  QString getUrl() const { return qsUrl; };

  // OTHER
public:
  /// Retrieves this object's content from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );
  /// Retrieves this object's content from the given GPX source (file)
  void parseGPX( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;
  /// Stores this object's content to the given GPX destination (file)
  void dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter ) const;

};

#endif // QVCT_CROUTEPOINT_HPP
