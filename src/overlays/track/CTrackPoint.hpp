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

#ifndef QVCT_CTRACKPOINT_HPP
#define QVCT_CTRACKPOINT_HPP

// QT
#include <QDataStream>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "overlays/COverlayPoint.hpp"


/// [UI] Track overlay point (item)
/**
 *  This class implements the overlay point (item) for the track overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CTrackPoint: public COverlayPoint
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Track point's fix type
  QString qsFixType;
  /// Track point's satellites count
  int iSatelliteCount;
  /// Track point's horizontal Dilution-of-Precision (HDOP)
  double fdDopHorizontal;
  /// Track point's vertical Dilution-of-Precision (VDOP)
  double fdDopVertical;
  /// Track point's positional Dilution-of-Precision (PDOP)
  double fdDopPositional;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CTrackPoint( const QString& _rqsName );
  CTrackPoint( const QString& _rqsName, const CDataPosition& _roDataPosition );
  virtual ~CTrackPoint() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void unserialize( QDataStream& _rqDataStream );

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent()->parent()->parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter ) {};
  virtual void showDetail();
  virtual void showEdit() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayPoint (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual bool matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const;


  //------------------------------------------------------------------------------
  // METHODS: COverlayVisibility (override)
  //------------------------------------------------------------------------------

public:
  virtual void toggleVisibility() { COverlayVisibility::toggleVisibility( false, true ); };


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Toggles this track point's selection status
  void toggleMultiSelected();
  /// Sets this track point's fix type
  void setFixType( const QString& _rqsFixType ) { qsFixType = _rqsFixType; };
  /// Sets this track point's satellite count
  void setSatelliteCount( int _iSatelliteCount ) { iSatelliteCount = _iSatelliteCount; };
  /// Sets this track point's horizontal Dilution-of-Precision (HDOP)
  void setDopHorizontal( double _fdDopHorizontal ) { fdDopHorizontal = _fdDopHorizontal; };
  /// Sets this track point's vertical Dilution-of-Precision (VDOP)
  void setDopVertical( double _fdDopVertical ) { fdDopVertical = _fdDopVertical; };
  /// Sets this track point's positional Dilution-of-Precision (PDOP)
  void setDopPositional( double _fdDopPositional ) { fdDopPositional = _fdDopPositional; };

  // GETTERS
public:
  /// Sets this track point's fix type
  QString getFixType() { return qsFixType; };
  /// Sets this track point's satellite count
  int getSatelliteCount() { return iSatelliteCount; };
  /// Sets this track point's horizontal Dilution-of-Precision (HDOP)
  double getDopHorizontal() { return fdDopHorizontal; };
  /// Sets this track point's vertical Dilution-of-Precision (VDOP)
  double getDopVertical() { return fdDopVertical; };
  /// Sets this track point's positional Dilution-of-Precision (PDOP)
  double getDopPositional() { return fdDopPositional; };

  // OTHER
public:
  /// Retrieves this object's content from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );
  /// Retrieves this object's content from the given GPX source (file)
  void parseGPX( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;
  /// Stores this object's content to the given GPX destination (file)
  void dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter ) const;

};

#endif // QVCT_CTRACKPOINT_HPP
