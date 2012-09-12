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

#ifndef QVCT_COVERLAYVISIBILITY_HPP
#define QVCT_COVERLAYVISIBILITY_HPP


/// Generic overlay visibility
/**
 *  This class defines the generic fields and methods to manage the visibility
 *  of overlay objects.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayVisibility
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Global (marker) visibility status
  /** @see setVisible(), toggleVisible(), isVisible() */
  bool bVisible;
  /// Name's (tag) visibility status
  /** @see setVisibleName(), toggleVisible(), isVisibleName() */
  bool bVisibleName;
  /// Position's visibility status
  /** @see setVisiblePosition(), toggleVisible(), isVisiblePosition() */
  bool bVisiblePosition;
  /// Course's visibility status
  /** @see setVisibleCourse(), toggleVisible(), isVisibleCourse() */
  bool bVisibleCourse;
  /// Routing's (data) visibility status
  /** @see setVisibleRouting(), toggleVisible(), isVisibleRouting() */
  bool bVisibleRouting;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayVisibility();
  virtual ~COverlayVisibility() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the point's (marker) visibility status
  void setVisible( bool _bVisible ) { bVisible = _bVisible; };
  /// Sets the name's (tag) visibility status
  virtual void setVisibleName( bool _bVisibleName ) { bVisibleName = _bVisibleName; };
  /// Sets the position's visibility status
  virtual void setVisiblePosition( bool _bVisiblePosition ) { bVisiblePosition = _bVisiblePosition; };
  /// Sets the course's visibility status
  virtual void setVisibleCourse( bool _bVisibleCourse ) { bVisibleCourse = _bVisibleCourse; };
  /// Sets the routing's (data) visibility status
  virtual void setVisibleRouting( bool _bVisibleRouting ) { bVisibleRouting = _bVisibleRouting; };

protected:
  /// Toggle the visibility status
  virtual void toggleVisible( bool _bIncludeCourse, bool _bIncludeRouting );

  // GETTERS
public:
  /// Returns the point's (marker) visibility status
  bool isVisible() const { return bVisible; };
  /// Returns the tag's (name) visibility status
  bool isVisibleName() const { return bVisibleName; };
  /// Returns the position's visibility status
  bool isVisiblePosition() const { return bVisiblePosition; };
  /// Returns the course's visibility status
  bool isVisibleCourse() const { return bVisibleCourse; };
  /// Returns the routing's (data) visibility status
  bool isVisibleRouting() const { return bVisibleRouting; };

};

#endif // QVCT_COVERLAYVISIBILITY_HPP
