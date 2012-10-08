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
#include <QGridLayout>
#include <QDockWidget>
#include <QPalette>
#include <QPointF>
#include <QSizeF>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/vessel/widgets/CVesselCockpitGeneralAviation.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGAAltimeter.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGAArtificialHorizon.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGAChronograph.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGAHorizontalSituation.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGASpeed.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGASpeedVertical.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselCockpitGeneralAviation::CVesselCockpitGeneralAviation( QWidget* _pqParent )
  : CVesselCockpit( "QVCT - "+tr("General Aviation Cockpit"), _pqParent )
{
  QPalette __qPalette( QWidget::palette() );
  __qPalette.setColor( QPalette::Background, Qt::black );
  QWidget::setPalette( __qPalette );
  QWidget::setAutoFillBackground(true);
  constructLayout();
  QWidget::resize( 600, 400 );
}

void CVesselCockpitGeneralAviation::constructLayout()
{
  // Add instruments
  poSpeed = new CInstrumentGASpeed( this );
  pqGridLayout->addWidget( poSpeed, 1, 1 );
  poArtificialHorizon = new CInstrumentGAArtificialHorizon( this );
  pqGridLayout->addWidget( poArtificialHorizon, 1, 2 );
  poAltimeter = new CInstrumentGAAltimeter( this );
  pqGridLayout->addWidget( poAltimeter, 1, 3 );
  poChronograph = new CInstrumentGAChronograph( this );
  pqGridLayout->addWidget( poChronograph, 2, 1 );
  poHorizontalSituation = new CInstrumentGAHorizontalSituation( this );
  pqGridLayout->addWidget( poHorizontalSituation, 2, 2 );
  poSpeedVertical = new CInstrumentGASpeedVertical( this );
  pqGridLayout->addWidget( poSpeedVertical, 2, 3 );
}

//------------------------------------------------------------------------------
// METHODS: CVesselWidget (implement/override)
//------------------------------------------------------------------------------

void CVesselCockpitGeneralAviation::refreshContent()
{
  if( !QWidget::isVisible() ) return;
  if( poVesselPoint )
  {
    poSpeed->update();
    poArtificialHorizon->update();
    poAltimeter->update();
    poHorizontalSituation->update();
    poSpeedVertical->update();
  }
  poChronograph->update();
}

void CVesselCockpitGeneralAviation::resetContent()
{
  poSpeed->update();
  poArtificialHorizon->update();
  poAltimeter->update();
  poChronograph->update();
  poHorizontalSituation->update();
  poSpeedVertical->update();
}
