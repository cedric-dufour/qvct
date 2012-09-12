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
#ifndef QVCT_CCHARTTABLE_HPP
#define QVCT_CCHARTTABLE_HPP

// QT
#include <QTabWidget>
#include <QWidget>

// QVCT
#include "data/CDataPosition.hpp"
#include "overlays/COverlayPoint.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
class CChart;


/// [UI] Virtual "chart table" (view)
/**
 *  This class acts as the virtual "chart table" of the navigator environment,
 *  allowing to load several maps (each in a different tab).
 *
 *  All loaded maps shall share the same current position, in geographical
 *  coordinates corresponding to the center of the map.
 *
 *  Zooming shall be performed according to a logarithmic scaling factor:
 *   1 [pixel] <-> pow( 10, scale ) [meter]
 *  which is more natural for chart handling and which can (optionally) be
 *  shared by all loaded maps.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChartTable: public QTabWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Reference chart (geographical) position, shared by all "locked" maps)
  CDataPosition oGeoPositionReference;

  /// Reference scale factor (shared by all "locked" maps)
  double fdScaleReference;

  /// Resolution, in Dots-per-Inch (DPI)
  int iDpi;

  /// Flag that disables position, scale and pointer path updates
  //  (used when switching tabs and synchronizing the chart controls)
  bool bIgnoreUpdate;

  /// Mouse position (saved dragging purposes)
  QPointF qPointFMouse;
  /// Mouse-pressed status
  bool bMousePressed;
  /// Mouse-drag status
  bool bMouseDrag;

  /// Pointer path status
  bool bPointerPath;
  /// Pointer single-segment path status
  bool bPointerPathSingle;

  /// Overlay point being moved (non-0 only when a move is being performed)
  COverlayPoint* poOverlayPointMove;

  /// Vessel point to synchronize to
  CVesselPoint* poVesselPointSynchronize;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChartTable( QWidget* _pqParent );
  virtual ~CChartTable() {};

private:
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QWidget (override)
  //------------------------------------------------------------------------------

public:
  virtual QSize sizeHint() const;


  //------------------------------------------------------------------------------
  // METHODS: QTabWidget (override)
  //------------------------------------------------------------------------------

private:
  virtual void clear();
  virtual bool eventFilter( QObject* _pqObject, QEvent* _pqEvent );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  void slotLoad();
  void slotSave();
  void slotLoadChart();
  void slotPrintChart();
  void slotChangeTab( int _iTabIndex );
  void slotCloseTab();
  void slotCloseTab( int _iTabIndex );
  void slotScaleActual();
  void slotScaleFit();
  void slotPointerPath( bool _bEnable );
  void slotPointerPathSingle( bool _bEnable );

  // HANDLERS
private:
  bool handlerKeyEvent( QKeyEvent* _pqKeyEvent );
  bool handlerMouseEvent( QMouseEvent* _pqMouseEvent );
  bool handlerWheelEvent( QWheelEvent* _pqWheelEvent );

  // SETTERS
public:
  void setGeoPosition( const CDataPosition& _roGeoPosition, bool _bSkipCurrent = false );
  void stepScrPosition( bool _bHorizontal, bool _bIncrease, bool _bBigStep = true );
  void dragScrPosition( const QPointF& _rqPointFScrPositionOffset );
  void setPositionHome();
  void lockPosition( bool _bLock );
  void setScale( double _fdScale, bool _bSkipCurrent = false, bool _bUpdateControl = true );
  void stepScale( bool _bIncrease, bool _bBigStep = true );
  void setScaleActual();
  void setScaleFit();
  void setScaleArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2, double _fdScaleCorrection = 1.0 );
  void lockScale( bool _bLock );
  void enablePointerPath( bool _bEnable );
  void enablePointerPathSingle( bool _bEnable );
  void setOverlayPointMove( COverlayPoint* _poOverlayPoint );
  void setVesselPointSynchronize( CVesselPoint* _poVesselPoint ) { poVesselPointSynchronize = _poVesselPoint; };

  // GETTERS
public:
  CDataPosition getGeoPosition() const { return oGeoPositionReference; };
  double getScale() const { return fdScaleReference; };
  COverlayPoint* getOverlayPointMove() const { return poOverlayPointMove; };
  CVesselPoint* getVesselPointSynchronize() const { return poVesselPointSynchronize; };

  // USERS
public:
  CChart* useChart() { return (CChart*)currentWidget(); };

  // OTHER
public:
  void synchronizeVesselPoint();

public:
  void load( const QString& _rqsFilename );
  void save( const QString& _rqsFilename ) const;
  void loadChart( const QString& _rqsFilename );
  void updateChart();

protected:
  double toZoom( double _fdScale, const CChart* _poChart = 0 );
  double toScale( double _fdZoom, const CChart* _poChart = 0 );

};

#endif // QVCT_CCHARTTABLE_HPP
