#include "debugdialog.h"
#include "ui_debugdialog.h"

#include "debug.h"

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    ui->setupUi(this);
    connect(ui->network,SIGNAL(toggled(bool)),this,SLOT(checkBoxClicked(bool)));
}

DebugDialog::~DebugDialog()
{
    delete ui;
}

void DebugDialog::checkBoxClicked(bool checked)
{
    QObject *s = sender();
    if (s == ui->network)
        toggleFlag(checked,DEBUG_NETWORK);
    if (s == ui->disk)
        toggleFlag(checked,DEBUG_DISK);
}

void DebugDialog::toggleFlag(bool checked, int flag)
{
    if (checked)
    {
        debugflags |= flag;
    }
    else
    {
        debugflags &= ~flag;
    }
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
