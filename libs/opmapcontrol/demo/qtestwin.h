#ifndef QTESTWIN_H
#define QTESTWIN_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include <opmapwidget.h>
#include "MapWidget.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class QTestWin : public QWidget
{
    Q_OBJECT

public:
    explicit QTestWin(QWidget *parent = 0);

    virtual int setupLayout(void);
    
signals:
    
public slots:

    void actFatchMap(void);
    void maptypeChanged(int index);

    void zoomChanged(int newZoom);

    void mapWidget_mouseMoveEvent(QMouseEvent *event);

    virtual void timerEvent(QTimerEvent *event);

protected:
    //mapcontrol::OPMapWidget     *m_mapWidget;
    MapWidget                   *m_mapWidget;
    QPushButton                 *m_btnFatchMap;
    QComboBox                   *m_cbMapType;

    mapcontrol::UAVItem         *m_uav;
};

#endif // QTESTWIN_H
