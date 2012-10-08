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

#ifndef QVCT_CINSTRUMENTGAHORIZONTALSITUATION_HPP
#define QVCT_CINSTRUMENTGAHORIZONTALSITUATION_HPP

// QT
#include <QTransform>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/instruments/CInstrument.hpp"
#include "units/CUnitElevation.hpp"
class CVesselCockpit;


/// General aviation horizontal situation indicator / HSI (instrument)
/**
 *  This class allows to draw a general aviation (analogic) horizontal situation
 *  indicator (HSI).
 *  The required radial can be adjusted by clicking on the left (decrease) or
 *  right (increase) hand side of the instrument.
 *  It includes the logic that allows to cache the necessary QPixmap(s) required
 *  to draw the instrument and avoid to "re-render" it each time it is "refreshed".
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CInstrumentGAHorizontalSituation: public CInstrument
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------
private:
  /// Pixmap buffer used to render and cache the instrument's (static) background
  QPixmap qPixmapBufferBackground;
  /// Pixmap buffer used to render and cache the instrument's (static) bearing corona
  QPixmap qPixmapBufferBearing;
  /// Pixmap buffer used to render and cache the instrument's (static) target indicator
  QPixmap qPixmapBufferTarget;
  /// Pixmap buffer used to render and cache the instrument's (static) foreground
  QPixmap qPixmapBufferForeground;
  /// Pixmap buffer used to render and cache the instrument
  QPixmap qPixmapBuffer;

  /// Forces the redraw of the instrument (not matter its cache content)
  bool bForceRedraw;
  /// (Vessel) bearing at which the last rendering was achieved
  double fdBearingVessel;
  /// Target (position) for which the last rendering was achieved
  CDataPosition oPositionTarget;

  /// Radial bearing
  double fdBearingRadial;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CInstrumentGAHorizontalSituation( CVesselCockpit* _poVesselCockpit );
  virtual ~CInstrumentGAHorizontalSituation() {};


  //------------------------------------------------------------------------------
  // METHODS: QWidget (implement/override)
  //------------------------------------------------------------------------------

protected:
  virtual void mouseReleaseEvent( QMouseEvent* _pqMouseEvent );
  virtual void mouseDoubleClickEvent( QMouseEvent* _pqMouseEvent );


  //------------------------------------------------------------------------------
  // METHODS: CInstrument (implement/override)
  //------------------------------------------------------------------------------

  // OTHER
private:
  virtual void draw();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
private:
  void drawStatic();
  void drawDynamic( const CVesselPoint* _poVesselPoint, const CPointerPoint* _poPointerPoint );

};

#endif // QVCT_CINSTRUMENTGAHORIZONTALSITUATION_HPP
