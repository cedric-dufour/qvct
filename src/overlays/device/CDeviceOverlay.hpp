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

/**
 * \brief      Device overlay
 * \author     Cedric Dufour <http://cedric.dufour.name>
 * \version    @version@
 */


#ifndef QVCT_CDEVICEOVERLAY_HPP
#define QVCT_CDEVICEOVERLAY_HPP

// QT
#include <QDomElement> // QtXml module
#include <QList>
#include <QMimeData>
#include <QPainter>
#include <QPointF>
#include <QStringList>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "overlays/COverlayBaseTree.hpp"
class CChart;
class CDevice;
class CDevicePoint;


/// [UI] Device overlay container
/**
 *  This class implements the (base) overlay corresponding to device overlay.
 *  The device overlay is not exactly an overlay since it is not meant to be
 *  drawn on top of the displayed chart. On the other hand, having devices
 *  available for inspection/configuration in the tree view (dock widget) is
 *  quite handy.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceOverlay: public COverlayBaseTree
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Implemented tree-widget columns
  enum EColumn {
    NAME = 0,    ///< Device name
    SELECT = 1   ///< Device selection status
  };


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceOverlay( QWidget* _pqParent );
  virtual ~CDeviceOverlay();


  //------------------------------------------------------------------------------
  // METHODS: COverlay (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void drawContent( const CChart* _poChart, QPainter* _pqPainter ) const {};
  virtual void showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const;
  virtual void setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const {};
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const { return 0; };


  //------------------------------------------------------------------------------
  // METHODS: COverlayBaseTree (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void onChange( QTreeWidgetItem* _pqTreeWidgetItem, int __iColumn );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Returns the device matching the given name (0 if none is found)
  CDevice* pickDevice( const QString& _rqsName );
  /// Deletes selected devices from this overlay
  int deleteSelection();
  /// Clear the entire content of this overlay
  void clear();

public:
  /// Load this object's content from the given file and returns the last loaded device (0 if none)
  CDevice* load( const QString& _rqsFilename, bool _bSilent = false );
  /// Save this object's content (device) to the given file (all selected items if no device is given)
  void save( const QString& _rqsFilename, CDevice* _poDevice = 0, bool _bApplicationDump = false ) const;
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement, CDevice** _ppoDevice = 0 );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CDevice* _poDevice = 0, bool _bApplicationDump = false ) const;

};

#endif // QVCT_CDEVICEOVERLAY_HPP
