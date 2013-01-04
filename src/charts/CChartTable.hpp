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
#include <QDomElement> // QtXml module
#include <QGestureEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "data/CDataPosition.hpp"
#include "overlays/COverlayPoint.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
class CChart;
class CChartControl;


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
  friend class CChartControl;

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Flag to track project changes (and trigger user confirmation before discarding unsaved data)
  bool bProjectModified;

  /// Reference chart (geographical) position (shared by all "locked" maps)
  /** @see setGeoPosition(), stepScrPosition(), dragScrPosition(), setPositionHome(), getGeoPosition() */
  CDataPosition oGeoPositionReference;

  /// Reference scale factor (shared by all "locked" maps)
  /** @see setScale(), stepScale(), setScaleActual(), setScaleFit(), setScaleArea(), getScale() */
  double fdScaleReference;

  /// Resolution, in Dots-per-Inch (DPI)
  int iDpi;

  /// Flag that disables position, scale and pointer updates
  //  (used when switching tabs and synchronizing the chart controls)
  bool bIgnoreUpdate;

  /// Mouse position (saved dragging purposes)
  QPointF qPointFMouse;
  /// Mouse-pressed status
  bool bMousePressed;
  /// Mouse-drag status
  bool bMouseDrag;

  /// Keep track of when a gesture event last occured
  double fdGestureTimeLast;
  /// Reference zoom for pinch gesture
  double fdGestureZoomReference;

  /// Pointer target status
  /** @see enablePointerTarget() */
  bool bPointerTarget;
  /// Pointer path status
  /** @see enablePointerPath() */
  bool bPointerPath;
  /// Pointer single-segment path status
  /** @see enablePointerPathSingle() */
  bool bPointerPathSingle;

  /// Overlay point being moved (non-0 only when a move is being performed)
  /** @see setOverlayPointMove(), getOverlayPointMove() */
  COverlayPoint* poOverlayPointMove;

  /// Vessel point to synchronize to
  /** @see setVesselPointSynchronize(), resetVesselPointSynchronize(), getVesselPointSynchronize() */
  CVesselPoint* poVesselPointSynchronize;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChartTable( QWidget* _pqParent );
  virtual ~CChartTable() {};

private:
  /// Constructs the layout of the user-interface
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
  /// [UI:Slot] Slot to load a full project's content from file
  void slotLoad();
  /// [UI:Slot] Slot to save the full project's content to file
  void slotSave();
  /// [UI:Slot] Slot to load a new chart
  void slotLoadChart();
  /// [UI:Slot] Slot to print the currently displayed chart
  void slotPrintChart();
  /// [UI:Slot] Slot to add elevation data to the currently displayed chart
  void slotAddElevation();
  /// [UI:Slot] Slot to handle active tab change
  /** @see QTabWidget::currentChanged() */
  void slotChangeTab( int _iTabIndex );
  /// [UI:Slot] Slot to close the current tab
  void slotCloseTab();
  /// [UI:Slot] Slot to close the requested tab
  /** @see QTabWidget::tabCloseRequested() */
  void slotCloseTab( int _iTabIndex );
  /// [UI:Slot] Slot to trigger scaling to "actual" scale
  void slotScaleActual();
  /// [UI:Slot] Slot to trigger scaling to "fit" scale
  void slotScaleFit();
  /// [UI:Slot] Slot to enable/disable pointer target
  void slotPointerTarget( bool _bEnable );
  /// [UI:Slot] Slot to enable/disable ongoing measurements (pointer path)
  void slotPointerPath( bool _bEnable );
  /// [UI:Slot] Slot to enable/disable single measurement (pointer path)
  void slotPointerPathSingle( bool _bEnable );

  /// [APP:Slot] Slot to handle vessel point destruction
  void slotVesselPointDestroyed( QObject* _pqObject );

  // HANDLERS
private:
  /// Key events handler
  /** @see eventFilter() */
  bool handlerKeyEvent( QKeyEvent* _pqKeyEvent );
  /// Mouse events handler
  /** @see eventFilter() */
  bool handlerMouseEvent( QMouseEvent* _pqMouseEvent );
  /// Wheel events handler
  /** @see eventFilter() */
  bool handlerWheelEvent( QWheelEvent* _pqWheelEvent );
  /// Gesture events handler
  /** @see eventFilter() */
  bool handlerGestureEvent( QGestureEvent* _pqGestureEvent );

  // SETTERS
public:
  /// Sets the status of the project to modified (data have changed and need saving)
  void setProjectModified() { bProjectModified = true; };
  /// Sets the reference chart (geographical) position
  void setGeoPosition( const CDataPosition& _roGeoPosition, bool _bSkipCurrent = false );
  /// Shows the given (geographical) position on chart
  /** NOTE: if the position is not visible, it becomes the new reference chart (geographical) position,
   *  otherwise, the latter is left unchanged.
   *  @see setGeoPosition() */
  void showGeoPosition( const CDataPosition& _roGeoPosition );
  /// Discretely move the reference chart (geographical) position
  void stepScrPosition( bool _bHorizontal, bool _bIncrease, bool _bBigStep = true );
  /// Moves the reference chart (geographical) position according to the given screen offset
  void dragScrPosition( const QPointF& _rqPointFScrPositionOffset );
  /// Sets the reference chart (geographical) position to the current chart's center position
  void setPositionHome();
  /// Sets the position lock/unlock status
  void lockPosition( bool _bLock );
  /// Sets the reference scale factor
  void setScale( double _fdScale, bool _bSkipCurrent = false, bool _bUpdateControl = true );
  /// Discretely increases/decreases the reference scale factor
  void stepScale( bool _bIncrease, bool _bBigStep = true );
  /// Sets the reference scale factor to match the current chart's "actual" scale (1:1 zoom)
  void setScaleActual();
  /// Sets the reference scale factor to match the current chart's "fit" scale (entire chart visible)
  void setScaleFit();
  /// Sets the reference scale factor to display the given (geographical) area (defined by its opposite corners)
  void setScaleArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2, double _fdScaleCorrection = 1.0 );
  /// Sets the scale lock/unlock status
  void lockScale( bool _bLock );

private:
  /// Sets the reference zoom factor
  void setZoom( double _fdZoom, bool _bSkipCurrent = false, bool _bUpdateControl = true );

public:
  /// Enables/disables pointer target
  void enablePointerTarget( bool _bEnable );
  /// Sets the pointer target's position; returns true if is was actually set, false otherwise
  /** NOTE: The pointer target's position will be set only if: 1. is was previously enabled; 2. it has not yet been set.
   *  @see enablePointerTarget() */
  bool setPointerTarget( const CDataPosition& _roGeoPosition );
  /// Enables/disables ongoing measurements (pointer path)
  void enablePointerPath( bool _bEnable );
  /// Enables/disables single measurement (pointer path)
  void enablePointerPathSingle( bool _bEnable );
  /// Extends the pointer path; returns true if is was actually extended, false otherwise
  /** NOTE: The pointer path will be extended only if is was previously enabled.
   *  @see enablePointerPath(), enablePointerSingle() */
  bool extendPointerPath( const CDataPosition& _roGeoPosition );
  /// Sets the overlay point to be moved (and lock all other actions while it is ongoing)
  void setOverlayPointMove( COverlayPoint* _poOverlayPoint );
  /// Sets the vessel point used to synchronize the chart reference (geographical) position
  void setVesselPointSynchronize( CVesselPoint* _poVesselPoint );
  /// Resets (clears) the vessel point used to synchronize the chart reference (geographical) position
  void resetVesselPointSynchronize();

  // GETTERS
public:
  /// Returns whether the project has been modified (data have changed since last saved)
  bool isProjectModified() const { return bProjectModified; };
  /// Returns the reference chart (geographical) position
  CDataPosition getGeoPosition() const { return oGeoPositionReference; };
  /// Returns the reference scale factor
  double getScale() const { return fdScaleReference; };
  /// Returns the overlay point currently being moved (0 if none)
  COverlayPoint* getOverlayPointMove() const { return poOverlayPointMove; };
  /// Returns the vessel defined to synchronize the chart reference (geographical) position
  CVesselPoint* getVesselPointSynchronize() const { return poVesselPointSynchronize; };

  // USERS
public:
  /// Returns the currently displayed chart
  CChart* useChart() { return (CChart*)currentWidget(); };

  // OTHER
public:
  /// Synchronize the reference chart (geographical) position with the designed vessel
  /** @see setVesselPointSynchronize() */
  void synchronizeVesselPoint();

public:
  /// Load a full project's content from the given file
  void load( const QString& _rqsFilename );
  /// Save the full project's content to the given file
  void save( const QString& _rqsFilename ) const;
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;

public:
  /// Load the given chart from file
  CChart* loadChart( const QString& _rqsFilename );
  /// Update the (current) chart content (on screen)
  /** NOTE: This results in the scheduling of a QWidget::paintEvent() by Qt. No guarantees can thus be given about when the actual CChart::draw() will occur. */
  void updateChart();
  /// Add elevation data to the currently displayed chart (if any)
  bool addElevation( const QString& _rqsFilename );
  /// Returns whether the currently selected chart has been associated elevation data (if any)
  bool hasElevation();
  /// Display elevation data (instead of raster data) of the currently selected chart (if any)
  void showElevation( bool _bShow );

private:
  /// Converts the given scale factor to zoom factor (for the current position and - by default - the currently displayed chart)
  double toZoom( double _fdScale, const CChart* _poChart = 0 );
  /// Converts the given zoom factor to scale factor (for the current position and - by default - the currently displayed chart)
  double toScale( double _fdZoom, const CChart* _poChart = 0 );

};

#endif // QVCT_CCHARTTABLE_HPP
