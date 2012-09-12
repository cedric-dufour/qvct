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
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QStringList>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "settings/CSettingsUnitsEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CSettingsUnitsEditView::CSettingsUnitsEditView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CSettingsUnitsEditView::constructLayout()
{
  // Retrieve user settings
  CSettings* __poSettings = QVCTRuntime::useSettings();

  // Create layout
  QGridLayout* __pqGridLayout = new QGridLayout( this );

  // Add items
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Unit")+"</I>" ), 0, 1 );
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Precision")+"</I>" ), 0, 2 );
  int __iRow = 0;

  // ... date unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Date")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitDate = new QComboBox( this );
  __pqComboBoxUnitDate->addItems( QStringList( CUnitDate::symbols().values() ) );
  __pqComboBoxUnitDate->setCurrentIndex( __pqComboBoxUnitDate->findText( CUnitDate::toSymbol( __poSettings->getUnitDate() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitDate, __iRow, 1 );
  connect( __pqComboBoxUnitDate, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitDate(const QString) ) );

  // ... time unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Time")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitTime = new QComboBox( this );
  __pqComboBoxUnitTime->addItems( QStringList( CUnitTime::symbols().values() ) );
  __pqComboBoxUnitTime->setCurrentIndex( __pqComboBoxUnitTime->findText( CUnitTime::toSymbol( __poSettings->getUnitTime() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitTime, __iRow, 1 );
  connect( __pqComboBoxUnitTime, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitTime(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionTime = new QSpinBox( this );
  __pqSpinBoxPrecisionTime->setRange( 0, 6 );
  __pqSpinBoxPrecisionTime->setValue( __poSettings->getPrecisionTime() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionTime, __iRow, 2 );
  connect( __pqSpinBoxPrecisionTime, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionTime(int) ) );

  // ... time difference unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Duration")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitTimeDelta = new QComboBox( this );
  __pqComboBoxUnitTimeDelta->addItems( QStringList( CUnitTimeDelta::symbols().values() ) );
  __pqComboBoxUnitTimeDelta->setCurrentIndex( __pqComboBoxUnitTimeDelta->findText( CUnitTimeDelta::toSymbol( __poSettings->getUnitTimeDelta() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitTimeDelta, __iRow, 1 );
  connect( __pqComboBoxUnitTimeDelta, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitTimeDelta(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionTimeDelta = new QSpinBox( this );
  __pqSpinBoxPrecisionTimeDelta->setRange( 0, 6 );
  __pqSpinBoxPrecisionTimeDelta->setValue( __poSettings->getPrecisionTimeDelta() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionTimeDelta, __iRow, 2 );
  connect( __pqSpinBoxPrecisionTimeDelta, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionTimeDelta(int) ) );

  // ... timezone unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Timezone")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitTimeZone = new QComboBox( this );
  __pqComboBoxUnitTimeZone->addItems( QStringList( CUnitTimeZone::symbols().values() ) );
  __pqComboBoxUnitTimeZone->setCurrentIndex( __pqComboBoxUnitTimeZone->findText( CUnitTimeZone::toSymbol( __poSettings->getUnitTimeZone() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitTimeZone, __iRow, 1 );
  connect( __pqComboBoxUnitTimeZone, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitTimeZone(const QString) ) );

  // ... position unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Position")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitPosition = new QComboBox( this );
  __pqComboBoxUnitPosition->addItems( QStringList( CUnitPosition::symbols().values() ) );
  __pqComboBoxUnitPosition->setCurrentIndex( __pqComboBoxUnitPosition->findText( CUnitPosition::toSymbol( __poSettings->getUnitPosition() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitPosition, __iRow, 1 );
  connect( __pqComboBoxUnitPosition, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitPosition(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionPosition = new QSpinBox( this );
  __pqSpinBoxPrecisionPosition->setRange( 0, 6 );
  __pqSpinBoxPrecisionPosition->setValue( __poSettings->getPrecisionPosition() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionPosition, __iRow, 2 );
  connect( __pqSpinBoxPrecisionPosition, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionPosition(int) ) );

  // ... bearing unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Bearing")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitBearing = new QComboBox( this );
  __pqComboBoxUnitBearing->addItems( QStringList( CUnitBearing::symbols().values() ) );
  __pqComboBoxUnitBearing->setCurrentIndex( __pqComboBoxUnitBearing->findText( CUnitBearing::toSymbol( __poSettings->getUnitBearing() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitBearing, __iRow, 1 );
  connect( __pqComboBoxUnitBearing, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitBearing(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionBearing = new QSpinBox( this );
  __pqSpinBoxPrecisionBearing->setRange( 0, 3 );
  __pqSpinBoxPrecisionBearing->setValue( __poSettings->getPrecisionBearing() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionBearing, __iRow, 2 );
  connect( __pqSpinBoxPrecisionBearing, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionBearing(int) ) );

  // ... distance unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Distance")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitDistance = new QComboBox( this );
  __pqComboBoxUnitDistance->addItems( QStringList( CUnitDistance::symbols().values() ) );
  __pqComboBoxUnitDistance->setCurrentIndex( __pqComboBoxUnitDistance->findText( CUnitDistance::toSymbol( __poSettings->getUnitDistance() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitDistance, __iRow, 1 );
  connect( __pqComboBoxUnitDistance, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitDistance(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionDistance = new QSpinBox( this );
  __pqSpinBoxPrecisionDistance->setRange( 0, 3 );
  __pqSpinBoxPrecisionDistance->setValue( __poSettings->getPrecisionDistance() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionDistance, __iRow, 2 );
  connect( __pqSpinBoxPrecisionDistance, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionDistance(int) ) );

  // ... speed unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Speed (horizontal)")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitSpeed = new QComboBox( this );
  __pqComboBoxUnitSpeed->addItems( QStringList( CUnitSpeed::symbols().values() ) );
  __pqComboBoxUnitSpeed->setCurrentIndex( __pqComboBoxUnitSpeed->findText( CUnitSpeed::toSymbol( __poSettings->getUnitSpeed() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitSpeed, __iRow, 1 );
  connect( __pqComboBoxUnitSpeed, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitSpeed(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionSpeed = new QSpinBox( this );
  __pqSpinBoxPrecisionSpeed->setRange( 0, 3 );
  __pqSpinBoxPrecisionSpeed->setValue( __poSettings->getPrecisionSpeed() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionSpeed, __iRow, 2 );
  connect( __pqSpinBoxPrecisionSpeed, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionSpeed(int) ) );

  // ... elevation unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Elevation")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitElevation = new QComboBox( this );
  __pqComboBoxUnitElevation->addItems( QStringList( CUnitElevation::symbols().values() ) );
  __pqComboBoxUnitElevation->setCurrentIndex( __pqComboBoxUnitElevation->findText( CUnitElevation::toSymbol( __poSettings->getUnitElevation() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitElevation, __iRow, 1 );
  connect( __pqComboBoxUnitElevation, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitElevation(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionElevation = new QSpinBox( this );
  __pqSpinBoxPrecisionElevation->setRange( 0, 3 );
  __pqSpinBoxPrecisionElevation->setValue( __poSettings->getPrecisionElevation() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionElevation, __iRow, 2 );
  connect( __pqSpinBoxPrecisionElevation, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionElevation(int) ) );

  // ... vertical speed unit/precision
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Speed (vertical)")+":" ), __iRow, 0 );
  QComboBox* __pqComboBoxUnitSpeedVertical = new QComboBox( this );
  __pqComboBoxUnitSpeedVertical->addItems( QStringList( CUnitSpeedVertical::symbols().values() ) );
  __pqComboBoxUnitSpeedVertical->setCurrentIndex( __pqComboBoxUnitSpeedVertical->findText( CUnitSpeedVertical::toSymbol( __poSettings->getUnitSpeedVertical() ) ) );
  __pqGridLayout->addWidget( __pqComboBoxUnitSpeedVertical, __iRow, 1 );
  connect( __pqComboBoxUnitSpeedVertical, SIGNAL( currentIndexChanged(const QString) ), __poSettings, SLOT( slotUnitSpeedVertical(const QString) ) );
  QSpinBox* __pqSpinBoxPrecisionSpeedVertical = new QSpinBox( this );
  __pqSpinBoxPrecisionSpeedVertical->setRange( 0, 3 );
  __pqSpinBoxPrecisionSpeedVertical->setValue( __poSettings->getPrecisionSpeedVertical() );
  __pqGridLayout->addWidget( __pqSpinBoxPrecisionSpeedVertical, __iRow, 2 );
  connect( __pqSpinBoxPrecisionSpeedVertical, SIGNAL( valueChanged(int) ), __poSettings, SLOT( slotPrecisionSpeedVertical(int) ) );

  // Set the layout
  __iRow++;
  __pqGridLayout->addItem( new QSpacerItem( 0, this->height() ), __iRow, 0, 3 );
  QWidget::setLayout( __pqGridLayout );
}
