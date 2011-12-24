#include "syncergui.h"
#include "ui_syncergui.h"

SyncerGui::SyncerGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncerGui)
{
    ui->setupUi(this);
}

SyncerGui::~SyncerGui()
{
    delete ui;
}
