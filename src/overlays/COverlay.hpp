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

#ifndef QVCT_COVERLAY_HPP
#define QVCT_COVERLAY_HPP

// QT
#include <QPainter>
#include <QTreeWidgetItem>
#include <QWidget>

// QVCT
class CChart;
class COverlayPoint;


/// Generic overlay
/**
 *  This class defines the generic representation of a chart overlay.
 *  A chart overlay is a set of items that may be displayed on top of a chart,
 *  such as the mouse pointer, landmarks, routes, tracks, vessels, etc.
 *  This class already implements the logic that allows to cache the necessary
 *  QPixmap used to render the overlay graphical content on the currently
 *  displayed chart and avoid to "re-render" it each time an another overlay is
 *  modified and requires the CChart widget to be redrawn.
 *  @see CChart, CChartTable
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlay: public QTreeWidgetItem
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------
protected:
  /// Overlay name
  QString qsName;

  /// Overlay items' global visibility status
  /** @see setVisible(), isVisible() */
  bool bVisible;

  /// Pixmap buffer used to render and cache this overlay's graphical content
  QPixmap qPixmapBuffer;
  /// Chart for which the last rendering was achieved
  const CChart* poChartPixmap;
  /// Chart position at which the last rendering was achieved
  QPointF qPointFDatPositionPixmap;
  /// Zoom factor at which the last rendering was achieved
  double fdZoomPixmap;
  /// Forces this overlay's rendering (not matter its cache content)
  bool bForceRedraw;

  /// QFont used to draw text on this overlay
  QFont qFont;
  /// QBrush used to draw text on this overlay
  /** @see COverlayPoint::drawTag(), COverlayPoint::drawLine() */
  QBrush qBrushText;
  /// QPen used to draw text on this overlay
  /** @see COverlayPoint::drawTag(), COverlayPoint::drawLine() */
  QPen qPenText;
  /// QBrush used to draw markers on this overlay
  /** @see COverlayPoint::drawMarker() */
  QBrush qBrushMarker;
  /// QPen used to draw markers on this overlay
  /** @see COverlayPoint::drawMarker() */
  QPen qPenMarker;
  /// QPen used to draw lines on this overlay
  /** @see COverlayPoint::drawLine() */
  QPen qPenLine;
  /// QPen used to draw vectors on this overlay
  /** @see COverlayCourse::drawVector() */
  QPen qPenVector;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlay( const QString& _rqsName );
  virtual ~COverlay() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets this overlay items' global visibility status
  void setVisible( bool _bVisible ) { bVisible = _bVisible; };
  /// Forces this overlay's rendering (not matter its cache content)
  void forceRedraw() { bForceRedraw = true; };

  // GETTERS
public:
  /// Returns this overlay items' global visibility status
  bool isVisible() const { return bVisible; };
  bool isRedrawForced() const { return bForceRedraw; };
  const QFont& getFont() const { return qFont; };
  const QBrush& getBrushText() const { return qBrushText; };
  const QPen& getPenText() const { return qPenText; };
  const QBrush& getBrushMarker() const { return qBrushMarker; };
  const QPen& getPenMarker() const { return qPenMarker; };
  const QPen& getPenLine() const { return qPenLine; };
  const QPen& getPenVector() const { return qPenVector; };

  // OTHER
public:
  /// Draws this overlay
  void draw( const CChart* _poChart, QPainter* _pqPainter );
  /// Draws this overlay's content
  virtual void drawContent( const CChart* _poChart, QPainter* _pqPainter ) const = 0;
  /// Displays the given overlay object's details (in the appropriate widget/view)
  virtual void showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const = 0;
  /// Centers the chart on the given overlay object's position
  virtual void setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const = 0;
  /// Returns the overlay's point that (first) matches the given screen position (0 if none is found)
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const = 0;

public:
  /// Returns a valid name for a new sibling of this object
  /** This method checks and returns a valid name for a new sibling of this
   *  object, such as no duplicate is created. If the given name matches an
   *  existing sibling, it is automatically suffixed with a number, prefixed
   *  with the given quantity of "0" (e.g. "Object(01)") */
  QString newChildName( const QString& _rqsName, int __iZeroPrefix = 0, bool __bForceSuffix = false ) const;

};

#endif // QVCT_COVERLAY_HPP
