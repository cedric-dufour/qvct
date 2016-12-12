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
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/CVesselOverlayActionsView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselOverlayActionsView::CVesselOverlayActionsView()
{
  constructLayout();
}

void CVesselOverlayActionsView::constructLayout()
{
  // Create the buttons
  // ... order invert
  pqPushButtonOrderInvert = new QPushButton( QIcon( ":icons/32x32/order_invert.png" ), "" );
  pqPushButtonOrderInvert->setToolTip( tr("[overlay/flotilla] Invert order") );
  pqPushButtonOrderInvert->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderInvert, SIGNAL( clicked() ), this, SLOT( slotOrderInvert() ) );
  // ... order ascending
  pqPushButtonOrderAscending = new QPushButton( QIcon( ":icons/32x32/order_ascending.png" ), "" );
  pqPushButtonOrderAscending->setToolTip( tr("[overlay/flotilla] Sort in ascending order") );
  pqPushButtonOrderAscending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderAscending, SIGNAL( clicked() ), this, SLOT( slotOrderAscending() ) );
  // ... order descending
  pqPushButtonOrderDescending = new QPushButton( QIcon( ":icons/32x32/order_descending.png" ), "" );
  pqPushButtonOrderDescending->setToolTip( tr("[overlay/flotilla] Sort in descending order") );
  pqPushButtonOrderDescending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderDescending, SIGNAL( clicked() ), this, SLOT( slotOrderDescending() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "" );
  pqPushButtonSave->setToolTip( tr("Save selected vessels to disk") );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete selected vessels") );
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

void CVesselOverlayActionsView::slotOrderInvert()
{
  do // error-catching block
  {
    CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poVesselOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    bool __bDynamic = false;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::CONTAINER:
      __bDynamic = ((CVesselContainer*)__pqTreeWidgetItem_current)->isDynamic();
    case COverlayObject::OVERLAY:
      {
        int __iHead = __bDynamic ? 1 : 0, __iTail = __pqTreeWidgetItem_current->childCount()-1;
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

void CVesselOverlayActionsView::slotOrderAscending()
{
  do // error-catching block
  {
    CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poVesselOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    bool __bDynamic = false;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::CONTAINER:
      __bDynamic = ((CVesselContainer*)__pqTreeWidgetItem_current)->isDynamic();
    case COverlayObject::OVERLAY:
      {
        QTreeWidgetItem* __pqTreeWidgetItem_device = 0;
        if( __bDynamic )
        {
          __pqTreeWidgetItem_device = __pqTreeWidgetItem_current->child( 0 );
          __pqTreeWidgetItem_current->removeChild( __pqTreeWidgetItem_device );
        }
        __pqTreeWidgetItem_current->sortChildren( CVesselOverlay::NAME, Qt::AscendingOrder );
        if( __bDynamic )
        {
          __pqTreeWidgetItem_current->insertChild( 0, __pqTreeWidgetItem_device );
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

void CVesselOverlayActionsView::slotOrderDescending()
{
  do // error-catching block
  {
    CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poVesselOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    bool __bDynamic = false;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::CONTAINER:
      __bDynamic = ((CVesselContainer*)__pqTreeWidgetItem_current)->isDynamic();
    case COverlayObject::OVERLAY:
      {
        QTreeWidgetItem* __pqTreeWidgetItem_device = 0;
        if( __bDynamic )
        {
          __pqTreeWidgetItem_device = __pqTreeWidgetItem_current->child( 0 );
          __pqTreeWidgetItem_current->removeChild( __pqTreeWidgetItem_device );
        }
        __pqTreeWidgetItem_current->sortChildren( CVesselOverlay::NAME, Qt::DescendingOrder );
        if( __bDynamic )
        {
          __pqTreeWidgetItem_current->insertChild( 0, __pqTreeWidgetItem_device );
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

void CVesselOverlayActionsView::slotSave()
{
  do // error-catching block
  {
    QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Vessel"), tr("QVCT Files")+" (*.qvct)" );
    if( __qsFilename.isEmpty() ) break;
    QFileInfo __qFileInfo( __qsFilename );
    if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
    QStringList __qsListExtensions; __qsListExtensions << "qvct";
    if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) break;
    QVCTRuntime::useVesselOverlay()->save( __qsFilename, 0 ); // no container = save selection
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CVesselOverlayActionsView::slotDelete()
{
  do // error-catching block
  {
    if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected vessel(s)") ) ) break;
    QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
    CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
    __pqMutexDataChange->lock();
    bool __bModified = __poVesselOverlay->deleteSelection();
    __pqMutexDataChange->unlock();
    if( __bModified )
    {
      __poVesselOverlay->forceRedraw();
      QVCTRuntime::useChartTable()->updateChart();
      QVCTRuntime::useChartTable()->setProjectModified();
    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}
