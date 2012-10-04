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
#include "overlays/vessel/widget/CVesselWidgetAdaptive.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselWidgetAdaptive::CVesselWidgetAdaptive( const QString& _qsTitle, QWidget* _pqParent )
  : CVesselWidget( _qsTitle, _pqParent )
{
  pqBoxLayout = new QBoxLayout( QBoxLayout::TopToBottom, pqWidget );
  pqBoxLayout->setContentsMargins( 5, 5, 5, 5 );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

void CVesselWidgetAdaptive::resizeEvent( QResizeEvent* _pqResizeEvent )
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

void CVesselWidgetAdaptive::slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea )
{
  if( _qDockWidgetArea == Qt::TopDockWidgetArea || _qDockWidgetArea == Qt::BottomDockWidgetArea )
    pqBoxLayout->setDirection( QBoxLayout::LeftToRight );
  else
    pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}

void CVesselWidgetAdaptive::slotTopLevelChanged( bool _bTopLevel )
{
  if( _bTopLevel ) pqBoxLayout->setDirection( QBoxLayout::TopToBottom );
}
