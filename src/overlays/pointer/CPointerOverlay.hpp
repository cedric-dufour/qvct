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

#ifndef QVCT_CPOINTEROVERLAY_HPP
#define QVCT_CPOINTEROVERLAY_HPP

// QT
#include <QPainter>
#include <QPointF>
#include <QWidget>

// QVCT
#include "overlays/COverlayBase.hpp"
class CChart;
class CDataPosition;
class CPointerContainer;
class CPointerPoint;


/// [UI] Pointer overlay
/**
 *  This class implements the (base) overlay corresponding to pointer overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CPointerOverlay: public COverlayBase
{


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CPointerOverlay();
  virtual ~CPointerOverlay();


  //------------------------------------------------------------------------------
  // METHODS: COverlay (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void drawContent( const CChart* _poChart, QPainter* _pqPainter ) const;
  virtual void showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const;
  virtual void setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const;
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const { return 0; };


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // GETTERS
public:
  /// Returns whether a pointer path has been defined
  bool hasPath() const;
  /// Returns the quantity of segments along the pointer path (0 if no path is defined)
  int getPathSegments() const;

  // USERS
public:
  /// Returns the pointer point corresponding to the mouse pointer
  CPointerPoint* usePointerPoint();
  /// Returns the pointer point corresponding to the target
  CPointerPoint* usePointerTarget();

  // OTHER
public:
  /// Imports settings from the application's global settings
  void importSettings();
  /// Sets the mouse pointer or target at the given screen position
  void setPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition, bool _bTarget = false );
  /// Sets the mouse pointer or targetat the given geographical position
  void setPosition( const CDataPosition& _roDataPosition, bool _bTarget = false );
  /// Clears the mouse pointer or target
  void clearPosition( bool _bTarget = false );
  /// Adds the given screen position to the pointer path
  void setPath( const CChart* _poChart, const QPointF& _rqPointFScrPosition );
  /// Adds the given geographical position to the pointer path
  void setPath( const CDataPosition& _roDataPosition );
  /// Returns the pointer path (rhumb-line) length along its segments
  double getPathLengthRL();
  /// Returns the equivalent (great-circle) length between the first and last position of the pointer path
  double getPathLengthGC();
  /// Clears the pointer path
  void clearPath();

};

#endif // QVCT_CPOINTEROVERLAY_HPP
