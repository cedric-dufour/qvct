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

#ifndef QVCT_COVERLAYBASETREE_HPP
#define QVCT_COVERLAYBASETREE_HPP

// QT
#include <QTreeWidget>
#include <QWidget>

// QVCT
#include "overlays/COverlay.hpp"


/// Generic overlay base (tree widget)
/**
 *  This class defines the generic representation of an overlay (base object),
 *  including the QTreeWidget required to display its content.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayBaseTree: public QTreeWidget, public COverlay
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayBaseTree( QWidget* _pqParent, const QString& _rqsName );
  virtual ~COverlayBaseTree() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to handle current item changes in the underlying QTreeWidget
  void slotCurrentItemChanged( QTreeWidgetItem* _pqTreeWidgetItem, QTreeWidgetItem* _pqTreeWidgetItemPrevious );
  /// Slot to handle item click in the underlying QTreeWidget
  void slotItemClicked( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn );
  /// Slot to handle item double-click in the underlying QTreeWidget
  void slotItemDoubleClicked( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn );
  /// Slot to handle item (content) changes in the underlying QTreeWidget
  void slotItemChanged( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn );

  // OTHER
public:
  /// Handles item (content) changes in the underlying QTreeWidget
  virtual void onChange( QTreeWidgetItem* _pqTreeWidgetItem, int __iColumn ) = 0;

};

#endif // QVCT_COVERLAYBASETREE_HPP
