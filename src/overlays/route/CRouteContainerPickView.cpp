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
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

// QVCT
#include "overlays/route/CRouteContainerPickView.hpp"
#include "overlays/route/CRouteOverlay.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteContainerPickView::CRouteContainerPickView( const CRouteOverlay* _poRouteOverlay, COverlayObject** _ppoOverlayObject )
  : COverlayObjectPickView( _poRouteOverlay, _ppoOverlayObject )
{
  constructLayout();
}

void CRouteContainerPickView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add icon and data
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  // ... icon
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/route.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0 );
  // ... picker
  pqComboBox = new QComboBox();
  int __iCount = COverlayObjectPickView::pqTreeWidgetItem->childCount();
  for( int __i = 0; __i < __iCount; __i++ ) pqComboBox->addItem( ((COverlayObject*)COverlayObjectPickView::pqTreeWidgetItem->child( __i ))->getName() );
  __pqHBoxLayoutHeader->addWidget( pqComboBox, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Ok, Qt::Horizontal );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  COverlayObjectPickView::setLayout( __pqVBoxLayout );

}
