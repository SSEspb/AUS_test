#ifndef EDITROWDIALOG_H
#define EDITROWDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlTableModel>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>

#include <QDebug>

namespace Ui {
class EditRowDialog;
}

class EditRowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditRowDialog(const QSqlRecord r, QWidget *parent = nullptr);
    ~EditRowDialog();

    QSqlRecord getRec() const;

    void setRelationModelForColumn(const QString &field, QSqlTableModel *m);

    void setup();

private:
    Ui::EditRowDialog *ui;

    QSqlRecord rec;

    QMap<QString, QSqlTableModel *> relationModels;

private slots:
    void le_editingFinished();
    void sp_valueChanged(double val);
    void cb_currentIndexChanged(int idx);
};

#endif // EDITROWDIALOG_H
