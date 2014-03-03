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
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/CDevice.hpp"
#include "devices/CDeviceCreateView.hpp"
#include "overlays/device/CDeviceOverlayActionsView.hpp"
#include "overlays/device/CDeviceOverlayListView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceOverlayListView::CDeviceOverlayListView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CDeviceOverlayListView::constructLayout()
{
  // Create the overlay/tree
  poDeviceOverlay = new CDeviceOverlay( this );
  QVCTRuntime::registerDeviceOverlay( poDeviceOverlay );

  // Create the buttons
  // ... load
  pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/load.png" ), "", this );
  pqPushButtonLoad->setToolTip( tr("Load device from disk") );
  pqPushButtonLoad->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonLoad, SIGNAL( clicked() ), this, SLOT( slotLoad() ) );
  // ... add
  pqPushButtonAdd = new QPushButton( QIcon( ":icons/32x32/add.png" ), "", this );
  pqPushButtonAdd->setToolTip( tr("Create a new device") );
  pqPushButtonAdd->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonAdd, SIGNAL( clicked() ), this, SLOT( slotAdd() ) );
  // ... up
  pqPushButtonUp = new QPushButton( QIcon( ":icons/32x32/move_up.png" ), "", this );
  pqPushButtonUp->setToolTip( tr("[device] Move up") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/move_down.png" ), "", this );
  pqPushButtonDown->setToolTip( tr("[device] Move down") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );
  // ... actions
  pqPushButtonActions = new QPushButton( QIcon( ":icons/32x32/more.png" ), "", this );
  pqPushButtonActions->setToolTip( tr("Additional actions")+"..." );
  pqPushButtonActions->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonActions, SIGNAL( clicked() ), this, SLOT( slotActions() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add the overlay/tree
  __pqVBoxLayout->addWidget( poDeviceOverlay, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAdd, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonLoad, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonUp, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDown, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonActions, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  QWidget::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CDeviceOverlayListView::slotLoad()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Device"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  CDevice* __poDevice = __poDeviceOverlay->load( __qsFilename );
  if( !__poDevice ) return;
  __poDeviceOverlay->setCurrentItem( __poDevice );
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CDeviceOverlayListView::slotAdd()
{
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  CDevice* __poDevice = 0;
  CDeviceCreateView* __poDeviceCreateView = new CDeviceCreateView( &__poDevice );
  __poDeviceCreateView->exec();
  if( __poDevice )
  {
    __poDeviceOverlay->addChild( __poDevice );
    __poDeviceOverlay->setCurrentItem( __poDevice );
    __poDevice->showEdit();
    QVCTRuntime::useChartTable()->setProjectModified();
  }
  delete __poDeviceCreateView;
}

void CDeviceOverlayListView::slotUp()
{
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex <= 0 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex-1, __pqTreeWidgetItem_current );
      __poDeviceOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}

void CDeviceOverlayListView::slotDown()
{
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex >= __pqTreeWidgetItem_parent->childCount()-1 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex+1, __pqTreeWidgetItem_current );
      __poDeviceOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}

void CDeviceOverlayListView::slotActions()
{
  CDeviceOverlayActionsView* __poDeviceOverlayActionsView = new CDeviceOverlayActionsView();
  __poDeviceOverlayActionsView->exec();
  delete __poDeviceOverlayActionsView;
}
