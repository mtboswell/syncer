#ifndef SYNCERGUI_H
#define SYNCERGUI_H

#include <QWidget>

namespace Ui {
class SyncerGui;
}

class SyncerGui : public QWidget
{
    Q_OBJECT
    
public:
    explicit SyncerGui(QWidget *parent = 0);
    ~SyncerGui();
    
private:
    Ui::SyncerGui *ui;
};

#endif // SYNCERGUI_H
