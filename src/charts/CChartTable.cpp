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

// C/C++
#include <cmath>

// QT
#include <QDialog>
#include <QDomDocument> // QtXml module
#include <QFileInfo>
#include <QGesture>
#include <QGestureEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPinchGesture>
#include <QPrinter>
#include <QPrintDialog>
#include <QRect>
#include <QTabBar>
#include <QTabWidget>
#include <QWheelEvent>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "charts/CChartControl.hpp"
#include "charts/CChartTable.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/vessel/CVesselPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChartTable::CChartTable( QWidget* _pqParent )
  : QTabWidget( _pqParent )
  , oGeoPositionReference()
  , fdScaleReference( -1.0 )
  , iDpi( 96 )
  , bIgnoreUpdate( true )
  , bMousePressed( false )
  , bMouseDrag( false )
  , fdGestureTimeLast( 0.0 )
  , fdGestureZoomReference( 0.0 )
  , bPointerPath( false )
  , bPointerPathSingle( false )
  , poOverlayPointMove( 0 )
  , poVesselPointSynchronize( 0 )
{
  constructLayout();
  QWidget::setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
  tabBar()->installEventFilter( this );

  iDpi = QVCTRuntime::useSettings()->getScreenDpi();
}

void CChartTable::constructLayout()
{
  QTabWidget::setTabPosition( QTabWidget::South );
  QTabWidget::setMovable( true );
  QTabWidget::setTabsClosable( true );
  QTabWidget::setUsesScrollButtons( true );
  QTabWidget::setElideMode( Qt::ElideLeft );
  connect( tabBar(), SIGNAL( tabCloseRequested(int) ), this, SLOT( slotCloseTab(int) ) );
  connect( tabBar(), SIGNAL( currentChanged(int) ), this, SLOT( slotChangeTab(int) ) );
}


//------------------------------------------------------------------------------
// METHODS: QWidget (override)
//------------------------------------------------------------------------------

QSize CChartTable::sizeHint() const
{
  return QVCTRuntime::useMainWindow()->size();
}


//------------------------------------------------------------------------------
// METHODS: QTabWidget (override)
//------------------------------------------------------------------------------

bool CChartTable::eventFilter( QObject* _pqObject, QEvent* _pqEvent )
{
  //qDebug( "DEBUG[%s] %d", Q_FUNC_INFO, _pqEvent->type() );
  switch( _pqEvent->type() )
  {

  case QEvent::KeyPress:
    return handlerKeyEvent( static_cast<QKeyEvent *>(_pqEvent) );

  case QEvent::Wheel:
    if( _pqObject == tabBar() ) return false;
    return handlerWheelEvent( static_cast<QWheelEvent *>(_pqEvent) );

  case QEvent::MouseButtonDblClick:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
    if( _pqObject != (QObject*)QTabWidget::currentWidget() ) return false;
    if( microtime() - fdGestureTimeLast < 0.350 ) return false; // Ongoing gestures tend to send QMouseEvent artefacts; let's get rid of those
    return handlerMouseEvent( static_cast<QMouseEvent *>(_pqEvent) );

  case QEvent::Gesture:
    if( _pqObject != (QObject*)QTabWidget::currentWidget() ) return false;
    fdGestureTimeLast = microtime();
    return handlerGestureEvent( static_cast<QGestureEvent *>(_pqEvent) );

  default:; // Ignore other events

  }
  return QTabWidget::eventFilter( _pqObject, _pqEvent );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CChartTable::slotLoad()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Project"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  clear();
  __pqMutexDataChange->unlock();
  load( __qsFilename );
  updateChart();
}

void CChartTable::slotSave()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::SAVE, tr("Save Project"), tr("QVCT Files")+" (*.qvct)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::SAVE, __qsFilename ) ) return;
  save( __qsFilename );
}

void CChartTable::slotLoadChart()
{
  QString __qsFilename = QVCTRuntime::useMainWindow()->fileDialog( QVCT::OPEN, tr("Load Chart"), tr("GeoTIFF Files")+" (*.tif *.tiff)" );
  if( __qsFilename.isEmpty() ) return;
  if( !QVCTRuntime::useMainWindow()->fileCheck( QVCT::OPEN, __qsFilename ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  loadChart( __qsFilename );
  __pqMutexDataChange->unlock();
}

void CChartTable::slotPrintChart()
{
  if( QTabWidget::currentIndex() < 0 ) return;
  bool __bPrintHighRes = QVCTRuntime::useSettings()->isPrintHighRes();
  if( __bPrintHighRes
      && QMessageBox::warning( 0, tr("WARNING"),
                               tr("High-resolution printing is enabled!\nPrinting may take several seconds, during which the application will appear frozen.\nDo you want to proceed?"),
                               QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Cancel ) != QMessageBox::Ok ) return;
  QPrinter __qPrinter( __bPrintHighRes ? QPrinter::HighResolution : QPrinter::ScreenResolution );
  // QPrinter __qPrinter;
  QPrintDialog* __pqPrintDialog = new QPrintDialog( &__qPrinter );
  __pqPrintDialog->addEnabledOption( QAbstractPrintDialog::PrintToFile );
  int __iResult = __pqPrintDialog->exec();
  delete __pqPrintDialog;
  if( __iResult != QDialog::Accepted ) return;

  // Print
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  // ... set draw area, zoom and DPI to printer's
  double __fdZoomScreen = __poChart->getZoom();
  CDataPosition oGeoPositionLower = __poChart->toGeoPosition( __poChart->getDrawArea().bottomLeft() );
  CDataPosition oGeoPositionUpper = __poChart->toGeoPosition( __poChart->getDrawArea().topRight() );
  iDpi = ( __qPrinter.paperRect( QPrinter::DevicePixel ).width() / __qPrinter.paperRect( QPrinter::Inch ).width() );
  __poChart->setDrawArea( QRectF( 0, 0, __qPrinter.pageRect( QPrinter::DevicePixel ).width(), __qPrinter.pageRect( QPrinter::DevicePixel ).height() ) );
  __poChart->setZoom( __poChart->getZoomArea( oGeoPositionLower, oGeoPositionUpper ) );
  // ... print
  __poChart->print( &__qPrinter );
  // ... set draw area, zoom and DPI back to screen's
  iDpi = QVCTRuntime::useSettings()->getScreenDpi();
  __poChart->resetDrawArea();
  __poChart->setZoom( __fdZoomScreen );
  __pqMutexDataChange->unlock();
}

void CChartTable::slotChangeTab( int _iTabIndex )
{
  CChartControl* __poChartControl = QVCTRuntime::useChartControl();
  bIgnoreUpdate = true;
  if( _iTabIndex < 0 )
  {
    __poChartControl->enableControls( false );
  }
  else
  {
    CChart* __poChart = (CChart*)QTabWidget::currentWidget();
    __poChartControl->lockPosition( __poChart->isPositionLocked() );
    __poChartControl->lockScale( __poChart->isZoomLocked() );
    __poChartControl->setScale( toScale( __poChart->getZoom(), __poChart ) );
    __poChartControl->enableControls( true );
    bIgnoreUpdate = false;
  }
}

void CChartTable::slotCloseTab()
{
  int __iTabIndex = QTabWidget::currentIndex();
  if( __iTabIndex >= 0 ) removeTab( __iTabIndex );
}

void CChartTable::slotCloseTab( int _iTabIndex )
{
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CChart* __poChart = (CChart*)widget( _iTabIndex );
  removeTab( _iTabIndex );
  delete __poChart;
  if( QTabWidget::currentIndex() < 0 )
  {
    oGeoPositionReference = CDataPosition::UNDEFINED;
    fdScaleReference = -1.0;
  }
  __pqMutexDataChange->unlock();
}

void CChartTable::slotScaleActual()
{
  setScaleActual();
}

void CChartTable::slotScaleFit()
{
  setScaleFit();
}

void CChartTable::slotPointerPath( bool _bEnable )
{
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  enablePointerPath( _bEnable );
  __pqMutexDataChange->unlock();
}

void CChartTable::slotPointerPathSingle( bool _bEnable )
{
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  enablePointerPathSingle( _bEnable );
  __pqMutexDataChange->unlock();
}

//
// HANDLERS
//

bool CChartTable::handlerKeyEvent( QKeyEvent* _pqKeyEvent )
{
  if( QTabWidget::currentIndex() < 0 ) return false;
  bool __bReturn = false;
  bool __bControlKey = _pqKeyEvent->modifiers() & Qt::ControlModifier;
  switch( _pqKeyEvent->key() )
  {
    // Position
  case Qt::Key_Home: setPositionHome(); __bReturn = true; break;
  case Qt::Key_Up: stepScrPosition( false, false, __bControlKey ); __bReturn = true; break;
  case Qt::Key_Right: stepScrPosition( true, true, __bControlKey ); __bReturn = true; break;
  case Qt::Key_Down: stepScrPosition( false, true, __bControlKey ); __bReturn = true; break;
  case Qt::Key_Left: stepScrPosition( true, false, __bControlKey ); __bReturn = true; break;
    // Scale
  case Qt::Key_Plus: stepScale( true, __bControlKey ); __bReturn = true; break;
  case Qt::Key_Minus: stepScale( false, __bControlKey ); __bReturn = true; break;
  case Qt::Key_Asterisk: setScaleActual(); __bReturn = true; break;
  case Qt::Key_Slash: setScaleFit(); __bReturn = true; break;
  default:;
  }
  return __bReturn;
}

bool CChartTable::handlerMouseEvent( QMouseEvent* _pqMouseEvent )
{
  if( QTabWidget::currentIndex() < 0 ) return false;
  QPointF __qPointFMouse = _pqMouseEvent->posF();
  int __iMouseButton = _pqMouseEvent->button();
  switch( _pqMouseEvent->type() )
  {

  case QEvent::MouseButtonDblClick:
    switch( __iMouseButton )
    {
    case Qt::LeftButton:
      {
        CChart* __poChart = (CChart*)QTabWidget::currentWidget();
        CVesselPoint* __poVesselPoint = (CVesselPoint*)QVCTRuntime::useVesselOverlay()->matchScrPosition( __poChart, __qPointFMouse );
        if( __poVesselPoint ) { __poVesselPoint->toggleMultiSelected(); break; }
        CTrackPoint* __poTrackPoint = (CTrackPoint*)QVCTRuntime::useTrackOverlay()->matchScrPosition( __poChart, __qPointFMouse );
        if( __poTrackPoint ) { __poTrackPoint->toggleMultiSelected(); break; }
        CRoutePoint* __poRoutePoint = (CRoutePoint*)QVCTRuntime::useRouteOverlay()->matchScrPosition( __poChart, __qPointFMouse );
        if( __poRoutePoint ) { __poRoutePoint->toggleMultiSelected(); break; }
        CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)QVCTRuntime::useLandmarkOverlay()->matchScrPosition( __poChart, __qPointFMouse );
        if( __poLandmarkPoint ) { __poLandmarkPoint->toggleMultiSelected(); break; }
      }
      setScaleActual();
      break;
    case Qt::RightButton:
      setScaleActual();
      break;
    default:; // we don't care about other buttons
    }
    break;

  case QEvent::MouseButtonPress:
    switch( __iMouseButton )
    {
    case Qt::LeftButton:
      bMousePressed = true;
      qPointFMouse = __qPointFMouse;
      break;
    case Qt::RightButton:
      {
        CChart* __poChart = (CChart*)QTabWidget::currentWidget();
        setGeoPosition( __poChart->toGeoPosition( __qPointFMouse ) );
      }
      break;
    default:; // we don't care about other buttons
    }
    break;

  case QEvent::MouseButtonRelease:
    if( __iMouseButton == Qt::LeftButton )
    {
      bMousePressed = false;
      if( bMouseDrag )
      {
        dragScrPosition( __qPointFMouse - qPointFMouse );
        bMouseDrag = false;
      }
      else
      {
        CChart* __poChart = (CChart*)QTabWidget::currentWidget();

        // Match mouse position with overlays' item
        CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
        CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
        CDataPosition __oGeoPosition;
        do
        {
          __poPointerPoint->resetPosition();
          __poPointerOverlay->forceRedraw();
          if( !poOverlayPointMove )
          {

            // ... vessel
            CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
            CVesselPoint* __poVesselPoint = (CVesselPoint*)__poVesselOverlay->matchScrPosition( __poChart, __qPointFMouse );
            if( __poVesselPoint )
            {
              __oGeoPosition = *__poVesselPoint;
              if( bPointerPath || bPointerPathSingle ) break;
              __poVesselOverlay->setCurrentItem( __poVesselPoint );
              __poVesselPoint->showDetail();
              break;
            }

            // ... track
            CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
            CTrackPoint* __poTrackPoint = (CTrackPoint*)__poTrackOverlay->matchScrPosition( __poChart, __qPointFMouse );
            if( __poTrackPoint )
            {
              __oGeoPosition = *__poTrackPoint;
              if( bPointerPath || bPointerPathSingle ) break;
              __poTrackOverlay->setCurrentItem( __poTrackPoint );
              __poTrackPoint->showDetail();
              break;
            }

            // ... route
            CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
            CRoutePoint* __poRoutePoint = (CRoutePoint*)__poRouteOverlay->matchScrPosition( __poChart, __qPointFMouse );
            if( __poRoutePoint )
            {
              __oGeoPosition = *__poRoutePoint;
              if( bPointerPath || bPointerPathSingle ) break;
              __poRouteOverlay->setCurrentItem( __poRoutePoint );
              __poRoutePoint->showDetail();
              break;
            }

            // ... landmark
            CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
            CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)__poLandmarkOverlay->matchScrPosition( __poChart, __qPointFMouse );
            if( __poLandmarkPoint )
            {
              __oGeoPosition = *__poLandmarkPoint;
              if( bPointerPath || bPointerPathSingle ) break;
              __poLandmarkOverlay->setCurrentItem( __poLandmarkPoint );
              __poLandmarkPoint->showDetail();
              break;
            }

          }

          // ... pointer
          __oGeoPosition = __poChart->toGeoPosition( __qPointFMouse );
          __oGeoPosition.resetElevation();
            if( poOverlayPointMove ) break;
          __poPointerPoint->setPosition( __oGeoPosition );
          __poPointerOverlay->showDetail( __poPointerOverlay->usePointerPoint() );

        }
        while( false );

        // Extend pointer path
        do
        {
          if( bPointerPath || bPointerPathSingle )
          {
            if( bPointerPathSingle && __poPointerOverlay->getPathSegments() )
            {
              if( !bPointerPath )
              {
                QVCTRuntime::useChartControl()->enableMeasureSingle( false );
                break;
              }
              __poPointerOverlay->clearPath();
            }
            __poPointerOverlay->setPath( __oGeoPosition );
            __poPointerPoint->showDetail();
            __poPointerOverlay->forceRedraw();
          }
          __poChart->update();
        }
        while( false );

        // Overlay point move
        if( poOverlayPointMove )
        {
          poOverlayPointMove->setPosition( CDataPosition( __oGeoPosition.getLongitude(), __oGeoPosition.getLatitude(), poOverlayPointMove->getElevation() ) );
          poOverlayPointMove->showDetail();
          poOverlayPointMove->useOverlay()->forceRedraw();
          __poChart->update();
        }

      }
    }
    break;

  case QEvent::MouseMove:
    if( bMouseDrag )
    {
      dragScrPosition( qPointFMouse - __qPointFMouse );
      qPointFMouse = __qPointFMouse;
    }
    else if( bMousePressed )
    {
      QPointF __qPointFDelta = __qPointFMouse - qPointFMouse;
      if( __qPointFDelta.x()*__qPointFDelta.x() + __qPointFDelta.y()*__qPointFDelta.y() > 100 ) bMouseDrag = true;
    }
    break;

  default: return false; // Other events don't get here but better safe than sorry

  }
  return true;
}

bool CChartTable::handlerWheelEvent( QWheelEvent* _pqWheelEvent )
{
  if( QTabWidget::currentIndex() < 0 ) return false;
  int __iMouseDelta = _pqWheelEvent->delta() / 120;
  if( !__iMouseDelta ) return true;
  bool __bIncrease = true;
  if( __iMouseDelta < 0 ) { __bIncrease = false; __iMouseDelta = -__iMouseDelta; }
  for( int __i=0; __i<__iMouseDelta; __i++ ) stepScale( __bIncrease, true );
  return true;
}

bool CChartTable::handlerGestureEvent( QGestureEvent* _pqGestureEvent )
{
  if( QTabWidget::currentIndex() < 0 ) return false;
  bool __bReturn = false;
  QGesture* __pqGesture = _pqGestureEvent->gesture( Qt::PinchGesture );
  if( __pqGesture )
  {
    // NOTE: To provide a good "user experience", we must use the underlying
    //       chart's linear zoom factor rather than the UI's logarithmic scale
    //       factor.
    QPinchGesture* __pqPinchGesture = static_cast<QPinchGesture *>(__pqGesture);
    switch( __pqPinchGesture->state() )
    {

    case Qt::GestureStarted:
      __pqPinchGesture->setGestureCancelPolicy( QGesture::CancelAllInContext );
      _pqGestureEvent->accept( __pqPinchGesture );
      fdGestureZoomReference = ((CChart*)QTabWidget::currentWidget())->getZoom();
      break;

    case Qt::GestureUpdated:
    case Qt::GestureFinished:
      if( __pqPinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged )
        setZoom( fdGestureZoomReference * __pqPinchGesture->totalScaleFactor() );
      break;

    default:;

    }
    __bReturn = true;
  }
  return __bReturn;
}


//
// SETTERS
//

void CChartTable::setGeoPosition( const CDataPosition& _roGeoPosition, bool _bSkipCurrent )
{
  if( bIgnoreUpdate || count() < 1 ) return;
  int __iWidgetCurrentIndex = QTabWidget::currentIndex();
  bool __bLocked = ((CChart*)QTabWidget::currentWidget())->isPositionLocked();
  if( __bLocked ) oGeoPositionReference = _roGeoPosition;
  for( int __iWidgetIndex = count() - 1; __iWidgetIndex >= 0; __iWidgetIndex-- )
  {
    CChart* __poChart = (CChart*)widget( __iWidgetIndex );
    if( __iWidgetIndex != __iWidgetCurrentIndex && __bLocked && __poChart->isPositionLocked() )
    {
      __poChart->setGeoPosition( _roGeoPosition );
    }
    if( __iWidgetIndex == __iWidgetCurrentIndex && !_bSkipCurrent )
    {
      __poChart->setGeoPosition( _roGeoPosition );
      __poChart->update();
    }
  }
}

void CChartTable::stepScrPosition( bool _bHorizontal, bool _bIncrease, bool _bBigStep )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  int __iXOffset = _bHorizontal ? ( _bIncrease ? 1 : -1 ) * ( _bBigStep ? __poChart->getDrawArea().width()/5 : __poChart->getDrawArea().width()/50 ) : 0;
  int __iYOffset = !_bHorizontal ? ( _bIncrease ? 1 : -1 ) * ( _bBigStep ? __poChart->getDrawArea().height()/5 : __poChart->getDrawArea().height()/50 ) : 0;
  dragScrPosition( QPoint( __iXOffset, __iYOffset ) );
}

void CChartTable::dragScrPosition( const QPointF& _rqPointFScrPositionOffset )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 || _rqPointFScrPositionOffset.isNull() ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  /*
   * NOTE: We do not move the chart by calling this->setGeoPosition directly because it leads
   *       to extra offset artefacts due to the pixel<->geographical position back and forth conversion.
   *       Instead, we move the chart directly by modifying the pixel positon and
   *       update the reference geographical position accordingly.
   */
  __poChart->move( _rqPointFScrPositionOffset );
  __poChart->update();
  if( __poChart->isPositionLocked() ) setGeoPosition( __poChart->getGeoPosition(), true );
}

void CChartTable::setPositionHome()
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  setGeoPosition( __poChart->getGeoPositionCenter() );
}

void CChartTable::lockPosition( bool _bLock )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  if( _bLock ) setGeoPosition( oGeoPositionReference );
  __poChart->lockPosition( _bLock );
  //if( _bLock ) setGeoPosition( __poChart->getGeoPosition(), true );
}

void CChartTable::setScale( double _fdScale, bool _bSkipCurrent, bool _bUpdateControl )
{
  if( bIgnoreUpdate || count() < 1 ) return;
  //qDebug( "DEBUG[%s] %f", Q_FUNC_INFO, _fdScale );
  int __iWidgetCurrentIndex = QTabWidget::currentIndex();
  bool __bLocked = ((CChart*)QTabWidget::currentWidget())->isZoomLocked();
  if( __bLocked ) fdScaleReference = _fdScale;
  for( int __iWidgetIndex = count() - 1; __iWidgetIndex >= 0; __iWidgetIndex-- )
  {
    CChart* __poChart = (CChart*)widget( __iWidgetIndex );
    if( __iWidgetIndex != __iWidgetCurrentIndex && __bLocked && __poChart->isZoomLocked() )
    {
      __poChart->setZoom( toZoom( _fdScale, __poChart ) );
    }
    if( __iWidgetIndex == __iWidgetCurrentIndex && !_bSkipCurrent )
    {
      __poChart->setZoom( toZoom( _fdScale, __poChart ) );
      if( _bUpdateControl ) QVCTRuntime::useChartControl()->setScale( _fdScale );
      __poChart->update();
    }
  }
}

void CChartTable::stepScale( bool _bIncrease, bool _bBigStep )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  QVCTRuntime::useChartControl()->stepScale( _bIncrease, _bBigStep );
}

void CChartTable::setScaleActual()
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  setScale( toScale( __poChart->getZoomActual(), __poChart ) );
}

void CChartTable::setScaleFit()
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  setGeoPosition( __poChart->getGeoPositionCenter() );
  setScale( toScale( __poChart->getZoomFit(), __poChart ) );
}

void CChartTable::setScaleArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2, double _fdScaleCorrection )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  setGeoPosition( CDataPosition( ( _roGeoPosition1.getLongitude() + _roGeoPosition2.getLongitude() ) / 2.0, ( _roGeoPosition1.getLatitude() + _roGeoPosition2.getLatitude() ) / 2.0 ) );
  setScale( toScale( __poChart->getZoomArea( _roGeoPosition1, _roGeoPosition2 ) * _fdScaleCorrection, __poChart ) );
}

void CChartTable::lockScale( bool _bLock )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  if( _bLock ) setScale( fdScaleReference );
  __poChart->lockZoom( _bLock );
  //if( _bLock ) setScale( toScale( __poChart->getZoom(), __poChart ), true );
}

void CChartTable::setZoom( double _fdZoom, bool _bSkipCurrent, bool _bUpdateControl )
{
  if( count() < 1 ) return;
  //qDebug( "DEBUG[%s] %f", Q_FUNC_INFO, _fdZoom );
  double __fdScale = toScale( _fdZoom );
  int __iWidgetCurrentIndex = QTabWidget::currentIndex();
  bool __bLocked = ((CChart*)QTabWidget::currentWidget())->isZoomLocked();
  if( __bLocked ) fdScaleReference = __fdScale;
  for( int __iWidgetIndex = count() - 1; __iWidgetIndex >= 0; __iWidgetIndex-- )
  {
    CChart* __poChart = (CChart*)widget( __iWidgetIndex );
    if( __iWidgetIndex != __iWidgetCurrentIndex && __bLocked && __poChart->isZoomLocked() )
    {
      __poChart->setZoom( _fdZoom );
    }
    if( __iWidgetIndex == __iWidgetCurrentIndex && !_bSkipCurrent )
    {
      __poChart->setZoom( _fdZoom );
      if( _bUpdateControl ) QVCTRuntime::useChartControl()->setScale( __fdScale );
      __poChart->update();
    }
  }
}

void CChartTable::enablePointerPath( bool _bEnable )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  bIgnoreUpdate = true;
  bPointerPath = _bEnable;
  QVCTRuntime::useChartControl()->enableMeasure( bPointerPath );
  if( !bPointerPath )
  {
    if( bPointerPathSingle )
    {
      bPointerPathSingle = false;
      QVCTRuntime::useChartControl()->enableMeasureSingle( false );
    }
    CChart* __poChart = (CChart*)QTabWidget::currentWidget();
    QVCTRuntime::usePointerOverlay()->clearPath();
    QVCTRuntime::usePointerPointDetailView()->refreshContent();
    QVCTRuntime::usePointerOverlay()->forceRedraw();
    __poChart->update();
  }
  bIgnoreUpdate = false;
}

void CChartTable::enablePointerPathSingle( bool _bEnable )
{
  if( bIgnoreUpdate || QTabWidget::currentIndex() < 0 ) return;
  bIgnoreUpdate = true;
  bPointerPathSingle = _bEnable;
  QVCTRuntime::useChartControl()->enableMeasureSingle( bPointerPathSingle );
  if( !bPointerPathSingle )
  {
    CChart* __poChart = (CChart*)QTabWidget::currentWidget();
    QVCTRuntime::usePointerOverlay()->clearPath();
    QVCTRuntime::usePointerPointDetailView()->refreshContent();
    QVCTRuntime::usePointerOverlay()->forceRedraw();
    __poChart->update();
  }
  bIgnoreUpdate = false;
}

void CChartTable::setOverlayPointMove( COverlayPoint* _poOverlayPoint )
{
  poOverlayPointMove = _poOverlayPoint;
  QVCTRuntime::useChartControl()->allowMeasure( poOverlayPointMove ? false : true );
}

//
// OTHER
//

void CChartTable::synchronizeVesselPoint()
{
  if( !poVesselPointSynchronize ) return;
  oGeoPositionReference.setPosition( *poVesselPointSynchronize );
  for( int __i = count() - 1; __i >= 0; __i-- )
  {
    CChart* __poChart = (CChart*)widget( __i );
    if( __poChart->isPositionLocked() ) __poChart->setGeoPosition( oGeoPositionReference );
  }
}

void CChartTable::clear()
{
  QTabWidget::clear();
  QVCTRuntime::usePointerOverlay()->clearPath();
  QVCTRuntime::useLandmarkOverlay()->clear();
  QVCTRuntime::useRouteOverlay()->clear();
  QVCTRuntime::useTrackOverlay()->clear();
  QVCTRuntime::useVesselOverlay()->clear();
}

void CChartTable::load( const QString& _rqsFilename )
{
  QFileInfo __qFileInfo( _rqsFilename );
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
    if( __qDomElement.isNull() || ( __qDocType != "QVCT" ) )
    {
      __qsError = QString( "Invalid XML document type (%1); expected: 'QVCT'" ).arg( __qFile.fileName() );
      break;
    }
    // ... chart table
    if( parseQVCT( __qDomElement ) )
    {
      QTabWidget::setCurrentIndex( 0 );
      CChartControl* __poChartControl = QVCTRuntime::useChartControl();
      CChart* __poChart = (CChart*)QTabWidget::currentWidget();
      bIgnoreUpdate = true;
      __poChartControl->lockPosition( __poChart->isPositionLocked() );
      __poChartControl->lockScale( __poChart->isZoomLocked() );
      __poChartControl->setScale( toScale( __poChart->getZoom(), __poChart ) );
      __poChartControl->enableControls( true );
      bIgnoreUpdate = false;
    }
    // ... overlays
    QVCTRuntime::useLandmarkOverlay()->parseQVCT( __qDomElement );
    QVCTRuntime::useLandmarkOverlay()->QTreeWidgetItem::setExpanded( true );
    QVCTRuntime::useRouteOverlay()->parseQVCT( __qDomElement );
    QVCTRuntime::useRouteOverlay()->QTreeWidgetItem::setExpanded( true );
    QVCTRuntime::useTrackOverlay()->parseQVCT( __qDomElement );
    QVCTRuntime::useTrackOverlay()->QTreeWidgetItem::setExpanded( true );
    QVCTRuntime::useVesselOverlay()->parseQVCT( __qDomElement );
    QVCTRuntime::useVesselOverlay()->QTreeWidgetItem::setExpanded( true );
  }
  while( false ); // error-catching context [end]
  if( !__qsError.isEmpty() )
  {
    qCritical( "ERROR[%s]: %s", Q_FUNC_INFO, qPrintable( __qsError ) );
    QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return;
  }
}

void CChartTable::save( const QString& _rqsFilename ) const
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
  __qXmlStreamWriter.writeStartElement( "QVCT" );
  // ... chart table
  dumpQVCT( __qXmlStreamWriter );
  // ... overlays
  QVCTRuntime::useLandmarkOverlay()->dumpQVCT( __qXmlStreamWriter, 0, true );
  QVCTRuntime::useRouteOverlay()->dumpQVCT( __qXmlStreamWriter, 0, true );
  QVCTRuntime::useTrackOverlay()->dumpQVCT( __qXmlStreamWriter, 0, true );
  QVCTRuntime::useVesselOverlay()->dumpQVCT( __qXmlStreamWriter, 0, true );
  // ... [end]
  __qXmlStreamWriter.writeEndElement(); // QVCT

  // XML [end]
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

int CChartTable::parseQVCT( const QDomElement& _rqDomElement )
{
  // Chart table
  QDomElement __qDomElementChartTable = _rqDomElement.firstChildElement( "ChartTable" );
  if( __qDomElementChartTable.isNull() ) return 0;
  if( __qDomElementChartTable.hasAttribute( "longitude" ) && __qDomElementChartTable.hasAttribute( "longitude" ) )
  {
    oGeoPositionReference.setPosition( __qDomElementChartTable.attribute( "longitude" ).toDouble(),
                                       __qDomElementChartTable.attribute( "latitude" ).toDouble() );
  }
  if( __qDomElementChartTable.hasAttribute( "scale" ) )
    fdScaleReference = __qDomElementChartTable.attribute( "scale" ).toDouble();
  // ... charts
  int __iCount = 0;
  for( QDomElement __qDomElementChart = __qDomElementChartTable.firstChildElement( "Chart" );
       !__qDomElementChart.isNull();
       __qDomElementChart = __qDomElementChart.nextSiblingElement( "Chart" ) )
  {
    CChart* __poChart = loadChart( __qDomElementChart.attribute( "file" ) );
    if( !__poChart ) continue;
    __poChart->parseQVCT( __qDomElementChart );
    __iCount++;
  }
  return __iCount;
}

void CChartTable::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // Chart table
  _rqXmlStreamWriter.writeStartElement( "ChartTable" );
  if( oGeoPositionReference != CDataPosition::UNDEFINED )
  {
    _rqXmlStreamWriter.writeAttribute( "longitude", QString::number( oGeoPositionReference.getLongitude() ) );
    _rqXmlStreamWriter.writeAttribute( "latitude", QString::number( oGeoPositionReference.getLatitude() ) );
  }
  if( fdScaleReference >= 0.0 )
    _rqXmlStreamWriter.writeAttribute( "scale", QString::number( fdScaleReference ) );
  // ... charts
  int __iCount = QTabWidget::count();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CChart*)QTabWidget::widget( __i ))->dumpQVCT( _rqXmlStreamWriter );
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // ChartTable
}

CChart* CChartTable::loadChart( const QString& _rqsFilename )
{
  // Create new chart widget
  CChart* __poChart = new CChart( this, _rqsFilename );
  if( __poChart->getStatus() != QVCT::OK )
  {
    delete __poChart;
    QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, _rqsFilename );
    return 0;
  }

  // Set reference position
  if( oGeoPositionReference == CDataPosition::UNDEFINED ) oGeoPositionReference = __poChart->getGeoPosition();
  else __poChart->setGeoPosition( oGeoPositionReference );

  // Set reference scale factor
  if( fdScaleReference < 0 ) fdScaleReference = toScale( __poChart->getZoom(), __poChart );
  else __poChart->setZoom( toZoom( fdScaleReference,  __poChart ) );

  // Have the chart table manage chart events
  __poChart->installEventFilter( this );
  // ... including pinch gestures
  if( QVCTRuntime::useSettings()->isScreenGestures() ) __poChart->grabGesture( Qt::PinchGesture );

  // Add chart to chart table
  QFileInfo __qFileInfo( _rqsFilename );
  addTab( __poChart, __qFileInfo.baseName() );
  setCurrentWidget( __poChart );
  return __poChart;
}

void CChartTable::updateChart()
{
  if( QTabWidget::currentIndex() < 0 ) return;
  CChart* __poChart = (CChart*)QTabWidget::currentWidget();
  __poChart->update();
}

double CChartTable::toZoom( double _fdScale, const CChart* _poChart )
{
  //qDebug( "DEBUG[%s] Scale: %f", Q_FUNC_INFO, _fdScale );
  if( !_poChart && QTabWidget::currentIndex() < 0 ) return 1;
  const CChart* __poChart = _poChart ? _poChart : (CChart*)QTabWidget::currentWidget();
  /*
   * The zoom factor is computed based on the ratio between:
   *  - the "actual" chart scale,
   *    computed as the ratio between the screen resolution
   *    and the chart resolution at the current position
   *  - the absolute reference scale (10'000 to 10'000'000),
   *    computed from the (normalized) reference scale factor
   */
  //                screen res. [px/m]   chart res. [m/px]            absolute reference scale
  double __fdZoom = 39.37*iDpi *         __poChart->getResolution() / pow( 10, 4.0+4.0*_fdScale );
  //qDebug( "DEBUG[%s] Zoom: %f", Q_FUNC_INFO, __fdZoom );
  return __fdZoom;
}

double CChartTable::toScale( double _fdZoom, const CChart* _poChart )
{
  //qDebug( "DEBUG[%s] Zoom: %f", Q_FUNC_INFO, _fdZoom );
  if( !_poChart && QTabWidget::currentIndex() < 0 ) return 0;
  const CChart* __poChart = _poChart ? _poChart : (CChart*)QTabWidget::currentWidget();
  double __fdScale = ( log10( 39.37*iDpi * __poChart->getResolution() / _fdZoom ) - 4.0 ) / 4.0;
  //qDebug( "DEBUG[%s] Scale: %f", Q_FUNC_INFO, __fdScale );
  return __fdScale;
}
