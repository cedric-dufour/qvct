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
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackSubContainer.hpp"
#include "overlays/track/CTrackSubContainerDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackSubContainerDetailView::CTrackSubContainerDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CTrackSubContainerDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "", this );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setToolTip( tr("Toggle this segment's visibility") );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "", this );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setToolTip( tr("Center chart on this segment's mean position") );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this segment") );
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
  __pqLabelIcon->setToolTip( tr("Segment") );
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
  poTextLengthRL->setToolTip( tr("Length (rhumb-lines along segment)") );
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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( new QLabel(), 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CTrackSubContainerDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)poOverlayObject;
  poTextName->setText( __poTrackSubContainer->getName() );
  poTextContent->setText( QString::number( __poTrackSubContainer->childCount() )+" "+tr("Point(s)") );
  poTextLengthRL->setText( CUnitDistance::toString( __poTrackSubContainer->getLengthRL() ) );
  poTextTimeElapsed->setText( CUnitTimeDelta::toString( __poTrackSubContainer->getTimeElapsed() ) );
}

void CTrackSubContainerDetailView::resetContent()
{
  poTextName->resetText();
  poTextContent->resetText();
  poTextLengthRL->resetText();
  poTextTimeElapsed->resetText();
}

void CTrackSubContainerDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
  }
}

void CTrackSubContainerDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CTrackSubContainerDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)poOverlayObject;
  bool __bVisible = __poTrackSubContainer->isVisible();
  __poTrackSubContainer->toggleVisible();
  if( __bVisible != __poTrackSubContainer->isVisible() )
  {
    __poTrackSubContainer->setCheckState( CTrackOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poTrackSubContainer->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CTrackSubContainerDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CDataPosition __oDataPositionLower, __oDataPositionUpper;
  int __iCount = COverlayPoint::getPositionBox( poOverlayObject, &__oDataPositionLower, &__oDataPositionUpper );
  if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
  else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
}

void CTrackSubContainerDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CTrackOverlay* __poTrackOverlay = (CTrackOverlay*)poOverlayObject->useOverlay();
  CTrackContainer* __poTrackContainer = (CTrackContainer*)poOverlayObject->QTreeWidgetItem::parent();
  CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)poOverlayObject;
  __poTrackContainer->removeChild( __poTrackSubContainer );
  delete __poTrackSubContainer;
  QTreeWidgetItem* __pqTreeWidgetItem = __poTrackOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poTrackOverlay->showDetail( __pqTreeWidgetItem );
  __poTrackOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}
