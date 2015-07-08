
#include <QtCore>
#include <QtGui>
#include <QMenu>

#include "MapWidget.h"


MapWidget::MapWidget(QWidget *parent) :
    mapcontrol::OPMapWidget(parent)
{
    m_conf = NULL;

    configuration->SetAccessMode(core::AccessMode::CacheOnly);
    configuration->SetTileMemorySize(200);
    configuration->SetCacheLocation("./data/");

    SetZoom(4);
    SetMinZoom(4);
    SetMaxZoom(18);
    SetMapType(MapType::BingSatellite);

    setupMenu();
}

MapWidget::~MapWidget()
{

}

void MapWidget::setConf(QSettings *conf)
{
    m_conf = conf;

    if( m_conf != NULL ) {
        MapType::Types              mapType;
        core::AccessMode::Types     accessMode;
        QString                     cacheLocation;

        // load settings
        accessMode    = (core::AccessMode::Types) m_conf->value("mapWidget_accessMode",
                                          (int)(core::AccessMode::CacheOnly)).toInt();
        mapType       = (MapType::Types) m_conf->value("mapWidget_mapType",
                                          (int)(MapType::GoogleSatellite)).toInt();
        cacheLocation = m_conf->value("mapWidget_cacheLocation", "./data/").toString();

        // set configurations
        configuration->SetAccessMode(accessMode);
        configuration->SetCacheLocation(cacheLocation);
        //SetMapType(mapType);

        // sync to file
        m_conf->setValue("mapWidget_mapType", (int)(mapType));
        m_conf->setValue("mapWidget_accessMode", (int)(accessMode));
        m_conf->setValue("mapWidget_cacheLocation", cacheLocation);
        m_conf->sync();

        // set accessMode actions
        if( accessMode == core::AccessMode::ServerAndCache ) {
            m_actMapAccess_ServerAndCache->setChecked(true);
            m_actMapAccess_Cache->setChecked(false);
        } else if ( accessMode == core::AccessMode::CacheOnly ) {
            m_actMapAccess_ServerAndCache->setChecked(false);
            m_actMapAccess_Cache->setChecked(true);
        }

        // set mapType actions
        if( mapType == MapType::GoogleMap ) {
            m_actMapType_GoogleSatellate->setChecked(false);
            m_actMapType_GoogleHybrid->setChecked(false);
            m_actMapType_GoogleMap->setChecked(true);
        } else if( mapType == MapType::GoogleSatellite ) {
            m_actMapType_GoogleSatellate->setChecked(true);
            m_actMapType_GoogleHybrid->setChecked(false);
            m_actMapType_GoogleMap->setChecked(false);
        } else if( mapType == MapType::GoogleHybrid ) {
            m_actMapType_GoogleSatellate->setChecked(false);
            m_actMapType_GoogleHybrid->setChecked(true);
            m_actMapType_GoogleMap->setChecked(false);
        }
    }
}

int MapWidget::setupMenu(void)
{
    // setup actions
    m_actMapType_GoogleSatellate = new QAction(tr("Google Satellate"), this);
    m_actMapType_GoogleSatellate->setCheckable(true);
    m_actMapType_GoogleSatellate->setChecked(false);
    connect(m_actMapType_GoogleSatellate, SIGNAL(triggered()),
            this, SLOT(actMapType_GoogleSatellate()));

    m_actMapType_GoogleHybrid = new QAction(tr("Google Hybrid"), this);
    m_actMapType_GoogleHybrid->setCheckable(true);
    m_actMapType_GoogleHybrid->setChecked(true);
    connect(m_actMapType_GoogleHybrid, SIGNAL(triggered()),
            this, SLOT(actMapType_GoogleHybrid()));

    m_actMapType_GoogleMap = new QAction(tr("Google Map"), this);
    m_actMapType_GoogleMap->setCheckable(true);
    m_actMapType_GoogleMap->setChecked(false);
    connect(m_actMapType_GoogleMap, SIGNAL(triggered()),
            this, SLOT(actMapType_GoogleMap()));


    m_actMapAccess_ServerAndCache = new QAction(tr("ServerAndCache"), this);
    m_actMapAccess_ServerAndCache->setCheckable(true);
    m_actMapAccess_ServerAndCache->setChecked(false);
    connect(m_actMapAccess_ServerAndCache, SIGNAL(triggered()),
            this, SLOT(actMapAccess_ServerAndCache()));

    m_actMapAccess_Cache = new QAction(tr("Cache"), this);
    m_actMapAccess_Cache->setCheckable(true);
    m_actMapAccess_Cache->setChecked(true);
    connect(m_actMapAccess_Cache, SIGNAL(triggered()),
            this, SLOT(actMapAccess_Cache()));


    // setup menu
    m_popupMenu = new QMenu("Menu");

    //QMenu *menuMapType = m_popupMenu->addMenu("Map Type");
    //menuMapType->addAction(m_actMapType_GoogleSatellate);
    //menuMapType->addAction(m_actMapType_GoogleHybrid);
    //menuMapType->addAction(m_actMapType_GoogleMap);

    QMenu *menuAccessMode = m_popupMenu->addMenu("Access Type");
    menuAccessMode->addAction(m_actMapAccess_ServerAndCache);
    menuAccessMode->addAction(m_actMapAccess_Cache);

    return 0;
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::RightButton ) {
        m_popupMenu->popup(event->globalPos());
    } else {
        mapcontrol::OPMapWidget::mousePressEvent(event);
    }
}


void MapWidget::actMapType_GoogleSatellate(void)
{
    m_actMapType_GoogleSatellate->setChecked(true);
    m_actMapType_GoogleHybrid->setChecked(false);
    m_actMapType_GoogleMap->setChecked(false);

    SetMapType(MapType::GoogleSatellite);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_mapType", (int)(MapType::GoogleSatellite));
        m_conf->sync();
    }
}

void MapWidget::actMapType_GoogleHybrid(void)
{
    m_actMapType_GoogleSatellate->setChecked(false);
    m_actMapType_GoogleHybrid->setChecked(true);
    m_actMapType_GoogleMap->setChecked(false);

    SetMapType(MapType::GoogleHybrid);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_mapType", (int)(MapType::GoogleHybrid));
        m_conf->sync();
    }
}

void MapWidget::actMapType_GoogleMap(void)
{
    m_actMapType_GoogleSatellate->setChecked(false);
    m_actMapType_GoogleHybrid->setChecked(false);
    m_actMapType_GoogleMap->setChecked(true);

    SetMapType(MapType::GoogleMap);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_mapType", (int)(MapType::GoogleMap));
        m_conf->sync();
    }
}

void MapWidget::actMapAccess_ServerAndCache(void)
{
    m_actMapAccess_ServerAndCache->setChecked(true);
    m_actMapAccess_Cache->setChecked(false);

    configuration->SetAccessMode(core::AccessMode::ServerAndCache);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_accessMode", (int)(core::AccessMode::ServerAndCache));
        m_conf->sync();
    }
}

void MapWidget::actMapAccess_Cache(void)
{
    m_actMapAccess_ServerAndCache->setChecked(false);
    m_actMapAccess_Cache->setChecked(true);

    configuration->SetAccessMode(core::AccessMode::CacheOnly);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_accessMode", (int)(core::AccessMode::CacheOnly));
        m_conf->sync();
    }
}
