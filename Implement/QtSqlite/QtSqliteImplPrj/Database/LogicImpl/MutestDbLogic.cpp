#include "MutestDbLogic.h"
#include "../DbInterfaceImpl/DbInterfaceImplForQtSqlite.h"

#include "../../QtCommon/qtstring.hpp"
#include "../../QtCommon/qtfileoperate.hpp"
#include "DbCommonHeader.h"
#include <QFileDialog>
#include <QFile>


CMutestDbLogic::CMutestDbLogic()
    :CDbLogic (PATH_ROAMING)
{

}

CMutestDbLogic::~CMutestDbLogic()
{

}

bool CMutestDbLogic::IsDBOpenedImpl()
{
    if( m_db )
    {
        return m_db->isDbOpen();
    }
    return false ;
}

bool CMutestDbLogic::CreatePathImpl(const std::string &strPath )
{
    // 获得全路径
    QString strFullPath = QString::fromStdString(strPath);

    // 判断目录是否已经存在了
    if( CQtFileOperate::isPathExist(strFullPath) )
    {// 如果是，要根据参数来报错
        return true;
    }
    // 如果不存在，就创建之
    return CQtFileOperate::makePath(QString::fromStdString(strPath) );
}

bool CMutestDbLogic::CopyFileImpl(const std::string &strSrc, const std::string &strDst )
{
    return CQtFileOperate::copyFile(
                QString::fromStdString(strSrc),
                QString::fromStdString(strDst),
                true);
}

bool CMutestDbLogic::OpenDbImpl(const std::string &strPath)
{
    if (!m_db)
    {
        m_db = new CDbInterfaceImplForQtSqlite;
        return m_db->Connect(strPath);
    }
    return true;
}

bool CMutestDbLogic::CloseDbImpl()
{
    if (m_db)
    {
        //
        delete m_db;
        m_db = nullptr;
    }
    return true;
}

std::string CMutestDbLogic::GetUuidImpl()
{
    return CQtString::GetUuid().toStdString();
}

bool CMutestDbLogic::ClearDirectoryImpl(std::string &strDir, bool bIncludeSelf)
{
    CQtFileOperate::delDir(QString::fromStdString(strDir), bIncludeSelf );
    return true;
}

std::string CMutestDbLogic::GetSavePathImpl()
{
    return QFileDialog::getSaveFileName(nullptr,("请选择保存路径"),(""),
                                        ("测试文件 (*.test)") ).toStdString();
}

string CMutestDbLogic::NewDbImpl()
{
    return "";
}
