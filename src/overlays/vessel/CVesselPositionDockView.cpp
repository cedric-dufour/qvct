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
#include "overlays/vessel/CVesselPositionDockView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPositionDockView::CVesselPositionDockView( QWidget* _pqParent )
  : QDockWidget( tr("Vessel Position"), _pqParent )
  , poVesselPoint( 0 )
{
  QDockWidget::setObjectName( "VesselPosition" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );
  constructLayout();
  QObject::connect( this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT( slotLocationChanged(Qt::DockWidgetArea) ) );
  QObject::connect( this, SIGNAL(topLevelChanged(bool)), this, SLOT( slotTopLevelChanged(bool) ) );
}

void CVesselPositionDockView::constructLayout()
{
  // Create holder widget and layout
  pqWidget = new QWidget( this );
  pqWidget->setStyleSheet( ".QWidget { BACKGROUND-COLOR: rgba(0,0,0,255); } .QLabel { COLOR: rgba(255,255,255,255); }" );
  pqBoxLayout = new QBoxLayout( QBoxLayout::TopToBottom, pqWidget );
  pqBoxLayout->setContentsMargins( 5, 5, 5, 5 );

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

  // Finalize
  QDockWidget::setWidget( pqWidget );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

void CVesselPositionDockView::resizeEvent( QResizeEvent* _pqResizeEvent )
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
  poTextLongitude->setFont( __qFontData );
  poTextLatitude->setFont( __qFontData );
  poTextElevation->setFont( __qFontData );
  QDockWidget::resizeEvent( _pqResizeEvent );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselPositionDockView::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poVesselPoint != _pqObject ) return;
  resetVesselPoint();
}

void CVesselPositionDockView::slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea )
{
  if( _qDockWidgetArea == Qt::TopDockWidgetArea || _qDockWidgetArea == Qt::BottomDockWidgetArea )
    pqBoxLayout->setDirection( QBoxLayout::LeftToRight );
  else
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

void CVesselPositionDockView::slotTopLevelChanged( bool _bTopLevel )
{
  if( _bTopLevel ) pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

//
// SETTERS
//

void CVesselPositionDockView::setVesselPoint( CVesselPoint* _poVesselPoint )
{
  if( poVesselPoint == _poVesselPoint ) return;
  poVesselPoint = _poVesselPoint;
  QObject::connect( poVesselPoint, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
}

void CVesselPositionDockView::resetVesselPoint()
{
  if( poVesselPoint ) QObject::disconnect( (QObject*)poVesselPoint, 0, this, 0 );
  poVesselPoint = 0;
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
}

//
// OTHER
//

void CVesselPositionDockView::refreshContent()
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
