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

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/route/CRouteOverlayActionsView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteOverlayActionsView::CRouteOverlayActionsView()
{
  constructLayout();
}

void CRouteOverlayActionsView::constructLayout()
{
  // Create the buttons
  // ... order invert
  pqPushButtonOrderInvert = new QPushButton( QIcon( ":icons/32x32/order_invert.png" ), "", this );
  pqPushButtonOrderInvert->setToolTip( tr("[overlay/route] Invert order") );
  pqPushButtonOrderInvert->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderInvert, SIGNAL( clicked() ), this, SLOT( slotOrderInvert() ) );
  // ... order ascending
  pqPushButtonOrderAscending = new QPushButton( QIcon( ":icons/32x32/order_ascending.png" ), "", this );
  pqPushButtonOrderAscending->setToolTip( tr("[overlay] Sort in ascending order") );
  pqPushButtonOrderAscending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderAscending, SIGNAL( clicked() ), this, SLOT( slotOrderAscending() ) );
  // ... order descending
  pqPushButtonOrderDescending = new QPushButton( QIcon( ":icons/32x32/order_descending.png" ), "", this );
  pqPushButtonOrderDescending->setToolTip( tr("[overlay] Sort in descending order") );
  pqPushButtonOrderDescending->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonOrderDescending, SIGNAL( clicked() ), this, SLOT( slotOrderDescending() ) );
  // ... save (dummy)
  QPushButton* __pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "", this );
  __pqPushButtonSave->setMaximumSize( 36, 34 );
  __pqPushButtonSave->setEnabled( false );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete selected waypoints") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderInvert, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderAscending, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonOrderDescending, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( __pqPushButtonSave, 0, Qt::AlignLeft );
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

void CRouteOverlayActionsView::slotOrderInvert()
{
  do // error-catching block
  {
    CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poRouteOverlay->currentItem();
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
        if( __pqTreeWidgetItem_current->type() == COverlayObject::CONTAINER )
        {
          __poRouteOverlay->forceRedraw();
          QVCTRuntime::useChartTable()->updateChart();
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

void CRouteOverlayActionsView::slotOrderAscending()
{
  do // error-catching block
  {
    CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poRouteOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
      __pqTreeWidgetItem_current->sortChildren( CRouteOverlay::NAME, Qt::AscendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CRouteOverlayActionsView::slotOrderDescending()
{
  do // error-catching block
  {
    CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
    QTreeWidgetItem* __pqTreeWidgetItem_current = __poRouteOverlay->currentItem();
    if( !__pqTreeWidgetItem_current ) break;
    switch( __pqTreeWidgetItem_current->type() )
    {

    case COverlayObject::OVERLAY:
      __pqTreeWidgetItem_current->sortChildren( CRouteOverlay::NAME, Qt::DescendingOrder );
      QVCTRuntime::useChartTable()->setProjectModified();
      break;

    default:;

    }
  }
  while( false ); // error-catching block
  QDialog::accept();
}

void CRouteOverlayActionsView::slotDelete()
{
  do // error-catching block
  {
    if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected waypoint(s)") ) ) break;
    QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
    CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
    __pqMutexDataChange->lock();
    bool __bModified = __poRouteOverlay->deleteSelection();
    __pqMutexDataChange->unlock();
    if( __bModified )
    {
      __poRouteOverlay->forceRedraw();
      QVCTRuntime::useChartTable()->updateChart();
      QVCTRuntime::useChartTable()->setProjectModified();
    };
  }
  while( false ); // error-catching block
  QDialog::accept();
}
