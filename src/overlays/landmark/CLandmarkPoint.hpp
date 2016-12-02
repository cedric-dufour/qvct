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

#ifndef QVCT_CLANDMARKPOINT_HPP
#define QVCT_CLANDMARKPOINT_HPP

// QT
#include <QDataStream>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "overlays/COverlayPoint.hpp"


/// [UI] Landmark overlay point (item)
/**
 *  This class implements the overlay point (item) for the landmark overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CLandmarkPoint: public COverlayPoint
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Landmark's type
  QString qsType;
  /// Landmark's description
  QString qsDescription;
  /// Landmark's comment
  QString qsComment;
  /// Landmark's symbol
  /** @see CMainWindow::symbolExists(), CMainWindow::symbolPixmap() */
  QString qsSymbol;
  /// Landmark's URL
  QString qsUrl;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CLandmarkPoint( const QString& _rqsName );
  CLandmarkPoint( const QString& _rqsName, const CDataPosition& _roDataPosition );
  virtual ~CLandmarkPoint() {};


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
  virtual void toggleVisibility() { COverlayVisibility::toggleVisibility( false, false ); };


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Toggles this landmark's selection status
  void toggleMultiSelected();
  /// Sets this landmark's type
  void setType( const QString& _rqsType ) { qsType = _rqsType; };
  /// Sets this landmark's description
  void setDescription( const QString& _rqsDescription ) { qsDescription = _rqsDescription; };
  /// Sets this landmark's comment
  void setComment( const QString& _rqsComment ) { qsComment = _rqsComment; };
  /// Sets this landmark's symbol
  void setSymbol( const QString& _rqsSymbol ) { qsSymbol = _rqsSymbol; };
  /// Sets this landmark's URL
  void setUrl( const QString& _rqsUrl ) { qsUrl = _rqsUrl; };

  // GETTERS
public:
  /// Returns this landmark's type
  QString getType() const { return qsType; };
  /// Returns this landmark's description
  QString getDescription() const { return qsDescription; };
  /// Returns this landmark's comment
  QString getComment() const { return qsComment; };
  /// Returns this landmark's symbol
  QString getSymbol() const { return qsSymbol; };
  /// Returns this landmark's URL
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

protected:
  using COverlayPoint::toggleVisibility;
};

#endif // QVCT_CLANDMARKPOINT_HPP
