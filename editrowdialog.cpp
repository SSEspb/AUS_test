#include "editrowdialog.h"
#include "ui_editrowdialog.h"

EditRowDialog::EditRowDialog(const QSqlRecord r, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditRowDialog),
    rec(r)
{
    ui->setupUi(this);
    // setup();
}

EditRowDialog::~EditRowDialog()
{
    delete ui;
}

void EditRowDialog::setup()
{
    for (int i = 0; i < rec.count(); i++)
    {
        qDebug() << __FUNCTION__ << rec.field(i).name() << rec.value(i).toString() << rec.field(i).type();
        QLabel *l = new QLabel(rec.field(i).name());

        QWidget *w = nullptr;
        switch (rec.field(i).type()) {
        case QVariant::Double: {
            QDoubleSpinBox *sp = new QDoubleSpinBox();
            sp->setDecimals(3);
            sp->setRange(-999.999, 999.999);
            sp->setValue(rec.value(i).toDouble());
            connect (sp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &EditRowDialog::sp_valueChanged);
            w = sp;
            break;
        }
        default:
            if (!relationModels.contains(rec.field(i).name()))
            {
                QLineEdit *le = new QLineEdit(rec.value(i).toString());
                connect (le, &QLineEdit::editingFinished, this, &EditRowDialog::le_editingFinished);
                w = le;
            }
            else
            {
                QComboBox *box = new QComboBox();
                box->setModel(relationModels.value(rec.field(i).name()));
                box->setModelColumn(relationModels.value(rec.field(i).name())->fieldIndex("name"));
                connect (box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditRowDialog::cb_currentIndexChanged);
                w = box;
            }
            break;
        }

        w->setObjectName(rec.field(i).name());

        ui->gridLayout->addWidget(l, i, 0);
        ui->gridLayout->addWidget(w, i, 1);
    }
}

QSqlRecord EditRowDialog::getRec() const
{
    return rec;
}

void EditRowDialog::setRelationModelForColumn(const QString &field, QSqlTableModel *m)
{
    qDebug() << __func__ << field << (m == nullptr);
    relationModels.insert(field, m);
}

void EditRowDialog::le_editingFinished()
{
    QLineEdit *le = qobject_cast<QLineEdit *>(sender());
    if (le == nullptr)
        return;

    rec.setValue(le->objectName(), le->text());
}

void EditRowDialog::sp_valueChanged(double val)
{
    rec.setValue(sender()->objectName(), val);
}

void EditRowDialog::cb_currentIndexChanged(int idx)
{
    rec.setValue(sender()->objectName(), idx);
}
