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

// QVCT
#include "overlays/COverlayVisibility.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayVisibility::COverlayVisibility()
  : bVisible( true )
  , bVisibleName( true )
  , bVisiblePosition( false )
  , bVisibleCourse( false )
  , bVisibleRouting( false )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void COverlayVisibility::toggleVisibility( bool _bIncludeCourse, bool _bIncludeRouting )
{
  if( !bVisible ) bVisible = true;
  else
  {
    if( bVisiblePosition )
    {
      bVisibleName = false;
      bVisiblePosition = false;
      bVisibleCourse = false;
      bVisibleRouting = false;
    }
    else if( bVisibleCourse ) bVisiblePosition = true;
    else if( bVisibleName )
    {
      if( _bIncludeCourse ) bVisibleCourse = true;
      else bVisiblePosition = true;
    }
    else if( bVisibleRouting ) bVisibleName = true;
    else
    {
      if( _bIncludeRouting ) bVisibleRouting = true;
      else bVisibleName = true;
    }
  }
}

void COverlayVisibility::setVisibility( int _iVisibilityCode )
{
  bVisible = (bool)( _iVisibilityCode & 1 );
  bVisibleName = (bool)( _iVisibilityCode & 2 );
  bVisiblePosition = (bool)( _iVisibilityCode & 4 );
  bVisibleCourse = (bool)( _iVisibilityCode & 8 );
  bVisibleRouting = (bool)( _iVisibilityCode & 16 );
}

//
// GETTERS
//

int COverlayVisibility::getVisibility() const
{
  int __iVisibilityCode = 0;
  __iVisibilityCode |= bVisible ? 1 : 0;
  __iVisibilityCode |= bVisibleName ? 2 : 0;
  __iVisibilityCode |= bVisiblePosition ? 4 : 0;
  __iVisibilityCode |= bVisibleCourse ? 8 : 0;
  __iVisibilityCode |= bVisibleRouting ? 16 : 0;
  return __iVisibilityCode;
}
