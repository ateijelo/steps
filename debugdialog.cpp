#include <QSettings>

#include "debugdialog.h"
#include "ui_debugdialog.h"

#include "debug.h"

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    ui->setupUi(this);
    int i=0;
    QSettings settings;
    debugflags = settings.value("DebugFlags",0).toULongLong();
    QGridLayout *l = new QGridLayout();
    ui->verticalLayout->insertLayout(1,l);
    while (true)
    {
        const char *label = flagLabels[i];
        if (label == NULL)
            break;
        QCheckBox *b = new QCheckBox(label,this);
        l->addWidget(b,i/2,i%2);
        connect(b,SIGNAL(toggled(bool)),this,SLOT(checkBoxClicked(bool)));
        boxes.insert(b,Q_UINT64_C(1) << i);
        if (debugflags & Q_UINT64_C(1) << i)
            b->setChecked(true);
        i++;
    }
}

DebugDialog::~DebugDialog()
{
    delete ui;
}

void DebugDialog::checkBoxClicked(bool checked)
{
    QCheckBox *b = static_cast<QCheckBox*>(sender());
    toggleFlag(checked,boxes.value(b));
}

void DebugDialog::toggleFlag(bool checked, quint64 flag)
{
    if (checked)
    {
        debugflags |= flag;
    }
    else
    {
        debugflags &= ~flag;
    }
    QSettings settings;
    settings.setValue("DebugFlags",debugflags);
}

void DebugDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
