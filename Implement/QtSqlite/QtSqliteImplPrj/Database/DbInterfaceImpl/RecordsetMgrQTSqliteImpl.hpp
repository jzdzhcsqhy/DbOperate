#ifndef RECORDSETMGRQTSQLITEIMPL_HPP
#define RECORDSETMGRQTSQLITEIMPL_HPP

#include <QtSql>
#include "RecordsetMgr.hpp"

class CRecordsetMgrQtSqliteImpl : public CRecordsetMgr
{

public:
    CRecordsetMgrQtSqliteImpl( QSqlQuery* pQuery )
        :m_pRecordSet(pQuery)
    {

    }
    virtual ~CRecordsetMgrQtSqliteImpl()
    {

    }

public:
    inline virtual void get_value( int iColIndex, int& val )
    {
        if(!m_pRecordSet)
        {
            return ;
        }

        val = m_pRecordSet->value(iColIndex).toInt();
    }
    inline virtual void get_value( int iColIndex, double& val )
    {
        if(!m_pRecordSet)
        {
            return ;
        }

        val = m_pRecordSet->value(iColIndex).toDouble();
    }
    inline virtual void get_value( int iColIndex, bool& val )
    {
        if(!m_pRecordSet)
        {
            return ;
        }

        val = m_pRecordSet->value(iColIndex).toBool();
    }
    inline virtual void get_value( int iColIndex, std::string& val )
    {
        if(!m_pRecordSet)
        {
            return ;
        }

        val = m_pRecordSet->value(iColIndex).toString().toStdString();
    }
public:
    QSqlQuery* m_pRecordSet;
};
#endif // RECORDSETMGRQTSQLITEIMPL_HPP
