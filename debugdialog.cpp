/*
 * debugdialog.cpp -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
