#include <QtDebug>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QFileSystemModel>

#include "mainwindow.h"
#include "mgmreader.h"
#include "mainscene.h"
#include "constants.h"
#include "pathsdockmodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    loadRecents();

    preferences = new Preferences(this);
    debugDialog = new DebugDialog(this);
    debugShortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+D","Show Debug Dialog")),this);
    debugShortcut->setContext(Qt::ApplicationShortcut);
    connect(debugShortcut,SIGNAL(activated()),debugDialog,SLOT(show()));

//    zoomSlider.setMinimum(0);
//    zoomSlider.setMaximum(ui.mapView->maxZoomLevel());
//    zoomSlider.setOrientation(Qt::Horizontal);
//    zoomSlider.setFixedWidth(30+ui.mapView->maxZoomLevel()*4);
//    zoomSlider.setTickInterval(1);
//    zoomSlider.setTickPosition(QSlider::TicksBelow);
//    zoomSlider.setPageStep(1);

//    ui.toolBar->insertWidget(ui.zoomInAction, &zoomSlider);
//    connect(&zoomSlider, SIGNAL(valueChanged(int)), ui.mapView, SLOT(setZoomLevel(int)));

//    cacheStyles.setSizeAdjustPolicy(QComboBox::AdjustToContents);

//    ui.toolBar->insertSeparator(0);
//    ui.toolBar->insertWidget(0, &cacheStyles);

//    updateCacheStyles();
//    connect(&cacheStyles, SIGNAL(currentIndexChanged(QString)), ui.mapView, SLOT(setCacheStyle(QString)));

//    latLabel.setFixedWidth(90);
//    lonLabel.setFixedWidth(100);
//    showLatLonAsToolTip.setText("Show as &tooltip");

//    ui.toolBar->insertSeparator(0);
//    ui.toolBar->insertWidget(0, &latLabel);
//    ui.toolBar->insertWidget(0, &lonLabel);
//    ui.toolBar->insertWidget(0, &showLatLonAsToolTip);

//    connect(&showLatLonAsToolTip, SIGNAL(clicked(bool)), ui.mapView, SLOT(showLatLonAsToolTip(bool)));

    connect(ui.zoomInAction,&QAction::triggered,ui.mapView,&MapView::zoomIn);
    connect(ui.zoomOutAction,&QAction::triggered,ui.mapView,&MapView::zoomOut);
    connect(ui.rotRightAction,&QAction::triggered,ui.mapView,&MapView::rotRight);
    connect(ui.rotLeftAction,&QAction::triggered,ui.mapView,&MapView::rotLeft);
    //connect(ui.openAction,&QAction::triggered,this,&MainWindow::openCacheDirectory);
    connect(ui.action_Preferences, &QAction::triggered, preferences, &Preferences::show);
//    connect(ui.mapView,SIGNAL(canZoomIn(bool)),ui.zoomInAction,SLOT(setEnabled(bool)));
//    connect(ui.mapView,SIGNAL(canZoomOut(bool)),ui.zoomOutAction,SLOT(setEnabled(bool)));
//    connect(ui.mapView,SIGNAL(zoomChanged(int)),&zoomSlider,SLOT(setValue(int)));
//    connect(ui.mapView,&MapView::mouseMoved,this,&MainWindow::updateLatLonLabels);
    connect(ui.aboutAction,&QAction::triggered,this,&MainWindow::aboutDialog);
    connect(ui.openMBTilesAction,&QAction::triggered, this, &MainWindow::openMBTiles);
    connect(ui.mapView, &MapView::loadedMBTiles, this, &MainWindow::addRecent);
    connect(ui.newPathAction, &QAction::triggered, this, &MainWindow::newPath);

    ui.zoomInAction->setShortcut(QKeySequence::ZoomIn);
    ui.zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QSettings settings;
//    showLatLonAsToolTip.setChecked(settings.value(SettingsKeys::ShowLatLonAsToolTip, false).toBool());

    ui.zoomInAction->setEnabled(ui.mapView->canZoomIn());
    ui.zoomOutAction->setEnabled(ui.mapView->canZoomOut());
//    zoomSlider.setValue(ui.mapView->zoomLevel());

    restoreGeometry(settings.value(SettingsKeys::WindowGeometry).toByteArray());
    restoreState(settings.value(SettingsKeys::WindowState).toByteArray());

    pathsModel = new PathsDockModel();

    ui.pathsTreeView->setModel(pathsModel);

    //connect(&recentsMapper, SIGNAL(mapped(QString)), this, SLOT(updateCacheDirectory(QString)));
}

void MainWindow::loadRecents()
{
    QSettings settings;

    recentPaths = settings.value(SettingsKeys::RecentPaths).toStringList();

    if (recentPaths.isEmpty())
        return;

    for (QAction* a: recentPathActions) {
        ui.menu_File->removeAction(a);
        a->deleteLater();
    }
    recentPathActions.clear();

    QAction *action;
    for (int i = 0; i < recentPaths.length(); i++)
    {
        action = new QAction(
            QString("&%1 %2").arg(i+1).arg(recentPaths[i]),
            this->ui.menu_File
        );
        recentPathActions.insert(action);
        connect(action, &QAction::triggered, [this,i](){
            openRecentPath(i);
        });
        this->ui.menu_File->insertAction(this->ui.action_Exit, action);
    }

    action = this->ui.menu_File->insertSeparator(this->ui.action_Exit);
}

void MainWindow::addRecent(const QString& path)
{
    QSettings settings;

    recentPaths.removeAll(path);
    recentPaths.push_front(path);

    while (recentPaths.length() > 10)
        recentPaths.removeLast();

    settings.setValue(SettingsKeys::RecentPaths, recentPaths);

    loadRecents();
}

void MainWindow::openRecentPath(int index)
{
    loadMBTilesFile(recentPaths.at(index));
}

void MainWindow::openMBTiles()
{
    QSettings settings;
    QString dir = "";
    QFileInfo fi(settings.value(SettingsKeys::MBTilesPath,"").toString());
    if (fi.exists())
        dir = fi.dir().absolutePath();
    QString path = QFileDialog::getOpenFileName(this,
                                                "Open MBTiles file",
                                                dir,
                                                "*.mbtiles");
    if (!path.isEmpty())
    {
        loadMBTilesFile(path);
    }
}

void MainWindow::loadMBTilesFile(const QString &path)
{
    QSettings settings;
    QString currentPath = settings.value(SettingsKeys::MBTilesPath, "").toString();
    if (path == currentPath)
        return;

    settings.setValue(SettingsKeys::MBTilesPath, path);
    ui.mapView->reload();
}

//void MainWindow::updateLatLonLabels(const QPointF& latLon)
//{
//    latLabel.setText(QString("Lat: %1").arg(latLon.y()));
//    lonLabel.setText(QString("Lon: %1").arg(latLon.x()));
//}

void MainWindow::aboutDialog()
{
    QMessageBox::about(this,"About Steps","Copyright (C) 2009-2015 Andy Teijelo <ateijelo@gmail.com>\n"
                                          "Steps is distributed under the terms of the MIT License");
}

void MainWindow::newPath()
{
    auto r = ui.mapView->rect();
    int m = qMin(r.width(), r.height()) / 2;
    QPoint from(r.center().x() - m/2, r.center().y() + m/2);
    QPoint to(r.center().x() + m/2, r.center().y() - m/2);

    auto sfrom = ui.mapView->mapToScene(from);
    auto sto = ui.mapView->mapToScene(to);

    qreal pw = GeoTools::projectionWidth();
    auto a = ui.mapView->mapToScene(r.bottomLeft());
    auto b = ui.mapView->mapToScene(r.topRight());
    if (qAbs(a.x() - b.x()) > pw) {
        sto = GeoTools::LatLon2Meters({-85,-65});
        sfrom = GeoTools::LatLon2Meters({85,65});
    }

    qDebug() << "mapView visible world region:" << ui.mapView->sceneRect();
    auto path = new Path(sfrom, sto);
    pathsModel->addPath(path);
    ui.mapView->addPath(path);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(SettingsKeys::WindowGeometry, saveGeometry());
    settings.setValue(SettingsKeys::WindowState, saveState());
    event->accept();
}
