/*
 * preferences.cpp -- part of Steps, a simple maps app
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

#include "preferences.h"
#include "ui_preferences.h"
#include "constants.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Preferences)
{
    m_ui->setupUi(this);

    connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(acceptButtonTriggered()));
    connect(m_ui->applyButton, SIGNAL(clicked()), this, SLOT(applyButtonTriggered()));
}

void Preferences::show()
{
    QSettings settings;
    QString defaultWheelOption;
#ifdef Q_OS_MAC
    defaultWheelOption = WheelOptions::ZoomsWithCtrlKeyPressed;
#else
    defaultWheelOption = WheelOptions::ZoomsWithNoKeyPressed;
#endif
    QString wheelOption = settings.value(SettingsKeys::WheelOption, defaultWheelOption).toString();
    if (wheelOption == WheelOptions::ZoomsWithCtrlKeyPressed)
    {
        m_ui->withCtrlOption->setChecked(true);
    }
    else if (wheelOption == WheelOptions::ZoomsWithNoKeyPressed)
    {
        m_ui->withNoKeyOption->setChecked(true);
    }
    else if (defaultWheelOption == WheelOptions::ZoomsWithCtrlKeyPressed) \
    {
        m_ui->withNoKeyOption->setChecked(true);
    }
    else //(defaultWheelOption == WheelOptions::ZoomsWithNoKeyPressed)
    {
        m_ui->withNoKeyOption->setChecked(true);
    }
    QDialog::show();
}

void Preferences::applyButtonTriggered()
{
    QString wheelOption;
    if (m_ui->withCtrlOption->isChecked())
    {
        wheelOption = WheelOptions::ZoomsWithCtrlKeyPressed;
    }
    else //(m_ui->withNoKeyOption->isChecked())
    {
        wheelOption = WheelOptions::ZoomsWithNoKeyPressed;
    }

    QSettings settings;
    settings.setValue(SettingsKeys::WheelOption, wheelOption);
}

void Preferences::acceptButtonTriggered()
{
    applyButtonTriggered();
    hide();
}

Preferences::~Preferences()
{
    delete m_ui;
}

void Preferences::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
