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
#include <QAction>
#include <QApplication>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMessageBox>
#include <QPixmap>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include <QTreeWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "CMainWindow.hpp"
#include "settings/CSettingsEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CMainWindow::CMainWindow()
  : fdTimeLastRedraw( 0.0 )
{
  setWindowTitle( tr("Qt Virtual Chart Table") );
  constructLayout();
  constructMenus();

  QTimer* __qTimerRefresh = QVCTRuntime::useTimerRefresh();
  QObject::connect( __qTimerRefresh, SIGNAL( timeout() ), this, SLOT( slotTimerRefresh() ) );
  __qTimerRefresh->start( QVCTRuntime::useSettings()->getRateRefresh() );
}

void CMainWindow::constructLayout()
{
  // Create layout
  QWidget* __pqWidget = new QWidget( this );
  QHBoxLayout* __pqHBoxLayout = new QHBoxLayout( __pqWidget );

  // Add the "virtual" chart table
  CChartTable* __poChartTable = new CChartTable( __pqWidget );
  QVCTRuntime::registerChartTable( __poChartTable );
  __pqHBoxLayout->addWidget( __poChartTable, 1 );

  // Add the chart control
  CChartControl* __poChartControl = new CChartControl( __pqWidget );
  QVCTRuntime::registerChartControl( __poChartControl );
  __poChartControl->installEventFilter( __poChartTable );
  __pqHBoxLayout->addWidget( __poChartControl );

  // Add the overlay docks
  // ... list
  COverlayListView* __poOverlayListView = new COverlayListView( this );
  QVCTRuntime::registerOverlayListView( __poOverlayListView );
  QMainWindow::addDockWidget( Qt::LeftDockWidgetArea, __poOverlayListView );
  // ... detail
  COverlayDetailView* __poOverlayDetailView = new COverlayDetailView( this );
  QVCTRuntime::registerOverlayDetailView( __poOverlayDetailView );
  QMainWindow::addDockWidget( Qt::LeftDockWidgetArea, __poOverlayDetailView );
  __poOverlayDetailView->resize( __poOverlayDetailView->width(), 100 );

  // Add other docks
  // ... time
  CTimeView* __poTimeView = new CTimeView( this );
  QVCTRuntime::registerTimeView( __poTimeView );
  QMainWindow::addDockWidget( Qt::TopDockWidgetArea, __poTimeView );
  // ... vessel target
  CVesselTarget* __poVesselTarget = new CVesselTarget( this );
  QVCTRuntime::registerVesselTarget( __poVesselTarget );
  QMainWindow::addDockWidget( Qt::TopDockWidgetArea, __poVesselTarget );
  // ... vessel position
  CVesselPosition* __poVesselPosition = new CVesselPosition( this );
  QVCTRuntime::registerVesselPosition( __poVesselPosition );
  QMainWindow::addDockWidget( Qt::BottomDockWidgetArea, __poVesselPosition );
  // ... vessel course
  CVesselCourse* __poVesselCourse = new CVesselCourse( this );
  QVCTRuntime::registerVesselCourse( __poVesselCourse );
  QMainWindow::addDockWidget( Qt::BottomDockWidgetArea, __poVesselCourse );
  // ... vessel cockpit
  CVesselCockpitGeneralAviation* __poVesselCockpitGeneralAviation = new CVesselCockpitGeneralAviation( this );
  QVCTRuntime::registerVesselCockpit( __poVesselCockpitGeneralAviation );
  // QMainWindow::addDockWidget( Qt::NoDockWidgetArea, __poVesselCockpitGeneralAviation );

  // Set the layout
  __pqWidget->setLayout( __pqHBoxLayout );
  QMainWindow::setCentralWidget( __pqWidget );
}

void CMainWindow::constructMenus()
{
  // Construct menus
  QMenu* __pqMenuFile = new QMenu( tr("&File"), this );
  QAction* __pqActionLoad = new QAction( tr("L&oad Project..." ), this );
  __pqActionLoad->setShortcuts( QKeySequence::Open );
  QMainWindow::connect( __pqActionLoad, SIGNAL( triggered() ), QVCTRuntime::useChartTable(), SLOT( slotLoad() ) );
  __pqMenuFile->addAction( __pqActionLoad );
  QAction* __pqActionSave = new QAction( tr("&Save Project..." ), this );
  __pqActionSave->setShortcuts( QKeySequence::Save );
  QMainWindow::connect( __pqActionSave, SIGNAL( triggered() ), QVCTRuntime::useChartTable(), SLOT( slotSave() ) );
  __pqMenuFile->addAction( __pqActionSave );
  __pqMenuFile->addSeparator();
  QAction* __pqActionLoadChart = new QAction( tr("L&oad Chart..." ), this );
  QMainWindow::connect( __pqActionLoadChart, SIGNAL( triggered() ), QVCTRuntime::useChartTable(), SLOT( slotLoadChart() ) );
  __pqMenuFile->addAction( __pqActionLoadChart );
  QAction* __pqActionAddElevation = new QAction( tr("Add &Elevation..." ), this );
  QMainWindow::connect( __pqActionAddElevation, SIGNAL( triggered() ), QVCTRuntime::useChartControl(), SLOT( slotElevationAdd() ) );
  __pqMenuFile->addAction( __pqActionAddElevation );
  QAction* __pqActionLoadLandmarks = new QAction( tr("Load &Landmarks..." ), this );
  QMainWindow::connect( __pqActionLoadLandmarks, SIGNAL( triggered() ), QVCTRuntime::useLandmarkOverlayListView(), SLOT( slotLoad() ) );
  __pqMenuFile->addAction( __pqActionLoadLandmarks );
  QAction* __pqActionLoadRoute = new QAction( tr("Load &Route..." ), this );
  QMainWindow::connect( __pqActionLoadRoute, SIGNAL( triggered() ), QVCTRuntime::useRouteOverlayListView(), SLOT( slotLoad() ) );
  __pqMenuFile->addAction( __pqActionLoadRoute );
  QAction* __pqActionLoadTrack = new QAction( tr("Load &Track..." ), this );
  QMainWindow::connect( __pqActionLoadTrack, SIGNAL( triggered() ), QVCTRuntime::useTrackOverlayListView(), SLOT( slotLoad() ) );
  __pqMenuFile->addAction( __pqActionLoadTrack );
  QAction* __pqActionLoadVessel = new QAction( tr("Load &Vessel..." ), this );
  QMainWindow::connect( __pqActionLoadVessel, SIGNAL( triggered() ), QVCTRuntime::useVesselOverlayListView(), SLOT( slotLoad() ) );
  __pqMenuFile->addAction( __pqActionLoadVessel );
  __pqMenuFile->addSeparator();
  QAction* __pqActionSettings = new QAction( tr("S&ettings..."), this);
  __pqActionSettings->setShortcuts( QKeySequence::Preferences );
  QMainWindow::connect( __pqActionSettings, SIGNAL( triggered() ), this, SLOT( slotShowSettings() ) );
  __pqMenuFile->addAction( __pqActionSettings );
  __pqMenuFile->addSeparator();
  QAction* __pqActionPrint = new QAction( tr("&Print..."), this);
  __pqActionPrint->setShortcuts( QKeySequence::Print );
  QMainWindow::connect( __pqActionPrint, SIGNAL( triggered() ), QVCTRuntime::useChartTable(), SLOT( slotPrintChart() ) );
  __pqMenuFile->addAction( __pqActionPrint );
  __pqMenuFile->addSeparator();
  QAction* __pqActionExit = new QAction( tr("E&xit"), this);
  __pqActionExit->setShortcuts( QKeySequence::Quit );
  QMainWindow::connect( __pqActionExit, SIGNAL( triggered() ), this, SLOT( slotExit() ) );
  __pqMenuFile->addAction( __pqActionExit );
  menuBar()->addMenu( __pqMenuFile );

  QMenu* __pqMenuWindow = new QMenu( tr("&Window"), this );
  QAction* __pqActionShowOverlayListView = new QAction( tr("Show Overlay &List" ), this );
  QMainWindow::connect( __pqActionShowOverlayListView, SIGNAL( triggered() ), this, SLOT( slotShowOverlayListView() ) );
  __pqMenuWindow->addAction( __pqActionShowOverlayListView );
  QAction* __pqActionShowOverlayDetailView = new QAction( tr("Show Overlay &Detail" ), this );
  QMainWindow::connect( __pqActionShowOverlayDetailView, SIGNAL( triggered() ), this, SLOT( slotShowOverlayDetailView() ) );
  __pqMenuWindow->addAction( __pqActionShowOverlayDetailView );
  QAction* __pqActionShowTimeView = new QAction( tr("Show System &Time" ), this );
  QMainWindow::connect( __pqActionShowTimeView, SIGNAL( triggered() ), this, SLOT( slotShowTimeView() ) );
  __pqMenuWindow->addAction( __pqActionShowTimeView );
  QAction* __pqActionShowVesselTarget = new QAction( tr("Show Vessel T&arget" ), this );
  QMainWindow::connect( __pqActionShowVesselTarget, SIGNAL( triggered() ), this, SLOT( slotShowVesselTarget() ) );
  __pqMenuWindow->addAction( __pqActionShowVesselTarget );
  QAction* __pqActionShowVesselPosition = new QAction( tr("Show Vessel &Position" ), this );
  QMainWindow::connect( __pqActionShowVesselPosition, SIGNAL( triggered() ), this, SLOT( slotShowVesselPosition() ) );
  __pqMenuWindow->addAction( __pqActionShowVesselPosition );
  QAction* __pqActionShowVesselCourse = new QAction( tr("Show Vessel &Course" ), this );
  QMainWindow::connect( __pqActionShowVesselCourse, SIGNAL( triggered() ), this, SLOT( slotShowVesselCourse() ) );
  __pqMenuWindow->addAction( __pqActionShowVesselCourse );
  QAction* __pqActionShowVesselCockpit = new QAction( tr("Show Vessel Coc&kpit" ), this );
  QMainWindow::connect( __pqActionShowVesselCockpit, SIGNAL( triggered() ), this, SLOT( slotShowVesselCockpit() ) );
  __pqMenuWindow->addAction( __pqActionShowVesselCockpit );
  __pqMenuWindow->addSeparator();
  QAction* __pqActionToggleFullscreen = new QAction( tr("Toggle &Fullscreen" ), this );
  QMainWindow::connect( __pqActionToggleFullscreen, SIGNAL( triggered() ), this, SLOT( slotToggleFullscreen() ) );
  __pqMenuWindow->addAction( __pqActionToggleFullscreen );
  menuBar()->addMenu( __pqMenuWindow );

  QMenu* __pqMenuHelp = new QMenu( tr("&Help"), this );
  QAction* __pqActionAbout = new QAction( tr("&About"), this );
  QMainWindow::connect( __pqActionAbout, SIGNAL( triggered() ), this, SLOT( slotAbout() ) );
  __pqMenuHelp->addAction( __pqActionAbout );
  QAction* __pqActionAboutQt = new QAction( tr("About &Qt"), this );
  QMainWindow::connect( __pqActionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
  __pqMenuHelp->addAction( __pqActionAboutQt );
  QMainWindow::menuBar()->addMenu( __pqMenuHelp );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CMainWindow::slotExit()
{
  if( QVCTRuntime::useChartTable()->isProjectModified() && !deleteConfirm( tr("Unsaved project data") ) ) return;
  close();
}

void CMainWindow::slotAbout()
{
  QMessageBox::about( this, tr("Qt Virtual Chart Table (QVCT)"),
                      QString( "<H3 ALIGN=\"center\">Qt&nbsp;Virtual&nbsp;Chart&nbsp;Table&nbsp;(QVCT)<BR/>" ) +
                      QString( VER_STRING ) +
                      QString( "</H3>\n" ) +
                      tr( "<P><B>Authors:</B></P>\n"
                          "<P>C&eacute;dric Dufour &lt;<A HREF=\"http://cedric.dufour.name\">http://cedric.dufour.name</A>&gt;</P>\n"
                          "<P><B>Website:</B></P>\n"
                          "<P><A HREF=\"http://cedric.dufour.name/software/qvct\">http://cedric.dufour.name/software/qvct</A></P>\n"
                          "<P><B>Copyright:</B></P>\n"
                          "<P>The Qt Virtual Chart Table (QVCT) is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, Version 3.</P>\n"
                          "<P>The Qt Virtual Chart Table (QVCT) is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</P>\n"
                          "<P>See the <A HREF=\"http://www.gnu.org/copyleft/gpl.html\">GNU General Public License</A> for more details.</P>\n"
                          "<P><B>Donation:</B></P>\n"
                          "<P><A HREF=\"http://cedric.dufour.name/software/qvct/donate.html\">http://cedric.dufour.name/software/qvct/donate.html</A></P>\n" ) );
}

void CMainWindow::slotShowSettings()
{
  CSettingsEditView* __poSettingsEditView = new CSettingsEditView( this );
  __poSettingsEditView->exec();
  delete __poSettingsEditView;

  // Update refresh timer rate
  QTimer* __qTimerRefresh = QVCTRuntime::useTimerRefresh();
  __qTimerRefresh->stop();
  __qTimerRefresh->start( QVCTRuntime::useSettings()->getRateRefresh() );
}

void CMainWindow::slotShowOverlayListView()
{
  QVCTRuntime::useOverlayListView()->show();
}

void CMainWindow::slotShowOverlayDetailView()
{
  QVCTRuntime::useOverlayDetailView()->show();
}

void CMainWindow::slotShowTimeView()
{
  QVCTRuntime::useTimeView()->show();
}

void CMainWindow::slotShowVesselTarget()
{
  QVCTRuntime::useVesselTarget()->show();
}

void CMainWindow::slotShowVesselPosition()
{
  QVCTRuntime::useVesselPosition()->show();
}

void CMainWindow::slotShowVesselCourse()
{
  QVCTRuntime::useVesselCourse()->show();
}

void CMainWindow::slotShowVesselCockpit()
{
  QVCTRuntime::useVesselCockpit()->show();
}

void CMainWindow::slotToggleFullscreen()
{
  QWidget::setWindowState( QWidget::windowState() ^ Qt::WindowFullScreen );
}

void CMainWindow::slotTimerRefresh()
{
  //qDebug( "DEBUG[%s]: Begin", Q_FUNC_INFO );
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  QVCTRuntime::useTimeView()->refreshContent();
  QVCTRuntime::useTrackContainerDetailView()->refreshContent();
  QVCTRuntime::useTrackSubContainerDetailView()->refreshContent();
  QVCTRuntime::useVesselPointDetailView()->refreshContent();
  QVCTRuntime::useVesselPosition()->refreshContent();
  QVCTRuntime::useVesselCourse()->refreshContent();
  QVCTRuntime::useVesselTarget()->refreshContent();
  QVCTRuntime::useVesselCockpit()->refreshContent();
  double __fdSystemTime = microtime();
  if( __fdSystemTime - fdTimeLastRedraw >= (double)QVCTRuntime::useSettings()->getRateRedraw() )
  {
    fdTimeLastRedraw = __fdSystemTime;
    QVCTRuntime::useVesselOverlay()->forceRedraw();
  }
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
  //qDebug( "DEBUG[%s]: End", Q_FUNC_INFO );
}

void CMainWindow::slotWarning( const QString& _rqsMessage )
{
  warningMessage( _rqsMessage );
}

void CMainWindow::slotError( const QString& _rqsMessage )
{
  errorMessage( _rqsMessage );
}

//
// OTHER
//

void CMainWindow::warningMessage( const QString& _rqsMessage )
{
  QMessageBox::warning( 0, "[QVCT] "+tr("WARNING"), _rqsMessage );
}

void CMainWindow::errorMessage( const QString& _rqsMessage )
{
  QMessageBox::critical( 0, "[QVCT] "+tr("ERROR"), _rqsMessage );
}

bool CMainWindow::deleteConfirm( const QString& _rqsName )
{
  return( QMessageBox::question( 0, "[QVCT] "+tr("Please Confirm")+"...", tr("The following data are about to be deleted")+QString(":\n  %1\n").arg( _rqsName )+tr("Do you want to proceed?"), QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Cancel ) == QMessageBox::Ok );
}

QString CMainWindow::fileDialog( QVCT::EFileOperation _eFileOperation, const QString& _rqsTitle, const QString& _rqsFilter )
{
  QString __qsFilename;
  switch( _eFileOperation )
  {
  case QVCT::OPEN:
    __qsFilename = QFileDialog::getOpenFileName( 0, "[QVCT] "+_rqsTitle, QVCTRuntime::useSettings()->getPathWorkingDirectory(), _rqsFilter );
    break;
  case QVCT::SAVE:
    __qsFilename = QFileDialog::getSaveFileName( 0, "[QVCT] "+_rqsTitle, QVCTRuntime::useSettings()->getPathWorkingDirectory(), _rqsFilter );
    break;
  }
  if( !__qsFilename.isEmpty() )
  {
    QFileInfo __qFileInfo( __qsFilename );
    QVCTRuntime::useSettings()->setPathWorkingDirectory( __qFileInfo.absolutePath() );
  }
  return __qsFilename;
}

bool CMainWindow::fileCheck( QVCT::EFileOperation _eFileOperation, const QString& _rqsFilename, const QStringList* _pqsListExtensions )
{
  QFileInfo __qFileInfo( _rqsFilename );

  // Check file
  switch( _eFileOperation )
  {

  case QVCT::OPEN:
    if( !__qFileInfo.exists() )
    {
      errorMessage( tr("The requested file cannot be found")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
      return false;
    }
    if( !__qFileInfo.isFile() || !__qFileInfo.isReadable() )
    {
      errorMessage( tr("The requested file cannot be read from")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
      return false;
    }
    break;

  case QVCT::SAVE:
    if( __qFileInfo.exists() )
    {
      if( !__qFileInfo.isFile() || !__qFileInfo.isWritable() )
      {
        errorMessage( tr("The requested file cannot be written to")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
        return false;
      }
    }
    break;

  }

  // Check extension
  if( _pqsListExtensions )
  {
    QString __qsExtension = __qFileInfo.suffix();
    if( !_pqsListExtensions->contains( __qsExtension ) )
    {
      {
        errorMessage( tr("The file format/extension is not supported")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
        return false;
      }
    }
  }

  return true;
}

void CMainWindow::fileError( QVCT::EFileOperation _eFileOperation, const QString& _rqsFilename )
{
  QFileInfo __qFileInfo( _rqsFilename );
  switch( _eFileOperation )
  {
  case QVCT::OPEN:
    errorMessage( tr("The requested file could not be loaded")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
    break;
  case QVCT::SAVE:
    errorMessage( tr("The requested file could not be saved")+QString(":\n  .../%1").arg( __qFileInfo.fileName() ) );
    break;
  }
}

void CMainWindow::parseError( const QString& _rqsString )
{
  errorMessage( tr("The following data could not be parsed")+QString(":\n  %1").arg( _rqsString ) );
}

bool CMainWindow::symbolExists( const QString& _rqsSymbol )
{
  if( _rqsSymbol.isEmpty() ) return false;
  QFileInfo __qFileInfo( _rqsSymbol.toLower().remove( ' ' ) );
  __qFileInfo = QFileInfo( QVCTRuntime::useSettings()->getPathSymbolsDirectory()+"/"+__qFileInfo.baseName()+".png" );
  return( __qFileInfo.exists() && __qFileInfo.isReadable() );
}

QPixmap CMainWindow::symbolPixmap( const QString& _rqsSymbol )
{
  QFileInfo __qFileInfo( _rqsSymbol.toLower().remove( ' ' ) );
  __qFileInfo = QFileInfo( QVCTRuntime::useSettings()->getPathSymbolsDirectory()+"/"+__qFileInfo.baseName()+".png" );
  return QPixmap( __qFileInfo.absoluteFilePath() );
}
