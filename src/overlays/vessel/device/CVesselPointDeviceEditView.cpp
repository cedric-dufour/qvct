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
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

// QVCT
#include "overlays/vessel/device/CVesselPointDevice.hpp"
#include "overlays/vessel/device/CVesselPointDeviceEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointDeviceEditView::CVesselPointDeviceEditView( CVesselPointDevice* _poVesselPointDevice )
  : COverlayObjectEditView( _poVesselPointDevice )
{
  constructLayout();
}

void CVesselPointDeviceEditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_device.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  QLabel* __pqLabelEdit = new QLabel( tr("Edit")+"...", this );
  __pqLabelEdit->setFont( __qFontHeader );
  __pqHBoxLayoutHeader->addWidget( __pqLabelEdit, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)poOverlayObject;
  QGridLayout* __pqGridLayout = new QGridLayout();
  int __iRow = -1;

  // ... name
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Device/Source")+":" ), __iRow, 0 );
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Device name (matching one of the defined devices)") );
  poTextName->setText( __poVesselPointDevice->getName() );
  __pqGridLayout->addWidget( poTextName, __iRow, 1, Qt::AlignHCenter );
  poTextSourceName = new COverlayText( this );
  poTextSourceName->setToolTip( tr("Source name/filter (exactly matching the device provided source name)") );
  poTextSourceName->setText( __poVesselPointDevice->getSourceName() );
  __pqGridLayout->addWidget( poTextSourceName, __iRow, 3, Qt::AlignHCenter );

  // ... separator
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<B>"+tr("Synchronize/Update")+"</B>" ), __iRow, 2, Qt::AlignHCenter );

  // ... position & time (header)
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Position")+"</I>" ), __iRow, 1, Qt::AlignHCenter );
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Elevation")+"</I>" ), __iRow, 2, Qt::AlignHCenter );
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Time")+"</I>" ), __iRow, 3, Qt::AlignHCenter );

  // ... position & time (data)
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Position & Time")+":" ), __iRow, 0 );
  pqCheckBoxPosition = new QCheckBox( this );
  pqCheckBoxPosition->setToolTip( tr("Whether to use this device to update the vessel position (longitude/latitude)") );
  pqCheckBoxPosition->setCheckState( __poVesselPointDevice->isSynchronizedPosition() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxPosition, __iRow, 1, Qt::AlignHCenter );
  pqCheckBoxElevation = new QCheckBox( this );
  pqCheckBoxElevation->setToolTip( tr("Whether to use this device to update the vessel elevation") );
  pqCheckBoxElevation->setCheckState( __poVesselPointDevice->isSynchronizedElevation() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxElevation, __iRow, 2, Qt::AlignHCenter );
  pqCheckBoxTime = new QCheckBox( this );
  pqCheckBoxTime->setToolTip( tr("Whether to use this device to update the vessel time") );
  pqCheckBoxTime->setCheckState( __poVesselPointDevice->isSynchronizedTime() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxTime, __iRow, 3, Qt::AlignHCenter );

  // ... course (header)
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Bearing")+"</I>" ), __iRow, 1, Qt::AlignHCenter );
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Speed (horizontal)")+"</I>" ), __iRow, 2, Qt::AlignHCenter );
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Speed (vertical)")+"</I>" ), __iRow, 3, Qt::AlignHCenter );

  // ... ground course
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Ground Course")+":" ), __iRow, 0 );
  pqCheckBoxGroundBearing = new QCheckBox( this );
  pqCheckBoxGroundBearing->setToolTip( tr("Whether to use this device to update the vessel ground (true) bearing") );
  pqCheckBoxGroundBearing->setCheckState( __poVesselPointDevice->isSynchronizedGroundBearing() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxGroundBearing, __iRow, 1, Qt::AlignHCenter );
  pqCheckBoxGroundSpeed = new QCheckBox( this );
  pqCheckBoxGroundSpeed->setToolTip( tr("Whether to use this device to update the vessel ground (true) horizontal speed") );
  pqCheckBoxGroundSpeed->setCheckState( __poVesselPointDevice->isSynchronizedGroundSpeed() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxGroundSpeed, __iRow, 2, Qt::AlignHCenter );
  pqCheckBoxGroundSpeedVertical = new QCheckBox( this );
  pqCheckBoxGroundSpeedVertical->setToolTip( tr("Whether to use this device to update the vessel ground (true) vertical speed") );
  pqCheckBoxGroundSpeedVertical->setCheckState( __poVesselPointDevice->isSynchronizedGroundSpeedVertical() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxGroundSpeedVertical, __iRow, 3, Qt::AlignHCenter );

  // ... apparent course
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Apparent Course")+":" ), __iRow, 0 );
  pqCheckBoxApparentBearing = new QCheckBox( this );
  pqCheckBoxApparentBearing->setToolTip( tr("Whether to use this device to update the vessel apparent (relative) bearing") );
  pqCheckBoxApparentBearing->setCheckState( __poVesselPointDevice->isSynchronizedApparentBearing() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxApparentBearing, __iRow, 1, Qt::AlignHCenter );
  pqCheckBoxApparentSpeed = new QCheckBox( this );
  pqCheckBoxApparentSpeed->setToolTip( tr("Whether to use this device to update the vessel apparent (relative) horizontal speed") );
  pqCheckBoxApparentSpeed->setCheckState( __poVesselPointDevice->isSynchronizedApparentSpeed() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxApparentSpeed, __iRow, 2, Qt::AlignHCenter );
  pqCheckBoxApparentSpeedVertical = new QCheckBox( this );
  pqCheckBoxApparentSpeedVertical->setToolTip( tr("Whether to use this device to update the vessel apparent (relative) vertical speed") );
  pqCheckBoxApparentSpeedVertical->setCheckState( __poVesselPointDevice->isSynchronizedApparentSpeedVertical() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxApparentSpeedVertical, __iRow, 3, Qt::AlignHCenter );

  // ... other (header)
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( "<I>"+tr("Additional Text")+"</I>" ), __iRow, 1, Qt::AlignHCenter );

  // ... other
  __iRow++;
  __pqGridLayout->addWidget( new QLabel( tr("Other")+":" ), __iRow, 0 );
  pqCheckBoxText = new QCheckBox( this );
  pqCheckBoxText->setToolTip( tr("Whether to use this device to update the vessel additional textual information") );
  pqCheckBoxText->setCheckState( __poVesselPointDevice->isSynchronizedText() ? Qt::Checked : Qt::Unchecked );
  __pqGridLayout->addWidget( pqCheckBoxText, __iRow, 1, Qt::AlignHCenter );

  // ... [end]
  __pqVBoxLayout->addLayout( __pqGridLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Save, Qt::Horizontal, this );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  COverlayObjectEditView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CVesselPointDeviceEditView::accept()
{
  CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)poOverlayObject;

  // Set data
  __poVesselPointDevice->setSynchronized( pqCheckBoxPosition->checkState() == Qt::Checked,
                                          pqCheckBoxElevation->checkState() == Qt::Checked,
                                          pqCheckBoxTime->checkState() == Qt::Checked,
                                          pqCheckBoxGroundBearing->checkState() == Qt::Checked,
                                          pqCheckBoxGroundSpeed->checkState() == Qt::Checked,
                                          pqCheckBoxGroundSpeedVertical->checkState() == Qt::Checked,
                                          pqCheckBoxApparentBearing->checkState() == Qt::Checked,
                                          pqCheckBoxApparentSpeed->checkState() == Qt::Checked,
                                          pqCheckBoxApparentSpeedVertical->checkState() == Qt::Checked,
                                          pqCheckBoxText->checkState() == Qt::Checked );

  // Done
  QDialog::accept();
}
