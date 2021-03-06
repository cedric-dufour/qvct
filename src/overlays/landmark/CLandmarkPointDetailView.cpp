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
#include "overlays/COverlayText.hpp"
#include "overlays/COverlayUrl.hpp"
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/landmark/CLandmarkPointDetailView.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRoutePoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkPointDetailView::CLandmarkPointDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CLandmarkPointDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "" );
  pqPushButtonVisible->setToolTip( tr("Toggle this landmark's visibility") );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "" );
  pqPushButtonCenter->setToolTip( tr("Center chart on this landmark") );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "" );
  pqPushButtonEdit->setToolTip( tr("Edit this landmark") );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... move
  pqPushButtonMove = new QPushButton( QIcon( ":icons/32x32/move.png" ), "" );
  pqPushButtonMove->setToolTip( tr("Change this landmark's position)") );
  pqPushButtonMove->setMaximumSize( 36, 34 );
  pqPushButtonMove->setEnabled( false );
  pqPushButtonMove->setCheckable( true );
  QWidget::connect( pqPushButtonMove, SIGNAL( toggled(bool) ), this, SLOT( slotMove(bool) ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete this landmark") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... Add route
  pqPushButtonAddRoute = new QPushButton( QIcon( ":icons/32x32/route_add.png" ), "" );
  pqPushButtonAddRoute->setToolTip( tr("Create a new (route) waypoint at the landmark position") );
  pqPushButtonAddRoute->setMaximumSize( 36, 34 );
  pqPushButtonAddRoute->setEnabled( false );
  QWidget::connect( pqPushButtonAddRoute, SIGNAL( clicked() ), this, SLOT( slotAddRoute() ) );
    
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/landmark_point.png" ) );
  __pqLabelIcon->setToolTip( tr("Landmark") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText();
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  pqLabelSymbol = new QLabel();
  pqLabelSymbol->setMaximumSize( 32, 32 );
  pqLabelSymbol->setVisible( false );
  __pqHBoxLayoutHeader->addWidget( pqLabelSymbol );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget();
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... position
  QWidget* __poWidgetPosition = new QWidget();
  QVBoxLayout* __pqVBoxLayoutPosition = new QVBoxLayout();
  __qFontData.setPixelSize( 20 );
  poTextLongitude = new COverlayText();
  poTextLongitude->setToolTip( tr("Longitude") );
  poTextLongitude->setFont( __qFontData );
  poTextLongitude->setIndent( 10 );
  poTextLongitude->setAlignment( Qt::AlignHCenter );
  poTextLongitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLongitude );
  poTextLatitude = new COverlayText();
  poTextLatitude->setToolTip( tr("Latitude") );
  poTextLatitude->setFont( __qFontData );
  poTextLatitude->setIndent( 10 );
  poTextLatitude->setAlignment( Qt::AlignHCenter );
  poTextLatitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLatitude );
  poTextElevation = new COverlayText();
  poTextElevation->setToolTip( tr("Elevation") );
  poTextElevation->setFont( __qFontData );
  poTextElevation->setIndent( 10 );
  poTextElevation->setAlignment( Qt::AlignHCenter );
  poTextElevation->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextElevation, 1 );
  __poWidgetPosition->setLayout( __pqVBoxLayoutPosition );
  __poTabWidget->addTab( __poWidgetPosition, tr("Position") );

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
  poTextDescription->setWordWrap( true );
  poTextDescription->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextDescription );
  __qFontData.setPixelSize( 10 );
  poTextComment = new COverlayText();
  poTextComment->setToolTip( tr("Comment") );
  poTextComment->setFont( __qFontData );
  poTextComment->setIndent( 10 );
  poTextComment->setWordWrap( true );
  poTextComment->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextComment );
  poUrl = new COverlayUrl();
  poUrl->setToolTip( tr("URL") );
  poUrl->setFont( __qFontData );
  poUrl->setIndent( 10 );
  poUrl->setWordWrap( true );
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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonMove, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddRoute, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CLandmarkPointDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CMainWindow* __poMainWindow = QVCTRuntime::useMainWindow();
  CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)poOverlayObject;

  // ... name
  poTextName->setText( __poLandmarkPoint->getName() );
  QString __qsSymbol = __poLandmarkPoint->getSymbol();
  if( __poMainWindow->symbolExists( __qsSymbol ) )
  {
    pqLabelSymbol->setPixmap( __poMainWindow->symbolPixmap( __qsSymbol ) );
    pqLabelSymbol->setVisible( true );
  }
  else
    pqLabelSymbol->setVisible( false );

  // ... position
  if( __poLandmarkPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && __poLandmarkPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    poTextLongitude->setText( CUnitPosition::toString( __poLandmarkPoint->getLongitude(), CUnitPosition::LONGITUDE ) );
    poTextLatitude->setText( CUnitPosition::toString( __poLandmarkPoint->getLatitude(), CUnitPosition::LATITUDE ) );
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
  }

  // ... elevation
  if( __poLandmarkPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    poTextElevation->setText( CUnitElevation::toString( __poLandmarkPoint->getElevation() ) );
  else
    poTextElevation->resetText();

  // ... info
  poTextType->setText( __poLandmarkPoint->getType() );
  poTextDescription->setText( __poLandmarkPoint->getDescription() );
  poTextComment->setText( __poLandmarkPoint->getComment() );
  poUrl->setText( __poLandmarkPoint->getUrl() );
}

void CLandmarkPointDetailView::resetContent()
{
  poTextName->resetText();
  pqLabelSymbol->setVisible( false );
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
  poTextType->resetText();
  poTextDescription->resetText();
  poTextComment->resetText();
  poUrl->resetText();
}

void CLandmarkPointDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonMove->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonAddRoute->setEnabled( true );
  }
}

void CLandmarkPointDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonMove->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonAddRoute->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CLandmarkPointDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)poOverlayObject;
  bool __bVisible = __poLandmarkPoint->isVisible();
  __poLandmarkPoint->toggleVisibility();
  if( __bVisible != __poLandmarkPoint->isVisible() )
  {
    __poLandmarkPoint->setCheckState( CLandmarkOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poLandmarkPoint->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CLandmarkPointDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)poOverlayObject;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poLandmarkPoint );
  QVCTRuntime::useChartTable()->updateChart();
}

void CLandmarkPointDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CLandmarkPoint*)poOverlayObject)->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CLandmarkPointDetailView::slotMove( bool _bEnable )
{
  if( !poOverlayObject ) return;
  pqPushButtonVisible->setEnabled( !_bEnable );
  pqPushButtonEdit->setEnabled( !_bEnable );
  pqPushButtonDelete->setEnabled( !_bEnable );
  pqPushButtonAddRoute->setEnabled( !_bEnable );
  QVCTRuntime::useOverlayListView()->setEnabled( !_bEnable );
  QVCTRuntime::useChartTable()->setOverlayPointMove( _bEnable ? (CLandmarkPoint*)poOverlayObject : 0 );
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CLandmarkPointDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
  CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)((QTreeWidgetItem*)poOverlayObject)->parent();
  CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)poOverlayObject;
  __pqMutexDataChange->lock();
  __poLandmarkContainer->removeChild( __poLandmarkPoint );
  __pqMutexDataChange->unlock();
  delete __poLandmarkPoint;
  QTreeWidgetItem* __pqTreeWidgetItem = __poLandmarkOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poLandmarkOverlay->showDetail( __pqTreeWidgetItem );
  __poLandmarkOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CLandmarkPointDetailView::slotAddRoute()
{
  if( !poOverlayObject ) return;
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  CRouteContainer* __poRouteContainer = __poRouteOverlay->pickContainer();
  if( !__poRouteContainer ) return;
  CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)poOverlayObject;
  QString __qsName = __poRouteContainer->newChildName( __poLandmarkPoint->getName() );
  CRoutePoint* __poRoutePoint = __poRouteContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject, true );
  if( !__poRoutePoint ) return;
  __poRoutePoint->setType( __poLandmarkPoint->getType() );
  __poRoutePoint->setDescription( __poLandmarkPoint->getDescription() );
  __poRoutePoint->setComment( __poLandmarkPoint->getComment() );
  __poRoutePoint->setSymbol( __poLandmarkPoint->getSymbol() );
  __poRoutePoint->setUrl( __poLandmarkPoint->getUrl() );
  __poRouteOverlay->setCurrentItem( __poRoutePoint );
  __poRouteOverlay->forceRedraw();
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __poRoutePoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}
