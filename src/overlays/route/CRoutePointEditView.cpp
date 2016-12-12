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
#include <QTextEdit>
#include <QVBoxLayout>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/route/CRoutePointEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRoutePointEditView::CRoutePointEditView( CRoutePoint* _poRoutePoint )
  : COverlayObjectEditView( _poRoutePoint )
{
  constructLayout();
}

void CRoutePointEditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/route_point.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  QLabel* __pqLabelEdit = new QLabel( tr("Edit")+"..." );
  __pqLabelEdit->setFont( __qFontHeader );
  __pqHBoxLayoutHeader->addWidget( __pqLabelEdit, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;
  QFormLayout* __pqFormLayout = new QFormLayout();

  // ... name
  pqLineEditName = new QLineEdit();
  pqLineEditName->setToolTip( tr("Name") );
  pqLineEditName->setText( __poRoutePoint->getName() );
  __pqFormLayout->addRow( tr("Name")+":", pqLineEditName );
  bool __bPositionDefined = __poRoutePoint->CDataPosition::operator!=( CDataPosition::UNDEFINED );

  // ... position
  QHBoxLayout* __pqHBoxLayoutPosition = new QHBoxLayout();
  pqLineEditLongitude = new QLineEdit();
  pqLineEditLongitude->setToolTip( tr("Longitude") );
  if( __bPositionDefined ) pqLineEditLongitude->setText( CUnitPosition::toString( __poRoutePoint->getLongitude(), CUnitPosition::LONGITUDE ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditLongitude );
  pqLineEditLatitude = new QLineEdit();
  pqLineEditLatitude->setToolTip( tr("Latitude") );
  if( __bPositionDefined ) pqLineEditLatitude->setText( CUnitPosition::toString( __poRoutePoint->getLatitude(), CUnitPosition::LATITUDE ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditLatitude );
  pqLineEditElevation = new QLineEdit();
  pqLineEditElevation->setToolTip( tr("Elevation") );
  if( __poRoutePoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION ) pqLineEditElevation->setText( CUnitElevation::toString( __poRoutePoint->getElevation() ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditElevation );
  __pqFormLayout->addRow( tr("Position")+":", __pqHBoxLayoutPosition );

  // ... type/symbol
  QHBoxLayout* __pqHBoxLayoutTypeSymbol = new QHBoxLayout();
  pqLineEditType = new QLineEdit();
  pqLineEditType->setToolTip( tr("Type") );
  pqLineEditType->setText( __poRoutePoint->getType() );
  __pqHBoxLayoutTypeSymbol->addWidget( pqLineEditType );
  pqLineEditSymbol = new QLineEdit();
  pqLineEditSymbol->setToolTip( tr("Symbol") );
  pqLineEditSymbol->setText( __poRoutePoint->getSymbol() );
  __pqHBoxLayoutTypeSymbol->addWidget( pqLineEditSymbol );
  __pqFormLayout->addRow( tr("Type & Symbol")+":", __pqHBoxLayoutTypeSymbol );

  // ... description
  pqTextEditDescription = new QTextEdit();
  pqTextEditDescription->setToolTip( tr("Comment") );
  pqTextEditDescription->setAcceptRichText( false );
  pqTextEditDescription->setPlainText( __poRoutePoint->getDescription() );
  __pqFormLayout->addRow( tr("Description")+":", pqTextEditDescription );

  // ... comment
  pqTextEditComment = new QTextEdit();
  pqTextEditComment->setToolTip( tr("Comment") );
  pqTextEditComment->setAcceptRichText( false );
  pqTextEditComment->setPlainText( __poRoutePoint->getComment() );
  __pqFormLayout->addRow( tr("Comment")+":", pqTextEditComment );

  // ... url
  pqLineEditUrl = new QLineEdit();
  pqLineEditUrl->setToolTip( tr("URL") );
  pqLineEditUrl->setText( __poRoutePoint->getUrl() );
  __pqFormLayout->addRow( tr("URL")+":", pqLineEditUrl );

  // ... [end]
  __pqVBoxLayout->addLayout( __pqFormLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Save, Qt::Horizontal );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  COverlayObjectEditView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CRoutePointEditView::accept()
{
  CRoutePoint* __poRoutePoint = (CRoutePoint*)poOverlayObject;

  // Check data
  bool __bOK;
  double __fdLongitude = CDataPosition::UNDEFINED_LONGITUDE;
  double __fdLatitude = CDataPosition::UNDEFINED_LATITUDE;
  double __fdElevation = CDataPosition::UNDEFINED_ELEVATION;
  if( !pqLineEditLongitude->text().isEmpty() && !pqLineEditLatitude->text().isEmpty() )
  {
    __fdLongitude = CUnitPosition::fromString( pqLineEditLongitude->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditLongitude->text() );
      return;
    }
    __fdLatitude = CUnitPosition::fromString( pqLineEditLatitude->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditLatitude->text() );
      return;
    }
  }
  if( !pqLineEditElevation->text().isEmpty() )
  {
    __fdElevation = CUnitElevation::fromString( pqLineEditElevation->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditElevation->text() );
      return;
    }
  }

  // Set data
  // ... name
  __poRoutePoint->setText( CRouteOverlay::NAME, pqLineEditName->text() ); // NOTE: Item's name will be updated via QTreeWidget::itemChanged()
  // ... position/elevation
  __poRoutePoint->setPosition( __fdLongitude, __fdLatitude, __fdElevation );
  // ... type
  __poRoutePoint->setType( pqLineEditType->text() );
  // ... description
  __poRoutePoint->setDescription( pqTextEditDescription->toPlainText() );
  // ... comment
  __poRoutePoint->setComment( pqTextEditComment->toPlainText() );
  // ... symbol
  __poRoutePoint->setSymbol( pqLineEditSymbol->text() );
  // ... url
  __poRoutePoint->setUrl( pqLineEditUrl->text() );

  // Done
  QDialog::accept();
}
