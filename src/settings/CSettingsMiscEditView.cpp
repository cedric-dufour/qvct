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
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSpinBox>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "settings/CSettingsMiscEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CSettingsMiscEditView::CSettingsMiscEditView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CSettingsMiscEditView::constructLayout()
{
  // Retrieve user settings
  CSettings* __poSettings = QVCTRuntime::useSettings();

  // Create layout
  QGridLayout* __pqGridLayout = new QGridLayout( this );

  // Add items
  int __iRow = -1;

  // ... screen resolution (DPI)
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Screen Resolution")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxScreenDpi = new QSpinBox( this );
  __pqSpinBoxScreenDpi->setToolTip( "[DPI]" );
  __pqSpinBoxScreenDpi->setRange( 1, 1200 );
  __pqSpinBoxScreenDpi->setValue( __poSettings->getScreenDpi() );
  __pqGridLayout->addWidget( __pqSpinBoxScreenDpi, __iRow, 1 );
  connect( __pqSpinBoxScreenDpi, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotScreenDpi(int) ) );

  // ... screen gestures
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Screen Gestures")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxScreenGestures = new QCheckBox( this );
  __pqCheckBoxScreenGestures->setCheckState( __poSettings->isScreenGestures() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( __pqCheckBoxScreenGestures, __iRow, 1 );
  connect( __pqCheckBoxScreenGestures, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotScreenGestures(int) ) );
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("WARNING: this leads to unresponsive interface on (evdev) touchscreens!")+"</I>" ), __iRow, 1 );

  // ... refresh rate
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Refresh Rate (detail)")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxRateRefresh = new QSpinBox( this );
  __pqSpinBoxRateRefresh->setToolTip( "[ms]" );
  __pqSpinBoxRateRefresh->setRange( 100, 5000 );
  __pqSpinBoxRateRefresh->setValue( __poSettings->getRateRefresh() );
  __pqGridLayout->addWidget( __pqSpinBoxRateRefresh, __iRow, 1 );
  connect( __pqSpinBoxRateRefresh, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotRateRefresh(int) ) );
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("WARNING: setting too low affects performances!")+"</I>" ), __iRow, 1 );

  // ... redraw rate
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Redraw Rate (chart)")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxRateRedraw = new QSpinBox( this );
  __pqSpinBoxRateRedraw->setToolTip( "[s]" );
  __pqSpinBoxRateRedraw->setRange( 1, 300 );
  __pqSpinBoxRateRedraw->setValue( __poSettings->getRateRedraw() );
  __pqGridLayout->addWidget( __pqSpinBoxRateRedraw, __iRow, 1 );
  connect( __pqSpinBoxRateRedraw, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotRateRedraw(int) ) );
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("WARNING: setting too low affects performances!")+"</I>" ), __iRow, 1 );

  // ... chart opacity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Chart Opacity")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxChartOpacity = new QSpinBox( this );
  __pqSpinBoxChartOpacity->setRange( 0, 100 );
  __pqSpinBoxChartOpacity->setValue( __poSettings->getChartOpacity() );
  __pqGridLayout->addWidget( __pqSpinBoxChartOpacity, __iRow, 1 );
  connect( __pqSpinBoxChartOpacity, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotChartOpacity(int) ) );

  // ... symbols visibility
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Symbols Visibility")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxVisibleSymbols = new QCheckBox( this );
  __pqCheckBoxVisibleSymbols->setCheckState( __poSettings->isVisibleSymbols() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( __pqCheckBoxVisibleSymbols, __iRow, 1 );
  connect( __pqCheckBoxVisibleSymbols, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotVisibleSymbols(int) ) );

  // ... printing resolution
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("High-Resolution Printing")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxPrintHighRes = new QCheckBox( this );
  __pqCheckBoxPrintHighRes->setCheckState( __poSettings->isPrintHighRes() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( __pqCheckBoxPrintHighRes, __iRow, 1 );
  connect( __pqCheckBoxPrintHighRes, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotPrintHighRes(int) ) );
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("WARNING: enabling leads to very long print delay!")+"</I>" ), __iRow, 1 );

  // Set the layout
  __iRow++;
  __pqGridLayout->addItem( new QSpacerItem( 0, this->height() ), __iRow, 0, 3 );
  QWidget::setLayout( __pqGridLayout );
}
