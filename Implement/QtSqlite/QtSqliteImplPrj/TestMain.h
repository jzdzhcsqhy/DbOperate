#ifndef TESTMAIN_H
#define TESTMAIN_H

#include <QMainWindow>

namespace Ui {
class CTestMain;
}

class CTestMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit CTestMain(QWidget *parent = nullptr);
    ~CTestMain();

private:
    Ui::CTestMain *ui;
};

#endif // TESTMAIN_H
