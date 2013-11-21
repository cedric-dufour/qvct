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
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

// QVCT
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainerDeviceEditView::CVesselContainerDeviceEditView( CVesselContainerDevice* _poVesselContainerDevice )
  : COverlayObjectEditView( _poVesselContainerDevice )
{
  constructLayout();
}

void CVesselContainerDeviceEditView::constructLayout()
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
  CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)poOverlayObject;
  QFormLayout* __pqFormLayout = new QFormLayout();
  // ... name
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Device Name") );
  poTextName->setText( __poVesselContainerDevice->getName() );
  __pqFormLayout->addRow( tr("Name")+":", poTextName );
  // ... (vessels) time-to-live
  pqSpinBoxTTL = new QSpinBox( this );
  pqSpinBoxTTL->setRange( 5, 86400 );
  pqSpinBoxTTL->setToolTip( tr("Vessels Time-To-Live (after no corresponding data are received) [seconds]") );
  pqSpinBoxTTL->setValue( __poVesselContainerDevice->getTTL() );
  __pqFormLayout->addRow( tr("TTL")+":", pqSpinBoxTTL );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqFormLayout );

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

void CVesselContainerDeviceEditView::accept()
{
  CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)poOverlayObject;

  // Set data
  __poVesselContainerDevice->setTTL( pqSpinBoxTTL->value() );

  // Done
  QDialog::accept();
}
