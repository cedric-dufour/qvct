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
#include <QFileInfo>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/CDevice.hpp"
#include "devices/CDeviceCreateView.hpp"
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
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "", this );
  pqPushButtonSave->setToolTip( tr("Save selected devices to disk") );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete selected devices") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... up
  pqPushButtonUp = new QPushButton( QIcon( ":icons/32x32/sort_ascending.png" ), "", this );
  pqPushButtonUp->setToolTip( tr("[device] Move up; [overlay] Sort in ascending order") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/sort_descending.png" ), "", this );
  pqPushButtonDown->setToolTip( tr("[device] Move down; [overlay] Sort in descending order") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );

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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonLoad, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAdd, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSave, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonUp, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDown, 0, Qt::AlignRight );
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
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  CDevice* __poDevice = __poDeviceOverlay->load( __qsFilename );
  if( !__poDevice ) return;
  __poDeviceOverlay->setCurrentItem( __poDevice );
  __pqMutexDataChange->lock();
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
  }
  delete __poDeviceCreateView;
}

void CDeviceOverlayListView::slotSave()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Device"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QFileInfo __qFileInfo( __qsFilename );
  if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
  QStringList __qsListExtensions; __qsListExtensions << "qvct";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useDeviceOverlay()->save( __qsFilename, 0 ); // no device = save selection
}

void CDeviceOverlayListView::slotDelete()
{
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected device(s)") ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  __poDeviceOverlay->deleteSelection();
  __pqMutexDataChange->unlock();
}

void CDeviceOverlayListView::slotUp()
{
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CDeviceOverlay::NAME, Qt::AscendingOrder );
    break;

  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex <= 0 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex-1, __pqTreeWidgetItem_current );
      __poDeviceOverlay->setCurrentItem( __pqTreeWidgetItem_current );
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

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CDeviceOverlay::NAME, Qt::DescendingOrder );
    break;

  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex >= __pqTreeWidgetItem_parent->childCount()-1 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex+1, __pqTreeWidgetItem_current );
      __poDeviceOverlay->setCurrentItem( __pqTreeWidgetItem_current );
    }
    break;

  default:;

  }
}
