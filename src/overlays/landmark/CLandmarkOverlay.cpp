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
#include <QByteArray>
#include <QDataStream>
#include <QDomDocument> // QtXml module
#include <QFileInfo>
#include <QList>
#include <QMimeData>
#include <QPainter>
#include <QPointF>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkContainerPickView.hpp"
#include "overlays/landmark/CLandmarkOverlay.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkOverlay::CLandmarkOverlay( QWidget* _pqParent )
  : COverlayBaseTree( _pqParent, tr("Landmarks") )
{
  importSettings();

  // Tree widget
  // ... columns
  QTreeWidget::setColumnCount( 3 );
  QTreeWidget::setColumnWidth( NAME, 184 );
  // ... header
  QTreeWidgetItem* __pqTreeWidgetItem = new QTreeWidgetItem();
  __pqTreeWidgetItem->setText( NAME, tr("Name") );
  __pqTreeWidgetItem->setIcon( VISIBLE, QIcon( ":icons/16x16/visible.png" ) );
  __pqTreeWidgetItem->setIcon( SELECT, QIcon( ":icons/16x16/select.png" ) );
  QTreeWidget::setHeaderItem( __pqTreeWidgetItem );
  QTreeWidget::resizeColumnToContents( VISIBLE );
  QTreeWidget::resizeColumnToContents( SELECT );
  // ... top-level item
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
  QTreeWidgetItem::setText( NAME, COverlay::qsName );
  QTreeWidgetItem::setCheckState( VISIBLE, Qt::Checked );
  // ... drag 'n drop
  QTreeWidget::setDragDropMode( QAbstractItemView::InternalMove );
}

CLandmarkOverlay::~CLandmarkOverlay()
{
  clear();
}


//------------------------------------------------------------------------------
// METHODS: QTreeWidget (implement/override)
//------------------------------------------------------------------------------

QStringList CLandmarkOverlay::mimeTypes() const
{
  static const QStringList __qStringList( "application/qvct.landmarkpoint" );
  return __qStringList;
}

QMimeData* CLandmarkOverlay::mimeData( const QList<QTreeWidgetItem*> _qListTreeWidgetItems ) const
{
  QByteArray __qByteArray;
  QDataStream __qDataStream( &__qByteArray, QIODevice::WriteOnly );
  for( int __i=0; __i < _qListTreeWidgetItems.count(); __i++ )
  {
    QTreeWidgetItem* __pqTreeWidgetItem = _qListTreeWidgetItems.at( __i );
    if( __pqTreeWidgetItem->type() != COverlayObject::ITEM ) continue;
    ((CLandmarkPoint*)__pqTreeWidgetItem)->serialize( __qDataStream );
  }
  QMimeData* __pqMimeData = new QMimeData();
  __pqMimeData->setData( "application/qvct.landmarkpoint", __qByteArray );
  return __pqMimeData;
}

bool CLandmarkOverlay::dropMimeData ( QTreeWidgetItem* _pqTreeWidgetItem, int _iIndex, const QMimeData* _pqMimeData, Qt::DropAction eAction )
{
  if( !_pqTreeWidgetItem ) return false;
  QByteArray __qByteArray = _pqMimeData->data( "application/qvct.landmarkpoint" );
  QDataStream __qDataStream( &__qByteArray, QIODevice::ReadOnly );
  CLandmarkPoint* __poLandmarkPoint = 0;
  while( !__qDataStream.atEnd() )
  {
    __poLandmarkPoint = new CLandmarkPoint( "#MIMEDATA#" );
    __poLandmarkPoint->unserialize( __qDataStream );
    _pqTreeWidgetItem->insertChild( _iIndex, __poLandmarkPoint );
  }
  if( __poLandmarkPoint ) QTreeWidget::setCurrentItem( __poLandmarkPoint );
  return true;
}


//------------------------------------------------------------------------------
// METHODS: COverlay (implement/override)
//------------------------------------------------------------------------------

void CLandmarkOverlay::drawContent( const CChart* _poChart, QPainter* _pqPainter ) const
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CLandmarkContainer*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CLandmarkOverlay::showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( !_pqTreeWidgetItem ) return;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    {
      CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)_pqTreeWidgetItem;
      __poLandmarkContainer->showDetail();
    }
    break;

  case COverlayObject::ITEM:
    {
      CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)_pqTreeWidgetItem;
      CChartTable* __poChartTable = QVCTRuntime::useChartTable();
      if( __poLandmarkPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED )
          && ( __poChartTable->setPointerTarget( *__poLandmarkPoint )
               || __poChartTable->extendPointerPath( *__poLandmarkPoint ) ) )
        __poChartTable->showGeoPosition( *__poLandmarkPoint );
      else
        __poLandmarkPoint->showDetail();
    }
    break;

  default:;

  }
}

void CLandmarkOverlay::setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    if( _pqTreeWidgetItem->isExpanded() ) return; // NOTE: Expansion switch is handled *AFTER* this method is called
    {
      CDataPosition __oDataPositionLower, __oDataPositionUpper;
      int __iCount = COverlayPoint::getPositionBox( (CLandmarkContainer*)_pqTreeWidgetItem, &__oDataPositionLower, &__oDataPositionUpper );
      if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
      else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
    }
    break;

  case COverlayObject::ITEM:
    {
      CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)_pqTreeWidgetItem;
      if( __poLandmarkPoint->CDataPosition::operator==( CDataPosition::UNDEFINED ) ) return;
      QVCTRuntime::useChartTable()->setGeoPosition( *__poLandmarkPoint );
    }
    break;

  default:;

  }
}

COverlayPoint* CLandmarkOverlay::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CLandmarkOverlay::draw() to pick the correct (overlapping) item
  {
    CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)QTreeWidgetItem::child( __i );
    COverlayPoint* __poOverlayPoint = __poLandmarkContainer->matchScrPosition( _poChart, _rqPointFScrPosition );
    if( __poOverlayPoint ) return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayBaseTree (implement/override)
//------------------------------------------------------------------------------

void CLandmarkOverlay::onChange( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
{
  bool bRedraw = false;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::OVERLAY:
    {
      switch( _iColumn )
      {
      case VISIBLE:
        COverlay::setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  case COverlayObject::CONTAINER:
    {
      CLandmarkContainer* __poLandmarkContainer = (CLandmarkContainer*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poLandmarkContainer->setName( _pqTreeWidgetItem->text( NAME ) );
        break;
      case VISIBLE:
        __poLandmarkContainer->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  case COverlayObject::ITEM:
    {
      CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poLandmarkPoint->setName( _pqTreeWidgetItem->text( NAME ) );
        bRedraw = true;
        break;
      case VISIBLE:
        __poLandmarkPoint->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      case SELECT:
        __poLandmarkPoint->setMultiSelected( _pqTreeWidgetItem->checkState( SELECT ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  default:;

  }

  // Redraw
  if( bRedraw )
  {
    COverlay::forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void CLandmarkOverlay::importSettings()
{
  QColor __qColor = QVCTRuntime::useSettings()->getColorLandmark();
  __qColor.setAlpha( 128 );
  qBrushMarker.setColor( __qColor );
  __qColor.setAlpha( 192 );
  qPenText.setColor( __qColor );
  qPenMarker.setColor( __qColor );
  qPenLine.setColor( __qColor );
  qPenVector.setColor( __qColor );
}

CLandmarkContainer* CLandmarkOverlay::addContainer( const QString& _rqsName )
{
  CLandmarkContainer* __poLandmarkContainer = new CLandmarkContainer( _rqsName );
  QTreeWidgetItem::addChild( __poLandmarkContainer );
  QTreeWidgetItem::setExpanded( true );
  return __poLandmarkContainer;
}

CLandmarkContainer* CLandmarkOverlay::pickContainer()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return addContainer( COverlay::newChildName( "Landmarks", 1, true ) );
  if( __iCount == 1 ) return (CLandmarkContainer*)QTreeWidgetItem::child( 0 );
  // else: we must have the user choose among the available containers
  COverlayObject* __poOverlayObject = 0;
  CLandmarkContainerPickView* __poLandmarkContainerPickView = new CLandmarkContainerPickView( this, &__poOverlayObject );
  __poLandmarkContainerPickView->exec();
  delete __poLandmarkContainerPickView;
  return (CLandmarkContainer*)__poOverlayObject;
}

int CLandmarkOverlay::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    __iCount += ((CLandmarkContainer*)QTreeWidgetItem::child( __i ))->deleteSelection();
  return __iCount;
}

void CLandmarkOverlay::clear()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CLandmarkContainer*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}

CLandmarkContainer* CLandmarkOverlay::load( const QString& _rqsFilename )
{
  QFileInfo __qFileInfo( _rqsFilename );
  CLandmarkContainer* __poLandmarkContainer = 0;
  QString __qsError;
  do // error-catching context [begin]
  {
    // File
    QFileInfo __qFileInfo( _rqsFilename );
    QFile __qFile( __qFileInfo.absoluteFilePath() );
    if( !__qFile.open( QIODevice::ReadOnly ) )
    {
      __qsError = QString( "Failed to open file (%1)" ).arg( __qFile.fileName() );
      break;
    }
    QDomDocument __qDocDocument;
    if( !__qDocDocument.setContent( &__qFile ) )
    {
      __qsError = QString( "Failed to parse XML (%1)" ).arg( __qFile.fileName() );
      __qFile.close();
      break;
    }
    __qFile.close();

    // XML
    QDomElement __qDomElement = __qDocDocument.documentElement();
    QString __qDocType = __qDomElement.nodeName();
    if( __qDomElement.isNull() || ( __qDocType != "QVCT" && __qDocType != "gpx" ) )
    {
      __qsError = QString( "Invalid XML document type (%1); expected: 'QVCT' or 'gpx'" ).arg( __qFile.fileName() );
      break;
    }
    if( __qDocType == "QVCT" ) parseQVCT( __qDomElement, &__poLandmarkContainer );
    else if( __qDocType == "gpx" )
    {
      __poLandmarkContainer = new CLandmarkContainer( COverlay::newChildName( __qFileInfo.baseName() ) );
      __poLandmarkContainer->parseGPX( __qDomElement );
      QTreeWidgetItem::addChild( __poLandmarkContainer );
    }
  }
  while( false ); // error-catching context [end]
  if( !__qsError.isEmpty() )
  {
    qCritical( "ERROR[%s]: %s", Q_FUNC_INFO, qPrintable( __qsError ) );
    QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return 0;
  }
  QTreeWidgetItem::setExpanded( true );
  return __poLandmarkContainer;
}

int CLandmarkOverlay::parseQVCT( const QDomElement& _rqDomElement, CLandmarkContainer** _ppoLandmarkContainer )
{
  CLandmarkContainer* __poLandmarkContainer = 0;
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Landmarks" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Landmarks" ) )
  {
    __iCount++;
    __poLandmarkContainer = new CLandmarkContainer( COverlay::newChildName( __qDomElement.attribute( "name", tr("Landmarks") ) ) );
    __poLandmarkContainer->parseQVCT( __qDomElement );
    QTreeWidgetItem::addChild( __poLandmarkContainer );
  }
  if( _ppoLandmarkContainer ) *_ppoLandmarkContainer = __poLandmarkContainer;
  return __iCount;
}

void CLandmarkOverlay::save( const QString& _rqsFilename, CLandmarkContainer* _poLandmarkContainer ) const
{
  QFileInfo __qFileInfo( _rqsFilename );
  QString __qsFormat = __qFileInfo.suffix();
  if( __qsFormat != "qvct" && __qsFormat != "gpx" )
  {
    qCritical( "ERROR[%s]: Invalid file format/extention (%s); expected: 'qvct' or 'gpx'", Q_FUNC_INFO, qPrintable( __qsFormat ) );
    return;
  }

  // File [open]
  QFile __qFile( __qFileInfo.absoluteFilePath() );
  if( !__qFile.open( QIODevice::WriteOnly ) )
  {
    qCritical( "ERROR[%s]: Failed to open file (%s)", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    return;
  }

  // XML [start]
  QXmlStreamWriter __qXmlStreamWriter( &__qFile );
  __qXmlStreamWriter.setAutoFormatting( true );
  __qXmlStreamWriter.writeStartDocument();

  // Data
  if( __qsFormat == "qvct" ) dumpQVCT( __qXmlStreamWriter, _poLandmarkContainer );
  else if( __qsFormat == "gpx" ) dumpGPX( __qXmlStreamWriter, _poLandmarkContainer );

  // XML [end]
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

void CLandmarkOverlay::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CLandmarkContainer* _poLandmarkContainer, bool _bProjectDump ) const
{
  // Data
  if( !_bProjectDump ) _rqXmlStreamWriter.writeStartElement( "QVCT" );
  // ... containers
  if( _poLandmarkContainer )
  {
    _poLandmarkContainer->dumpQVCT( _rqXmlStreamWriter, false );
  }
  else // no container given; assume selection dump or full dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
      ((CLandmarkContainer*)QTreeWidgetItem::child( __i ))->dumpQVCT( _rqXmlStreamWriter, !_bProjectDump );
  }
  // ... [end]
  if( !_bProjectDump ) _rqXmlStreamWriter.writeEndElement(); // QVCT
}

void CLandmarkOverlay::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, CLandmarkContainer* _poLandmarkContainer ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "gpx" );
  _rqXmlStreamWriter.writeAttribute( "version", "1.1" );
  _rqXmlStreamWriter.writeAttribute( "creator", "Qt Virtual Chart Table (QVCT)" );
  // ... containers
  if( _poLandmarkContainer )
  {
    _poLandmarkContainer->dumpGPX( _rqXmlStreamWriter, false );
  }
  else // no container given; assume selection dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
      ((CLandmarkContainer*)QTreeWidgetItem::child( __i ))->dumpGPX( _rqXmlStreamWriter, true );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // gpx
}
