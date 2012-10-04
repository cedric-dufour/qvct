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

// C/C++
#include <cmath>

// QT
#include <QDateTime>
#include <QDockWidget>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/widget/CVesselTarget.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselTarget::CVesselTarget( QWidget* _pqParent )
  : CVesselWidgetAdaptive( tr("Target Course"), _pqParent )
  , bContentDisplayed( false )
{
  QDockWidget::setObjectName( "VesselTarget" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );
  constructLayout();
  QObject::connect( this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT( slotLocationChanged(Qt::DockWidgetArea) ) );
  QObject::connect( this, SIGNAL(topLevelChanged(bool)), this, SLOT( slotTopLevelChanged(bool) ) );
}

void CVesselTarget::constructLayout()
{
  // Add data
  // ... target
  poTextBearing = new COverlayText( this );
  poTextBearing->setToolTip( tr("Bearing") );
  poTextBearing->setAlignment( Qt::AlignCenter );
  poTextBearing->resetText();
  pqBoxLayout->addWidget( poTextBearing );
  poTextDistance = new COverlayText( this );
  poTextDistance->setToolTip( tr("Distance") );
  poTextDistance->setAlignment( Qt::AlignCenter );
  poTextDistance->resetText();
  pqBoxLayout->addWidget( poTextDistance );
  poTextEte = new COverlayText( this );
  poTextEte->setToolTip( tr("Estimated Time En-Route (ETE)") );
  poTextEte->setAlignment( Qt::AlignCenter );
  poTextEte->resetText();
  pqBoxLayout->addWidget( poTextEte );
  poTextEta = new COverlayText( this );
  poTextEta->setToolTip( tr("Estimated Time of Arrival (ETA)") );
  poTextEta->setAlignment( Qt::AlignCenter );
  poTextEta->resetText();
  pqBoxLayout->addWidget( poTextEta );
}


//------------------------------------------------------------------------------
// METHODS: CVesselWidget (implement/override)
//------------------------------------------------------------------------------

void CVesselTarget::refreshContent()
{
  if( !poVesselPoint || !QWidget::isVisible() ) return;
  CPointerPoint* __poPointerPoint = QVCTRuntime::usePointerOverlay()->usePointerPoint( true );
  if( __poPointerPoint->CDataPosition::operator==( CDataPosition::UNDEFINED ) )
  {
    if( bContentDisplayed ) resetContent();
    return;
  }

  // ... target
  if( poVesselPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && poVesselPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    bContentDisplayed = true;
    // ... bearing
    double __fdBearingTarget = CDataPosition::bearingRL( *poVesselPoint, *__poPointerPoint );
    poTextBearing->setText( CUnitBearing::toString( __fdBearingTarget ), !poVesselPoint->isValidPosition() );
    // ... distance
    double __fdDistance = CDataPosition::distanceRL( *poVesselPoint, *__poPointerPoint );
    poTextDistance->setText( CUnitDistance::toString( __fdDistance ), !poVesselPoint->isValidPosition() );
    // ... ETE/ETA
    bool __bTimeValid = false;
    do // processing-breaking loop
    {
      double __fdBearingCourse = poVesselPoint->GroundCourse.getBearing();
      if( __fdBearingCourse == CDataCourse::UNDEFINED_BEARING ) break;
      double __fdSpeed = poVesselPoint->GroundCourse.getSpeed();
      if( __fdSpeed == __fdSpeed == CDataCourse::UNDEFINED_SPEED ) break;
      double __fdSpeedCosine = __fdSpeed * cos( ( __fdBearingCourse - __fdBearingTarget ) * 0.01745329 );
      if( __fdSpeedCosine <= 0 ) break; // we can't get any ETE/ETA if we're moving away from the target (or not moving at all)
      double __fdDuration = __fdDistance / __fdSpeedCosine;
      if( __fdDuration > 86400 ) break; // if the ETE is more than 24 hours, let's assume it's not worth displaying it
      QDateTime __qDateTime = QDateTime::currentDateTime();
      double __fdTime = __qDateTime.toUTC().toTime_t()+__qDateTime.time().msec()/1000.0;
      __bTimeValid = true;
      poTextEte->setText( CUnitTimeDelta::toString( __fdDuration ), !poVesselPoint->isValidPosition() || !poVesselPoint->GroundCourseValidity.isValidBearing() || !poVesselPoint->GroundCourseValidity.isValidSpeed() );
      poTextEta->setText( CUnitTime::toString( __fdTime+__fdDuration ), !poVesselPoint->isValidPosition() || !poVesselPoint->GroundCourseValidity.isValidBearing() || !poVesselPoint->GroundCourseValidity.isValidSpeed() );
    }
    while( false ); // processing-breaking loop
    if( !__bTimeValid )
    {
      poTextEte->resetText();
      poTextEta->resetText();
    }
  }
  else
    resetContent();
}

void CVesselTarget::resetContent()
{
  bContentDisplayed = false;
  poTextBearing->resetText();
  poTextDistance->resetText();
  poTextEte->resetText();
  poTextEta->resetText();
}


//------------------------------------------------------------------------------
// METHODS: CVesselWidgetAdaptive (implement/override)
//------------------------------------------------------------------------------

void CVesselTarget::setFont( QFont _qFont )
{
  poTextBearing->setFont( _qFont );
  poTextDistance->setFont( _qFont );
  poTextEte->setFont( _qFont );
  poTextEta->setFont( _qFont );
}
