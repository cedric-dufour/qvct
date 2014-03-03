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
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackOverlayListView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackOverlayListView::CTrackOverlayListView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CTrackOverlayListView::constructLayout()
{
  // Create the overlay/tree
  poTrackOverlay = new CTrackOverlay( this );
  QVCTRuntime::registerTrackOverlay( poTrackOverlay );

  // Create the buttons
  // ... add (dummy)
  QPushButton* __pqPushButtonAdd = new QPushButton( QIcon( ":icons/32x32/add.png" ), "", this );
  __pqPushButtonAdd->setMaximumSize( 36, 34 );
  __pqPushButtonAdd->setEnabled( false );
  // ... load
  pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/load.png" ), "", this );
  pqPushButtonLoad->setToolTip( tr("Load track from disk") );
  pqPushButtonLoad->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonLoad, SIGNAL( clicked() ), this, SLOT( slotLoad() ) );
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
  pqPushButtonUp->setToolTip( tr("[waypoint/track] Move up; [overlay] Sort in ascending order") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/sort_descending.png" ), "", this );
  pqPushButtonDown->setToolTip( tr("[waypoint/track] Move down; [overlay] Sort in descending order") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add the overlay/tree
  __pqVBoxLayout->addWidget( poTrackOverlay, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( __pqPushButtonAdd, 0, Qt::AlignLeft );
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

void CTrackOverlayListView::slotLoad()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Track"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  CTrackContainer* __poTrackContainer = __poTrackOverlay->load( __qsFilename );
  if( !__poTrackContainer ) return;
  __poTrackOverlay->setCurrentItem( __poTrackContainer );
  __poTrackOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CTrackOverlayListView::slotDelete()
{
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected point(s)") ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  __pqMutexDataChange->lock();
  bool __bModified = __poTrackOverlay->deleteSelection();
  __pqMutexDataChange->unlock();
  if( __bModified )
  {
    __poTrackOverlay->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
    QVCTRuntime::useChartTable()->setProjectModified();
  };
}

void CTrackOverlayListView::slotUp()
{
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poTrackOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CTrackOverlay::NAME, Qt::AscendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex <= 0 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex-1, __pqTreeWidgetItem_current );
      __poTrackOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
      {
        __poTrackOverlay->forceRedraw();
        QVCTRuntime::useChartTable()->updateChart();
      }
      QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}

void CTrackOverlayListView::slotDown()
{
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poTrackOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CTrackOverlay::NAME, Qt::DescendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex >= __pqTreeWidgetItem_parent->childCount()-1 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex+1, __pqTreeWidgetItem_current );
      __poTrackOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
      {
        __poTrackOverlay->forceRedraw();
        QVCTRuntime::useChartTable()->updateChart();
      }
      QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}
