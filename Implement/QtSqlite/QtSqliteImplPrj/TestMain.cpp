#include "TestMain.h"
#include "ui_TestMain.h"

#include "./QtCommon/QtDbMgr.hpp"

CTestMain::CTestMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CTestMain)
{
    ui->setupUi(this);


    auto & dblogic = CQtDbMgr::GetDb();
}

CTestMain::~CTestMain()
{
    delete ui;
}


