#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "common/ftableview.h"
#include "common/ftreeview.h"
#include <QSqlTableModel>

#include "fappconfigurator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setup();

private:
    Ui::MainWindow *ui;

    QLabel *dbStatus;

    FTableView *objectsView;
    FTableView *objectTypesView;

    FTreeView *objects_groupDist;
    FTreeView *objects_groupName;
    FTreeView *objects_groupType;
    FTreeView *objects_groupDatereg;

    QSqlDatabase db;
    QString dbConnectionName;
    bool dbConnected;

    void connectToDatabase(const QString &addr, const int port, const QString &dbName, const QString &username, const QString &password);

private slots:
    void actionObjects_triggered();
    void actionObjectTypes_triggered();

    void actionGroupDist_triggered();
    void actionGroupName_triggered();
    void actionGroupType_triggered();
    void actionGroupDatereg_triggered();

    void actionDbConn_triggered();
};
#endif // MAINWINDOW_H
