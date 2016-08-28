/*
 * debugdialog.h -- part of Steps, a simple maps app
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
