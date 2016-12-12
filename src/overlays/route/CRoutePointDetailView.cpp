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
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/route/CRoutePointDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRoutePointDetailView::CRoutePointDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CRoutePointDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "" );
  pqPushButtonVisible->setToolTip( tr("Toggle this waypoint's visibility") );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "" );
  pqPushButtonCenter->setToolTip( tr("Center chart on this waypoint") );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "" );
  pqPushButtonEdit->setToolTip( tr("Edit this waypoint") );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... move
  pqPushButtonMove = new QPushButton( QIcon( ":icons/32x32/move.png" ), "" );
  pqPushButtonMove->setToolTip( tr("Change this waypoint's position)") );
  pqPushButtonMove->setMaximumSize( 36, 34 );
  pqPushButtonMove->setEnabled( false );
  pqPushButtonMove->setCheckable( true );
  QWidget::connect( pqPushButtonMove, SIGNAL( toggled(bool) ), this, SLOT( slotMove(bool) ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete this waypoint") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... Add landmark
  pqPushButtonAddLandmark = new QPushButton( QIcon( ":icons/32x32/landmark_add.png" ), "" );
  pqPushButtonAddLandmark->setToolTip( tr("Create a new landmark at the waypoint position") );
  pqPushButtonAddLandmark->setMaximumSize( 36, 34 );
  pqPushButtonAddLandmark->setEnabled( false );
  QWidget::connect( pqPushButtonAddLandmark, SIGNAL( clicked() ), this, SLOT( slotAddLandmark() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/route_point.png" ) );
  __pqLabelIcon->setToolTip( tr("Waypoint") );
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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddLandmark, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CRoutePointDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CMainWindow* __poMainWindow = QVCTRuntime::useMainWindow();
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;

  // ... name
  poTextName->setText( __poRoutePoint->getName() );
  QString __qsSymbol = __poRoutePoint->getSymbol();
  if( __poMainWindow->symbolExists( __qsSymbol ) )
  {
    pqLabelSymbol->setPixmap( __poMainWindow->symbolPixmap( __qsSymbol ) );
    pqLabelSymbol->setVisible( true );
  }
  else
    pqLabelSymbol->setVisible( false );

  // ... position
  if( __poRoutePoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && __poRoutePoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    poTextLongitude->setText( CUnitPosition::toString( __poRoutePoint->getLongitude(), CUnitPosition::LONGITUDE ) );
    poTextLatitude->setText( CUnitPosition::toString( __poRoutePoint->getLatitude(), CUnitPosition::LATITUDE ) );
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
  }

  // ... elevation
  if( __poRoutePoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    poTextElevation->setText( CUnitElevation::toString( __poRoutePoint->getElevation() ) );
  else
    poTextElevation->resetText();

  // ... info
  poTextType->setText( __poRoutePoint->getType() );
  poTextDescription->setText( __poRoutePoint->getDescription() );
  poTextComment->setText( __poRoutePoint->getComment() );
  poUrl->setText( __poRoutePoint->getUrl() );
}

void CRoutePointDetailView::resetContent()
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

void CRoutePointDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonMove->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonAddLandmark->setEnabled( true );
  }
}

void CRoutePointDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonMove->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonAddLandmark->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CRoutePointDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;
  bool __bVisible = __poRoutePoint->isVisible();
  __poRoutePoint->toggleVisibility();
  if( __bVisible != __poRoutePoint->isVisible() )
  {
    __poRoutePoint->setCheckState( CRouteOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poRoutePoint->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CRoutePointDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poRoutePoint );
  QVCTRuntime::useChartTable()->updateChart();
}

void CRoutePointDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CRoutePoint*)poOverlayObject)->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CRoutePointDetailView::slotMove( bool _bEnable )
{
  if( !poOverlayObject ) return;
  pqPushButtonVisible->setEnabled( !_bEnable );
  pqPushButtonEdit->setEnabled( !_bEnable );
  pqPushButtonDelete->setEnabled( !_bEnable );
  pqPushButtonAddLandmark->setEnabled( !_bEnable );
  QVCTRuntime::useOverlayListView()->setEnabled( !_bEnable );
  QVCTRuntime::useChartTable()->setOverlayPointMove( _bEnable ? (CRoutePoint*)poOverlayObject : 0 );
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CRoutePointDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  CRouteContainer* __poRouteContainer = (CRouteContainer*)((QTreeWidgetItem*)poOverlayObject)->parent();
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;
  __pqMutexDataChange->lock();
  __poRouteContainer->removeChild( __poRoutePoint );
  __pqMutexDataChange->unlock();
  delete __poRoutePoint;
  QTreeWidgetItem* __pqTreeWidgetItem = __poRouteOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poRouteOverlay->showDetail( __pqTreeWidgetItem );
  __poRouteOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CRoutePointDetailView::slotAddLandmark()
{
  if( !poOverlayObject ) return;
  CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
  CLandmarkContainer* __poLandmarkContainer = __poLandmarkOverlay->pickContainer();
  if( !__poLandmarkContainer ) return;
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;
  QString __qsName = __poLandmarkContainer->newChildName( __poRoutePoint->getName() );
  CLandmarkPoint* __poLandmarkPoint = __poLandmarkContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject );
  if( !__poLandmarkPoint ) return;
  __poLandmarkPoint->setType( __poRoutePoint->getType() );
  __poLandmarkPoint->setDescription( __poRoutePoint->getDescription() );
  __poLandmarkPoint->setComment( __poRoutePoint->getComment() );
  __poLandmarkPoint->setSymbol( __poRoutePoint->getSymbol() );
  __poLandmarkPoint->setUrl( __poRoutePoint->getUrl() );
  __poLandmarkOverlay->setCurrentItem( __poLandmarkPoint );
  __poLandmarkOverlay->forceRedraw();
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __poLandmarkPoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}
