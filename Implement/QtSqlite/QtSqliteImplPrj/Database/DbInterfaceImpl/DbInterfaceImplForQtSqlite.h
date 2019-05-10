////////////////////////////////////////////////////////////////////////////////
//
// File Name:	DbInterfaceImpl.h
// Class Name:  CDbInterfaceImpl
// Description:	数据库操作接口在qt环境下的实现
// Author:		罗志彬
// Date:		2019年4月22日
//
////////////////////////////////////////////////////////////////////////////////


#ifndef DBINTERFACEIMPL_H
#define DBINTERFACEIMPL_H

#include <QtSql>

#include "DbInterface.hpp"
#include <QString>

class CDbInterfaceImplForQtSqlite : public CDbInterface
{
public:
    CDbInterfaceImplForQtSqlite();
    virtual ~CDbInterfaceImplForQtSqlite();

////////////// 纯虚接口函数的实现
public:
    // 连接Sqlite数据库
    virtual bool Connect( const std::string& strDbFileName );
    // 断开Sqlite数据库
    virtual bool Close();

    // 插入修改删除
    virtual bool Insert(std::string strInsert);
    virtual bool Update(std::string strUpdate);
    virtual bool Delete(std::string strDelete);

    // 事务控制
    virtual bool begin();
    virtual bool rollback();
    virtual bool commit();

    virtual bool isDbOpen();


    // 查询符合条件的数据
    virtual bool select(const std::string& strSql,
                        std::function<CEntityBase*()> funcCreater,
                        std::vector<CEntityBase*>& vOut );
    // 执行返回单一结果的查询
    virtual std::string select(const std::string& strSql );

    // 查询符合条件一条记录，如果查询到多条，就返回第一个
   virtual bool select_single( const std::string& strSql, CEntityBase* pEntity );

///////////// 功能函数
public:
    // 辅助函数，用来根据语句查询第一条记录
    bool select_singlle_record( const char* pStrQuery,CEntityBase* pEntity);
    // 辅助函数，根据语句查询
    std::string exec_select(  const char* pstrQuery);

///////内部数据处理函数
public:




public:
    std::string m_strDbFile;
    QSqlDatabase m_Db;
};

#endif // DBINTERFACEIMPL_H
