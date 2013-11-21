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
#include <QDateTime>
#include <QDomElement> // QtXml module
#include <QFileInfo>
#include <QFileDialog>
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/CVesselContainerEditView.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselPointDevice.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainer::CVesselContainer( const QString& _rqsName )
  : COverlayContainer( COverlayObject::CONTAINER, _rqsName )
  , bDynamic( false )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled );
  QTreeWidgetItem::setText( CVesselOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CVesselOverlay::VISIBLE, Qt::Checked );
}

CVesselContainer::~CVesselContainer()
{
  clear();
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CVesselContainer::serialize( QDataStream& _rqDataStream ) const
{
  COverlayObject::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsUrl;
}

void CVesselContainer::unserialize( QDataStream& _rqDataStream )
{
  COverlayObject::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsUrl;
  QTreeWidgetItem::setText( CVesselOverlay::NAME, qsName );
}

void CVesselContainer::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CVesselContainer::showDetail()
{
  QVCTRuntime::useVesselContainerDetailView()->setOverlayObject( this );
  QVCTRuntime::useVesselContainerDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::VESSEL_CONTAINER );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::VESSEL );
}

void CVesselContainer::showEdit()
{
  CVesselContainerEditView* __poVesselContainerEditView = new CVesselContainerEditView( this );
  if( __poVesselContainerEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poVesselContainerEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayContainer (implement/override)
//------------------------------------------------------------------------------

COverlayPoint* CVesselContainer::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= bDynamic ? 1 : 0; __i-- ) // we must go in the reverse order of CVesselContainer::draw() to pick the correct (overlapping) item
  {
    COverlayPoint* __poOverlayPoint = (COverlayPoint*)QTreeWidgetItem::child( __i );
    if( __poOverlayPoint->matchScrPosition( _poChart, _rqPointFScrPosition ) )
      return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayVisibility (override)
//------------------------------------------------------------------------------

void CVesselContainer::setVisibleName( bool _bVisibleName )
{
  bVisibleName = _bVisibleName;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->setVisibleName( bVisibleName );
}

void CVesselContainer::setVisiblePosition( bool _bVisiblePosition )
{
  bVisiblePosition = _bVisiblePosition;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->setVisiblePosition( bVisiblePosition );
}

void CVesselContainer::setVisibleCourse( bool _bVisibleCourse )
{
  bVisibleCourse = _bVisibleCourse;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->setVisibleCourse( bVisibleCourse );
}

void CVesselContainer::toggleVisible()
{
  if( !bVisible ) bVisible = true;
  else
  {
    int __iCount = QTreeWidgetItem::childCount();
    // Retrieve points visibility
    bVisibleName = false;
    bVisiblePosition = false;
    bVisibleCourse = false;
    for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
      bVisibleName |= __poVesselPoint->isVisibleName();
      bVisiblePosition |= __poVesselPoint->isVisiblePosition();
      bVisibleCourse |= __poVesselPoint->isVisibleCourse();
    }
    // Toggle global visibility
    COverlayVisibility::toggleVisible( true, false );
    // Set points visibility
    for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
      __poVesselPoint->setVisibleName( bVisibleName );
      __poVesselPoint->setVisiblePosition( bVisiblePosition );
      __poVesselPoint->setVisibleCourse( bVisibleCourse );
    }
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTERS
//

bool CVesselContainer::isDeviceConnected() const
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    if( !((CVesselPoint*)QTreeWidgetItem::child( __i ))->isDeviceConnected() ) return false;
  return true;
}

//
// OTHER
//

void CVesselContainer::setDevice( CVesselContainerDevice* _poVesselContainerDevice )
{
  clear();
  bDynamic = true;
  QTreeWidgetItem::addChild( _poVesselContainerDevice );
  _poVesselContainerDevice->connectDevice();
}

void CVesselContainer::connectDevice()
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->connectDevice();
}

void CVesselContainer::disconnectDevice()
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = bDynamic ? 1 : 0; __i < __iCount; __i++ )
    ((CVesselPoint*)QTreeWidgetItem::child( __i ))->disconnectDevice();
}

void CVesselContainer::clearDevice()
{
  if( !bDynamic ) return;
  clear();
}

CVesselPoint* CVesselContainer::addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
{
  if( bDynamic ) return 0;
  CVesselPoint* __poVesselPoint = new CVesselPoint( _rqsName, _roDataPosition );
  QTreeWidgetItem::addChild( __poVesselPoint );
  return __poVesselPoint;
}

void CVesselContainer::addPointDynamic( const QString& _rqsName, const QString& _rqsDeviceName )
{
  if( !bDynamic ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 1; __i < __iCount; __i++ )
  {
    CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
    if( __poVesselPoint->getName() == _rqsName ) return;
  }
  CVesselPoint* __poVesselPoint = new CVesselPoint( _rqsName, true );
  __poVesselPoint->setType( tr("Dynamic Vessel") );
  __poVesselPoint->setDescription( tr("This vessel has been automatically added as a result of the parent flotilla's device activity.") );
  __poVesselPoint->setSymbol( "DynamicVessel" );
  QTreeWidgetItem::addChild( __poVesselPoint );
  CVesselPointDevice* __poVesselPointDevice = new CVesselPointDevice( _rqsDeviceName, _rqsName, true );
  __poVesselPointDevice->setSynchronized( true, true, true,    // position & elevation
                                          true, true, true,    // ground course
                                          false, false, false, // apparent course
                                          true );              // additional textual data
  __poVesselPoint->addChild( __poVesselPointDevice );
  __poVesselPointDevice->connectDevice();
}

int CVesselContainer::cleanPointDynamic( int _iTTL )
{
  if( !bDynamic ) return 0;
  double __fdCurrentTime = (double)QDateTime::currentDateTime().toTime_t();
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 1; __i-- )
  {
    CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
    double __fdVesselTime = __poVesselPoint->getTime();
    if( __fdVesselTime != CDataTime::UNDEFINED_TIME
        && __fdCurrentTime - __fdVesselTime > (double)_iTTL )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poVesselPoint );
      delete __poVesselPoint;
    }
  }
  return __iCount;
}

int CVesselContainer::parseQVCT( const QDomElement& _rqDomElement )
{
  qsType = _rqDomElement.attribute( "type" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();
  QDomElement __qDomElement = _rqDomElement.firstChildElement( "Device" );
  if( !__qDomElement.isNull() )
  {
    CVesselContainerDevice* __poVesselContainerDevice = new CVesselContainerDevice( __qDomElement.attribute( "name" ) );
    __poVesselContainerDevice->parseQVCT( __qDomElement );
    setDevice( __poVesselContainerDevice );
    return 0;
  }
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Vessel" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Vessel" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( __qsName.isEmpty() ) COverlayObject::newChildName( tr("Vessel"), 1, __iCount );
    CVesselPoint* __poVesselPoint = new CVesselPoint( __qsName );
    __poVesselPoint->parseQVCT( __qDomElement );
    addChild( __poVesselPoint );
  }
  return __iCount;
}

void CVesselContainer::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // Data
  _rqXmlStreamWriter.writeStartElement( "Flotilla" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... type
  if( !qsType.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "type", qsType );
  // ... url
  if( !qsUrl.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "url", qsUrl );
  // ... description
  if( !qsDescription.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "Description" );
    _rqXmlStreamWriter.writeCDATA( qsDescription );
    _rqXmlStreamWriter.writeEndElement(); // Description
  }
  // ... comment
  if( !qsComment.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "Comment" );
    _rqXmlStreamWriter.writeCDATA( qsComment );
    _rqXmlStreamWriter.writeEndElement(); // Comment
  }
  // ... points
  if( !bDynamic )
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
      if( bOnlySelected && !__poVesselPoint->isMultiSelected() ) continue;
      __poVesselPoint->dumpQVCT( _rqXmlStreamWriter );
    }
  }
  else
  {
    CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)QTreeWidgetItem::child( 0 );
    __poVesselContainerDevice->dumpQVCT( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Flotilla
}

int CVesselContainer::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= bDynamic ? 1 : 0; __i-- )
  {
    CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::child( __i );
    if( __poVesselPoint->isMultiSelected() )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poVesselPoint );
      delete __poVesselPoint;
    }
  }
  return __iCount;
}

void CVesselContainer::clear()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  if( __pqTreeWidgetItem && bDynamic )
  {
    delete (CVesselContainerDevice*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
  while( __pqTreeWidgetItem )
  {
    delete (CVesselPoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
  bDynamic = false;
}
