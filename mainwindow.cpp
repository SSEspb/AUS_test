#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    dbStatus(new QLabel("Нет соединения с БД", this)),
    objectsView(nullptr),
    objectTypesView(nullptr),
    objects_groupDist(nullptr),
    objects_groupName(nullptr),
    objects_groupType(nullptr),
    objects_groupDatereg(nullptr),
    dbConnectionName("db_aus_connection"),
    dbConnected(false)
{
    ui->setupUi(this);
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;

    if (objects_groupDatereg != nullptr) { delete objects_groupDatereg; objects_groupDatereg = nullptr; }
    if (objects_groupType != nullptr) { delete objects_groupType; objects_groupType = nullptr; }
    if (objects_groupName != nullptr) { delete objects_groupName; objects_groupName = nullptr; }
    if (objects_groupDist != nullptr) { delete objects_groupDist; objects_groupDist = nullptr; }

    if (objectTypesView != nullptr) { delete objectTypesView; objectTypesView = nullptr; }
    if (objectsView != nullptr) { delete objectsView; objectsView = nullptr; }
}

void MainWindow::setup()
{
    ui->statusbar->addWidget(dbStatus);

    connect (ui->actionObjects, &QAction::triggered, this, &MainWindow::actionObjects_triggered);
    connect (ui->actionObjectTypes, &QAction::triggered, this, &MainWindow::actionObjectTypes_triggered);

    connect (ui->actionGroupDist, &QAction::triggered, this, &MainWindow::actionGroupDist_triggered);
    connect (ui->actionGroupName, &QAction::triggered, this, &MainWindow::actionGroupName_triggered);
    connect (ui->actionGroupType, &QAction::triggered, this, &MainWindow::actionGroupType_triggered);
    connect (ui->actionGroupDatereg, &QAction::triggered, this, &MainWindow::actionGroupDatereg_triggered);

    connect (ui->actionDbConn, &QAction::triggered, this, &MainWindow::actionDbConn_triggered);
}

void MainWindow::connectToDatabase(const QString &addr, const int port, const QString &dbName, const QString &username, const QString &password)
{
    if (db.isOpen())
    {
        db.close();
        QSqlDatabase::removeDatabase(dbConnectionName);
    }

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(addr);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(username);
    db.setPassword(password);

    dbConnected = db.open();

    if (dbConnected)
        dbStatus->setText(QString("Подключена БД: %1:%2/%3").arg(addr, QString::number(port), dbName));
    else
        dbStatus->setText("Нет соединения с БД");
}

void MainWindow::actionObjects_triggered()
{
    if (objectsView == nullptr)
    {
        objectsView = new FTableView(db, "objects", this);
        ui->stackedWidget->addWidget(objectsView);
    }

    ui->stackedWidget->setCurrentWidget(objectsView);
    setWindowTitle("AUS - объекты");
}

void MainWindow::actionObjectTypes_triggered()
{
    if (objectTypesView == nullptr)
    {
        objectTypesView = new FTableView(db, "objecttype", this);
        ui->stackedWidget->addWidget(objectTypesView);
    }

    ui->stackedWidget->setCurrentWidget(objectTypesView);
    setWindowTitle("AUS - типы объектов");
}

void MainWindow::actionGroupDist_triggered()
{
    if (objects_groupDist == nullptr)
    {
        objects_groupDist = new FTreeView(db, "v_objects_dist", QString(), this);
        ui->stackedWidget->addWidget(objects_groupDist);
    }

    ui->stackedWidget->setCurrentWidget(objects_groupDist);
    setWindowTitle("AUS - группы объектов по расстоянию");
}

void MainWindow::actionGroupName_triggered()
{
    if (objects_groupName == nullptr)
    {
        objects_groupName = new FTreeView(db, "v_objects_name", "#", this);
        ui->stackedWidget->addWidget(objects_groupName);
    }

    ui->stackedWidget->setCurrentWidget(objects_groupName);
    setWindowTitle("AUS - группы объектов по имени");
}

void MainWindow::actionGroupType_triggered()
{
    if (objects_groupType == nullptr)
    {
        objects_groupType = new FTreeView(db, "v_objects_type", "Разное", this);
        ui->stackedWidget->addWidget(objects_groupType);
    }

    ui->stackedWidget->setCurrentWidget(objects_groupType);
    setWindowTitle("AUS - группы объектов по типу");
}

void MainWindow::actionGroupDatereg_triggered()
{
    if (objects_groupDatereg == nullptr)
    {
        objects_groupDatereg = new FTreeView(db, "v_objects_date", QString(), this);
        ui->stackedWidget->addWidget(objects_groupDatereg);
    }

    ui->stackedWidget->setCurrentWidget(objects_groupDatereg);
    setWindowTitle("AUS - группы объектов по времени создания");
}

void MainWindow::actionDbConn_triggered()
{
    FAppConfigurator *app = new FAppConfigurator(this);
    if (app->exec() == QDialog::Accepted)
    {
        auto vals = app->loadDbSettings();

        QString addr = QString();
        int port = -1;
        QString dbName = QString();
        QString user = QString();
        QString password = QString();

        std::tie(addr, port, dbName, user, password) = vals;

        connectToDatabase(addr, port, dbName, user, password);
    }

    delete app;
}
