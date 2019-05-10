#include "EntityBase.hpp"
#include "DbInterfaceImplForQtSqlite.h"
#include <QString>
#include <QTranslator>
#include "DbCommonHeader.h"
#include "RecordsetMgrQTSqliteImpl.hpp"
CDbInterfaceImplForQtSqlite::CDbInterfaceImplForQtSqlite()
{
    m_Db = QSqlDatabase::addDatabase("QSQLITE");
}

CDbInterfaceImplForQtSqlite::~CDbInterfaceImplForQtSqlite()
{
    if( m_Db.isValid() )
    {
        m_Db.removeDatabase("QSQLITE");
    }
}

bool CDbInterfaceImplForQtSqlite::Connect(const std::string& strDbFileName)
{
    if( strDbFileName.empty() )
    {
        m_strLastError = "输入的文件名为空！";
        return false;
    }

    m_Db.setDatabaseName(QString::fromStdString(strDbFileName));

    if( !m_Db.open() )
    {
        m_strLastError = m_Db.lastError().text().toStdString();
        return false;
    }
    m_strDbFile = strDbFileName;
    return true;
}

bool CDbInterfaceImplForQtSqlite::Close()
{
    m_Db.close();
    return true;
}

bool CDbInterfaceImplForQtSqlite::Insert(std::string strInsert  )
{
    return  QSqlQuery(m_Db).exec(QString::fromStdString(strInsert));
}

bool CDbInterfaceImplForQtSqlite::Update(std::string strUpdate)
{
    return  QSqlQuery(m_Db).exec(QString::fromStdString(strUpdate));
}

bool CDbInterfaceImplForQtSqlite::Delete(std::string strDelete)
{
    return  QSqlQuery(m_Db).exec(QString::fromStdString(strDelete));
}

bool CDbInterfaceImplForQtSqlite::begin()
{
    m_Db.transaction();
    return true;
}

bool CDbInterfaceImplForQtSqlite::rollback()
{
    m_Db.rollback();
    return true;
}

bool CDbInterfaceImplForQtSqlite::commit()
{
    m_Db.commit();
    return true;
}

bool CDbInterfaceImplForQtSqlite::isDbOpen()
{
    return m_Db.isOpen();
}

bool CDbInterfaceImplForQtSqlite::select(const std::string &strSql,
                                         std::function<CEntityBase*()> funcCreater,
                                         std::vector<CEntityBase *> &vOut)
{
    QSqlQuery query(m_Db);

    try
    {
        if( !query.exec( QString::fromStdString(strSql) ) )
        {
            m_strLastError = query.lastError().text().toStdString();
            return false;
        }

        while( query.next() )
        {
            CEntityBase* pEntity = funcCreater();
            if( !pEntity )
            {
                m_strLastError = QObject::tr("获取数据失败！").toStdString();
                CCommonAlgorithm::clear(vOut);

                return false;
            }

            CRecordsetMgrQtSqliteImpl rs(&query);
            pEntity->Produce( &rs );
            vOut.push_back(pEntity);
        }
    } catch ( QException e)
    {
        const char* p = e.what();
        if( !p )
        {
            m_strLastError = QObject::tr("未知错误").toStdString();
        }
        else
        {
            m_strLastError = p;
        }
        CCommonAlgorithm::clear(vOut);
        return false;
    }

    return true;
}


std::string CDbInterfaceImplForQtSqlite::select(const std::string &strSql)
{
    QSqlQuery query(m_Db);

    try
    {
        if( !query.exec( QString::fromStdString(strSql) ) )
        {
            m_strLastError = query.lastError().text().toStdString();
            return  "";
        }

        while( query.next() )
        {
            return query.value(0).toString().toStdString();
        }
    } catch ( QException e)
    {
        const char* p = e.what();
        if( !p )
        {
            m_strLastError = QObject::tr("未知错误").toStdString();
        }
        else
        {
            m_strLastError = p;
        }
        return "";
    }
    return "";
}

bool CDbInterfaceImplForQtSqlite::select_single(const string &strSql, CEntityBase *pEntity)
{
    QSqlQuery query(m_Db);

    try
    {
        if( !pEntity )
        {
            m_strLastError = "传入的参数出错！";
            return false;
        }

        if( !query.exec( QString::fromStdString(strSql) ) )
        {
            m_strLastError = query.lastError().text().toStdString();
            return false;
        }

        while( query.next() )
        {
            CRecordsetMgrQtSqliteImpl rs(&query);
            pEntity->Produce( &rs );
            return true;
        }
    } catch ( QException e)
    {
        const char* p = e.what();
        if( !p )
        {
            m_strLastError = QObject::tr("未知错误").toStdString();
        }
        else
        {
            m_strLastError = p;
        }
        return false;
    }

    return false;
}
