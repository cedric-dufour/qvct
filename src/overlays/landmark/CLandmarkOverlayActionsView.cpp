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
#include "overlays/landmark/CLandmarkOverlayActionsView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkOverlayActionsView::CLandmarkOverlayActionsView()
{
  constructLayout();
}

void CLandmarkOverlayActionsView::constructLayout()
{
  // Create the buttons
  // ... order invert
  pqPushButtonOrderInvert = new QPushButton( QIcon( ":icons/32x32/order_invert.png" ), "" );
  pqPushButtonOrderInvert->setToolTip( tr("[overlay/landmarks set] Invert order") );
  pqPushButtonOrderInvert->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderInvert, SIGNAL( clicked() ), this, SLOT( slotOrderInvert() ) );
  // ... order ascending
  pqPushButtonOrderAscending = new QPushButton( QIcon( ":icons/32x32/order_ascending.png" ), "" );
  pqPushButtonOrderAscending->setToolTip( tr("[overlay/landmarks set] Sort in ascending order") );
  pqPushButtonOrderAscending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderAscending, SIGNAL( clicked() ), this, SLOT( slotOrderAscending() ) );
  // ... order descending
  pqPushButtonOrderDescending = new QPushButton( QIcon( ":icons/32x32/order_descending.png" ), "" );
  pqPushButtonOrderDescending->setToolTip( tr("[overlay/landmarks set] Sort in descending order") );
  pqPushButtonOrderDescending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderDescending, SIGNAL( clicked() ), this, SLOT( slotOrderDescending() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "" );
  pqPushButtonSave->setToolTip( tr("Save selected landmarks to disk") );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete selected landmarks") );
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

void CLandmarkOverlayActionsView::slotOrderInvert()
{
  do // error-catching block
  {
    CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poLandmarkOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
    case COverlayObject::CONTAINER:
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

void CLandmarkOverlayActionsView::slotOrderAscending()
{
  do // error-catching block
  {
    CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poLandmarkOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
    case COverlayObject::CONTAINER:
      __pqTreeWidgetItem_current->sortChildren( CLandmarkOverlay::NAME, Qt::AscendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CLandmarkOverlayActionsView::slotOrderDescending()
{
  do // error-catching block
  {
    CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poLandmarkOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
    case COverlayObject::CONTAINER:
      __pqTreeWidgetItem_current->sortChildren( CLandmarkOverlay::NAME, Qt::DescendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CLandmarkOverlayActionsView::slotSave()
{
  do // error-catching block
  {
    QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Landmarks"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
    if( __qsFilename.isEmpty() ) break;
    QFileInfo __qFileInfo( __qsFilename );
    if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
    QStringList __qsListExtensions; __qsListExtensions << "qvct" << "gpx";
    if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) break;
    QVCTRuntime::useLandmarkOverlay()->save( __qsFilename, 0 ); // no container = save selection
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CLandmarkOverlayActionsView::slotDelete()
{
  do // error-catching block
  {
    if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected landmark(s)") ) ) break;
    QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
    CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
    __pqMutexDataChange->lock();
    bool __bModified = __poLandmarkOverlay->deleteSelection();
    __pqMutexDataChange->unlock();
    if( __bModified )
    {
      __poLandmarkOverlay->forceRedraw();
      QVCTRuntime::useChartTable()->updateChart();
      QVCTRuntime::useChartTable()->setProjectModified();
    };
  }
  while( false ); // error-catching block
  QDialog::accept();
}
