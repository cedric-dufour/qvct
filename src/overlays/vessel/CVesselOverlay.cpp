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
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/CVesselOverlay.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselPointDevice.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselOverlay::CVesselOverlay( QWidget* _pqParent )
  : COverlayBaseTree( _pqParent, tr("Vessels") )
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

CVesselOverlay::~CVesselOverlay()
{
  COverlayBaseTree::destroy();
  clear();
}


//------------------------------------------------------------------------------
// METHODS: QTreeWidget (implement/override)
//------------------------------------------------------------------------------

QStringList CVesselOverlay::mimeTypes() const
{
  static const QStringList __qStringList( "application/qvct.vesselpoint" );
  return __qStringList;
}

QMimeData* CVesselOverlay::mimeData( const QList<QTreeWidgetItem*> _qListTreeWidgetItems ) const
{
  QByteArray __qByteArray;
  QDataStream __qDataStream( &__qByteArray, QIODevice::WriteOnly );
  for( int __i=0; __i < _qListTreeWidgetItems.count(); __i++ )
  {
    QTreeWidgetItem* __pqTreeWidgetItem = _qListTreeWidgetItems.at( __i );
    if( __pqTreeWidgetItem->type() != COverlayObject::ITEM ) continue;
    CVesselPoint* __poVesselPoint = (CVesselPoint*)__pqTreeWidgetItem;
    __poVesselPoint->serialize( __qDataStream );
  }
  QMimeData* __pqMimeData = new QMimeData();
  __pqMimeData->setData( "application/qvct.vesselpoint", __qByteArray );
  return __pqMimeData;
}

bool CVesselOverlay::dropMimeData ( QTreeWidgetItem* _pqTreeWidgetItem, int _iIndex, const QMimeData* _pqMimeData, Qt::DropAction eAction )
{
  if( !_pqTreeWidgetItem ) return false;
  QByteArray __qByteArray = _pqMimeData->data( "application/qvct.vesselpoint" );
  QDataStream __qDataStream( &__qByteArray, QIODevice::ReadOnly );
  CVesselPoint* __poVesselPoint = 0;
  while( !__qDataStream.atEnd() )
  {
    __poVesselPoint = new CVesselPoint( "#MIMEDATA#" );
    __poVesselPoint->unserialize( __qDataStream );
    _pqTreeWidgetItem->insertChild( _iIndex, __poVesselPoint );
  }
  if( __poVesselPoint ) QTreeWidget::setCurrentItem( __poVesselPoint );
  return true;
}

void CVesselOverlay::dropEvent( QDropEvent* _pqDropEvent )
{
  QTreeWidget::dropEvent( _pqDropEvent );
  COverlay::forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

//------------------------------------------------------------------------------
// METHODS: COverlay (implement/override)
//------------------------------------------------------------------------------

void CVesselOverlay::drawContent( const CChart* _poChart, QPainter* _pqPainter ) const
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CVesselContainer*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CVesselOverlay::showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( !_pqTreeWidgetItem ) return;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    {
      CVesselContainer* __poVesselContainer = (CVesselContainer*)_pqTreeWidgetItem;
      __poVesselContainer->showDetail();
    }
    break;

  case COverlayObject::ITEM:
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)_pqTreeWidgetItem;
      CChartTable* __poChartTable = QVCTRuntime::useChartTable();
      if( __poVesselPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED )
          && ( __poChartTable->setPointerTarget( *__poVesselPoint )
               || __poChartTable->extendPointerPath( *__poVesselPoint ) ) )
        __poChartTable->showGeoPosition( *__poVesselPoint );
      else
        __poVesselPoint->showDetail();
    }
    break;

  case COverlayObject::SUBITEM1:
    {
      CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)_pqTreeWidgetItem;
      __poVesselContainerDevice->showDetail();
    }
    break;

  case COverlayObject::SUBITEM2:
    {
      CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)_pqTreeWidgetItem;
      __poVesselPointDevice->showDetail();
    }
    break;

  default:;

  }
}

void CVesselOverlay::setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::CONTAINER:
    if( _pqTreeWidgetItem->isExpanded() ) return; // NOTE: Expansion switch is handled *AFTER* this method is called
    {
      CDataPosition __oDataPositionLower, __oDataPositionUpper;
      int __iCount = COverlayPoint::getPositionBox( (CVesselContainer*)_pqTreeWidgetItem, &__oDataPositionLower, &__oDataPositionUpper );
      if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( __oDataPositionLower, __oDataPositionUpper, 0.9 );
      else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( __oDataPositionLower );
    }
    break;

  case COverlayObject::ITEM:
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)_pqTreeWidgetItem;
      if( __poVesselPoint->CDataPosition::operator==( CDataPosition::UNDEFINED ) ) return;
      QVCTRuntime::useChartTable()->setGeoPosition( *__poVesselPoint );
    }
    break;

  default:;

  }
}

COverlayPoint* CVesselOverlay::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CVesselOverlay::draw() to pick the correct (overlapping) item
  {
    CVesselContainer* __poVesselContainer = (CVesselContainer*)QTreeWidgetItem::child( __i );
    COverlayPoint* __poOverlayPoint = __poVesselContainer->matchScrPosition( _poChart, _rqPointFScrPosition );
    if( __poOverlayPoint ) return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayBaseTree (implement/override)
//------------------------------------------------------------------------------

void CVesselOverlay::onChange( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
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
      CVesselContainer* __poVesselContainer = (CVesselContainer*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poVesselContainer->setName( _pqTreeWidgetItem->text( NAME ) );
        break;
      case VISIBLE:
        __poVesselContainer->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      default:;
      }
    }
    break;

  case COverlayObject::ITEM:
    {
      CVesselPoint* __poVesselPoint = (CVesselPoint*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case NAME:
        __poVesselPoint->setName( _pqTreeWidgetItem->text( NAME ) );
        bRedraw = true;
        break;
      case VISIBLE:
        __poVesselPoint->setVisible( _pqTreeWidgetItem->checkState( VISIBLE ) == Qt::Checked );
        bRedraw = true;
        break;
      case SELECT:
        __poVesselPoint->setMultiSelected( _pqTreeWidgetItem->checkState( SELECT ) == Qt::Checked );
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

void CVesselOverlay::importSettings()
{
  QColor __qColor = QVCTRuntime::useSettings()->getColorVessel();
  __qColor.setAlpha( 128 );
  qBrushMarker.setColor( __qColor );
  __qColor.setAlpha( 192 );
  qPenText.setColor( QColor::fromHsv( __qColor.hue(), __qColor.saturation(), std::min( 128, __qColor.value() ), 192 ) );
  qPenMarker.setColor( __qColor );
  qPenLine.setColor( __qColor );
  qPenVector.setColor( __qColor );
}

CVesselContainer* CVesselOverlay::addContainer( const QString& _rqsName )
{
  CVesselContainer* __poVesselContainer = new CVesselContainer( _rqsName );
  QTreeWidgetItem::addChild( __poVesselContainer );
  QTreeWidgetItem::setExpanded( true );
  return __poVesselContainer;
}

int CVesselOverlay::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    __iCount += ((CVesselContainer*)QTreeWidgetItem::child( __i ))->deleteSelection();
  return __iCount;
}

void CVesselOverlay::clear()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CVesselContainer*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}

CVesselContainer* CVesselOverlay::load( const QString& _rqsFilename )
{
  QFileInfo __qFileInfo( _rqsFilename );
  CVesselContainer* __poVesselContainer = 0;
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
    if( __qDomElement.isNull() || __qDocType != "QVCT" )
    {
      __qsError = QString( "Invalid XML document type (%1); expected: 'QVCT'" ).arg( __qFile.fileName() );
      break;
    }
    parseQVCT( __qDomElement, &__poVesselContainer );
  }
  while( false ); // error-catching context [end]
  if( !__qsError.isEmpty() )
  {
    qCritical( "ERROR[%s]: %s", Q_FUNC_INFO, qPrintable( __qsError ) );
    QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return 0;
  }
  QTreeWidgetItem::setExpanded( true );
  return __poVesselContainer;
}

int CVesselOverlay::parseQVCT( const QDomElement& _rqDomElement, CVesselContainer** _ppoVesselContainer )
{
  CVesselContainer* __poVesselContainer = 0;
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Flotilla" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Flotilla" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( !__qsName.isEmpty() ) __qsName = COverlay::newChildName( __qsName );
    else __qsName = COverlay::newChildName( tr("Flotilla"), 1, true );
    __poVesselContainer = new CVesselContainer( __qsName );
    __poVesselContainer->parseQVCT( __qDomElement );
    QTreeWidgetItem::addChild( __poVesselContainer );
  }
  if( _ppoVesselContainer ) *_ppoVesselContainer = __poVesselContainer;
  return __iCount;
}

void CVesselOverlay::save( const QString& _rqsFilename, CVesselContainer* _poVesselContainer ) const
{
  QFileInfo __qFileInfo( _rqsFilename );
  QString __qsFormat = __qFileInfo.suffix();
  if( __qsFormat != "qvct" )
  {
    qCritical( "ERROR[%s]: Invalid file format/extention (%s); expected: 'qvct'", Q_FUNC_INFO, qPrintable( __qsFormat ) );
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
  dumpQVCT( __qXmlStreamWriter, _poVesselContainer );

  // XML [end]
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

void CVesselOverlay::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CVesselContainer* _poVesselContainer, bool _bProjectDump ) const
{
  // Data
  if( !_bProjectDump ) _rqXmlStreamWriter.writeStartElement( "QVCT" );
  // ... containers
  if( _poVesselContainer )
  {
    _poVesselContainer->dumpQVCT( _rqXmlStreamWriter, false );
  }
  else // no container given; assume selection dump or full dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
      ((CVesselContainer*)QTreeWidgetItem::child( __i ))->dumpQVCT( _rqXmlStreamWriter, !_bProjectDump );
  }
  // ... [end]
  if( !_bProjectDump ) _rqXmlStreamWriter.writeEndElement(); // QVCT
}
