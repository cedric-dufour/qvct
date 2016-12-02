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
#include <QTreeWidget>
#include <QWidget>

// QVCT
#include "overlays/COverlayBaseTree.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayBaseTree::COverlayBaseTree( QWidget* _pqparent, const QString& _rqsName )
  : QTreeWidget( _pqparent ), COverlay( _rqsName )
  , bCurrentItemJustChanged( false )
{
  // Tree widget
  QTreeWidget::setRootIsDecorated( false );
  QTreeWidget::setIndentation( 16 );
  QTreeWidget::addTopLevelItem( this );
  // ... signals
  QObject::connect( this, SIGNAL( itemClicked(QTreeWidgetItem*,int) ), this, SLOT( slotItemClicked(QTreeWidgetItem*,int) ) );
  QObject::connect( this, SIGNAL( itemDoubleClicked(QTreeWidgetItem*,int) ), this, SLOT( slotItemDoubleClicked(QTreeWidgetItem*,int) ) );
  QObject::connect( this, SIGNAL( itemChanged(QTreeWidgetItem*,int) ), this, SLOT( slotItemChanged(QTreeWidgetItem*,int) ) );
  QObject::connect( this, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*) ), this, SLOT( slotCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*) ) );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void COverlayBaseTree::slotCurrentItemChanged( QTreeWidgetItem* _pqTreeWidgetItem, QTreeWidgetItem* _pqTreeWidgetItemPrevious )
{
  showDetail( _pqTreeWidgetItem );
  bCurrentItemJustChanged = true;
}

void COverlayBaseTree::slotItemClicked( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
{
  if( !bCurrentItemJustChanged ) showDetail( _pqTreeWidgetItem );
  bCurrentItemJustChanged = false;
}

void COverlayBaseTree::slotItemDoubleClicked( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
{
  setPosition( _pqTreeWidgetItem );
}

void COverlayBaseTree::slotItemChanged( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
{
  onChange( _pqTreeWidgetItem, _iColumn );
}

//
// OTHER
//

void COverlayBaseTree::destroy()
{
  // NOTE: Since 'this' is both the QTreeWidget and its top-level QTreeWidgetItem,
  //       it MUST be removed as top-level QTreeWidgetItem before the QTreeWidget
  //       is destructed (and all its children as well). Failure to do so would
  //       result in the QTreeWidget destructing itself (and lead to segmentation
  //       fault on application exit).
  QTreeWidget::takeTopLevelItem( 0 ); // this
}
