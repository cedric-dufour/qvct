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
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRouteContainerEditView.hpp"
#include "overlays/route/CRouteOverlay.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteContainerEditView::CRouteContainerEditView( CRouteContainer* _poRouteContainer )
  : COverlayObjectEditView( _poRouteContainer )
{
  constructLayout();
}

void CRouteContainerEditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/route.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  QLabel* __pqLabelEdit = new QLabel( tr("Edit")+"...", this );
  __pqLabelEdit->setFont( __qFontHeader );
  __pqHBoxLayoutHeader->addWidget( __pqLabelEdit, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  CRouteContainer* __poRouteContainer = (CRouteContainer*)poOverlayObject;
  QFormLayout* __pqFormLayout = new QFormLayout();

  // ... name
  pqLineEditName = new QLineEdit( this );
  pqLineEditName->setToolTip( tr("Name") );
  pqLineEditName->setText( __poRouteContainer->getName() );
  __pqFormLayout->addRow( tr("Name")+":", pqLineEditName );

  // ... type
  pqLineEditType = new QLineEdit( this );
  pqLineEditType->setToolTip( tr("Type") );
  pqLineEditType->setText( __poRouteContainer->getType() );
  __pqFormLayout->addRow( tr("Type")+":", pqLineEditType );

  // ... description
  pqTextEditDescription = new QTextEdit( this );
  pqTextEditDescription->setToolTip( tr("Description") );
  pqTextEditDescription->setAcceptRichText( false );
  pqTextEditDescription->setPlainText( __poRouteContainer->getDescription() );
  __pqFormLayout->addRow( tr("Description")+":", pqTextEditDescription );

  // ... comment
  pqTextEditComment = new QTextEdit( this );
  pqTextEditComment->setToolTip( tr("Comment") );
  pqTextEditComment->setAcceptRichText( false );
  pqTextEditComment->setPlainText( __poRouteContainer->getComment() );
  __pqFormLayout->addRow( tr("Comment")+":", pqTextEditComment );

  // ... url
  pqLineEditUrl = new QLineEdit( this );
  pqLineEditUrl->setToolTip( tr("URL") );
  pqLineEditUrl->setText( __poRouteContainer->getUrl() );
  __pqFormLayout->addRow( tr("URL")+":", pqLineEditUrl );

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

void CRouteContainerEditView::accept()
{
  CRouteContainer* __poRouteContainer = (CRouteContainer*)poOverlayObject;

  // Set data
  // ... name
  __poRouteContainer->setText( CRouteOverlay::NAME, pqLineEditName->text() ); // NOTE: Container's name will be updated via QTreeWidget::containerChanged()
  // ... type
  __poRouteContainer->setType( pqLineEditType->text() );
  // ... description
  __poRouteContainer->setDescription( pqTextEditDescription->toPlainText() );
  // ... comment
  __poRouteContainer->setComment( pqTextEditComment->toPlainText() );
  // ... url
  __poRouteContainer->setUrl( pqLineEditUrl->text() );

  // Done
  QDialog::accept();
}
