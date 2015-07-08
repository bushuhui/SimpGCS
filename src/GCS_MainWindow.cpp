#include <QtGui>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include <rtk_debug.h>
#include <rtk_math.h>
#include <rtk_utils.h>

#include "GCS_MainWindow.h"
#include "utils_GPS.h"

using namespace std;
using namespace rtk;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GCS_MainWindow::GCS_MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_uasActive = NULL;
    m_uavItem = NULL;

    // get gloabl setting
    void *p = NULL;
    pa_get()->p("settings", &p);
    m_conf = (QSettings*) p;

    // setup layout
    setupLayout();

    // set window minimum size
    this->setMinimumSize(1000, 700);

    // window title
    m_sbTitleString = "SimpGCS";
    QString ts = m_sbTitleString + " - Link Lost";
    setWindowTitle(ts);

    m_mutex = new RMutex();

    // status bar
    m_sbString1 = "";
    m_sbString2 = "";
    statusBar()->hide();
    statusBar()->clearMessage();

    // create actions
    createActions();

    // create MainMenu
    createMainMenu();

    startTimer(30);     // 30-millisecond timer
}

GCS_MainWindow::~GCS_MainWindow()
{
    delete m_mutex;
}

int GCS_MainWindow::setupLayout(void)
{
    /////////////////////////////////////////////
    /// left pannel
    /////////////////////////////////////////////
    m_mapView    = new MapWidget(this);

    // set initial position & zoom level
    double lat, lng;
    int    zoom;
    lat  = m_conf->value("lastPos_lat", 34.257287).toDouble();
    lng  = m_conf->value("lastPos_lng", 108.888931).toDouble();
    zoom = m_conf->value("lastZoom", 11).toInt();

    internals::PointLatLng pos(lat, lng);
    m_mapView->SetCurrentPosition(pos);
    m_mapView->SetZoom(zoom);
    m_mapView->setConf(m_conf);

    // create signal/slot connections
    connect(m_mapView, SIGNAL(OnMapDrag()),      this, SLOT(mw_onMapDrag()));
    connect(m_mapView, SIGNAL(zoomChanged(int)), this, SLOT(mw_zoomChanged(int)));

    /////////////////////////////////////////////
    /// right pannel
    /////////////////////////////////////////////
    QWidget *wLeftPanel = new QWidget(this);
    QVBoxLayout *vl = new QVBoxLayout(wLeftPanel);
    wLeftPanel->setLayout(vl);

    m_ADI      = new QADI(this);
    m_Compass  = new QCompass(this);
    m_infoList = new QKeyValueListView(this);

    vl->addWidget(m_ADI,      0, Qt::AlignTop|Qt::AlignHCenter);
    vl->addWidget(m_Compass,  0, Qt::AlignTop|Qt::AlignHCenter);
    vl->addWidget(m_infoList, 2, 0);
    vl->setMargin(0);
    vl->setSpacing(4);

    /////////////////////////////////////////////
    /// overall layout
    /////////////////////////////////////////////
    QWidget *wAll = new QWidget(this);
    QHBoxLayout *hl = new QHBoxLayout(wAll);
    wAll->setLayout(hl);

    hl->addWidget(m_mapView, 1);
    hl->addWidget(wLeftPanel, 0);

    setCentralWidget(wAll);

    return 0;
}

void GCS_MainWindow::createActions(void)
{
    // create action list
    actionList = new QList<QAction*>;

    actClearPos = new QAction(tr("Clear position data"), this);
    actClearPos->setStatusTip(tr("Clear old position data"));
    actClearPos->setShortcut(QKeySequence::fromString(tr("ctrl+c")));
    connect(actClearPos, SIGNAL(triggered()), this, SLOT(action_ClearPos()));
    actionList->append(actClearPos);

    actShowHideStatusBar = new QAction(tr("Show/hide status bar"), this);
    actShowHideStatusBar->setStatusTip(tr("Show/hide status bar"));
    actShowHideStatusBar->setCheckable(true);
    actShowHideStatusBar->setChecked(false);
    connect(actShowHideStatusBar, SIGNAL(triggered()), this, SLOT(action_ShowHideStatusBar()));
    actionList->append(actShowHideStatusBar);

    actShowHideMenuBar = new QAction(tr("Show/hide main menu"), this);
    actShowHideMenuBar->setStatusTip(tr("Show/hide main menu"));
    actShowHideMenuBar->setShortcut(QKeySequence::fromString(tr("ctrl+m")));
    actShowHideMenuBar->setCheckable(true);
    actShowHideMenuBar->setChecked(true);
    connect(actShowHideMenuBar, SIGNAL(triggered()), this, SLOT(action_ShowHideMenuBar()));
    actionList->append(actShowHideMenuBar);

    // add all actions to main window
    foreach(QAction* action, *actionList) {
        //add to MainWindow so they work when menu is hidden
        addAction(action);
    }
}

void GCS_MainWindow::createToolBars(void)
{

}

void GCS_MainWindow::createMainMenu(void)
{
    QMenu *fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(actClearPos);

    QMenu *viewMenu = new QMenu(tr("&View"));
    viewMenu->addAction(actShowHideMenuBar);
    viewMenu->addAction(actShowHideStatusBar);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
}


void GCS_MainWindow::action_ClearPos(void)
{
    m_uasActive->clearHome();

    if( m_uavItem != NULL ) {
        m_mapView->DeleteUAV(0);
        m_uavItem = NULL;
    }
}

void GCS_MainWindow::action_ShowHideStatusBar(void)
{
    if( statusBar()->isHidden() )
        statusBar()->setHidden(false);
    else
        statusBar()->setHidden(true);
}

void GCS_MainWindow::action_ShowHideMenuBar(void)
{
    if( menuBar()->isHidden() )
        menuBar()->setHidden(false);
    else
        menuBar()->setHidden(true);
}


void GCS_MainWindow::keyPressEvent(QKeyEvent *event)
{
    int     key;

    key  = event->key();

    if( key == Qt::Key_C ) {
        //clearPos();
    }
}

void GCS_MainWindow::mousePressEvent(QMouseEvent *event)
{

}

void GCS_MainWindow::resizeEvent(QResizeEvent *event)
{
}

void GCS_MainWindow::timerEvent(QTimerEvent *event)
{
    static ru64 tmLast = 0, tmNow;

    if( m_uasActive != NULL ) {
        // get current time
        tmNow = tm_get_ms();

        // set window title
        QString ts;

        if( m_uasActive->link_connected() )
            ts = m_sbTitleString + " - Link Connected";
        else
            ts = m_sbTitleString + " - Link Lost";
        setWindowTitle(ts);

        // update widgets
        m_ADI->setData(m_uasActive->roll, m_uasActive->pitch);
        m_Compass->setData(m_uasActive->yaw, m_uasActive->gpAlt, m_uasActive->gpH);

        // add track to 2D/3D plot
        if( tmNow - tmLast > 100 ) {
            double      dx, dy, dz;

            // detect new position
            if( m_uasActive->homeSetCount == 0 && m_uasActive->gpsFixType >= 3 ) {
                // calc offset from home point
                calc_earth_offset(m_uasActive->lonHome, m_uasActive->latHome,
                                  m_uasActive->lon, m_uasActive->lat,
                                  dx, dy);
                dz = m_uasActive->gpH - m_uasActive->hHome;

                // set map view
                if( m_uavItem == NULL )
                    m_uavItem = m_mapView->AddUAV(0);

                internals::PointLatLng p(m_uasActive->lat, m_uasActive->lon);
                m_uavItem->SetUAVPos(p, m_uasActive->alt);
                m_uavItem->SetUAVHeading(m_uasActive->yaw);
            }

            // set last time
            tmLast = tmNow;
        }

        // update list
        ListMap lm;
        m_infoList->beginSetData();
        m_uasActive->gen_listmap_important(lm);
        m_infoList->getData() = lm;
        m_infoList->endSetData();
        m_infoList->listReload();
    }
}

void GCS_MainWindow::closeEvent(QCloseEvent *event)
{
    m_uasActive = NULL;
}


void GCS_MainWindow::mw_onMapDrag(void)
{
    internals::PointLatLng pos;

    pos = m_mapView->CurrentPosition();

    m_conf->setValue("lastPos_lat", pos.Lat());
    m_conf->setValue("lastPos_lng", pos.Lng());

    m_conf->sync();
}

void GCS_MainWindow::mw_zoomChanged(int newZoom)
{
    m_conf->setValue("lastZoom", newZoom);

    m_conf->sync();
}

