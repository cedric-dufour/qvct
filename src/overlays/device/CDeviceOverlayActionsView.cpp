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

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/device/CDeviceOverlayActionsView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceOverlayActionsView::CDeviceOverlayActionsView()
{
  constructLayout();
}

void CDeviceOverlayActionsView::constructLayout()
{
  // Create the buttons
  // ... order invert
  pqPushButtonOrderInvert = new QPushButton( QIcon( ":icons/32x32/order_invert.png" ), "" );
  pqPushButtonOrderInvert->setToolTip( tr("[overlay] Invert order") );
  pqPushButtonOrderInvert->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderInvert, SIGNAL( clicked() ), this, SLOT( slotOrderInvert() ) );
  // ... order ascending
  pqPushButtonOrderAscending = new QPushButton( QIcon( ":icons/32x32/order_ascending.png" ), "" );
  pqPushButtonOrderAscending->setToolTip( tr("[overlay] Sort in ascending order") );
  pqPushButtonOrderAscending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderAscending, SIGNAL( clicked() ), this, SLOT( slotOrderAscending() ) );
  // ... order descending
  pqPushButtonOrderDescending = new QPushButton( QIcon( ":icons/32x32/order_descending.png" ), "" );
  pqPushButtonOrderDescending->setToolTip( tr("[overlay] Sort in descending order") );
  pqPushButtonOrderDescending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderDescending, SIGNAL( clicked() ), this, SLOT( slotOrderDescending() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "" );
  pqPushButtonSave->setToolTip( tr("Save selected devices to disk") );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete selected devices") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderInvert, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderAscending, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderDescending, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSave, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignLeft );

  // Set the layout
  QDialog::setLayout( __pqHBoxLayoutButtons );

}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CDeviceOverlayActionsView::slotOrderInvert()
{
  do // error-catching block
  {
    CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
      {
        int __iHead = 0, __iTail = __pqTreeWidgetItem_current->childCount()-1;
        while( __iTail > __iHead )
        {
          QTreeWidgetItem* __pqTreeWidgetItem_head = __pqTreeWidgetItem_current->child( __iHead );
          QTreeWidgetItem* __pqTreeWidgetItem_tail = __pqTreeWidgetItem_current->child( __iTail );
          __pqTreeWidgetItem_current->removeChild( __pqTreeWidgetItem_head );
          __pqTreeWidgetItem_current->removeChild( __pqTreeWidgetItem_tail );
          __pqTreeWidgetItem_current->insertChild( __iHead++, __pqTreeWidgetItem_tail );
          __pqTreeWidgetItem_current->insertChild( __iTail--, __pqTreeWidgetItem_head );
        }
        QVCTRuntime::useChartTable()->setProjectModified();
      }
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CDeviceOverlayActionsView::slotOrderAscending()
{
  do // error-catching block
  {
    CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
      __pqTreeWidgetItem_current->sortChildren( CDeviceOverlay::NAME, Qt::AscendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CDeviceOverlayActionsView::slotOrderDescending()
{
  do // error-catching block
  {
    CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poDeviceOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
      __pqTreeWidgetItem_current->sortChildren( CDeviceOverlay::NAME, Qt::DescendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CDeviceOverlayActionsView::slotSave()
{
  do // error-catching block
  {
    QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Device"), tr("QVCT Files")+" (*.qvct)" );
    if( __qsFilename.isEmpty() ) break;
    QFileInfo __qFileInfo( __qsFilename );
    if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
    QStringList __qsListExtensions; __qsListExtensions << "qvct";
    if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) break;
    QVCTRuntime::useDeviceOverlay()->save( __qsFilename, 0 ); // no device = save selection
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CDeviceOverlayActionsView::slotDelete()
{
  do // error-catching block
  {
    if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected device(s)") ) ) break;
    QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
    CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
    __pqMutexDataChange->lock();
    __poDeviceOverlay->deleteSelection();
    __pqMutexDataChange->unlock();
    QVCTRuntime::useChartTable()->setProjectModified();
  }
  while( false ); // error-catching block
  QDialog::accept();
}
