////////////////////////////////////////////////////////////////////////////////
//
// File Name:	dbmgr.hpp
// Class Name:  CDbMgr
// Description:	用于管理唯一的静态数据库对象，
// Author:		罗志彬
// Date:		2019年4月22日
//
////////////////////////////////////////////////////////////////////////////////

#ifndef QT_DBMGR_HPP
#define QT_DBMGR_HPP

#include "../DbMgr.hpp"
#include "../../Database/LogicImpl/MutestDbLogic.h"
#include "../../Database/DbInterfaceImpl/DbInterfaceImplForQtSqlite.h"
#include <QtSql>

class CQtDbMgr : public CDbMgr<CMutestDbLogic>
{
public:

    // 获得QSqlDatabase对象的引用
    static QSqlDatabase GetQtDatabase()
    {
        // 先拿到静态对象
        auto& db = GetDb();
        // 获得数据库指针
        CDbInterfaceImplForQtSqlite * pDbInterface = (CDbInterfaceImplForQtSqlite*)db.get_db();
        if( pDbInterface )
        {
            return pDbInterface->m_Db;
        }
        return QSqlDatabase();
    }
};


#endif // DBMGR_HPP
