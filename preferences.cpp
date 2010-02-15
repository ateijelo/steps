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
