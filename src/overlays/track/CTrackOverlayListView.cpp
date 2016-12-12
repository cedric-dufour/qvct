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
#include "overlays/track/CTrackOverlayActionsView.hpp"
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
  poTrackOverlay = new CTrackOverlay();
  QVCTRuntime::registerTrackOverlay( poTrackOverlay );

  // Create the buttons
  // ... add (dummy)
  QPushButton* __pqPushButtonAdd = new QPushButton( QIcon( ":icons/32x32/add.png" ), "" );
  __pqPushButtonAdd->setMaximumSize( 36, 34 );
  __pqPushButtonAdd->setEnabled( false );
  // ... load
  pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/load.png" ), "" );
  pqPushButtonLoad->setToolTip( tr("Load track from disk") );
  pqPushButtonLoad->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonLoad, SIGNAL( clicked() ), this, SLOT( slotLoad() ) );
  // ... up
  pqPushButtonUp = new QPushButton( QIcon( ":icons/32x32/move_up.png" ), "" );
  pqPushButtonUp->setToolTip( tr("[track] Move up") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/move_down.png" ), "" );
  pqPushButtonDown->setToolTip( tr("[track] Move down") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );
  // ... actions
  pqPushButtonActions = new QPushButton( QIcon( ":icons/32x32/more.png" ), "" );
  pqPushButtonActions->setToolTip( tr("Additional actions")+"..." );
  pqPushButtonActions->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonActions, SIGNAL( clicked() ), this, SLOT( slotActions() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add the overlay/tree
  __pqVBoxLayout->addWidget( poTrackOverlay, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame();
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( __pqPushButtonAdd, 0, Qt::AlignLeft );
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

void CTrackOverlayListView::slotUp()
{
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poTrackOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

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

void CTrackOverlayListView::slotActions()
{
  CTrackOverlayActionsView* __poTrackOverlayActionsView = new CTrackOverlayActionsView();
  __poTrackOverlayActionsView->exec();
  delete __poTrackOverlayActionsView;
}
