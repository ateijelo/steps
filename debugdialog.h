#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class DebugDialog;
}

class DebugDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DebugDialog(QWidget *parent = 0);
    ~DebugDialog();
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void checkBoxClicked(bool checked);
    
private:
    void toggleFlag(bool checked, int flag);
    Ui::DebugDialog *ui;
};

#endif // DEBUGDIALOG_H
