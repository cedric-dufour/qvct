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
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/CVesselOverlayListView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselOverlayListView::CVesselOverlayListView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CVesselOverlayListView::constructLayout()
{
  // Create the overlay/tree
  poVesselOverlay = new CVesselOverlay( this );
  QVCTRuntime::registerVesselOverlay( poVesselOverlay );

  // Create the buttons
  // ... add
  pqPushButtonAdd = new QPushButton( QIcon( ":icons/32x32/add.png" ), "", this );
  pqPushButtonAdd->setToolTip( tr("Create a new flotilla") );
  pqPushButtonAdd->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonAdd, SIGNAL( clicked() ), this, SLOT( slotAdd() ) );
  // ... load
  pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/load.png" ), "", this );
  pqPushButtonLoad->setToolTip( tr("Load flotilla from disk") );
  pqPushButtonLoad->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonLoad, SIGNAL( clicked() ), this, SLOT( slotLoad() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save_select.png" ), "", this );
  pqPushButtonSave->setToolTip( tr("Save selected vessels to disk") );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete_select.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete selected vessels") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... up
  pqPushButtonUp = new QPushButton( QIcon( ":icons/32x32/sort_ascending.png" ), "", this );
  pqPushButtonUp->setToolTip( tr("[flotilla/vessel] Move up; [overlay] Sort in ascending order") );
  pqPushButtonUp->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonUp, SIGNAL( clicked() ), this, SLOT( slotUp() ) );
  // ... down
  pqPushButtonDown = new QPushButton( QIcon( ":icons/32x32/sort_descending.png" ), "", this );
  pqPushButtonDown->setToolTip( tr("[flotilla/vessel] Move down; [overlay] Sort in descending order") );
  pqPushButtonDown->setMaximumSize( 36, 34 );
  QWidget::connect( pqPushButtonDown, SIGNAL( clicked() ), this, SLOT( slotDown() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add the overlay/tree
  __pqVBoxLayout->addWidget( poVesselOverlay, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAdd, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonLoad, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSave, 0, Qt::AlignLeft );
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

void CVesselOverlayListView::slotAdd()
{
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  QString __qsName = __poVesselOverlay->newChildName( tr("Flotilla"), 1, true );
  CVesselContainer* __poVesselContainer = __poVesselOverlay->addContainer( __qsName );
  if( !__poVesselContainer ) return;
  __poVesselOverlay->setCurrentItem( __poVesselContainer );
  __poVesselContainer->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CVesselOverlayListView::slotLoad()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Vessel"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  CVesselContainer* __poVesselContainer = __poVesselOverlay->load( __qsFilename );
  if( !__poVesselContainer ) return;
  __poVesselOverlay->setCurrentItem( __poVesselContainer );
  __poVesselOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CVesselOverlayListView::slotSave()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Vessel"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QFileInfo __qFileInfo( __qsFilename );
  if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
  QStringList __qsListExtensions; __qsListExtensions << "qvct";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useVesselOverlay()->save( __qsFilename, 0 ); // no container = save selection
}

void CVesselOverlayListView::slotDelete()
{
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Selected vessel(s)") ) ) return;
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
  };
}

void CVesselOverlayListView::slotUp()
{
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poVesselOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CVesselOverlay::NAME, Qt::AscendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      bool __bDynamic = __pqTreeWidgetItem_current->type() == COverlayObject::ITEM
        ? ((CVesselContainer*)__pqTreeWidgetItem_parent)->isDynamic() : false;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex <= __bDynamic ? 1 : 0 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex-1, __pqTreeWidgetItem_current );
      __poVesselOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
      {
        __poVesselOverlay->forceRedraw();
        QVCTRuntime::useChartTable()->updateChart();
      }
      if( !__bDynamic ) QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}

void CVesselOverlayListView::slotDown()
{
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  QTreeWidgetItem* __pqTreeWidgetItem_current = __poVesselOverlay->currentItem();
  if( !__pqTreeWidgetItem_current ) return;
  switch( __pqTreeWidgetItem_current->type() )
  {

  case COverlayObject::OVERLAY:
    __pqTreeWidgetItem_current->sortChildren( CVesselOverlay::NAME, Qt::DescendingOrder );
    QVCTRuntime::useChartTable()->setProjectModified();
    break;

  case COverlayObject::CONTAINER:
  case COverlayObject::ITEM:
    {
      QTreeWidgetItem* __pqTreeWidgetItem_parent = __pqTreeWidgetItem_current->parent();
      if( !__pqTreeWidgetItem_parent ) return;
      bool __bDynamic = __pqTreeWidgetItem_current->type() == COverlayObject::ITEM
        ? ((CVesselContainer*)__pqTreeWidgetItem_parent)->isDynamic() : false;
      int __iIndex = __pqTreeWidgetItem_parent->indexOfChild( __pqTreeWidgetItem_current );
      if( __iIndex >= __pqTreeWidgetItem_parent->childCount()-1 ) return;
      __pqTreeWidgetItem_parent->removeChild( __pqTreeWidgetItem_current );
      __pqTreeWidgetItem_parent->insertChild( __iIndex+1, __pqTreeWidgetItem_current );
      __poVesselOverlay->setCurrentItem( __pqTreeWidgetItem_current );
      if( __pqTreeWidgetItem_current->type() == COverlayObject::ITEM )
      {
        __poVesselOverlay->forceRedraw();
        QVCTRuntime::useChartTable()->updateChart();
      }
      if( !__bDynamic ) QVCTRuntime::useChartTable()->setProjectModified();
    }
    break;

  default:;

  }
}
