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
#include "devices/CDevice.hpp"
#include "overlays/device/CDeviceOverlay.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceOverlay::CDeviceOverlay( QWidget* _pqParent )
  : COverlayBaseTree( _pqParent, tr("Devices") )
{
  // Tree widget
  // ... columns
  QTreeWidget::setColumnCount( 2 );
  QTreeWidget::setColumnWidth( NAME, 200 );
  // ... header
  QTreeWidgetItem* __pqTreeWidgetItem = new QTreeWidgetItem();
  __pqTreeWidgetItem->setText( NAME, tr("Name") );
  __pqTreeWidgetItem->setIcon( SELECT, QIcon( ":icons/16x16/select.png" ) );
  QTreeWidget::setHeaderItem( __pqTreeWidgetItem );
  QTreeWidget::resizeColumnToContents( SELECT );
  // ... top-level item
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
  QTreeWidgetItem::setText( NAME, COverlay::qsName );
}

CDeviceOverlay::~CDeviceOverlay()
{
  COverlayBaseTree::destroy();
  clear();
}


//------------------------------------------------------------------------------
// METHODS: COverlay (implement/override)
//------------------------------------------------------------------------------

void CDeviceOverlay::showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( !_pqTreeWidgetItem ) return;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::OVERLAY:
    // NOP; top-level overlay has no detail view
    break;

  case COverlayObject::ITEM:
    {
      CDevice* __poDevice = (CDevice*)_pqTreeWidgetItem;
      __poDevice->showDetail();
    }
    break;

  default:;

  }
}


//------------------------------------------------------------------------------
// METHODS: COverlayBaseTree (implement/override)
//------------------------------------------------------------------------------

void CDeviceOverlay::onChange( QTreeWidgetItem* _pqTreeWidgetItem, int _iColumn )
{
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::ITEM:
    {
      CDevice* __poDevice = (CDevice*)_pqTreeWidgetItem;
      switch( _iColumn )
      {
      case SELECT:
        __poDevice->setMultiSelected( _pqTreeWidgetItem->checkState( SELECT ) == Qt::Checked );
        break;
      default:;
      }
    }
    break;

  default:;

  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CDevice* CDeviceOverlay::pickDevice( const QString& _rqsName )
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CDevice* __poDevice = (CDevice*)QTreeWidgetItem::child( __i );
    if( __poDevice->getName() == _rqsName ) return __poDevice;
  }
  return 0;
}

int CDeviceOverlay::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
  {
    CDevice* __poDevice = (CDevice*)QTreeWidgetItem::child( __i );
    if( __poDevice->isMultiSelected() )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poDevice );
      delete __poDevice;
    }
  }
  return __iCount;
}

void CDeviceOverlay::clear()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CDevice*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}

CDevice* CDeviceOverlay::load( const QString& _rqsFilename, bool _bSilent )
{
  QFileInfo __qFileInfo( _rqsFilename );
  CDevice* __poDevice = 0;
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
    parseQVCT( __qDomElement, &__poDevice );
  }
  while( false ); // error-catching context [end]
  if( !__qsError.isEmpty() )
  {
    qCritical( "ERROR[%s]: %s", Q_FUNC_INFO, qPrintable( __qsError ) );
    if( !_bSilent ) QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return 0;
  }
  QTreeWidgetItem::setExpanded( true );
  return __poDevice;
}

int CDeviceOverlay::parseQVCT( const QDomElement& _rqDomElement, CDevice** _ppoDevice )
{
  CDevice* __poDevice = 0;
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Device" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Device" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( !__qsName.isEmpty() ) __qsName = COverlay::newChildName( __qsName );
    else __qsName = COverlay::newChildName( tr("Device"), 1, true );
    QString __qsDriver = __qDomElement.attribute( "driver" );
    CDeviceDriver::EDriver __eDriver = CDeviceDriver::fromCode( __qsDriver );
    if( __eDriver == CDeviceDriver::UNDEFINED )
    {
      qCritical( "ERROR[%s]: Missing or invalid device driver code (%s)", Q_FUNC_INFO, qPrintable( __qsDriver ) );
      continue;
    }
    __poDevice = CDeviceDriver::createDevice( __qsName, __eDriver );
    __poDevice->parseQVCT( __qDomElement );
    QTreeWidgetItem::addChild( __poDevice );
  }
  if( _ppoDevice ) *_ppoDevice = __poDevice;
  return __iCount;
}

void CDeviceOverlay::save( const QString& _rqsFilename, CDevice* _poDevice, bool _bApplicationDump ) const
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
  dumpQVCT( __qXmlStreamWriter, _poDevice, _bApplicationDump );

  // XML [end]
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

void CDeviceOverlay::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, CDevice* _poDevice, bool _bApplicationDump ) const
{
  // Data
  _rqXmlStreamWriter.writeStartElement( "QVCT" );
  // ... containers
  if( _poDevice )
  {
    _poDevice->dumpQVCT( _rqXmlStreamWriter );
  }
  else // no container given; assume selection dump or full dump
  {
    int __iCount = QTreeWidgetItem::childCount();
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CDevice* __poDevice = (CDevice*)QTreeWidgetItem::child( __i );
      if( !_bApplicationDump && !__poDevice->isMultiSelected() ) continue;
      __poDevice->dumpQVCT( _rqXmlStreamWriter );
    }
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // QVCT
}
