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
#include <QLineEdit>
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
  __pqSpinBoxScreenDpi->setToolTip( tr("Manually set the screen resolution for optimal rendering/sizing, in DPI") );
  __pqSpinBoxScreenDpi->setRange( 1, 1200 );
  __pqSpinBoxScreenDpi->setValue( __poSettings->getScreenDpi() );
  __pqGridLayout->addWidget( __pqSpinBoxScreenDpi, __iRow, 1 );
  connect( __pqSpinBoxScreenDpi, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotScreenDpi(int) ) );

  // ... screen gestures
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Screen Gestures")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxScreenGestures = new QCheckBox( this );
  __pqCheckBoxScreenGestures->setCheckState( __poSettings->isScreenGestures() ? Qt::Checked : Qt::Unchecked );
  __pqCheckBoxScreenGestures->setToolTip( tr("Whether to enable screen gestures, like pinch-to-zoom\nWARNING: this may lead to unresponsive interface on incompatible devices!") );
  __pqGridLayout->addWidget( __pqCheckBoxScreenGestures, __iRow, 1 );
  connect( __pqCheckBoxScreenGestures, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotScreenGestures(int) ) );

  // ... refresh rate
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Refresh Rate (detail)")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxRateRefresh = new QSpinBox( this );
  __pqSpinBoxRateRefresh->setToolTip( tr("How often to refresh the interface content (chart, overlays, textual data, etc.), in milli-seconds [ms]\nWARNING: the lower this setting, the higher the CPU usage!") );
  __pqSpinBoxRateRefresh->setRange( 100, 5000 );
  __pqSpinBoxRateRefresh->setValue( __poSettings->getRateRefresh() );
  __pqGridLayout->addWidget( __pqSpinBoxRateRefresh, __iRow, 1 );
  connect( __pqSpinBoxRateRefresh, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotRateRefresh(int) ) );

  // ... redraw rate
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Redraw Rate (chart)")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxRateRedraw = new QSpinBox( this );
  __pqSpinBoxRateRedraw->setToolTip( tr("How often to inconditionally redraw the charted content (chart and overlays), in seconds [s]\nWARNING: the lower this setting, the higher the CPU usage!") );
  __pqSpinBoxRateRedraw->setRange( 1, 300 );
  __pqSpinBoxRateRedraw->setValue( __poSettings->getRateRedraw() );
  __pqGridLayout->addWidget( __pqSpinBoxRateRedraw, __iRow, 1 );
  connect( __pqSpinBoxRateRedraw, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotRateRedraw(int) ) );

  // ... chart opacity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Chart Opacity")+":" ), __iRow, 0 );
  QSpinBox* __pqSpinBoxChartOpacity = new QSpinBox( this );
  __pqSpinBoxChartOpacity->setToolTip( tr("Chart opacity/dimming (to improve overlays contrast), in percent [%]") );
  __pqSpinBoxChartOpacity->setRange( 0, 100 );
  __pqSpinBoxChartOpacity->setValue( __poSettings->getChartOpacity() );
  __pqGridLayout->addWidget( __pqSpinBoxChartOpacity, __iRow, 1 );
  connect( __pqSpinBoxChartOpacity, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotChartOpacity(int) ) );

  // ... symbols path
  // TODO: Use a QFileDialog for user-friendliness
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Symbols Path")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditPathSymbolsDirectory = new QLineEdit( this );
  __pqLineEditPathSymbolsDirectory->setToolTip( tr("Directory path to symbols (PNG files) library\nNOTE: Symbol names (used in landmark, waypoint or vessel definition)\nwill be lower-cased and space-stripped before a library match is looked for") );
  __pqLineEditPathSymbolsDirectory->setText( __poSettings->getPathSymbolsDirectory() );
  __pqGridLayout->addWidget( __pqLineEditPathSymbolsDirectory, __iRow, 1 );
  connect( __pqLineEditPathSymbolsDirectory, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotPathSymbolsDirectory(const QString&) ) );

  // ... symbols visibility
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Symbols Visibility")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxVisibleSymbols = new QCheckBox( this );
  __pqCheckBoxVisibleSymbols->setToolTip( tr("Whether to draw the symbols associated with landmarks, waypoints or vessels") );
  __pqCheckBoxVisibleSymbols->setCheckState( __poSettings->isVisibleSymbols() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( __pqCheckBoxVisibleSymbols, __iRow, 1 );
  connect( __pqCheckBoxVisibleSymbols, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotVisibleSymbols(int) ) );

  // ... printing resolution
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("High-Resolution Printing")+":" ), __iRow, 0 );
  QCheckBox* __pqCheckBoxPrintHighRes = new QCheckBox( this );
  __pqCheckBoxPrintHighRes->setToolTip( tr("Whether to print charted content in high-resolution (1200 DPI) or screen resolution (see above Screen Resolution)\nWARNING: high-resolution prints may take very long to reach your printer!") );
  __pqCheckBoxPrintHighRes->setCheckState( __poSettings->isPrintHighRes() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( __pqCheckBoxPrintHighRes, __iRow, 1 );
  connect( __pqCheckBoxPrintHighRes, SIGNAL( stateChanged(int) ), __poSettings, SLOT( slotPrintHighRes(int) ) );

  // Set the layout
  __iRow++;
  __pqGridLayout->addItem( new QSpacerItem( 0, this->height() ), __iRow, 0, 3 );
  QWidget::setLayout( __pqGridLayout );
}
