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

#ifndef QVCT_CINSTRUMENT_HPP
#define QVCT_CINSTRUMENT_HPP

// QT
#include <QPixmap>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/CVesselCockpit.hpp"
class CVesselPoint;


/// Generic instrument
/**
 *  This class defines the base object required to draw a navigation instrument.
 *  It includes the logic that allows to cache the necessary QPixmap(s) required
 *  to draw the instrument and avoid to "re-render" it each time it is "refreshed".
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CInstrument: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CInstrument( CVesselCockpit* _poVesselCockpit );
  virtual ~CInstrument() {};


  //------------------------------------------------------------------------------
  // METHODS: QWidget (override)
  //------------------------------------------------------------------------------

protected:
  virtual void paintEvent( QPaintEvent* _pqPaintEvent );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // GETTER
protected:
  /// Returns parent cockpit's associated vessel (0 if the vessel is undefined)
  CVesselPoint* useVesselPoint();

  // OTHER
protected:
  /// Draws the instrument (resets if the vessel is undefined)
  virtual void draw() = 0;

};

#endif // QVCT_CINSTRUMENT_HPP
