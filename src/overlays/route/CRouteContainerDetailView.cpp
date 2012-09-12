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
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "data/CDataPosition.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/COverlayUrl.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/route/CRouteContainerDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteContainerDetailView::CRouteContainerDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CRouteContainerDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "", this );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setToolTip( tr("Toggle this route's visibility") );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "", this );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setToolTip( tr("Center chart on this route's mean position") );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save.png" ), "", this );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  pqPushButtonSave->setToolTip( tr("Save this route to disk") );
  pqPushButtonSave->setEnabled( false );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "", this );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setToolTip( tr("Edit this route") );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this route") );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... add point
  pqPushButtonAddPoint = new QPushButton( QIcon( ":icons/32x32/point_add.png" ), "", this );
  pqPushButtonAddPoint->setMaximumSize( 36, 34 );
  pqPushButtonAddPoint->setToolTip( tr("Add a new waypoint to this route") );
  pqPushButtonAddPoint->setEnabled( false );
  QWidget::connect( pqPushButtonAddPoint, SIGNAL( clicked() ), this, SLOT( slotAddPoint() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/route.png" ) );
  __pqLabelIcon->setToolTip( tr("Route") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget( this );
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... summary
  QWidget* __poWidgetSummary = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutSummary = new QVBoxLayout( __poWidgetSummary );
  __qFontData.setPixelSize( 20 );
  poTextContent = new COverlayText( this );
  poTextContent->setToolTip( tr("Waypoints count") );
  poTextContent->setFont( __qFontData );
  poTextContent->setIndent( 10 );
  poTextContent->setAlignment( Qt::AlignHCenter );
  poTextContent->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextContent, 0 );
  poTextLengthRL = new COverlayText( this );
  poTextLengthRL->setToolTip( tr("Length (rhumb-lines along route)") );
  poTextLengthRL->setFont( __qFontData );
  poTextLengthRL->setIndent( 10 );
  poTextLengthRL->setAlignment( Qt::AlignHCenter );
  poTextLengthRL->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextLengthRL, 1 );
  __poWidgetSummary->setLayout( __pqVBoxLayoutSummary );
  __poTabWidget->addTab( __poWidgetSummary, tr("Summary") );

  // ... info
  QScrollArea* __poScrollAreaInfo = new QScrollArea( __poTabWidget );
  __poScrollAreaInfo->setStyleSheet( ".QScrollArea, .QWidget { BACKGROUND-COLOR: rgba(0, 0, 0, 0); BORDER: none; }" );
  __poScrollAreaInfo->setWidgetResizable( true );
  QWidget* __poWidgetInfo = new QWidget( __poScrollAreaInfo );
  QVBoxLayout* __pqVBoxLayoutInfo = new QVBoxLayout( __poWidgetInfo );
  __qFontData.setPixelSize( 12 );
  __qFontData.setBold( true );
  poTextType = new COverlayText( this );
  poTextType->setToolTip( tr("Type") );
  poTextType->setFont( __qFontData );
  poTextType->setIndent( 10 );
  poTextType->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextType );
  __qFontData.setBold( false );
  poTextDescription = new COverlayText( this );
  poTextDescription->setToolTip( tr("Description") );
  poTextDescription->setFont( __qFontData );
  poTextDescription->setIndent( 10 );
  poTextDescription->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextDescription );
  __qFontData.setPixelSize( 10 );
  poTextComment = new COverlayText( this );
  poTextComment->setToolTip( tr("Comment") );
  poTextComment->setFont( __qFontData );
  poTextComment->setIndent( 10 );
  poTextComment->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextComment );
  poUrl = new COverlayUrl( this );
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
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonVisible, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenter, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonSave, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonEdit, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddPoint, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CRouteContainerDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CRouteContainer* __poRouteContainer = (CRouteContainer*)poOverlayObject;
  poTextName->setText( __poRouteContainer->getName() );
  poTextContent->setText( QString::number( __poRouteContainer->childCount() )+" "+tr("Waypoint(s)") );
  poTextLengthRL->setText( CUnitDistance::toString( __poRouteContainer->getLengthRL() ) );
  poTextType->setText( __poRouteContainer->getType() );
  poTextDescription->setText( __poRouteContainer->getDescription() );
  poTextComment->setText( __poRouteContainer->getComment() );
  poUrl->setText( __poRouteContainer->getUrl() );
}

void CRouteContainerDetailView::resetContent()
{
  poTextName->resetText();
  poTextContent->resetText();
  poTextLengthRL->resetText();
  poTextType->resetText();
  poTextDescription->resetText();
  poTextComment->resetText();
  poUrl->resetText();
}

void CRouteContainerDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonSave->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonAddPoint->setEnabled( true );
  }
}

void CRouteContainerDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonSave->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonAddPoint->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CRouteContainerDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CRouteContainer* __poRouteContainer = (CRouteContainer*)poOverlayObject;
  bool __bVisible = __poRouteContainer->isVisible();
  __poRouteContainer->toggleVisible();
  if( __bVisible != __poRouteContainer->isVisible() )
  {
    __poRouteContainer->setCheckState( CRouteOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poRouteContainer->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CRouteContainerDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CDataPosition __oDataPositionLower, __oDataPositionUpper;
  int __iCount = COverlayPoint::getPositionBox( poOverlayObject, &__oDataPositionLower, &__oDataPositionUpper );
  if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
  else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
}

void CRouteContainerDetailView::slotSave()
{
  if( !poOverlayObject ) return;
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Route"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QStringList __qsListExtensions; __qsListExtensions << "qvct" << "gpx";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useRouteOverlay()->save( __qsFilename, (CRouteContainer*)poOverlayObject );
}

void CRouteContainerDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CRouteContainer*)poOverlayObject)->showEdit();
}

void CRouteContainerDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CRouteOverlay* __poRouteOverlay = (CRouteOverlay*)poOverlayObject->useOverlay();
  CRouteContainer* __poRouteContainer = (CRouteContainer*)poOverlayObject;
  __poRouteOverlay->removeChild( __poRouteContainer );
  delete __poRouteContainer;
  QTreeWidgetItem* __pqTreeWidgetItem = __poRouteOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poRouteOverlay->showDetail( __pqTreeWidgetItem );
  __poRouteOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}

void CRouteContainerDetailView::slotAddPoint()
{
  if( !poOverlayObject ) return;
  QString __qsName = poOverlayObject->newChildName( tr("Waypoint"), 1, true );
  CRoutePoint* __poRoutePoint = ((CRouteContainer*)poOverlayObject)->addPoint( __qsName );
  if( !__poRoutePoint ) return;
  QVCTRuntime::useRouteOverlay()->setCurrentItem( __poRoutePoint );
  __poRoutePoint->showEdit();
}
