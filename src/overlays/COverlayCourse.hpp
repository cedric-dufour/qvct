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

#ifndef QVCT_COVERLAYCOURSE_HPP
#define QVCT_COVERLAYCOURSE_HPP

// QT
#include <QDataStream>
#include <QPainter>

// QVCT
#include "data/CDataCourse.hpp"
#include "overlays/COverlayPoint.hpp"
class CDataCourseValidity;


/// Generic overlay course
/**
 *  This class defines the generic representation of a (geographical) course 
 *  (bearing, horitontal/vertical speeds) used in an overlay.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayCourse: public COverlayPoint, public CDataCourseGA
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayCourse( const QString& _rqsName );
  COverlayCourse( const QString& _rqsName, const CDataPosition& _roDataPosition );
  COverlayCourse( const QString& _rqsName, const CDataPosition& _roDataPosition,
                  const CDataCourse& _roGroundGeoCourse, const CDataCourse& _roApparentGeoCourse );
  virtual ~COverlayCourse() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (override/implement)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const;
  virtual void unserialize( QDataStream& _rqDataStream );


  //------------------------------------------------------------------------------
  // METHODS: COverlayPoint (override/implement)
  //------------------------------------------------------------------------------

public:
  virtual void drawMarker( const CChart* _poChart, QPainter* _pqPainter, const CDataPositionValidity* _poDataPositionValidity = 0 );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Draws the course vectors (ground and apparent)
  virtual void drawVector( const CChart* _poChart, QPainter* _pqPainter, const CDataCourseValidityGA* _poDataCourseValidityGA = 0 );

};

#endif // QVCT_COVERLAYCOURSE_HPP
