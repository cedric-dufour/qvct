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

#ifndef QVCT_CINSTRUMENTGAARTIFICIALHORIZON_HPP
#define QVCT_CINSTRUMENTGAARTIFICIALHORIZON_HPP

// QT
#include <QTransform>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/instruments/CInstrument.hpp"
#include "units/CUnitElevation.hpp"
class CVesselCockpit;


/// General aviation artificial horizon / AH (instrument)
/**
 *  This class allows to draw a general aviation (analogic) artificial horizon
 *  (AH).
 *  The pitch and roll angles are artificially computed based on the vertical
 *  vs. horizontal speeds (pitch) and the rate of turn vs. horizontal speed
 * (roll).
 *  It includes the logic that allows to cache the necessary QPixmap(s) required
 *  to draw the instrument and avoid to "re-render" it each time it is "refreshed".
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CInstrumentGAArtificialHorizon: public CInstrument
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------
private:
  /// Pixmap buffer used to render and cache the instrument's (static) background
  QPixmap qPixmapBufferBackground;
  /// Pixmap buffer used to render and cache the instrument's (static) foreground
  QPixmap qPixmapBufferForeground;
  /// Pixmap buffer used to render and cache the instrument
  QPixmap qPixmapBuffer;

  /// Forces the redraw of the instrument (not matter its cache content)
  bool bForceRedraw;
  /// Pitch (angle) at which the last rendering was achieved
  double fdPitch;
  /// Roll (angle) at which the last rendering was achieved
  double fdRoll;
  /// Bearing at which the last rendering was achieved
  double fdBearing;
  /// Time at which the last rendering was achieved
  double fdTime;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CInstrumentGAArtificialHorizon( CVesselCockpit* _poVesselCockpit );
  virtual ~CInstrumentGAArtificialHorizon() {};


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
  void drawDynamic( const CVesselPoint* _poVesselPoint, double _fdPitch, double _fdRoll );

};

#endif // QVCT_CINSTRUMENTGAARTIFICIALHORIZON_HPP
