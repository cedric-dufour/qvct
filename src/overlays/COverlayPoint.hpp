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

#ifndef QVCT_COVERLAYPOINT_HPP
#define QVCT_COVERLAYPOINT_HPP

// QT
#include <QDataStream>
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>

// QVCT
#include "data/CDataPosition.hpp"
#include "data/CDataTime.hpp"
#include "overlays/COverlayItem.hpp"
#include "overlays/COverlayVisibility.hpp"
class CChart;
class CDataPositionValidity;
class CDataTimeValidity;


/// Generic overlay point
/**
 *  This class defines the generic representation of a point (geographical
 *  position and UTC time) used in an overlay.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayPoint: public COverlayItem, public COverlayVisibility, public CDataTime, public CDataPosition
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  enum ETagPosition { TAG_AUTO, TAG_LEFT, TAG_RIGHT };

public:
  /// Returns the minimal geographical positions box containing the points in the given overlay/container.
  /** @param [in] _pqTreeWidgetItem Overlay/container to search for points
   *  @param [in,out] _poDataPositionLower The lower coordinates of valid points [in] and bounding box [out]
   *  @param [in,out] _poDataPositionUpper The upper coordinates of valid points [in] and bounding box [out]
   *  @param [in] _iIndexMin Minimal index of valid points (within overlay/container)
   *  @param [in] _iIndexMax Maximal index of valid points (within overlay/container)
   *  @return The quantity of valid points in the box */
  static int getPositionBox( const QTreeWidgetItem* _pqTreeWidgetItem,
                             CDataPosition* _poDataPositionLower, CDataPosition* _poDataPositionUpper,
                             int _iIndexMin = 0, int _iIndexMax = -1 );


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayPoint( const QString& _rqsName );
  COverlayPoint( const QString& _rqsName, const CDataPosition& _roDataPosition );
  virtual ~COverlayPoint() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (override/implement)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const;
  virtual void unserialize( QDataStream& _rqDataStream );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Draws the given text, at the given screen position
  virtual void drawText( const CChart* _poChart, QPainter* _pqPainter, const QString& _rqsText, const QPointF& _rqPointFScrPosition, bool _bItalic = false );
  /// Draws a line (between this and the other specified point), including distance/bearing data
  virtual void drawLine( const CChart* _poChart, QPainter* _pqPainter, const COverlayPoint* _poOverlayPoint );
  /// Draws the point (marker)
  virtual void drawMarker( const CChart* _poChart, QPainter* _pqPainter, const CDataPositionValidity* _poDataPositionValidity = 0 );
  /// Draws the symbol (user-defined)
  virtual void drawSymbol( const CChart* _poChart, QPainter* _pqPainter, const QString& _rqsSymbol );
  /// Draws the tag (name)
  virtual void drawTag( const CChart* _poChart, QPainter* _pqPainter, ETagPosition _eTagPosition = TAG_AUTO,
                        const CDataTimeValidity* _poDataTimeValidity = 0, const CDataPositionValidity* _poDataPositionValidity = 0 );
  /// Returns whether this point matches the given screen position
  virtual bool matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const;

};

#endif // QVCT_COVERLAYPOINT_HPP
