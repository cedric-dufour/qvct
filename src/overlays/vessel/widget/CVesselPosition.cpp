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
#include <QDockWidget>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/widget/CVesselPosition.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPosition::CVesselPosition( QWidget* _pqParent )
  : CVesselWidgetAdaptive( tr("Vessel Position"), _pqParent )
{
  QDockWidget::setObjectName( "VesselPosition" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );
  constructLayout();
  QObject::connect( this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT( slotLocationChanged(Qt::DockWidgetArea) ) );
  QObject::connect( this, SIGNAL(topLevelChanged(bool)), this, SLOT( slotTopLevelChanged(bool) ) );
}

void CVesselPosition::constructLayout()
{
  // Add data
  // ... position
  poTextLongitude = new COverlayText( this );
  poTextLongitude->setToolTip( tr("Longitude") );
  poTextLongitude->setAlignment( Qt::AlignCenter );
  poTextLongitude->resetText();
  pqBoxLayout->addWidget( poTextLongitude );
  poTextLatitude = new COverlayText( this );
  poTextLatitude->setToolTip( tr("Latitude") );
  poTextLatitude->setAlignment( Qt::AlignCenter );
  poTextLatitude->resetText();
  pqBoxLayout->addWidget( poTextLatitude );
  poTextElevation = new COverlayText( this );
  poTextElevation->setToolTip( tr("Elevation") );
  poTextElevation->setAlignment( Qt::AlignCenter );
  poTextElevation->resetText();
  pqBoxLayout->addWidget( poTextElevation );
}


//------------------------------------------------------------------------------
// METHODS: CVesselWidget (implement/override)
//------------------------------------------------------------------------------

void CVesselPosition::refreshContent()
{
  if( !poVesselPoint || !QWidget::isVisible() ) return;

  // ... position
  if( poVesselPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && poVesselPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    poTextLongitude->setText( CUnitPosition::toString( poVesselPoint->getLongitude(), CUnitPosition::LONGITUDE ), !poVesselPoint->isValidPosition() );
    poTextLatitude->setText( CUnitPosition::toString( poVesselPoint->getLatitude(), CUnitPosition::LATITUDE ), !poVesselPoint->isValidPosition() );
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
  }

  // ... elevation
  if( poVesselPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    poTextElevation->setText( CUnitElevation::toString( poVesselPoint->getElevation() ), !poVesselPoint->isValidElevation() );
  else
    poTextElevation->resetText();
}

void CVesselPosition::resetContent()
{
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
}


//------------------------------------------------------------------------------
// METHODS: CVesselWidgetAdaptive (implement/override)
//------------------------------------------------------------------------------

void CVesselPosition::setFont( QFont _qFont )
{
  poTextLongitude->setFont( _qFont );
  poTextLatitude->setFont( _qFont );
  poTextElevation->setFont( _qFont );
}
