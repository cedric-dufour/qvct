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
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackOverlay.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/track/CTrackSubContainer.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackOverlay::CTrackOverlay( QWidget* _pqParent )
  : COverlayBaseTree( _pqParent, tr("Tracks") )
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

CTrackOverlay::~CTrackOverlay()
{
  COverlayBaseTree::destroy();
  clear();
}


//------------------------------------------------------------------------------
// METHODS: QTreeWidget (implement/override)
//------------------------------------------------------------------------------

QStringList CTrackOverlay::mimeTypes() const
{
  static const QStringList __qStringList( "application/qvct.trackpoint" );
  return __qStringList;
}

QMimeData* CTrackOverlay::mimeData( const QList<QTreeWidgetItem*> _qListTreeWidgetItems ) const
{
  QByteArray __qByteArray;
  QDataStream __qDataStream( &__qByteArray, QIODevice::WriteOnly );
  for( int __i=0; __i < _qListTreeWidgetItems.count(); __i++ )
  {
    QTreeWidgetItem* __pqTreeWidgetItem = _qListTreeWidgetItems.at( __i );
    if( __pqTreeWidgetItem->type() != COverlayObject::ITEM ) continue;
    ((CTrackPoint*)__pqTreeWidgetItem)->serialize( __qDataStream );
  }
  QMimeData* __pqMimeData = new QMimeData();
  __pqMimeData->setData( "application/qvct.trackpoint", __qByteArray );
  return __pqMimeData;
}

bool CTrackOverlay::dropMimeData ( QTreeWidgetItem* _pqTreeWidgetItem, int _iIndex, const QMimeData* _pqMimeData, Qt::DropAction eAction )
{
  if( !_pqTreeWidgetItem ) return false;
  QByteArray __qByteArray = _pqMimeData->data( "application/qvct.trackpoint" );
  QDataStream __qDataStream( &__qByteArray, QIODevice::ReadOnly );
  CTrackPoint* __poTrackPoint = 0;
  while( !__qDataStream.atEnd() )
  {
    __poTrackPoint = new CTrackPoint( "#MIMEDATA#" );
    __poTrackPoint->unserialize( __qDataStream );
    _pqTreeWidgetItem->insertChild( _iIndex, __poTrackPoint );
  }
  if( __poTrackPoint ) QTreeWidget::setCurrentItem( __poTrackPoint );
  return true;
}

void CTrackOverlay::dropEvent( QDropEvent* _pqDropEvent )
{
  QTreeWidget::dropEvent( _pqDropEvent );
  COverlay::forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

//------------------------------------------------------------------------------
// METHODS: COverlay (implement/override)
//------------------------------------------------------------------------------

void CTrackOverlay::drawContent( const CChart* _poChart, QPainter* _pqPainter ) const
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackContainer*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CTrackOverlay::showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( !_pqTreeWidgetItem ) return;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    {
      CTrackContainer* __poTrackContainer = (CTrackContainer*)_pqTreeWidgetItem;
      __poTrackContainer->showDetail();
    }
    break;

  case COverlayObject::SUBCONTAINER:
    {
      CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)_pqTreeWidgetItem;
      __poTrackSubContainer->showDetail();
    }
    break;

  case COverlayObject::ITEM:
    {
      CTrackPoint* __poTrackPoint = (CTrackPoint*)_pqTreeWidgetItem;
      CChartTable* __poChartTable = QVCTRuntime::useChartTable();
      if( __poTrackPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED )
          && ( __poChartTable->setPointerTarget( *__poTrackPoint )
               || __poChartTable->extendPointerPath( *__poTrackPoint ) ) )
        __poChartTable->showGeoPosition( *__poTrackPoint );
      else
        __poTrackPoint->showDetail();
    }
    break;

  default:;

  }
}

void CTrackOverlay::setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    if( _pqTreeWidgetItem->isExpanded() ) return; // NOTE: Expansion switch is handled *AFTER* this method is called
    {
      CTrackContainer* __poTrackContainer = (CTrackContainer*)_pqTreeWidgetItem;
      CDataPosition __oDataPositionLower( 180, 90, 999999 ), __oDataPositionUpper( -180, -90, -999999 );
      int __iCountBox = 0;
      int __iCount = __poTrackContainer->childCount();
      for( int __i = 0; __i < __iCount; __i++ )
      {
        CDataPosition __oDataPositionLower_aux, __oDataPositionUpper_aux;
        int __iCountBox_aux = COverlayPoint::getPositionBox( (COverlayObject*)__poTrackContainer->child( __i ), &__oDataPositionLower_aux, &__oDataPositionUpper_aux );
        if( __iCountBox_aux )
        {
          __iCountBox += __iCountBox_aux;
          __oDataPositionLower.setPosition( std::min( __oDataPositionLower.getLongitude(), __oDataPositionLower_aux.getLongitude() ),
                                           std::min( __oDataPositionLower.getLatitude(), __oDataPositionLower_aux.getLatitude() ),
                                           std::min( __oDataPositionLower.getElevation(), __oDataPositionLower_aux.getElevation() ) );
          __oDataPositionUpper.setPosition( std::max( __oDataPositionUpper.getLongitude(), __oDataPositionUpper_aux.getLongitude() ),
                                           std::max( __oDataPositionUpper.getLatitude(), __oDataPositionUpper_aux.getLatitude() ),
                                           std::max( __oDataPositionUpper.getElevation(), __oDataPositionUpper_aux.getElevation() ) );
        }
      }
      if( __iCountBox > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
      else if( __iCountBox == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
    }
    break;

  case COverlayObject::SUBCONTAINER:
    if( _pqTreeWidgetItem->isExpanded() ) return; // NOTE: Expansion switch is handled *AFTER* this method is called
    {
      CDataPosition __oDataPositionLower, __oDataPositionUpper;
      int __iCount = COverlayPoint::getPositionBox( (CTrackSubContainer*)_pqTreeWidgetItem, &__oDataPositionLower, &__oDataPositionUpper );
      if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
      else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
    }
    break;

  case COverlayObject::ITEM:
    {
      CTrackPoint* __poTrackPoint = (CTrackPoint*)_pqTreeWidgetItem;
      if( __poTrackPoint->CDataPosition::operator==( CDataPosition::UNDEFINED ) ) return;
      QVCTRuntime::useChartTable()->setGeoPosition( *__poTrackPoint );
    }
    break;

  default:;

  }
}

COverlayPoint* CTrackOverlay::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CTrackOverlay::draw() to pick the correct (overlapping) item
  {
    CTrackContainer* __poTrackContainer = (CTrackContainer*)QTreeWidgetItem::child( __i );
    COverlayPoint* __poOverlayPoint = __poTrackContainer->matchScrPosition( _poChart, _rqPointFScrPosition );
    if( __poOverlayPoint ) return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayBaseTree (implement/override)
//------------------------------------------------------------------------------

void CTrackOverlay::onChange( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
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
      CTrackContainer* __poTrackContainer = (CTrackContainer*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poTrackContainer->setName( _pqTreeWidgetItem->text( NAME ) );
        break;
      case VISIBLE:
        __poTrackContainer->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  case COverlayObject::SUBCONTAINER:
    {
      CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case VISIBLE:
        __poTrackSubContainer->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  case COverlayObject::ITEM:
    {
      CTrackPoint* __poTrackPoint = (CTrackPoint*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poTrackPoint->setName( _pqTreeWidgetItem->text( NAME ) );
        bRedraw = true;
        break;
      case VISIBLE:
        __poTrackPoint->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      case SELECT:
        __poTrackPoint->setMultiSelected( _pqTreeWidgetItem->checkState( SELECT ) == Qt::Checked );
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

void CTrackOverlay::importSettings()
{
  QColor __qColor = QVCTRuntime::useSettings()->getColorTrack();
  __qColor.setAlpha( 128 );
  qBrushMarker.setColor( __qColor );
  __qColor.setAlpha( 192 );
  qPenText.setColor( QColor::fromHsv( __qColor.hue(), __qColor.saturation(), std::min( 128, __qColor.value() ), 192 ) );
  qPenMarker.setColor( __qColor );
  qPenLine.setColor( __qColor );
  qPenVector.setColor( __qColor );
}

CTrackContainer* CTrackOverlay::pickContainer( const QString& _rqsName )
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CTrackContainer* __poTrackContainer = (CTrackContainer*)QTreeWidgetItem::child( __i );
    if( __poTrackContainer->getName() == _rqsName ) return __poTrackContainer;
  }
  return addContainer( _rqsName );
}

CTrackSubContainer* CTrackOverlay::pickSubContainer( const QString& _rqsName )
{
  CTrackContainer* __poTrackContainer = pickContainer( _rqsName );
  return __poTrackContainer->pickSubContainer();
}

CTrackContainer* CTrackOverlay::addContainer( const QString& _rqsName )
{
  CTrackContainer* __poTrackContainer = new CTrackContainer( _rqsName );
  QTreeWidgetItem::addChild( __poTrackContainer );
  QTreeWidgetItem::setExpanded( true );
  return __poTrackContainer;
}

int CTrackOverlay::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    __iCount += ((CTrackContainer*)QTreeWidgetItem::child( __i ))->deleteSelection();
  return __iCount;
}

void CTrackOverlay::clear()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CTrackContainer*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}

CTrackContainer* CTrackOverlay::load( const QString& _rqsFilename )
{
  QFileInfo __qFileInfo( _rqsFilename );
  CTrackContainer* __poTrackContainer = 0;
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
    if( __qDocType == "QVCT" ) parseQVCT( __qDomElement, &__poTrackContainer );
    else if( __qDocType == "gpx" ) parseGPX( __qDomElement, &__poTrackContainer );
  }
  while( false ); // error-catching context [end]
  if( !__qsError.isEmpty() )
  {
    qCritical( "ERROR[%s]: %s", Q_FUNC_INFO, qPrintable( __qsError ) );
    QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return 0;
  }
  QTreeWidgetItem::setExpanded( true );
  return __poTrackContainer;
}

int CTrackOverlay::parseQVCT( const QDomElement& _rqDomElement, CTrackContainer** _ppoTrackContainer )
{
  CTrackContainer* __poTrackContainer = 0;
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Track" ); !__qDomElement.isNull(); __qDomElement = __qDomElement.nextSiblingElement( "Track" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( !__qsName.isEmpty() ) __qsName = COverlay::newChildName( __qsName );
    else __qsName = COverlay::newChildName( tr("Track"), 1, true );
    __poTrackContainer = new CTrackContainer( __qsName );
    __poTrackContainer->parseQVCT( __qDomElement );
    QTreeWidgetItem::addChild( __poTrackContainer );
  }
  if( _ppoTrackContainer ) *_ppoTrackContainer = __poTrackContainer;
  return __iCount;
}

int CTrackOverlay::parseGPX( const QDomElement& _rqDomElement, CTrackContainer** _ppoTrackContainer )
{
  CTrackContainer* __poTrackContainer = 0;
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "trk" ); !__qDomElement.isNull(); __qDomElement = __qDomElement.nextSiblingElement( "trk" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.firstChildElement( "name" ).text();
    if( !__qsName.isEmpty() ) __qsName = COverlay::newChildName( __qsName );
    else __qsName = COverlay::newChildName( tr("Track"), 1, true );
    __poTrackContainer = new CTrackContainer( __qsName );
    __poTrackContainer->parseGPX( __qDomElement );
    QTreeWidgetItem::addChild( __poTrackContainer );
  }
  if( _ppoTrackContainer ) *_ppoTrackContainer = __poTrackContainer;
  return __iCount;
}

void CTrackOverlay::save( const QString& _rqsFilename, CTrackContainer* _poTrackContainer ) const
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
  if( __qsFormat == "qvct" ) dumpQVCT( __qXmlStreamWriter, _poTrackContainer );
  else if( __qsFormat == "gpx" ) dumpGPX( __qXmlStreamWriter, _poTrackContainer );

  // XML [end]
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

void CTrackOverlay::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CTrackContainer* _poTrackContainer, bool _bProjectDump ) const
{
  // Data
  if( !_bProjectDump ) _rqXmlStreamWriter.writeStartElement( "QVCT" );
  // ... containers
  if( _poTrackContainer )
  {
    _poTrackContainer->dumpQVCT( _rqXmlStreamWriter, false );
  }
  else // no container given; assume selection dump or full dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
      ((CTrackContainer*)QTreeWidgetItem::child( __i ))->dumpQVCT( _rqXmlStreamWriter, !_bProjectDump );
  }
  // ... [end]
  if( !_bProjectDump ) _rqXmlStreamWriter.writeEndElement(); // QVCT
}

void CTrackOverlay::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, CTrackContainer* _poTrackContainer ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "gpx" );
  _rqXmlStreamWriter.writeAttribute( "version", "1.1" );
  _rqXmlStreamWriter.writeAttribute( "creator", "Qt Virtual Chart Table (QVCT)" );
  // ... containers
  if( _poTrackContainer )
  {
    _poTrackContainer->dumpGPX( _rqXmlStreamWriter, false );
  }
  else // no container given; assume selection dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
      ((CTrackContainer*)QTreeWidgetItem::child( __i ))->dumpGPX( _rqXmlStreamWriter, true );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // gpx
}
