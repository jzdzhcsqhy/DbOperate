#ifndef MUTESTDBLOGIC_H
#define MUTESTDBLOGIC_H

#include "DbLogicBase.hpp"

class CMutestDbLogic : public CDbLogic
{
public:
    CMutestDbLogic();
    virtual ~CMutestDbLogic();


//////////////////////派生类需要实现的接口函数
// 这些函数都是平台相关的，但是在基类中需要使用的，通过接口的形式来让派生类实现
protected:
    // 用于判断数据库是否打开，平台相关
    virtual bool IsDBOpenedImpl() ;

    // 用于创建临时目录，平台相关
    virtual bool CreatePathImpl(const std::string& strPath) ;

    // 用于拷贝文件，平台相关
    virtual bool CopyFileImpl(const std::string& strSrc,
                              const std::string& strDst) ;
    // 用于打开数据库，平台相关，数据库类型相关
    virtual bool OpenDbImpl( const std::string& strPath ) ;

    // 用于关闭数据库，平台相关，数据库类型相关
    virtual bool CloseDbImpl();

    // 用于获取一个uuid，平台相关
    virtual std::string GetUuidImpl() ;

    // 用于清除一个目录，平台相关
    virtual bool ClearDirectoryImpl(std::string& strDir, bool bIncludeSelf) ;

    // 用于获得另存为的路径，平台相关
    virtual std::string GetSavePathImpl( ) ;

    // 用于新建数据库的时候，进行新建处理
    virtual std::string NewDbImpl();

    //
    virtual std::string GetAppPathImpl();
///////////////////////以下是个人见解
private:
    // 实现私有赋值操作符和拷贝构造函数，
    // 此类在全部范围内只允许有一个实例，所以只能使用指针或者引用，拒绝任何赋值类型的操作
    // 私有赋值操作符，拒绝赋值
    CMutestDbLogic& operator=(const CMutestDbLogic& r )
    {
        //Q_UNUSED(r);
        return *this;
    }
    // 私有拷贝构造函数，拒绝拷贝构造
    CMutestDbLogic( CMutestDbLogic& r ) : CDbLogic()
    {
        //Q_UNUSED(r);
    }
};

#endif // MUTESTDBLOGIC_H
