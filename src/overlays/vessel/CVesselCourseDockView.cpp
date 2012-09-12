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
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/CVesselCourseDockView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselCourseDockView::CVesselCourseDockView( QWidget* _pqParent )
  : QDockWidget( tr("Vessel Course"), _pqParent )
  , poVesselPoint( 0 )
{
  QDockWidget::setObjectName( "VesselCourse" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );
  constructLayout();
  QObject::connect( this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT( slotLocationChanged(Qt::DockWidgetArea) ) );
  QObject::connect( this, SIGNAL(topLevelChanged(bool)), this, SLOT( slotTopLevelChanged(bool) ) );
}

void CVesselCourseDockView::constructLayout()
{
  // Create holder widget and layout
  pqWidget = new QWidget( this );
  pqWidget->setStyleSheet( ".QWidget { BACKGROUND-COLOR: rgba(0,0,0,255); } .QLabel { COLOR: rgba(255,255,255,255); }" );
  pqBoxLayout = new QBoxLayout( QBoxLayout::TopToBottom, pqWidget );
  pqBoxLayout->setContentsMargins( 5, 5, 5, 5 );

  // Add data
  // ... course
  poTextBearing = new COverlayText( this );
  poTextBearing->setToolTip( tr("Bearing") );
  poTextBearing->setAlignment( Qt::AlignCenter );
  poTextBearing->resetText();
  pqBoxLayout->addWidget( poTextBearing );
  poTextSpeed = new COverlayText( this );
  poTextSpeed->setToolTip( tr("Speed (horizontal)") );
  poTextSpeed->setAlignment( Qt::AlignCenter );
  poTextSpeed->resetText();
  pqBoxLayout->addWidget( poTextSpeed );
  poTextSpeedVertical = new COverlayText( this );
  poTextSpeedVertical->setToolTip( tr("Speed (vertical)") );
  poTextSpeedVertical->setAlignment( Qt::AlignCenter );
  poTextSpeedVertical->resetText();
  pqBoxLayout->addWidget( poTextSpeedVertical );

  // Finalize
  QDockWidget::setWidget( pqWidget );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

void CVesselCourseDockView::resizeEvent( QResizeEvent* _pqResizeEvent )
{
  int __iWidth = pqWidget->width();
  int __iHeight = pqWidget->height();
  int __iFontSize;
  if( pqBoxLayout->direction() == QBoxLayout::LeftToRight ) __iFontSize = std::min( 0.6*__iHeight, 0.05*__iWidth );
  else __iFontSize = std::min( 0.225*__iHeight, 0.15*__iWidth );
  if( __iFontSize < 20 ) __iFontSize = 20;
  QFont __qFontData;
  __qFontData.setPixelSize( __iFontSize );
  __qFontData.setBold( true );
  poTextBearing->setFont( __qFontData );
  poTextSpeed->setFont( __qFontData );
  poTextSpeedVertical->setFont( __qFontData );
  QDockWidget::resizeEvent( _pqResizeEvent );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselCourseDockView::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poVesselPoint != _pqObject ) return;
  resetVesselPoint();
}

void CVesselCourseDockView::slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea )
{
  if( _qDockWidgetArea == Qt::TopDockWidgetArea || _qDockWidgetArea == Qt::BottomDockWidgetArea )
    pqBoxLayout->setDirection( QBoxLayout::LeftToRight );
  else
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

void CVesselCourseDockView::slotTopLevelChanged( bool _bTopLevel )
{
  if( _bTopLevel )
  {
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
    // pqWidget->setMaximumHeight( 150 );
  }
}

//
// SETTERS
//

void CVesselCourseDockView::setVesselPoint( CVesselPoint* _poVesselPoint )
{
  if( poVesselPoint == _poVesselPoint ) return;
  poVesselPoint = _poVesselPoint;
  QObject::connect( poVesselPoint, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
}

void CVesselCourseDockView::resetVesselPoint()
{
  if( poVesselPoint ) QObject::disconnect( (QObject*)poVesselPoint, 0, this, 0 );
  poVesselPoint = 0;
  poTextBearing->resetText();
  poTextSpeed->resetText();
  poTextSpeedVertical->resetText();
}

//
// OTHER
//

void CVesselCourseDockView::refreshContent()
{
  if( !poVesselPoint || !QWidget::isVisible() ) return;
  if( poVesselPoint->GroundCourse.operator!=( CDataCourse::UNDEFINED )
      || poVesselPoint->ApparentCourse.operator!=( CDataCourse::UNDEFINED ) )
  {

    // ... bearing
    {
      QString __qsText;
      bool __bInvalid = false;
      if( poVesselPoint->GroundCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
      {
        __qsText = CUnitBearing::toString( poVesselPoint->GroundCourse.getBearing() );
        __bInvalid = !poVesselPoint->GroundCourseValidity.isValidBearing();
      }
      if( poVesselPoint->ApparentCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
      {
        if( __qsText.isEmpty() ) __qsText += " ";
        __qsText += "("+CUnitBearing::toString( poVesselPoint->ApparentCourse.getBearing() )+")";
        __bInvalid |= !poVesselPoint->ApparentCourseValidity.isValidBearing();
      }
      poTextBearing->setText( __qsText, __bInvalid );
    }

    // ... horizontal speed
    {
      QString __qsText;
      bool __bInvalid = false;
      if( poVesselPoint->GroundCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
      {
        __qsText = CUnitSpeed::toString( poVesselPoint->GroundCourse.getSpeed() );
        __bInvalid = !poVesselPoint->GroundCourseValidity.isValidSpeed();
      }
      if( poVesselPoint->ApparentCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
      {
        if( __qsText.isEmpty() ) __qsText += " ";
        __qsText += "("+CUnitSpeed::toString( poVesselPoint->ApparentCourse.getSpeed() )+")";
        __bInvalid |= !poVesselPoint->ApparentCourseValidity.isValidSpeed();
      }
      poTextSpeed->setText( __qsText, __bInvalid );
    }

    // ... vertical speed
    {
      QString __qsText;
      bool __bInvalid = false;
      if( poVesselPoint->GroundCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
      {
        __qsText = CUnitSpeedVertical::toString( poVesselPoint->GroundCourse.getSpeedVertical() );
        __bInvalid = !poVesselPoint->GroundCourseValidity.isValidSpeedVertical();
      }
      if( poVesselPoint->ApparentCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
      {
        if( __qsText.isEmpty() ) __qsText += " ";
        __qsText += "("+CUnitSpeedVertical::toString( poVesselPoint->ApparentCourse.getSpeedVertical() )+")";
        __bInvalid |= !poVesselPoint->ApparentCourseValidity.isValidSpeedVertical();
      }
      poTextSpeedVertical->setText( __qsText, __bInvalid );
    }

  }
  else
  {
    poTextBearing->resetText();
    poTextSpeed->resetText();
    poTextSpeedVertical->resetText();
  }
}
