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
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

// QVCT
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceCreateView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainerDeviceCreateView::CVesselContainerDeviceCreateView( CVesselContainerDevice** _ppoVesselContainerDevice )
  : QDialog( 0 )
  , ppoVesselContainerDevice( _ppoVesselContainerDevice )
  , pqLineEditName( 0 )
{
  QDialog::setWindowTitle( tr("Create Device")+"..." );
  *ppoVesselContainerDevice = 0;
  constructLayout();
}

void CVesselContainerDeviceCreateView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add form
  QHBoxLayout* __pqHBoxLayout = new QHBoxLayout();
  // ... icon
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_device.png" ) );
  __pqHBoxLayout->addWidget( __pqLabelIcon, 0 );
  // ... form
  QFormLayout* __pqFormLayout = new QFormLayout();
  // ... name
  pqLineEditName = new QLineEdit( this );
  pqLineEditName->setToolTip( tr("Device Name") );
  __pqFormLayout->addRow( tr("Device")+":", pqLineEditName );
  // ... [end]
  __pqHBoxLayout->addLayout( __pqFormLayout, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Ok, Qt::Horizontal, this );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  QDialog::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CVesselContainerDeviceCreateView::accept()
{
  *ppoVesselContainerDevice = new CVesselContainerDevice( pqLineEditName->text() );
  QDialog::accept();
}
