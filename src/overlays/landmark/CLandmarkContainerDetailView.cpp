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
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "data/CDataPosition.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/landmark/CLandmarkContainerDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkContainerDetailView::CLandmarkContainerDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CLandmarkContainerDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "" );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setToolTip( tr("Toggle this landmarks set's visibility") );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "" );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setToolTip( tr("Center chart on this landmarks set's mean position") );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "" );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setToolTip( tr("Edit this landmarks set") );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save.png" ), "" );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  pqPushButtonSave->setToolTip( tr("Save this landmarks set to disk") );
  pqPushButtonSave->setEnabled( false );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this landmarks set") );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... add point
  pqPushButtonAddPoint = new QPushButton( QIcon( ":icons/32x32/point_add.png" ), "" );
  pqPushButtonAddPoint->setMaximumSize( 36, 34 );
  pqPushButtonAddPoint->setToolTip( tr("Add a new landmark to this landmarks set") );
  pqPushButtonAddPoint->setEnabled( false );
  QWidget::connect( pqPushButtonAddPoint, SIGNAL( clicked() ), this, SLOT( slotAddPoint() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/landmark.png" ) );
  __pqLabelIcon->setToolTip( tr("Landmarks set") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText();
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget();
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... summary
  QWidget* __poWidgetSummary = new QWidget();
  QVBoxLayout* __pqVBoxLayoutSummary = new QVBoxLayout();
  __qFontData.setPixelSize( 20 );
  poTextContent = new COverlayText();
  poTextContent->setToolTip( tr("Landmarks count") );
  poTextContent->setFont( __qFontData );
  poTextContent->setIndent( 10 );
  poTextContent->setAlignment( Qt::AlignHCenter );
  poTextContent->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextContent, 1 );
  __poWidgetSummary->setLayout( __pqVBoxLayoutSummary );
  __poTabWidget->addTab( __poWidgetSummary, tr("Summary") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame();
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonVisible, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenter, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonEdit, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSave, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddPoint, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CLandmarkContainerDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)poOverlayObject;
  poTextName->setText( __poLandmarkContainer->getName() );
  poTextContent->setText( QString::number( __poLandmarkContainer->childCount() )+" "+tr("Landmark(s)") );
}

void CLandmarkContainerDetailView::resetContent()
{
  poTextName->resetText();
  poTextContent->resetText();
}

void CLandmarkContainerDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonSave->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonAddPoint->setEnabled( true );
  }
}

void CLandmarkContainerDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonSave->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonAddPoint->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CLandmarkContainerDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)poOverlayObject;
  bool __bVisible = __poLandmarkContainer->isVisible();
  __poLandmarkContainer->toggleVisibility();
  if( __bVisible != __poLandmarkContainer->isVisible() )
  {
    __poLandmarkContainer->setCheckState( CLandmarkOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poLandmarkContainer->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CLandmarkContainerDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CDataPosition __oDataPositionLower, __oDataPositionUpper;
  int __iCount = COverlayPoint::getPositionBox( poOverlayObject, &__oDataPositionLower, &__oDataPositionUpper );
  if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
  else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
  QVCTRuntime::useChartTable()->updateChart();
}

void CLandmarkContainerDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CLandmarkContainer*)poOverlayObject)->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CLandmarkContainerDetailView::slotSave()
{
  if( !poOverlayObject ) return;
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Landmarks"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QFileInfo __qFileInfo( __qsFilename );
  if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
  QStringList __qsListExtensions; __qsListExtensions << "qvct" << "gpx";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useLandmarkOverlay()->save( __qsFilename, (CLandmarkContainer*)poOverlayObject );
}

void CLandmarkContainerDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CLandmarkOverlay* __poLandmarkOverlay = (CLandmarkOverlay*)poOverlayObject->useOverlay();
  CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)poOverlayObject;
  __pqMutexDataChange->lock();
  __poLandmarkOverlay->removeChild( __poLandmarkContainer );
  __pqMutexDataChange->unlock();
  delete __poLandmarkContainer;
  QTreeWidgetItem* __pqTreeWidgetItem = __poLandmarkOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poLandmarkOverlay->showDetail( __pqTreeWidgetItem );
  __poLandmarkOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CLandmarkContainerDetailView::slotAddPoint()
{
  if( !poOverlayObject ) return;
  QString __qsName = poOverlayObject->newChildName( tr("Landmark"), 1, true );
  CLandmarkPoint* __poLandmarkPoint = ((CLandmarkContainer*)poOverlayObject)->addPoint( __qsName );
  if( !__poLandmarkPoint ) return;
  QVCTRuntime::useLandmarkOverlay()->setCurrentItem( __poLandmarkPoint );
  __poLandmarkPoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}
