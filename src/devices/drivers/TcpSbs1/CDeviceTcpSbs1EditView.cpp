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
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "devices/drivers/TcpSbs1/CDeviceTcpSbs1.hpp"
#include "devices/drivers/TcpSbs1/CDeviceTcpSbs1EditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceTcpSbs1EditView::CDeviceTcpSbs1EditView( CDeviceTcpSbs1* _poDeviceTcpSbs1 )
  : COverlayObjectEditView( _poDeviceTcpSbs1 )
{
  constructLayout();
}

void CDeviceTcpSbs1EditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/device.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  QLabel* __pqLabelEdit = new QLabel( tr("Edit")+"...", this );
  __pqLabelEdit->setFont( __qFontHeader );
  __pqHBoxLayoutHeader->addWidget( __pqLabelEdit, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  CDeviceTcpSbs1* __poDeviceTcpSbs1 = (CDeviceTcpSbs1*)poOverlayObject;
  QFormLayout* __pqFormLayout = new QFormLayout();
  // ... name
  COverlayText* __poTextName = new COverlayText( this );
  __poTextName->setText( __poDeviceTcpSbs1->getName() );
  __pqFormLayout->addRow( tr("Name")+":", __poTextName );
  // ... host
  pqLineEditHost = new QLineEdit( this );
  pqLineEditHost->setText( __poDeviceTcpSbs1->getHost() );
  __pqFormLayout->addRow( tr("Host")+":", pqLineEditHost );
  // ... port
  pqLineEditPort = new QLineEdit( this );
  pqLineEditPort->setText( QString::number( __poDeviceTcpSbs1->getPort() ) );
  __pqFormLayout->addRow( tr("Port")+":", pqLineEditPort );
  // ... time zone
  pqComboBoxTimeZone = new QComboBox( this );
  pqComboBoxTimeZone->addItems( QStringList( CUnitTimeZone::symbols().values() ) );
  pqComboBoxTimeZone->setCurrentIndex( pqComboBoxTimeZone->findText( CUnitTimeZone::toSymbol( __poDeviceTcpSbs1->getTimeZone() ) ) );
  __pqFormLayout->addRow( tr("Time Zone")+":", pqComboBoxTimeZone );
  // ... callsign lookup
  pqCheckBoxCallsignLookup = new QCheckBox( this );
  pqCheckBoxCallsignLookup->setCheckState( __poDeviceTcpSbs1->isCallsignLookup() ? Qt::Checked : Qt::Unchecked );
  __pqFormLayout->addRow( tr("Use Callsign")+":", pqCheckBoxCallsignLookup );
  // ... ground traffic (inclusion/track)
  pqCheckBoxGroundTraffic = new QCheckBox( this );
  pqCheckBoxGroundTraffic->setCheckState( __poDeviceTcpSbs1->isGroundTraffic() ? Qt::Checked : Qt::Unchecked );
  __pqFormLayout->addRow( tr("Ground Traffic")+":", pqCheckBoxGroundTraffic );
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

void CDeviceTcpSbs1EditView::accept()
{
  CDeviceTcpSbs1* __poDeviceTcpSbs1 = (CDeviceTcpSbs1*)poOverlayObject;

  // Check data
  bool __bOK;
  int __iPort = -1;
  __iPort = pqLineEditPort->text().toInt( &__bOK );
  if( !__bOK || __iPort <= 0 || __iPort > 65535 )
  {
    QVCTRuntime::useMainWindow()->parseError( pqLineEditPort->text() );
    return;
  }

  // Set data
  __poDeviceTcpSbs1->setOperatingMode( CDevice::STOP );
  // ... host
  __poDeviceTcpSbs1->setHost( pqLineEditHost->text() );
  // ... port
  __poDeviceTcpSbs1->setPort( __iPort );
  // ... time zone
  __poDeviceTcpSbs1->setTimeZone( CUnitTimeZone::fromSymbol( pqComboBoxTimeZone->currentText() ) );
  // ... callsign lookup
  __poDeviceTcpSbs1->setCallsignLookup( pqCheckBoxCallsignLookup->checkState() == Qt::Checked );
  // ... ground traffic (inclusion/track)
  __poDeviceTcpSbs1->setGroundTraffic( pqCheckBoxGroundTraffic->checkState() == Qt::Checked );

  // Done
  QDialog::accept();
}
