#include "TestMain.h"
#include "ui_TestMain.h"

CTestMain::CTestMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CTestMain)
{
    ui->setupUi(this);
}

CTestMain::~CTestMain()
{
    delete ui;
}
