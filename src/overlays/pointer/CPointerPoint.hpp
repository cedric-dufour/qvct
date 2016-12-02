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

#ifndef QVCT_CPOINTERPOINT_HPP
#define QVCT_CPOINTERPOINT_HPP

// QT
#include <QPainter>
#include <QPointF>

// QVCT
#include "overlays/COverlayPoint.hpp"


/// [UI] Pointer overlay point (item)
/**
 *  This class implements the overlay point (item) for the pointer overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CPointerPoint: public COverlayPoint
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CPointerPoint( const QString& _rqsName );
  virtual ~CPointerPoint() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter );
  virtual void showDetail();
  virtual void showEdit() {}; // NOTE: does not apply to pointer


  //------------------------------------------------------------------------------
  // METHODS: COverlayVisibility (override)
  //------------------------------------------------------------------------------

public:
  virtual void toggleVisibility() { COverlayVisibility::toggleVisibility( false, false ); };

protected:
  using COverlayVisibility::toggleVisibility;
};

#endif // QVCT_CPOINTERPOINT_HPP
