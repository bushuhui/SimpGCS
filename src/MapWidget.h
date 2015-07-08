#ifndef __MAP_WIDGET_H__
#define __MAP_WIDGET_H__

#include <opmapwidget.h>

class MapWidget : public mapcontrol::OPMapWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    virtual ~MapWidget();

    void setConf(QSettings *conf);

protected:
    int         setupMenu(void);

    QMenu       *m_popupMenu;

    QAction     *m_actMapType_GoogleSatellate;
    QAction     *m_actMapType_GoogleHybrid;
    QAction     *m_actMapType_GoogleMap;

    QAction     *m_actMapAccess_ServerAndCache;
    QAction     *m_actMapAccess_Cache;

    QSettings   *m_conf;

public slots:
    void        actMapType_GoogleSatellate(void);
    void        actMapType_GoogleHybrid(void);
    void        actMapType_GoogleMap(void);

    void        actMapAccess_ServerAndCache(void);
    void        actMapAccess_Cache(void);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // end of __MAP_WIDGET_H__
