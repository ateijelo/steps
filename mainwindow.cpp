#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"
#include "mainscene.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    preferences = new Preferences(this);

    zoomSlider.setMinimum(0);
    zoomSlider.setMaximum(ui.mapView->maxZoomLevel());
    zoomSlider.setOrientation(Qt::Horizontal);
    zoomSlider.setFixedWidth(30+ui.mapView->maxZoomLevel()*4);
    zoomSlider.setTickInterval(1);
    zoomSlider.setTickPosition(QSlider::TicksBelow);
    zoomSlider.setPageStep(1);

    ui.toolBar->insertWidget(ui.zoomInAction, &zoomSlider);
    connect(&zoomSlider, SIGNAL(valueChanged(int)), ui.mapView, SLOT(setZoomLevel(int)));

    cacheStyles.setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &cacheStyles);

    updateCacheStyles();
    connect(&cacheStyles, SIGNAL(currentIndexChanged(QString)), ui.mapView, SLOT(setCacheStyle(QString)));

    latLabel.setFixedWidth(90);
    lonLabel.setFixedWidth(100);
    showLatLonAsToolTip.setText("Show as &tooltip");

    ui.toolBar->insertSeparator(0);
    ui.toolBar->insertWidget(0, &latLabel);
    ui.toolBar->insertWidget(0, &lonLabel);
    ui.toolBar->insertWidget(0, &showLatLonAsToolTip);

    connect(&showLatLonAsToolTip, SIGNAL(clicked(bool)), ui.mapView, SLOT(showLatLonAsToolTip(bool)));

    connect(ui.zoomInAction,SIGNAL(triggered()),ui.mapView,SLOT(zoomIn()));
    connect(ui.zoomOutAction,SIGNAL(triggered()),ui.mapView,SLOT(zoomOut()));
    connect(ui.rotRightAction,SIGNAL(triggered()),ui.mapView,SLOT(rotRight()));
    connect(ui.rotLeftAction,SIGNAL(triggered()),ui.mapView,SLOT(rotLeft()));
    connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openCacheDirectory()));
    connect(ui.action_Preferences, SIGNAL(triggered()), preferences, SLOT(show()));
    connect(ui.mapView,SIGNAL(canZoomIn(bool)),ui.zoomInAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(canZoomOut(bool)),ui.zoomOutAction,SLOT(setEnabled(bool)));
    connect(ui.mapView,SIGNAL(zoomChanged(int)),&zoomSlider,SLOT(setValue(int)));
    connect(ui.mapView,SIGNAL(mouseMoved(QPointF)),this,SLOT(updateLatLonLabels(QPointF)));

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QSettings settings;
    showLatLonAsToolTip.setChecked(settings.value(SettingsKeys::ShowLatLonAsToolTip, false).toBool());

    ui.zoomInAction->setEnabled(ui.mapView->canZoomIn());
    ui.zoomOutAction->setEnabled(ui.mapView->canZoomOut());
    zoomSlider.setValue(ui.mapView->zoomLevel());

    QPoint pos = settings.value(SettingsKeys::WindowPosition, QPoint(100, 100)).toPoint();
    QSize size = settings.value(SettingsKeys::WindowSize, QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    if (settings.value(SettingsKeys::WindowIsMaximized, false).toBool())
        showMaximized();
    else
        showNormal();
}

void MainWindow::updateCacheStyles()
{
    QSettings settings;
    QString cachePath = settings.value(SettingsKeys::CachePath, "").toString();
    QDir cacheDir(cachePath);
    QStringList cacheDirPattern("*_*");
    QFileInfoList list = cacheDir.entryInfoList(cacheDirPattern, QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList styles;
    foreach(QFileInfo fileInfo, list)
    {
        QString style = fileInfo.fileName().section("_", 0, 0);
        if (!styles.contains(style))
        {
            styles.append(style);
        }
    }
    QString tileStyle = settings.value(SettingsKeys::MapType, "").toString();
    if (tileStyle == "" && styles.length() != 0)
    {
        tileStyle = styles[0];
    }
    cacheStyles.clear();
    foreach(QString style, styles)
    {
        cacheStyles.addItem(style);
        if (style == tileStyle)
        {
            cacheStyles.setCurrentIndex(cacheStyles.count()-1);
        }
    }
}

void MainWindow::openCacheDirectory()
{
    QSettings settings;
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Open Cache Directory",
                                                     settings.value(SettingsKeys::CachePath,"").toString());
    if (path.compare("") != 0)
    {
        settings.setValue(SettingsKeys::CachePath,path);
        updateCacheStyles();
    }
}

void MainWindow::updateLatLonLabels(const QPointF& latLon)
{
    latLabel.setText(QString("Lat: %1").arg(latLon.y()));
    lonLabel.setText(QString("Lon: %1").arg(latLon.x()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(SettingsKeys::WindowPosition, pos());
    settings.setValue(SettingsKeys::WindowSize, size());
    settings.setValue(SettingsKeys::WindowIsMaximized, isMaximized());
    event->accept();
}
