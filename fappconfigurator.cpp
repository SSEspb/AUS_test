#include "fappconfigurator.h"
#include "ui_fappconfigurator.h"

FAppConfigurator::FAppConfigurator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FAppConfigurator),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRangeExp("^" + ipRange
               + "\\." + ipRange
               + "\\." + ipRange
               + "\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRangeExp, this))
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Применить"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));

    connect (ui->tb_checkConnection, &QToolButton::clicked, this, &FAppConfigurator::tb_checkConnection_clicked);
    connect (ui->buttonBox, &QDialogButtonBox::accepted, this, &FAppConfigurator::buttonBox_accepted);
    connect (ui->buttonBox, &QDialogButtonBox::rejected, this, &FAppConfigurator::buttonBox_rejected);

    ui->le_ipAddr->setValidator(ipValidator);

    restoreFields();
}

FAppConfigurator::~FAppConfigurator()
{
    delete ui;
}

std::tuple<QString, int, QString, QString, QString> FAppConfigurator::loadDbSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".conf", QSettings::IniFormat);

    return std::make_tuple<QString, int, QString, QString, QString>(settings.value("/HostName", "127.0.0.1").toString(),
                                                                    settings.value("/Port", 5432).toInt(),
                                                                    settings.value("/DatabaseName", "aus").toString(),
                                                                    settings.value("/UserName", "admin").toString(),
                                                                    settings.value("/Password").toString());
}

void FAppConfigurator::tb_checkConnection_clicked()
{
    // проверим коннект к базе по введенным параметрам и поставим нужную иконку
    QSqlDatabase checkConn = QSqlDatabase::addDatabase("QPSQL", "qt_pg_test_connection");
    checkConn.setHostName(ui->le_ipAddr->text());
    checkConn.setPort(ui->sb_port->value());
    checkConn.setDatabaseName(ui->le_dbName->text());
    checkConn.setUserName(ui->le_userName->text());
    checkConn.setPassword(ui->le_userPassword->text());

    if (checkConn.open())
    {
        // ui->tb_checkConnection->setIcon(awesome->icon(fa::check));
    }
    else
    {
        // ui->tb_checkConnection->setIcon(awesome->icon(fa::questioncircle));
    }

    checkConn.close();
    checkConn.removeDatabase("qt_pg_test_connection");
}

void FAppConfigurator::restoreFields()
{
    // БД
    auto sett = loadDbSettings();

    QString addr = QString();
    int port = -1;
    QString dbName = QString();
    QString user = QString();
    QString password = QString();

    std::tie(addr, port, dbName, user, password) = sett;

    ui->le_ipAddr->setText(addr);
    ui->sb_port->setValue(port);
    ui->le_dbName->setText(dbName);
    ui->le_userName->setText(user);
    ui->le_userPassword->setText(password);

    tb_checkConnection_clicked();
}

void FAppConfigurator::buttonBox_accepted()
{
    // сохраним конфиг файл в "нормальном" виде и выйдем
    QSettings settings(QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".conf", QSettings::IniFormat);
    settings.clear();

    settings.setValue("/HostName", ui->le_ipAddr->text());
    settings.setValue("/Port", ui->sb_port->value());
    settings.setValue("/DatabaseName", ui->le_dbName->text());
    settings.setValue("/UserName", ui->le_userName->text());
    settings.setValue("/Password", ui->le_userPassword->text());

    emit dbConnectionChanged(ui->le_ipAddr->text(),
                             ui->sb_port->value(),
                             ui->le_dbName->text(),
                             ui->le_userName->text(),
                             ui->le_userPassword->text());

    QDialog::accept();
}


void FAppConfigurator::buttonBox_rejected()
{
    QDialog::reject();
}
