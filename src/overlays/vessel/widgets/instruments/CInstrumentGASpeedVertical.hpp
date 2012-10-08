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

#ifndef QVCT_CINSTRUMENTGASPEEDVERTICAL_HPP
#define QVCT_CINSTRUMENTGASPEEDVERTICAL_HPP

// QT
#include <QTransform>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/instruments/CInstrument.hpp"
#include "units/CUnitSpeedVertical.hpp"
class CVesselCockpit;


/// General aviation vertical speed (instrument)
/**
 *  This class allows to draw a general aviation (analogic) speedVertical.
 *  It includes the logic that allows to cache the necessary QPixmap(s) required
 *  to draw the instrument and avoid to "re-render" it each time it is "refreshed".
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CInstrumentGASpeedVertical: public CInstrument
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
  CUnitSpeedVertical::EUnit eUnit;
  /// Vertical speed at which the last rendering was achieved
  double fdSpeedVertical;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CInstrumentGASpeedVertical( CVesselCockpit* _poVesselCockpit );
  virtual ~CInstrumentGASpeedVertical() {};


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
  void drawStatic( CUnitSpeedVertical::EUnit _eUnit );
  void drawDynamic( CUnitSpeedVertical::EUnit _eUnit, const CVesselPoint* _poVesselPoint );

};

#endif // QVCT_CINSTRUMENTGASPEEDVERTICAL_HPP
