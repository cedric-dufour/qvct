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

// C/C++
#include <algorithm>

// QT
#include <QFileInfo>
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
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/track/CTrackContainerDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackContainerDetailView::CTrackContainerDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CTrackContainerDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "", this );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setToolTip( tr("Toggle this track's visibility") );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "", this );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setToolTip( tr("Center chart on this track's mean position") );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... save
  pqPushButtonSave = new QPushButton( QIcon( ":icons/32x32/save.png" ), "", this );
  pqPushButtonSave->setMaximumSize( 36, 34 );
  pqPushButtonSave->setToolTip( tr("Save this track to disk") );
  pqPushButtonSave->setEnabled( false );
  QWidget::connect( pqPushButtonSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "", this );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setToolTip( tr("Edit this track") );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this track") );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/track.png" ) );
  __pqLabelIcon->setToolTip( tr("Track") );
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
  poTextContent->setToolTip( tr("Points count") );
  poTextContent->setFont( __qFontData );
  poTextContent->setIndent( 10 );
  poTextContent->setAlignment( Qt::AlignHCenter );
  poTextContent->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextContent, 0 );
  poTextLengthRL = new COverlayText( this );
  poTextLengthRL->setToolTip( tr("Length (rhumb-lines along track)") );
  poTextLengthRL->setFont( __qFontData );
  poTextLengthRL->setIndent( 10 );
  poTextLengthRL->setAlignment( Qt::AlignHCenter );
  poTextLengthRL->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextLengthRL, 0 );
  poTextTimeElapsed = new COverlayText( this );
  poTextTimeElapsed->setToolTip( tr("Elapsed time") );
  poTextTimeElapsed->setFont( __qFontData );
  poTextTimeElapsed->setIndent( 10 );
  poTextTimeElapsed->setAlignment( Qt::AlignHCenter );
  poTextTimeElapsed->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextTimeElapsed, 1 );
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
  __pqHBoxLayoutButtons->addWidget( new QLabel(), 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CTrackContainerDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CTrackContainer* __poTrackContainer = (CTrackContainer*)poOverlayObject;
  int __iCountPoints = 0;
  int __iCount = __poTrackContainer->childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    __iCountPoints += __poTrackContainer->child( __i )->childCount();
  poTextName->setText( __poTrackContainer->getName() );
  poTextContent->setText( QString::number( __iCountPoints )+" "+tr("Point(s)") );
  poTextLengthRL->setText( CUnitDistance::toString( __poTrackContainer->getLengthRL() ) );
  poTextTimeElapsed->setText( CUnitTimeDelta::toString( __poTrackContainer->getTimeElapsed() ) );
  poTextType->setText( __poTrackContainer->getType() );
  poTextDescription->setText( __poTrackContainer->getDescription() );
  poTextComment->setText( __poTrackContainer->getComment() );
  poUrl->setText( __poTrackContainer->getUrl() );
}

void CTrackContainerDetailView::resetContent()
{
  poTextName->resetText();
  poTextContent->resetText();
  poTextLengthRL->resetText();
  poTextTimeElapsed->resetText();
  poTextType->resetText();
  poTextDescription->resetText();
  poTextComment->resetText();
  poUrl->resetText();
}

void CTrackContainerDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonSave->setEnabled( true );
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
  }
}

void CTrackContainerDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonSave->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CTrackContainerDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CTrackContainer* __poTrackContainer = (CTrackContainer*)poOverlayObject;
  bool __bVisible = __poTrackContainer->isVisible();
  __poTrackContainer->toggleVisible();
  if( __bVisible != __poTrackContainer->isVisible() )
  {
    __poTrackContainer->setCheckState( CTrackOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poTrackContainer->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CTrackContainerDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CDataPosition __oDataPositionLower( 180, 90, 999999 ), __oDataPositionUpper( -180, -90, -999999 );
  int __iCountBox = 0;
  int __iCount = poOverlayObject->childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CDataPosition __oDataPositionLower_aux, __oDataPositionUpper_aux;
    int __iCountBox_aux = COverlayPoint::getPositionBox( (COverlayObject*)poOverlayObject->child( __i ), &__oDataPositionLower_aux, &__oDataPositionUpper_aux );
    if( __iCountBox_aux )
    {
      __iCountBox += __iCountBox_aux;
      __oDataPositionLower.setPosition( std::min( __oDataPositionLower.getLongitude(), __oDataPositionLower_aux.getLongitude() ),
                                       std::min( __oDataPositionLower.getLatitude(), __oDataPositionLower_aux.getLatitude() ),
                                       std::min( __oDataPositionLower.getElevation(), __oDataPositionLower_aux.getElevation() ) );
      __oDataPositionUpper.setPosition( std::max( __oDataPositionUpper.getLongitude(), __oDataPositionUpper_aux.getLongitude() ),
                                       std::max( __oDataPositionUpper.getLatitude(), __oDataPositionUpper_aux.getLatitude() ),
                                       std::max( __oDataPositionUpper.getElevation(), __oDataPositionUpper_aux.getElevation() ) );
    }
  }
  if( __iCountBox > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
  else if( __iCountBox == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
  QVCTRuntime::useChartTable()->updateChart();
}

void CTrackContainerDetailView::slotSave()
{
  if( !poOverlayObject ) return;
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Track"), tr("GPX Files")+" (*.gpx);;"+tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  QFileInfo __qFileInfo( __qsFilename );
  if( __qFileInfo.suffix().isEmpty() ) __qsFilename += ".qvct";
  QStringList __qsListExtensions; __qsListExtensions << "qvct" << "gpx";
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename, &__qsListExtensions ) ) return;
  QVCTRuntime::useTrackOverlay()->save( __qsFilename, (CTrackContainer*)poOverlayObject );
}

void CTrackContainerDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CTrackContainer*)poOverlayObject)->showEdit();
}

void CTrackContainerDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CTrackOverlay* __poTrackOverlay = (CTrackOverlay*)poOverlayObject->useOverlay();
  CTrackContainer* __poTrackContainer = (CTrackContainer*)poOverlayObject;
  __poTrackOverlay->removeChild( __poTrackContainer );
  delete __poTrackContainer;
  QTreeWidgetItem* __pqTreeWidgetItem = __poTrackOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poTrackOverlay->showDetail( __pqTreeWidgetItem );
  __poTrackOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}
