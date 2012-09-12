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
 * \brief      Landmark overlay
 * \author     Cedric Dufour <http://cedric.dufour.name>
 * \version    @version@
 */


#ifndef QVCT_CLANDMARKOVERLAY_HPP
#define QVCT_CLANDMARKOVERLAY_HPP

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
class CLandmarkContainer;
class CLandmarkPoint;


/// [UI] Landmark overlay container
/**
 *  This class implements the (base) overlay corresponding to landmark overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CLandmarkOverlay: public COverlayBaseTree
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Implemented tree-widget columns
  enum EColumn {
    NAME = 0,    ///< Landmark name
    VISIBLE = 1, ///< Landmark visibility status
    SELECT = 2   ///< Landmark selection status
  };


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CLandmarkOverlay( QWidget* _pqParent );
  virtual ~CLandmarkOverlay();


  //------------------------------------------------------------------------------
  // METHODS: QTreeWidget (implement/override)
  //------------------------------------------------------------------------------

private:
  virtual QStringList mimeTypes() const;
  virtual QMimeData* mimeData( const QList<QTreeWidgetItem*> _qListTreeWidgetItems ) const;
  virtual bool dropMimeData ( QTreeWidgetItem* _pqTreeWidgetItem, int _iIndex, const QMimeData* _pqMimeData, Qt::DropAction eAction );


  //------------------------------------------------------------------------------
  // METHODS: COverlay (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void drawContent( const CChart* _poChart, QPainter* _pqPainter ) const;
  virtual void showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const;
  virtual void setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const;
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const;


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
  /// Imports settings from the application's global settings
  void importSettings();
  /// Add a new landmarks set (container) to this overlay
  CLandmarkContainer* addContainer( const QString& _rqsName );
  /// Pick (select) a container among the available ones (0 if none is selected)
  /** This method returns a container according to the following logic:
   *   - if no container exists, a new one is automatically created (and returned)
   *   - if a single container exists, it is returned
   *   - if multiple containers exist, the user is asked to pick (select) one */
  CLandmarkContainer* pickContainer();
  /// Deletes selected items within this overlay's containers
  int deleteSelection();
  /// Clear the entire content of this overlay
  void clear();

public:
  /// Load this object's content from the given file and returns the last loaded container (0 if none)
  CLandmarkContainer* load( const QString& _rqsFilename );
  /// Save this object's content (container) to the given file (all selected items if no container is given)
  void save( const QString& _rqsFilename, CLandmarkContainer* _poLandmarkContainer = 0 ) const;
  /// Retrieves this object's content from the given QVCT source (file)
  int parseQVCT( const QDomElement& _rqDomElement, CLandmarkContainer** _ppoLandmarkContainer = 0 );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CLandmarkContainer* _poLandmarkContainer = 0, bool _bProjectDump = false ) const;
  /// Stores this object's content to the given GPX destination (file)
  void dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, CLandmarkContainer* _poLandmarkContainer = 0 ) const;

};

#endif // QVCT_CLANDMARKOVERLAY_HPP
