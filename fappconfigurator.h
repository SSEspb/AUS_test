#ifndef FAPPCONFIGURATOR_H
#define FAPPCONFIGURATOR_H

#include <QDialog>
#include <QRegExpValidator>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSettings>

namespace Ui {
class FAppConfigurator;
}

class FAppConfigurator : public QDialog
{
    Q_OBJECT

public:
    explicit FAppConfigurator(QWidget *parent = nullptr);
    ~FAppConfigurator();

    static std::tuple<QString, int, QString, QString, QString> loadDbSettings();

signals:
    void dbConnectionChanged(const QString host, const int port, const QString database, const QString userName, const QString password);

private slots:
    void tb_checkConnection_clicked();

    void buttonBox_accepted();
    void buttonBox_rejected();

private:
    Ui::FAppConfigurator *ui;

    QString ipRange;
    QRegExp ipRangeExp;
    QRegExpValidator *ipValidator;

    void restoreFields();
};

#endif // FAPPCONFIGURATOR_H
