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
#include "overlays/COverlayUrl.hpp"
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/CVesselContainerDetailView.hpp"
#include "overlays/vessel/CVesselOverlay.hpp"
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceCreateView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainerDetailView::CVesselContainerDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CVesselContainerDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "" );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setToolTip( tr("Toggle this flotilla's visibility") );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "" );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setToolTip( tr("Center chart on this flotilla's mean position") );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "" );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setToolTip( tr("Edit this flotilla") );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save.png" ), "" );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  pqPushButtonSave->setToolTip( tr("Save this flotilla to disk") );
  pqPushButtonSave->setEnabled( false );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this flotilla") );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... set device
  pqPushButtonSetDevice = new QPushButton( QIcon( ":icons/32x32/device_add.png" ), "" );
  pqPushButtonSetDevice->setMaximumSize( 36, 34 );
  pqPushButtonSetDevice->setToolTip( tr("Set a device for this (dynamic) flotilla") );
  pqPushButtonSetDevice->setEnabled( false );
  QWidget::connect( pqPushButtonSetDevice, SIGNAL( clicked() ), this, SLOT( slotSetDevice() ) );
  // ... add point
  pqPushButtonAddPoint = new QPushButton( QIcon( ":icons/32x32/vessel_add.png" ), "" );
  pqPushButtonAddPoint->setMaximumSize( 36, 34 );
  pqPushButtonAddPoint->setToolTip( tr("Add a new vessel to this flotilla") );
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
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel.png" ) );
  __pqLabelIcon->setToolTip( tr("Flotilla") );
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
  poTextContent->setToolTip( tr("Vessels count") );
  poTextContent->setFont( __qFontData );
  poTextContent->setIndent( 10 );
  poTextContent->setAlignment( Qt::AlignHCenter );
  poTextContent->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextContent, 1 );
  __poWidgetSummary->setLayout( __pqVBoxLayoutSummary );
  __poTabWidget->addTab( __poWidgetSummary, tr("Summary") );

  // ... info
  QScrollArea* __poScrollAreaInfo = new QScrollArea();
  __poScrollAreaInfo->setStyleSheet( ".QScrollArea, .QWidget { BACKGROUND-COLOR: rgba(0, 0, 0, 0); BORDER: none; }" );
  __poScrollAreaInfo->setWidgetResizable( true );
  QWidget* __poWidgetInfo = new QWidget();
  QVBoxLayout* __pqVBoxLayoutInfo = new QVBoxLayout();
  __qFontData.setPixelSize( 12 );
  __qFontData.setBold( true );
  poTextType = new COverlayText();
  poTextType->setToolTip( tr("Type") );
  poTextType->setFont( __qFontData );
  poTextType->setIndent( 10 );
  poTextType->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextType );
  __qFontData.setBold( false );
  poTextDescription = new COverlayText();
  poTextDescription->setToolTip( tr("Description") );
  poTextDescription->setFont( __qFontData );
  poTextDescription->setIndent( 10 );
  poTextDescription->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextDescription );
  __qFontData.setPixelSize( 10 );
  poTextComment = new COverlayText();
  poTextComment->setToolTip( tr("Comment") );
  poTextComment->setFont( __qFontData );
  poTextComment->setIndent( 10 );
  poTextComment->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextComment );
  poUrl = new COverlayUrl();
  poUrl->setToolTip( tr("URL") );
  poUrl->setFont( __qFontData );
  poUrl->setIndent( 10 );
  poUrl->resetText();
  __pqVBoxLayoutInfo->addWidget( poUrl, 1 );
  __poWidgetInfo->setLayout( __pqVBoxLayoutInfo );
  __poScrollAreaInfo->setWidget( __poWidgetInfo );
  __poTabWidget->addTab( __poScrollAreaInfo, tr("Info") );

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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSetDevice, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddPoint, 0, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CVesselContainerDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CVesselContainer* __poVesselContainer = (CVesselContainer*)poOverlayObject;
  poTextName->setText( __poVesselContainer->getName() );
  poTextContent->setText( QString::number( __poVesselContainer->childCount() - ( __poVesselContainer->isDynamic() ? 1 : 0 ) )+" "+tr("Vessel(s)") );
  poTextType->setText( __poVesselContainer->getType() );
  poTextDescription->setText( __poVesselContainer->getDescription() );
  poTextComment->setText( __poVesselContainer->getComment() );
  poUrl->setText( __poVesselContainer->getUrl() );

  // ... buttons
  bool __bDynamic = __poVesselContainer->isDynamic();
  pqPushButtonSetDevice->setEnabled( !__bDynamic );
  pqPushButtonAddPoint->setEnabled( !__bDynamic );
}

void CVesselContainerDetailView::resetContent()
{
  poTextName->resetText();
  poTextContent->resetText();
  poTextType->resetText();
  poTextDescription->resetText();
  poTextComment->resetText();
  poUrl->resetText();
}

void CVesselContainerDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonSave->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
  }
}

void CVesselContainerDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonSave->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonSetDevice->setEnabled( false );
  pqPushButtonAddPoint->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselContainerDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CVesselContainer* __poVesselContainer = (CVesselContainer*)poOverlayObject;
  bool __bVisible = __poVesselContainer->isVisible();
  __poVesselContainer->toggleVisibility();
  if( __bVisible != __poVesselContainer->isVisible() )
  {
    __poVesselContainer->setCheckState( CVesselOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poVesselContainer->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CVesselContainerDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CDataPosition __oDataPositionLower, __oDataPositionUpper;
  int __iCount = COverlayPoint::getPositionBox( poOverlayObject, &__oDataPositionLower, &__oDataPositionUpper );
  if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
  else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
  QVCTRuntime::useChartTable()->updateChart();
}

void CVesselContainerDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CVesselContainer*)poOverlayObject)->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CVesselContainerDetailView::slotSave()
{
  if( !poOverlayObject ) return;
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Vessel"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QFileInfo __qFileInfo( __qsFilename );
  if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
  QStringList __qsListExtensions; __qsListExtensions << "qvct";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useVesselOverlay()->save( __qsFilename, (CVesselContainer*)poOverlayObject );
}

void CVesselContainerDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CVesselOverlay* __poVesselOverlay = (CVesselOverlay*)poOverlayObject->useOverlay();
  CVesselContainer* __poVesselContainer = (CVesselContainer*)poOverlayObject;
  __pqMutexDataChange->lock();
  __poVesselOverlay->removeChild( __poVesselContainer );
  __pqMutexDataChange->unlock();
  delete __poVesselContainer;
  QTreeWidgetItem* __pqTreeWidgetItem = __poVesselOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poVesselOverlay->showDetail( __pqTreeWidgetItem );
  __poVesselOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CVesselContainerDetailView::slotSetDevice()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( tr("Defined vessel(s)") ) ) return;
  CVesselContainerDevice* __poVesselContainerDevice = 0;
  CVesselContainerDeviceCreateView* __poVesselContainerDeviceCreateView = new CVesselContainerDeviceCreateView( &__poVesselContainerDevice );
  __poVesselContainerDeviceCreateView->exec();
  if( __poVesselContainerDevice )
  {
    QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
    __pqMutexDataChange->lock();
    ((CVesselContainer*)poOverlayObject)->setDevice( __poVesselContainerDevice );
    __pqMutexDataChange->unlock();
    QVCTRuntime::useVesselOverlay()->setCurrentItem( __poVesselContainerDevice );
    __poVesselContainerDevice->showEdit();
    QVCTRuntime::useVesselOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
    QVCTRuntime::useChartTable()->setProjectModified();
  }
  delete __poVesselContainerDeviceCreateView;
}

void CVesselContainerDetailView::slotAddPoint()
{
  if( !poOverlayObject ) return;
  QString __qsName = poOverlayObject->newChildName( tr("Vessel"), 1, true );
  CVesselPoint* __poVesselPoint = ((CVesselContainer*)poOverlayObject)->addPoint( __qsName );
  if( !__poVesselPoint ) return;
  QVCTRuntime::useVesselOverlay()->setCurrentItem( __poVesselPoint );
  __poVesselPoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}
