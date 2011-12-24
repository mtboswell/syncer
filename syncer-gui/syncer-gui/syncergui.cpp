#include "syncergui.h"
#include "ui_syncergui.h"

SyncerGui::SyncerGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SyncerGui)
{
    ui->setupUi(this);
}

SyncerGui::~SyncerGui()
{
    delete ui;
}
