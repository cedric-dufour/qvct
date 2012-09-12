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

#ifndef QVCT_CTRACKCONTAINER_HPP
#define QVCT_CTRACKCONTAINER_HPP

// QT
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "data/CDataPosition.hpp"
#include "overlays/COverlayContainer.hpp"
#include "overlays/COverlayVisibility.hpp"
class CChart;
class CTrackOverlay;
class CTrackPoint;
class CTrackSubContainer;


/// [UI] Track overlay container
/**
 *  This class implements the overlay container for the track overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CTrackContainer: public COverlayContainer, public COverlayVisibility
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Track's type
  QString qsType;
  /// Track's description
  QString qsDescription;
  /// Track's comment
  QString qsComment;
  /// Track's URL
  QString qsUrl;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CTrackContainer( const QString& _rqsName );
  virtual ~CTrackContainer();


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
  virtual void setVisibleRouting( bool _bVisibleRouting );
  virtual void toggleVisible();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets this track's type
  void setType( const QString& _rqsType ) { qsType = _rqsType; };
  /// Sets this track's description
  void setDescription( const QString& _rqsDescription ) { qsDescription = _rqsDescription; };
  /// Sets this track's comment
  void setComment( const QString& _rqsComment ) { qsComment = _rqsComment; };
  /// Sets this track's URL
  void setUrl( const QString& _rqsUrl ) { qsUrl = _rqsUrl; };

  // GETTERS
public:
  /// Returns this track's type
  QString getType() const { return qsType; };
  /// Returns this track's description
  QString getDescription() const { return qsDescription; };
  /// Returns this track's comment
  QString getComment() const { return qsComment; };
  /// Returns this track's URL
  QString getUrl() const { return qsUrl; };

  // OTHER
public:
  /// Returns the last/current sub-container of this container
  /** NOTE: A new sub-container will be created if none exists */
  CTrackSubContainer* pickSubContainer();
  /// Add a new sub-container to this container
  /** NOTE: A new sub-container will be created only if none exists or the last sub-container is not empty) */
  CTrackSubContainer* addSubContainer();
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
  /// Deletes selected items within this container
  int deleteSelection();

};

#endif // QVCT_CTRACKCONTAINER_HPP
