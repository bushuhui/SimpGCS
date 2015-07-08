#ifndef __GCS_MAINWINDOW_H__
#define __GCS_MAINWINDOW_H__

#include <string>

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QTimer>
#include <QGLViewer/qglviewer.h>

#include <rtk_osa++.h>
#include <rtk_datetime.h>

#include "qFlightInstruments.h"
#include "MapWidget.h"
#include "UAS.h"


///
/// \brief The GCS_MainWindow class
///
class GCS_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GCS_MainWindow(QWidget *parent = 0);
    virtual ~GCS_MainWindow();

    virtual int setupLayout(void);

    // set/get UAS
    virtual int setActiveUAS(UAS *u) { m_uasActive = u; }
    virtual UAS* getActiveUAS(void) { return m_uasActive; }


protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

protected:
    void createActions(void);
    void createToolBars(void);
    void createMainMenu(void);

protected:
    // right-panel
    QADI                *m_ADI;
    QCompass            *m_Compass;
    QKeyValueListView   *m_infoList;

    // left-pannel
    MapWidget           *m_mapView;

    // status-bar message
    QString             m_sbTitleString;
    QString             m_sbString1, m_sbString2;

    UAS                     *m_uasActive;
    mapcontrol::UAVItem     *m_uavItem;


    rtk::RMutex         *m_mutex;
    QSettings           *m_conf;

public slots:
    void    action_ClearPos(void);
    void    action_ShowHideStatusBar(void);
    void    action_ShowHideMenuBar(void);

    void    mw_onMapDrag(void);
    void    mw_zoomChanged(int newZoom);

protected:
    QList<QAction*>     *actionList;
    QAction             *actClearPos;
    QAction             *actShowHideStatusBar;
    QAction             *actShowHideMenuBar;

    QToolBar            *menuToolBar;
};

#endif // end of __GCS_MAINWINDOW_H__
