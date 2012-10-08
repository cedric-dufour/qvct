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

// QT
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/vessel/widgets/instruments/CInstrument.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrument::CInstrument( CVesselCockpit* _poVesselCockpit )
  : QWidget( _poVesselCockpit )
{}


//------------------------------------------------------------------------------
// METHODS: QWidget (OVERRIDE)
//------------------------------------------------------------------------------

void CInstrument::paintEvent( QPaintEvent* _pqPaintEvent )
{
  draw();
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTER
//

CVesselPoint* CInstrument::useVesselPoint()
{
  return ((CVesselCockpit*)QWidget::parent())->poVesselPoint;
}
