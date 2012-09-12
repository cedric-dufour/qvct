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

#ifndef QVCT_COVERLAYOBJECT_HPP
#define QVCT_COVERLAYOBJECT_HPP

// QT
#include <QDataStream>
#include <QTreeWidgetItem>

// QVCT
class CChart;
class COverlay;


/// Generic overlay object
/**
 *  This class defines the generic representation of any (hierarchical) object
 *  used in an overlay.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayObject: public QObject, public QTreeWidgetItem
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Overlay object type
  enum EType {
    OVERLAY = 1001,      ///< (Base) overlay
    CONTAINER = 1010,    ///< Container
    SUBCONTAINER = 1011, ///< Sub-container
    ITEM = 1100,         ///< Item
    SUBITEM1 = 1101,     ///< Sub-item (type 1)
    SUBITEM2 = 1102      ///< Sub-item (type 2)
  };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Object name
  /** @see setName(), getName() */
  QString qsName;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayObject( EType _eType, const QString& _rqsName );
  virtual ~COverlayObject() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets this object's name
  void setName( const QString& _rqsName ) { qsName = _rqsName; };

  // GETTERS
public:
  /// Returns this object's name
  QString getName() const { return qsName; };

  // USERS
public:
  /// Returns this object's (base) overlay
  virtual COverlay* useOverlay() = 0;

  // OTHER
public:
  /// Serializes (store) this object's data to binary format
  virtual void serialize( QDataStream& _rqDataStream ) const;
  /// Unserializes (restore) this object's data from binary format
  virtual void unserialize( QDataStream& _rqDataStream );

public:
  /// Draws this object (itself or its content)
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter ) = 0;
  /// Displays this object's details (in the appropriate widget/view)
  virtual void showDetail() = 0;
  /// Displays this object's edit widget/view
  virtual void showEdit() = 0;

public:
  /// Returns a valid name for a new sibling of this object
  /** This method checks and returns a valid name for a new sibling of this
   *  object, such as no duplicate is created. If the given name matches an
   *  existing sibling, it is automatically suffixed with a number, prefixed
   *  with the given quantity of "0" (e.g. "Object(01)") */
  QString newChildName( const QString& _rqsName, int __iZeroPrefix = 0, bool __bForceSuffix = false ) const;

};

#endif // QVCT_COVERLAYOBJECT_HPP
