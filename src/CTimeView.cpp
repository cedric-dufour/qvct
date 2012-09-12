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
#include <QBoxLayout>
#include <QDateTime>
#include <QDockWidget>
#include <QLabel>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "CTimeView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTimeView::CTimeView( QWidget* _pqParent )
  : QDockWidget( tr("System Time"), _pqParent )
{
  QDockWidget::setObjectName( "SystemTime" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );
  constructLayout();
  QObject::connect( this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT( slotLocationChanged(Qt::DockWidgetArea) ) );
  QObject::connect( this, SIGNAL(topLevelChanged(bool)), this, SLOT( slotTopLevelChanged(bool) ) );
}

void CTimeView::constructLayout()
{
  // Create holder widget and layout
  pqWidget = new QWidget( this );
  pqWidget->setStyleSheet( ".QWidget { BACKGROUND-COLOR: rgba(0,0,0,255); } .QLabel { COLOR: rgba(255,255,255,255); }" );
  pqBoxLayout = new QBoxLayout( QBoxLayout::LeftToRight, pqWidget );
  pqBoxLayout->setContentsMargins( 5, 5, 5, 5 );

  // Add data
  // ... date/time
  pqLabelDate = new QLabel( this );
  pqLabelDate->setToolTip( tr("Date") );
  pqLabelDate->setAlignment( Qt::AlignCenter );
  pqBoxLayout->addWidget( pqLabelDate );
  pqLabelTime = new QLabel( this );
  pqLabelTime->setToolTip( tr("Time") );
  pqLabelTime->setAlignment( Qt::AlignCenter );
  pqBoxLayout->addWidget( pqLabelTime );
  pqLabelTime2 = new QLabel( this );
  pqLabelTime2->setToolTip( tr("Time (alternate timezone)") );
  pqLabelTime2->setAlignment( Qt::AlignCenter );
  pqBoxLayout->addWidget( pqLabelTime2 );

  // Finalize
  QDockWidget::setWidget( pqWidget );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

void CTimeView::resizeEvent( QResizeEvent* _pqResizeEvent )
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
  pqLabelDate->setFont( __qFontData );
  pqLabelTime->setFont( __qFontData );
  pqLabelTime2->setFont( __qFontData );
  QDockWidget::resizeEvent( _pqResizeEvent );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CTimeView::slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea )
{
  if( _qDockWidgetArea == Qt::TopDockWidgetArea || _qDockWidgetArea == Qt::BottomDockWidgetArea )
  {
    pqBoxLayout->setDirection( QBoxLayout::LeftToRight );
    // pqWidget->setMaximumHeight( 50 );
  }
  else
  {
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
    // pqWidget->setMaximumHeight( 150 );
  }
}

void CTimeView::slotTopLevelChanged( bool _bTopLevel )
{
  if( _bTopLevel )
  {
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
    // pqWidget->setMaximumHeight( 150 );
  }
}

//
// OTHER
//

void CTimeView::refreshContent()
{
  if( !QWidget::isVisible() ) return;
  QDateTime __qDateTime = QDateTime::currentDateTime();
  double __fdTime = __qDateTime.toUTC().toTime_t()+__qDateTime.time().msec()/1000.0;
  pqLabelDate->setText( CUnitDate::toString( __fdTime ) );
  pqLabelTime->setText( CUnitTime::toString( __fdTime ) );
  pqLabelTime2->setText( "("+CUnitTime::toString( __fdTime, true )+")" );
}
