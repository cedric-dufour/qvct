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
#include <QFont>
#include <QString>
#include <QWidget>

// QVCT
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/widgets/CVesselWidget.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselWidget::CVesselWidget( const QString& _qsTitle, QWidget* _pqParent )
  : QDockWidget( _qsTitle, _pqParent )
  , poVesselPoint( 0 )
{
  // Create holder widget and layout
  pqWidget = new QWidget();
  pqWidget->setStyleSheet( ".QWidget { BACKGROUND-COLOR: rgba(0,0,0,255); } .QLabel { COLOR: rgba(255,255,255,255); }" );
  pqBoxLayout = new QBoxLayout( QBoxLayout::TopToBottom );
  pqBoxLayout->setContentsMargins( 5, 5, 5, 5 );
  pqWidget->setLayout( pqBoxLayout );
  QDockWidget::setWidget( pqWidget );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

void CVesselWidget::resizeEvent( QResizeEvent* _pqResizeEvent )
{
  int __iWidth = pqWidget->width();
  int __iHeight = pqWidget->height();
  int __iFontSize;
  if( pqBoxLayout->direction() == QBoxLayout::LeftToRight ) __iFontSize = std::min( 0.6*__iHeight, 0.05*__iWidth );
  else __iFontSize = std::min( 0.225*__iHeight, 0.15*__iWidth );
  if( __iFontSize < 20 ) __iFontSize = 20;
  QFont __qFont;
  __qFont.setPixelSize( __iFontSize );
  __qFont.setBold( true );
  setFont( __qFont );
  QDockWidget::resizeEvent( _pqResizeEvent );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselWidget::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poVesselPoint != _pqObject ) return;
  resetVesselPoint();
}

void CVesselWidget::slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea )
{
  if( _qDockWidgetArea == Qt::TopDockWidgetArea || _qDockWidgetArea == Qt::BottomDockWidgetArea )
    pqBoxLayout->setDirection( QBoxLayout::LeftToRight );
  else
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

void CVesselWidget::slotTopLevelChanged( bool _bTopLevel )
{
  if( _bTopLevel ) pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

//
// SETTERS
//

void CVesselWidget::setVesselPoint( CVesselPoint* _poVesselPoint )
{
  if( poVesselPoint == _poVesselPoint ) return;
  poVesselPoint = _poVesselPoint;
  QObject::connect( poVesselPoint, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
}

void CVesselWidget::resetVesselPoint()
{
  if( poVesselPoint ) QObject::disconnect( (QObject*)poVesselPoint, 0, this, 0 );
  poVesselPoint = 0;
  resetContent();
}
