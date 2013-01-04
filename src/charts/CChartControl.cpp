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

//QT
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QVBoxLayout>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChartControl.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChartControl::CChartControl( QWidget* _pqParent )
  : QWidget( _pqParent )
  , bPointerEnable( true )
{
  constructLayout();
}

void CChartControl::constructLayout()
{
  // Retrieve the chart table
  CChartTable* __poChartTable = QVCTRuntime::useChartTable();

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add controls
  pqPushButtonPositionLock = new QPushButton( QIcon( ":icons/32x32/move_unlock.png" ), "", this );
  pqPushButtonPositionLock->setToolTip( tr("Keep chart position synchronized with other charts") );
  pqPushButtonPositionLock->installEventFilter( __poChartTable );
  pqPushButtonPositionLock->setMaximumSize( 36, 34 );
  pqPushButtonPositionLock->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonPositionLock );
  QWidget::connect( pqPushButtonPositionLock, SIGNAL( toggled(bool) ), this, SLOT( slotPositionLock(bool) ) );

  pqPushButtonScaleLock = new QPushButton( QIcon( ":icons/32x32/zoom_unlock.png" ), "", this );
  pqPushButtonScaleLock->setToolTip( tr("Keep chart scale synchronized with other charts") );
  pqPushButtonScaleLock->installEventFilter( __poChartTable );
  pqPushButtonScaleLock->setMaximumSize( 36, 34 );
  pqPushButtonScaleLock->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonScaleLock );
  QWidget::connect( pqPushButtonScaleLock, SIGNAL( toggled(bool) ), this, SLOT( slotScaleLock(bool) ) );

  QFrame* __pqFrameSeparator1 = new QFrame( this );
  __pqFrameSeparator1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator1 );

  pqPushButtonScaleActual = new QPushButton( QIcon( ":icons/32x32/zoom_actual.png" ), "", this );
  pqPushButtonScaleActual->setToolTip( tr("Match chart scale to screen resolution") );
  pqPushButtonScaleActual->installEventFilter( __poChartTable );
  pqPushButtonScaleActual->setMaximumSize( 36, 34 );
  __pqVBoxLayout->addWidget( pqPushButtonScaleActual );
  QWidget::connect( pqPushButtonScaleActual, SIGNAL( clicked() ), __poChartTable, SLOT( slotScaleActual() ) );

  pqPushButtonScaleIn = new QPushButton( QIcon( ":icons/32x32/zoom_in.png" ), "", this );
  pqPushButtonScaleIn->setToolTip( tr("Zoom chart (decrease scale)") );
  pqPushButtonScaleIn->installEventFilter( __poChartTable );
  pqPushButtonScaleIn->setMaximumSize( 36, 34 );
  __pqVBoxLayout->addWidget( pqPushButtonScaleIn );
  QWidget::connect( pqPushButtonScaleIn, SIGNAL( clicked() ), this, SLOT( slotScaleIn() ) );

  pqSliderScale = new QSlider( Qt::Vertical, this );
  pqSliderScale->installEventFilter( __poChartTable );
  pqSliderScale->setRange( 0, 1000 );
  pqSliderScale->setSingleStep( 1 );
  pqSliderScale->setPageStep( 10 );
  __pqVBoxLayout->addWidget( pqSliderScale, 1 );
  __pqVBoxLayout->setAlignment( pqSliderScale, Qt::AlignHCenter );
  QWidget::connect( pqSliderScale, SIGNAL( valueChanged(int) ), this, SLOT( slotScaleTo(int) ) );

  pqPushButtonScaleOut = new QPushButton( QIcon( ":icons/32x32/zoom_out.png" ), "", this );
  pqPushButtonScaleOut->setToolTip( tr("Unzoom chart (increase scale)") );
  pqPushButtonScaleOut->installEventFilter( __poChartTable );
  pqPushButtonScaleOut->setMaximumSize( 36, 34 );
  __pqVBoxLayout->addWidget( pqPushButtonScaleOut );
  QWidget::connect( pqPushButtonScaleOut, SIGNAL( clicked() ), this, SLOT( slotScaleOut() ) );

  pqPushButtonScaleFit = new QPushButton( QIcon( ":icons/32x32/zoom_fit.png" ), "", this );
  pqPushButtonScaleFit->setToolTip( tr("Adjust scale to display entire chart") );
  pqPushButtonScaleFit->installEventFilter( __poChartTable );
  pqPushButtonScaleFit->setMaximumSize( 36, 34 );
  __pqVBoxLayout->addWidget( pqPushButtonScaleFit );
  QWidget::connect( pqPushButtonScaleFit, SIGNAL( clicked() ), __poChartTable, SLOT( slotScaleFit() ) );

  QFrame* __pqFrameSeparator2 = new QFrame( this );
  __pqFrameSeparator2->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator2 );

  pqPushButtonTarget = new QPushButton( QIcon( ":icons/32x32/target.png" ), "", this );
  pqPushButtonTarget->setToolTip( tr("Enable target destination") );
  pqPushButtonTarget->installEventFilter( __poChartTable );
  pqPushButtonTarget->setMaximumSize( 36, 34 );
  pqPushButtonTarget->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonTarget );
  QWidget::connect( pqPushButtonTarget, SIGNAL( toggled(bool) ), __poChartTable, SLOT( slotPointerTarget(bool) ) );

  pqPushButtonMeasureSingle = new QPushButton( QIcon( ":icons/32x32/measure_single.png" ), "", this );
  pqPushButtonMeasureSingle->setToolTip( tr("Enable single segment measurement(s)") );
  pqPushButtonMeasureSingle->installEventFilter( __poChartTable );
  pqPushButtonMeasureSingle->setMaximumSize( 36, 34 );
  pqPushButtonMeasureSingle->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonMeasureSingle );
  QWidget::connect( pqPushButtonMeasureSingle, SIGNAL( toggled(bool) ), __poChartTable, SLOT( slotPointerPathSingle(bool) ) );

  pqPushButtonMeasure = new QPushButton( QIcon( ":icons/32x32/measure.png" ), "", this );
  pqPushButtonMeasure->setToolTip( tr("Enable ongoing measurements (pointer path)") );
  pqPushButtonMeasure->installEventFilter( __poChartTable );
  pqPushButtonMeasure->setMaximumSize( 36, 34 );
  pqPushButtonMeasure->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonMeasure );
  QWidget::connect( pqPushButtonMeasure, SIGNAL( toggled(bool) ), __poChartTable, SLOT( slotPointerPath(bool) ) );

  QFrame* __pqFrameSeparator3 = new QFrame( this );
  __pqFrameSeparator3->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator3 );

  pqPushButtonElevation = new QPushButton( QIcon( ":icons/32x32/elevation_add.png" ), "", this );
  pqPushButtonElevation->setToolTip( tr("Add, view or hide elevation data") );
  pqPushButtonElevation->installEventFilter( __poChartTable );
  pqPushButtonElevation->setMaximumSize( 36, 34 );
  pqPushButtonElevation->setCheckable( true );
  __pqVBoxLayout->addWidget( pqPushButtonElevation );
  QWidget::connect( pqPushButtonElevation, SIGNAL( toggled(bool) ), this, SLOT( slotElevation(bool) ) );

  QPushButton* __pqPushButtonLoad = new QPushButton( QIcon( ":icons/32x32/chart_load.png" ), "", this );
  __pqPushButtonLoad->setToolTip( tr("Load chart from disk") );
  __pqPushButtonLoad->installEventFilter( __poChartTable );
  __pqPushButtonLoad->setMaximumSize( 36, 34 );
  __pqVBoxLayout->addWidget( __pqPushButtonLoad );
  QWidget::connect( __pqPushButtonLoad, SIGNAL( clicked() ), __poChartTable, SLOT( slotLoadChart() ) );

  // Disable controls (since no chart is loaded yet)
  enableControls( false );

  // Set the layout
  QWidget::setLayout( __pqVBoxLayout );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CChartControl::slotPositionLock( bool _bLock )
{
  pqPushButtonPositionLock->setIcon( QIcon( _bLock ? ":icons/32x32/move_lock.png" : ":icons/32x32/move_unlock.png" ) );
  QVCTRuntime::useChartTable()->lockPosition( _bLock );
}

void CChartControl::slotScaleLock( bool _bLock )
{
  pqPushButtonScaleLock->setIcon( QIcon( _bLock ? ":icons/32x32/zoom_lock.png" : ":icons/32x32/zoom_unlock.png" ) );
  QVCTRuntime::useChartTable()->lockScale( _bLock );
}

void CChartControl::slotScaleTo( int _iValue )
{
  // NOTE: Scale range must is inverted and normalized when used as CChartTable's scale reference
  QVCTRuntime::useChartTable()->setScale( ( 1000 - _iValue ) / 1000.0, false, false );
}

void CChartControl::slotScaleIn()
{
  stepScale( true, true );
}

void CChartControl::slotScaleOut()
{
  stepScale( false, true );
}

void CChartControl::slotElevation( bool _bAddOrShow )
{
  // Retrieve the chart table
  CChartTable* __poChartTable = QVCTRuntime::useChartTable();

  // Manage elevation data
  if( _bAddOrShow )
  {
    if( !__poChartTable->hasElevation() ) __poChartTable->slotAddElevation();
    if( __poChartTable->hasElevation() )
    {
      __poChartTable->showElevation( true );
      pqPushButtonElevation->setIcon( QIcon( ":icons/32x32/elevation_visible.png" ) );
    }
    else
    {
      pqPushButtonElevation->setChecked( false );
    }
  }
  else
  {
    if( __poChartTable->hasElevation() )
    {
      __poChartTable->showElevation( false );
      pqPushButtonElevation->setIcon( QIcon( ":icons/32x32/elevation_hidden.png" ) );
    }
  }
}

void CChartControl::slotElevationAdd()
{
  pqPushButtonElevation->setChecked( true );
}


//
// SETTERS
//

void CChartControl::lockPosition( bool _bLock )
{
  pqPushButtonPositionLock->setChecked( _bLock );
}

void CChartControl::lockScale( bool _bLock )
{
  pqPushButtonScaleLock->setChecked( _bLock );
}

void CChartControl::setScale( double _fdScale )
{
  // NOTE: Scale range must is inverted and normalized when used as the CChartTable's scale reference
  pqSliderScale->setValue( 1000 * ( 1.0 - _fdScale ) );
}

void CChartControl::stepScale( bool _bIncrease, bool _bBigStep )
{
  pqSliderScale->setValue( pqSliderScale->value() + ( _bIncrease ? 1 : -1 ) * ( _bBigStep ? pqSliderScale->pageStep() : pqSliderScale->singleStep() ) );
}

void CChartControl::enableTarget( bool _bEnable )
{
  if( !pqPushButtonTarget->isEnabled() ) return;
  pqPushButtonTarget->setChecked( _bEnable );
}

void CChartControl::enableMeasureSingle( bool _bEnable )
{
  if( !pqPushButtonMeasureSingle->isEnabled() ) return;
  pqPushButtonMeasureSingle->setChecked( _bEnable );
}

void CChartControl::enableMeasure( bool _bEnable )
{
  if( !pqPushButtonMeasure->isEnabled() ) return;
  pqPushButtonMeasure->setChecked( _bEnable );
}


//
// OTHER
//

void CChartControl::enableControls( bool _bEnable )
{
  pqPushButtonPositionLock->setEnabled( _bEnable );
  pqPushButtonScaleLock->setEnabled( _bEnable );
  pqPushButtonScaleActual->setEnabled( _bEnable );
  pqPushButtonScaleIn->setEnabled( _bEnable );
  pqSliderScale->setEnabled( _bEnable );
  pqPushButtonScaleOut->setEnabled( _bEnable );
  pqPushButtonScaleFit->setEnabled( _bEnable );
  pqPushButtonElevation->setEnabled( _bEnable );
  if( !_bEnable )
  {
    pqPushButtonPositionLock->setChecked( false );
    pqPushButtonScaleLock->setChecked( false );
    pqPushButtonElevation->setChecked( false );
    if( !QVCTRuntime::useChartTable()->hasElevation() ) pqPushButtonElevation->setIcon( QIcon( ":icons/32x32/elevation_add.png" ) );
    pqSliderScale->setValue( 0 );
  }
  else
  {
    if( QVCTRuntime::useChartTable()->hasElevation() ) pqPushButtonElevation->setIcon( QIcon( ":icons/32x32/elevation_hidden.png" ) );
  }
  if( !_bEnable || bPointerEnable )
  {
    if( !_bEnable ) QVCTRuntime::useChartTable()->enablePointerPath( false );
    pqPushButtonTarget->setEnabled( _bEnable );
    pqPushButtonMeasureSingle->setEnabled( _bEnable );
    pqPushButtonMeasure->setEnabled( _bEnable );
  }
}

void CChartControl::enablePointer( bool _bEnable )
{
  bPointerEnable = _bEnable;
  if( !bPointerEnable ) QVCTRuntime::useChartTable()->enablePointerPath( false );
  pqPushButtonTarget->setEnabled( bPointerEnable );
  pqPushButtonMeasureSingle->setEnabled( bPointerEnable );
  pqPushButtonMeasure->setEnabled( bPointerEnable );
}
