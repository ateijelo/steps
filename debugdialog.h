#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QHash>
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
    void toggleFlag(bool checked, quint64 flag);
    Ui::DebugDialog *ui;
    QHash<QCheckBox*,quint64> boxes;
};

#endif // DEBUGDIALOG_H
