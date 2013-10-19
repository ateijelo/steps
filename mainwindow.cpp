#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "mgmreader.h"
#include "mainscene.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    updateRecents();

    preferences = new Preferences(this);
    debugDialog = new DebugDialog(this);
    debugShortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+D","Show Debug Dialog")),this);
    debugShortcut->setContext(Qt::ApplicationShortcut);
    connect(debugShortcut,SIGNAL(activated()),debugDialog,SLOT(show()));

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
    connect(ui.aboutAction,SIGNAL(triggered()),this,SLOT(aboutDialog()));

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QSettings settings;
    showLatLonAsToolTip.setChecked(settings.value(SettingsKeys::ShowLatLonAsToolTip, false).toBool());

    ui.zoomInAction->setEnabled(ui.mapView->canZoomIn());
    ui.zoomOutAction->setEnabled(ui.mapView->canZoomOut());
    zoomSlider.setValue(ui.mapView->zoomLevel());

    restoreGeometry(settings.value(SettingsKeys::WindowGeometry).toByteArray());
    restoreState(settings.value(SettingsKeys::WindowState).toByteArray());

    connect(&recentsMapper, SIGNAL(mapped(QString)), this, SLOT(updateCacheDirectory(QString)));
}

void MainWindow::updateRecents(QString top)
{
    QSettings settings;

    QStringList recents;
    int amount = settings.beginReadArray(SettingsKeys::RecentPaths);
    for (int i = 0; i < amount; i++)
    {
        settings.setArrayIndex(i);
        recents.append(settings.value(SettingsKeys::path).toString());
    }
    settings.endArray();

    if (top.isNull())
    {
        top = settings.value(SettingsKeys::CachePath, "").toString();

        if (recents.isEmpty())
        {
            if (!top.isEmpty())
            {
                recents.append(top);
            }
        }
        else
        {
            if (top.isEmpty())
            {
                top = recents[0];
            }
        }
    }
    else
    {
        if (recents.isEmpty())
        {
            recents.append(top);
        }
    }

    if (!recentPaths.isEmpty())
    {
        foreach(QAction *action, recentPaths)
        {
            this->ui.menu_File->removeAction(action);
            recentsMapper.removeMappings(action);
        }
        recentPaths.clear();
    }

    if (!recents.isEmpty())
    {
        recents.insert(0, top);
        int index = recents.indexOf(top, 1);
        if (index != -1)
        {
            recents.removeAt(index);
        }

        QAction *action;
        for (int i = 0; i < recents.length(); i++)
        {
            action = new QAction(QString("&%1 %2").arg(i+1).arg(recents[i]), this->ui.menu_File);
            recentsMapper.setMapping(action, recents[i]);
            connect(action, SIGNAL(triggered()), &recentsMapper, SLOT(map()));
            this->ui.menu_File->insertAction(this->ui.action_Exit, action);
            recentPaths.append(action);
        }

        action = this->ui.menu_File->insertSeparator(this->ui.action_Exit);
        recentPaths.append(action);
    }

    settings.beginWriteArray(SettingsKeys::RecentPaths);
    for (int i = 0; i < recents.length(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue(SettingsKeys::path, recents[i]);
    }
    settings.endArray();
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

void MainWindow::updateCacheDirectory(QString path)
{
    QSettings settings;

    settings.setValue(SettingsKeys::CachePath,path);
    updateCacheStyles();
    updateRecents(path);
}

void MainWindow::openCacheDirectory()
{
    QSettings settings;
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Open Cache Directory",
                                                     settings.value(SettingsKeys::CachePath,"").toString());
    if (path.compare("") != 0)
    {
        updateCacheDirectory(path);
    }
}

void MainWindow::updateLatLonLabels(const QPointF& latLon)
{
    latLabel.setText(QString("Lat: %1").arg(latLon.y()));
    lonLabel.setText(QString("Lon: %1").arg(latLon.x()));
}

void MainWindow::aboutDialog()
{
    QMessageBox::about(this,"About QtGMaps","Copyright (C) 2009-2013 Andy Teijelo <ateijelo@gmail.com>\n"
                            "QtGMaps is licensed under the GPL version 3 or later.");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(SettingsKeys::WindowGeometry, saveGeometry());
    settings.setValue(SettingsKeys::WindowState, saveState());
    event->accept();
}
