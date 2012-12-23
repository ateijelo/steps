#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
    class Preferences;
}

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QWidget *parent = 0);
    ~Preferences();

protected:
    void changeEvent(QEvent *e);

public slots:
    void show();

private slots:
    void applyButtonTriggered();
    void acceptButtonTriggered();

private:
    Ui::Preferences *m_ui;
};

#endif // PREFERENCES_H
