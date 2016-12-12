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
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "settings/CSettingsValidityEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CSettingsValidityEditView::CSettingsValidityEditView( QWidget* _pqParent )
  : QWidget( _pqParent )
{
  constructLayout();
}

void CSettingsValidityEditView::constructLayout()
{
  // Retrieve user settings
  CSettings* __poSettings = QVCTRuntime::useSettings();

  // Create layout
  QGridLayout* __pqGridLayout = new QGridLayout();

  // Add items
  QLabel* __pqLabelMinVariation = new QLabel( "<I>"+tr("Min. Variation")+"</I>" );
  __pqLabelMinVariation->setToolTip( tr("Minimum value variation (or speed) to consider for positioning/tracking data update") );
  __pqGridLayout->addWidget( __pqLabelMinVariation, 0, 1 );
  QLabel* __pqLabelMaxError = new QLabel( "<I>"+tr("Max. Error")+"</I>" );
  __pqLabelMaxError->setToolTip( tr("Maximum error to tolerate before positioning data are reported as invalid (italic text, dashed marker)") );
  __pqGridLayout->addWidget( __pqLabelMaxError, 0, 2 );
  QLabel* __pqLabelMaxAge = new QLabel( "<I>"+tr("Max. Age")+"</I>" );
  __pqLabelMaxAge->setToolTip( tr("Maximum age to tolerate before positioning data are reported as invalid (italic text, dashed marker)") );  __pqGridLayout->addWidget( __pqLabelMaxAge, 0, 3 );
  int __iRow = 0;

  // ... time validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Time")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMaxErrorTime = new QLineEdit();
  __pqLineEditMaxErrorTime->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxErrorTime->setInputMask( "09.99" );
  __pqLineEditMaxErrorTime->setText( QString::number( __poSettings->getMaxErrorTime(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorTime, __iRow, 2 );
  connect( __pqLineEditMaxErrorTime, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorTime(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgeTime = new QLineEdit();
  __pqLineEditMaxAgeTime->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgeTime->setInputMask( "09.99" );
  __pqLineEditMaxAgeTime->setText( QString::number( __poSettings->getMaxAgeTime(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgeTime, __iRow, 3 );
  connect( __pqLineEditMaxAgeTime, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgeTime(const QString&) ) );

  // ... position validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Position")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMinValuePosition = new QLineEdit();
  __pqLineEditMinValuePosition->setToolTip( tr("in meters [m]") );
  __pqLineEditMinValuePosition->setInputMask( "009.99" );
  __pqLineEditMinValuePosition->setText( QString::number( __poSettings->getMinValuePosition(), 'f', 2 ).prepend("  ").right(6) );
  __pqGridLayout->addWidget( __pqLineEditMinValuePosition, __iRow, 1 );
  connect( __pqLineEditMinValuePosition, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMinValuePosition(const QString&) ) );
  QLineEdit* __pqLineEditMaxErrorPosition = new QLineEdit();
  __pqLineEditMaxErrorPosition->setToolTip( tr("in meters [m]") );
  __pqLineEditMaxErrorPosition->setInputMask( "009.99" );
  __pqLineEditMaxErrorPosition->setText( QString::number( __poSettings->getMaxErrorPosition(), 'f', 2 ).prepend("  ").right(6) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorPosition, __iRow, 2 );
  connect( __pqLineEditMaxErrorPosition, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorPosition(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgePosition = new QLineEdit();
  __pqLineEditMaxAgePosition->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgePosition->setInputMask( "09.99" );
  __pqLineEditMaxAgePosition->setText( QString::number( __poSettings->getMaxAgePosition(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgePosition, __iRow, 3 );
  connect( __pqLineEditMaxAgePosition, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgePosition(const QString&) ) );

  // ... elevation validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Elevation")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMaxErrorElevation = new QLineEdit();
  __pqLineEditMaxErrorElevation->setToolTip( tr("in meters [m]") );
  __pqLineEditMaxErrorElevation->setInputMask( "009.99" );
  __pqLineEditMaxErrorElevation->setText( QString::number( __poSettings->getMaxErrorElevation(), 'f', 2 ).prepend("  ").right(6) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorElevation, __iRow, 2 );
  connect( __pqLineEditMaxErrorElevation, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorElevation(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgeElevation = new QLineEdit();
  __pqLineEditMaxAgeElevation->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgeElevation->setInputMask( "09.99" );
  __pqLineEditMaxAgeElevation->setText( QString::number( __poSettings->getMaxAgeElevation(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgeElevation, __iRow, 3 );
  connect( __pqLineEditMaxAgeElevation, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgeElevation(const QString&) ) );

  // ... bearing validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Bearing")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMinValueBearing = new QLineEdit();
  __pqLineEditMinValueBearing->setToolTip( tr("in degrees [deg]") );
  __pqLineEditMinValueBearing->setInputMask( "009.99" );
  __pqLineEditMinValueBearing->setText( QString::number( __poSettings->getMinValueBearing(), 'f', 2 ).prepend("  ").right(6) );
  __pqGridLayout->addWidget( __pqLineEditMinValueBearing, __iRow, 1 );
  connect( __pqLineEditMinValueBearing, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMinValueBearing(const QString&) ) );
  QLineEdit* __pqLineEditMaxErrorBearing = new QLineEdit();
  __pqLineEditMaxErrorBearing->setToolTip( tr("in degrees [deg]") );
  __pqLineEditMaxErrorBearing->setInputMask( "09.99" );
  __pqLineEditMaxErrorBearing->setText( QString::number( __poSettings->getMaxErrorBearing(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorBearing, __iRow, 2 );
  connect( __pqLineEditMaxErrorBearing, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorBearing(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgeBearing = new QLineEdit();
  __pqLineEditMaxAgeBearing->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgeBearing->setInputMask( "09.99" );
  __pqLineEditMaxAgeBearing->setText( QString::number( __poSettings->getMaxAgeBearing(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgeBearing, __iRow, 3 );
  connect( __pqLineEditMaxAgeBearing, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgeBearing(const QString&) ) );

  // ... horizontal speed validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Speed (horizontal)")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMinValueSpeed = new QLineEdit();
  __pqLineEditMinValueSpeed->setToolTip( tr("in meters-per-second [m/s]") );
  __pqLineEditMinValueSpeed->setInputMask( "09.99" );
  __pqLineEditMinValueSpeed->setText( QString::number( __poSettings->getMinValueSpeed(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMinValueSpeed, __iRow, 1 );
  connect( __pqLineEditMinValueSpeed, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMinValueSpeed(const QString&) ) );
  QLineEdit* __pqLineEditMaxErrorSpeed = new QLineEdit();
  __pqLineEditMaxErrorSpeed->setToolTip( tr("in meters-per-second [m/s]") );
  __pqLineEditMaxErrorSpeed->setInputMask( "09.99" );
  __pqLineEditMaxErrorSpeed->setText( QString::number( __poSettings->getMaxErrorSpeed(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorSpeed, __iRow, 2 );
  connect( __pqLineEditMaxErrorSpeed, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorSpeed(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgeSpeed = new QLineEdit();
  __pqLineEditMaxAgeSpeed->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgeSpeed->setInputMask( "09.99" );
  __pqLineEditMaxAgeSpeed->setText( QString::number( __poSettings->getMaxAgeSpeed(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgeSpeed, __iRow, 3 );
  connect( __pqLineEditMaxAgeSpeed, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgeSpeed(const QString&) ) );

  // ... vertical speed validity
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Speed (vertical)")+":" ), __iRow, 0 );
  QLineEdit* __pqLineEditMinValueSpeedVertical = new QLineEdit();
  __pqLineEditMinValueSpeedVertical->setToolTip( tr("in meters-per-second [m/s]") );
  __pqLineEditMinValueSpeedVertical->setInputMask( "09.99" );
  __pqLineEditMinValueSpeedVertical->setText( QString::number( __poSettings->getMinValueSpeedVertical(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMinValueSpeedVertical, __iRow, 1 );
  connect( __pqLineEditMinValueSpeedVertical, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMinValueSpeedVertical(const QString&) ) );
  QLineEdit* __pqLineEditMaxErrorSpeedVertical = new QLineEdit();
  __pqLineEditMaxErrorSpeedVertical->setToolTip( tr("in meters-per-second [m/s]") );
  __pqLineEditMaxErrorSpeedVertical->setInputMask( "09.99" );
  __pqLineEditMaxErrorSpeedVertical->setText( QString::number( __poSettings->getMaxErrorSpeedVertical(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxErrorSpeedVertical, __iRow, 2 );
  connect( __pqLineEditMaxErrorSpeedVertical, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxErrorSpeedVertical(const QString&) ) );
  QLineEdit* __pqLineEditMaxAgeSpeedVertical = new QLineEdit();
  __pqLineEditMaxAgeSpeedVertical->setToolTip( tr("in seconds [s]") );
  __pqLineEditMaxAgeSpeedVertical->setInputMask( "09.99" );
  __pqLineEditMaxAgeSpeedVertical->setText( QString::number( __poSettings->getMaxAgeSpeedVertical(), 'f', 2 ).prepend(" ").right(5) );
  __pqGridLayout->addWidget( __pqLineEditMaxAgeSpeedVertical, __iRow, 3 );
  connect( __pqLineEditMaxAgeSpeedVertical, SIGNAL( textChanged(const QString&) ), __poSettings, SLOT( slotMaxAgeSpeedVertical(const QString&) ) );

  // Set the layout
  __iRow++;
  __pqGridLayout->addItem( new QSpacerItem( 0, this->height() ), __iRow, 0, 4 );
  QWidget::setLayout( __pqGridLayout );
}
