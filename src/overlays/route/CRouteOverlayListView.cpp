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
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRouteOverlayListView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteOverlayListView::CRouteOverlayListView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CRouteOverlayListView::constructLayout()
{
  // Create the overlay/tree
  poRouteOverlay = new CRouteOverlay( this );
  QVCTRuntime::registerRouteOverlay( poRouteOverlay );

  // Create the buttons
  // ... add
  pqPushButtonAdd = new QPushButton( QIcon( ":icons/32x32/add.png" ), "", this );
  pqPushButtonAdd->setToolTip( tr("Create a new route") );
  pqPushButtonAdd->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonAdd, SIGNAL( clicked() ), this, SLOT( slotAdd() ) );
  // ... load
  pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/load.png" ), "", this );
  pqPushButtonLoad->setToolTip( tr("Load route from disk") );
  pqPushButtonLoad->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonLoad, SIGNAL( clicked() ), this, SLOT( slotLoad() ) );
  // // ... save
  // pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "", this );
  // pqPushButtonSave->setToolTip( tr("Save selected waypoints to disk") );
  // pqPushButtonSave->setMaximumSize( 36, 34 );
  // QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... save (dummy)
  QPushButton* __pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "", this );
  __pqPushButtonSave->setMaximumSize( 36, 34 );
  __pqPushButtonSave->setEnabled( false );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete selected waypoints") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... up
  pqPushButtonUp = new QPushButton( QIcon( ":icons/32x32/sort_ascending.png" ), "", this );
  pqPushButtonUp->setToolTip( tr("[waypoint/route] Move up; [overlay] Sort in ascending order") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/sort_descending.png" ), "", this );
  pqPushButtonDown->setToolTip( tr("[waypoint/route] Move down; [overlay] Sort in descending order") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add the overlay/tree
  __pqVBoxLayout->addWidget( poRouteOverlay, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAdd, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonLoad, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( __pqPushButtonSave, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 1, Qt::AlignLeft );
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

void CRouteOverlayListView::slotAdd()
{
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  QString __qsName = __poRouteOverlay->newChildName( tr("Route"), 1, true );
  CRouteContainer* __poRouteContainer = __poRouteOverlay->addContainer( __qsName );
  if( !__poRouteContainer ) return;
  __poRouteOverlay->setCurrentItem( __poRouteContainer );
  __poRouteContainer->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CRouteOverlayListView::slotLoad()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Route"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  CRouteContainer* __poRouteContainer = __poRouteOverlay->load( __qsFilename );
  if( !__poRouteContainer ) return;
  __poRouteOverlay->setCurrentItem( __poRouteContainer );
  __poRouteOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CRouteOverlayListView::slotDelete()
{
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected waypoint(s)") ) ) return;
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

void CRouteOverlayListView::slotUp()
{
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poRouteOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CRouteOverlay::NAME, Qt::AscendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex <= 0 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex-1, __pqTreeWidgetItem_current );
      __poRouteOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
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

void CRouteOverlayListView::slotDown()
{
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poRouteOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CRouteOverlay::NAME, Qt::DescendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex >= __pqTreeWidgetItem_parent->childCount()-1 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex+1, __pqTreeWidgetItem_current );
      __poRouteOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
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
