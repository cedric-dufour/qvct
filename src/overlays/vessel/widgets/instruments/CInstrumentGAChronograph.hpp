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

#ifndef QVCT_CINSTRUMENTGACHRONOGRAPH_HPP
#define QVCT_CINSTRUMENTGACHRONOGRAPH_HPP

// QT
#include <QTransform>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/instruments/CInstrument.hpp"
#include "units/CUnitTimeZone.hpp"
class CVesselCockpit;


/// General aviation chronograph (instrument)
/**
 *  This class allows to draw a general aviation (analogic) chronograph.
 *  It also include two stop-watches that can be controlled by (double-)clicking
 *  on their respective hand-side (of the instrument).
 *  It includes the logic that allows to cache the necessary QPixmap(s) required
 *  to draw the instrument and avoid to "re-render" it each time it is "refreshed".
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CInstrumentGAChronograph: public CInstrument
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------
private:
  /// Pixmap buffer used to render and cache the instrument's (static) background
  QPixmap qPixmapBufferBackground;
  /// Pixmap buffer used to render and cache the instrument
  QPixmap qPixmapBuffer;

  /// Forces the redraw of the instrument (not matter its cache content)
  bool bForceRedraw;
  /// Unit for which the last rendering was achieved
  CUnitTimeZone::EUnit eUnit;
  /// Time at which the last rendering was achieved
  double fdTime;

  /// Stop-watch 1 last started time (stopped if 0)
  double fdSW1TimeStarted;
  /// Stop-watch 1 accumulated time
  double fdSW1TimeAccumulated;
  /// Stop-watch 2 last started time (stopped if 0)
  double fdSW2TimeStarted;
  /// Stop-watch 2 accumulated time
  double fdSW2TimeAccumulated;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CInstrumentGAChronograph( CVesselCockpit* _poVesselCockpit );
  virtual ~CInstrumentGAChronograph() {};


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
  void drawStatic( CUnitTimeZone::EUnit _eUnit );
  void drawDynamic( CUnitTimeZone::EUnit _eUnit, double _fdTime );

};

#endif // QVCT_CINSTRUMENTGACHRONOGRAPH_HPP
