
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "qtestwin.h"

QTestWin::QTestWin(QWidget *parent) :
    QWidget(parent)
{
    // setup layout
    setupLayout();

    // set window title
    setWindowTitle("Map demo");
}

int QTestWin::setupLayout(void)
{
    // create OPMapWidget
    m_mapWidget = new MapWidget(this);

    /*
    //m_mapWidget->configuration->SetAccessMode(core::AccessMode::ServerAndCache);
    m_mapWidget->configuration->SetAccessMode(core::AccessMode::CacheOnly);
    m_mapWidget->configuration->SetTileMemorySize(200);
    m_mapWidget->configuration->SetCacheLocation("../../data/");    
    m_mapWidget->SetZoom(4);
    m_mapWidget->SetMinZoom(4);
    m_mapWidget->SetMaxZoom(18);
    m_mapWidget->SetMapType(MapType::GoogleSatellite);
    */

    internals::PointLatLng pos(34.257287, 108.888931);
    m_mapWidget->SetCurrentPosition(pos);
    m_mapWidget->SetZoom(11);

    connect(m_mapWidget, SIGNAL(zoomChanged(int)), this, SLOT(zoomChanged(int)));
    connect(m_mapWidget, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mapWidget_mouseMoveEvent(QMouseEvent*)));

    m_uav = m_mapWidget->AddUAV(0);


    // create right pannel
    QWidget *wLeftPanel = new QWidget(this);
    QVBoxLayout *vl = new QVBoxLayout(wLeftPanel);
    wLeftPanel->setLayout(vl);
    wLeftPanel->setFocusPolicy(Qt::NoFocus);

    m_btnFatchMap = new QPushButton(this);
    m_btnFatchMap->setText("Fatch map");
    connect(m_btnFatchMap, SIGNAL(clicked()), this, SLOT(actFatchMap()));

    m_cbMapType = new QComboBox(this);
    m_cbMapType->addItem("Google Satellite");
    m_cbMapType->addItem("Google Hybird");
    m_cbMapType->addItem("Google Map");
    connect(m_cbMapType, SIGNAL(activated(int)), this, SLOT(maptypeChanged(int)));


    vl->addWidget(m_btnFatchMap, 0, Qt::AlignTop|Qt::AlignHCenter);
    vl->addWidget(m_cbMapType,   0, Qt::AlignTop|Qt::AlignHCenter);
    vl->setMargin(0);
    vl->setSpacing(4);

    // overall layout
    QHBoxLayout *hl = new QHBoxLayout(this);
    this->setLayout(hl);

    hl->addWidget(m_mapWidget, 1);
    hl->addWidget(wLeftPanel, 0);

    this->startTimer(30);
}


void QTestWin::actFatchMap(void)
{
    internals::RectLatLng  rect(34.257287, 108.888931, 0.1, 0.1);

    m_mapWidget->SetSelectedArea(rect);
    m_mapWidget->RipMap();
}

void QTestWin::maptypeChanged(int index)
{
    switch(index) {
    case 0:
        m_mapWidget->SetMapType(MapType::GoogleSatellite);
        break;

    case 1:
        m_mapWidget->SetMapType(MapType::GoogleHybrid);
        break;

    case 2:
        m_mapWidget->SetMapType(MapType::GoogleMap);
        break;
    }
}

void QTestWin::zoomChanged(int newZoom)
{
    printf(">>> zoom to: %d\n", newZoom);
}

void QTestWin::mapWidget_mouseMoveEvent(QMouseEvent *event)
{
    internals::PointLatLng p;

    p = m_mapWidget->currentMousePosition();
    printf("p  = %f %f\n", p.Lat(), p.Lng());

    m_uav->SetUAVPos(p, 1000);
}

void QTestWin::timerEvent(QTimerEvent *event)
{
    //internals::PointLatLng p;
    //p = m_mapWidget->currentMousePosition();
    //printf("p3 = %f %f\n", p.Lat(), p.Lng());
}
