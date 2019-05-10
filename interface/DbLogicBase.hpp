////////////////////////////////////////////////////////////////////////////////
//
// File Name:	DbLogicBase.hpp
// Class Name:  CDbLogic
// Description:	基础数据库逻辑接口类，封装操作数据库表中数据的方法
// Author:		罗志彬
// Date:		2019年4月22日
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DBLOGIC_H
#define DBLOGIC_H

#include "DbInterface.hpp"
#include <QString>
#include <vector>
#include <memory>

class CEntityBase;

#ifndef ENTITY_BASE_MACRO
#define ENTITY_BASE_MACRO

#define CONDITION(l, r) std::make_pair(std::string(l),r)

#endif
class CDbLogic
{
public:
    // 打开数据库
	CDbLogic(std::string strRoamingPath)
		:m_strRoamingPath(strRoamingPath)
	{
	}

	virtual ~CDbLogic()
	{

	}

/////////////// 纯虚函数，平台相关函数，由派生类实现，不允许直接调用
protected:
    // 用于判断数据库是否打开，平台相关
    virtual bool IsDBOpenedImpl() = 0;
    // 用于创建临时目录，平台相关
    virtual bool CreatePathImpl(const std::string& strPath) = 0;
    // 用于拷贝文件，平台相关
    virtual bool CopyFileImpl(const std::string& strSrc,
                                const std::string& strDst) = 0;
    // 用于打开数据库，平台相关，数据库类型相关
    virtual bool OpenDbImpl( const std::string& strPath ) = 0;
    // 用于关闭数据库，平台相关，数据库类型相关
    virtual bool CloseDbImpl() = 0;
    // 用于获取一个uuid，平台相关
    virtual std::string GetUuidImpl() = 0;
    // 用于清除一个目录，平台相关
    virtual bool ClearDirectoryImpl(std::string& strDir, bool bIncludeSelf) = 0;
    // 用于获得另存为的路径，平台相关
    virtual std::string GetSavePathImpl( ) = 0;
    // 用于新建数据库的时候，进行新建处理
    virtual std::string NewDbImpl() = 0;

/////////////// 通用功能函数，
public:

	// 判断数据库是否打开
	inline bool IsDBOpened()
	{
		// 直接返回派生类实现的结果
		return IsDBOpenedImpl();
	}
	
	// 连接数据库
	bool OpenDB(std::__cxx11::string strFile)
	{
		//
		if(strFile.empty())
		{
			m_strLastError = "数据库文件名为空！";
			return false;
		}

		// 关闭数据库
		CloseDB ();

		// 设置属性
		m_strFile = strFile;
		m_strCurrentTempPath = m_strRoamingPath + GetUuidImpl() + "\\";
		m_strCurrentFile = m_strCurrentTempPath + "_current";
		m_strCurrentFileTmp = m_strCurrentFile + "_tmp";
		m_iOperateCurrent = 0;
		m_iOperateMax = 0;
		m_bSaved = true;

		// 创建临时工作目录
		if( ! CreatePathImpl(m_strCurrentTempPath) )
		{
			m_strLastError = "无法创建临时目录！";
			return false;
		}

		// 向临时目录中复制临时文件
		if( ! CopyFileImpl (m_strFile, m_strCurrentFile) )
		{
			m_strLastError = "无法复制临时文件！";
			return false;
		}
		// 打开数据库
		bool bRet = OpenDbImpl(m_strCurrentFile);
		if (!bRet)
		{// 如果打开失败，
			// 记录错误信息并返回
			m_strLastError  = m_db->GetLastError();
			CloseDB ();
		}
		return bRet;
	}

	// 新建数据库文件
	bool NewDB()
	{
		// 关闭数据库
		CloseDB ();

		// 设置属性
		m_strFile = "";
		m_strCurrentTempPath = m_strRoamingPath + GetUuidImpl() + "\\";
		m_strCurrentFile = m_strCurrentTempPath + "_current";
		m_strCurrentFileTmp = m_strCurrentFile + "_tmp";
		m_iOperateCurrent = 0;
		m_iOperateMax = 0;
		m_bSaved = true;

		// 创建临时工作目录
		if( ! CreatePathImpl(m_strCurrentTempPath) )
		{
			m_strLastError = "无法创建临时目录！";
			return false;
		}

		// 获得模板文件的数据库路径
		std::string strTemplateDbFile = NewDbImpl();
		if( strTemplateDbFile.empty() )
		{
			m_strLastError = "新建数据库文件失败";
			return false;
		}

		// 向临时目录中复制临时文件
		if( ! CopyFileImpl (strTemplateDbFile, m_strCurrentFile) )
		{
			m_strLastError = "无法复制临时文件！";
			return false;
		}
		// 打开数据库
		bool bRet = OpenDbImpl(m_strCurrentFile);
		if (!bRet)
		{// 如果打开失败，
			// 记录错误信息并返回
			m_strLastError  = m_db->GetLastError();
			CloseDB ();
		}
		return bRet;
	}

	// 关闭数据库
	void CloseDB()
	{
		if (!IsDBOpened ())
		{// 如果数据库没打开，就直接返回
			return;
		}

		// 调用关闭
		CloseDbImpl ();

		// 删除文件
		ClearDirectoryImpl(m_strCurrentTempPath,true);

		// 重置属性
		m_strFile = ("");
		m_strCurrentTempPath = ("");
		m_strCurrentFile = ("");
		m_strCurrentFileTmp = ("");
		m_iOperateCurrent = 0;
		m_iOperateMax = 0;
		m_bSaved = true;

	}

	// 备份操作
	void BackupOperate()
	{
		// 有修改才调用，所以要把保存标志置为false
		m_bSaved = false;
		// todo 目前不提供备份功能的实现，可以增加
	}

	// 是否可以撤销
	bool CanGoHistory()
	{
		// todo 待需要的时候实现
		return false;
	}

	// 撤销
	void GoHistory()
	{
		// todo 待需要的时候实现
	}

	// 是否可以重做
	bool CanGoForward()
	{
		// todo 待需要的时候实现
		return false;
	}

	// 重做
	void GoForward()
	{
		// todo 待需要的时候实现
	}


	// 保存
	void Save()
	{
		if (!CanSave ())
		{// 如果不需要保存
			return;
		}
		if (m_strFile.empty() )
		{// 文件名为空，说明是新建文件，需要选定一个保存路径
			m_strFile = GetSavePathImpl();
		}
		// 关闭数据库连接，因为数据库文件时独占式打开，所以得关掉才行
		CloseDbImpl();
		// 复制被操作的临时文件到指定路径
		m_bSaved = CopyFileImpl(m_strCurrentFile, m_strFile);
		// 重新打开临时工作文件，
		OpenDbImpl(m_strCurrentFile);
	}

	// 另存为
	bool SaveAs()
	{
		// 选择一个保存路径
		std::string strSavePath = GetSavePathImpl();
		if( strSavePath.empty() )
		{// 如果路径是空的，
			m_strLastError = "所选路径为空!";
			return false;
		}
		return CopyFileImpl(m_strCurrentFile, strSavePath);
	}


   
    //////////////////////////以下实现普通通用数据操作

    // 读取一张数据表数据
    template<typename table_entity, typename ...Condition>
    bool getTable( std::vector<table_entity*>& vctOut,
                   Condition&&... args )
    {
        return table_entity(m_db).getTable(
                    *( reinterpret_cast< std::vector<CEntityBase*> *>(&(vctOut)) )
                    ,args...);
    }

    // 读取第一条f利条件的数据记录
    template<typename table_entity, typename ...Condition>
    std::shared_ptr<table_entity> getRecord(Condition&&... arg )
    {
        table_entity* pEntity = new table_entity( m_db );
        if( pEntity && pEntity->fullfillData(arg...) )
        {
            return std::shared_ptr<table_entity>(pEntity);
        }
        return nullptr;
    }

    // 读取id为指定值的数据
    template<typename table_entity >
    std::shared_ptr<table_entity> getRecord( int iId )
    {
       return getRecord<table_entity>(CONDITION("ID",iId));
    }

/////////////// 内联函数
public:

    // 检查是否可保存
    inline bool CanSave ()
    {
        return !m_bSaved;
    }
    // 获取数据库访问对象指针
    inline CDbInterface *get_db ()
    {
        return m_db;
    }
    // 获取错误信息
    std::string GetErrorMsg()
    {
        return m_strLastError;
    }

///////////// 成员变量
protected:
    // 数据库对象
    CDbInterface *m_db;
    // 通过参数传进来的属性值
    std::string m_strFile;
    // 临时文件目录
    std::string m_strRoamingPath;
    // 当前临时文件目录
    std::string m_strCurrentTempPath;
    // 当前操作的文件路径名
    std::string m_strCurrentFile;
    // 当前操作临时文件路径名
    std::string m_strCurrentFileTmp;
    // 最近一次发生的错误信息
    std::string m_strLastError;

    // 涉及回退和前进的属性值
    // 当前操作步骤标识
    int m_iOperateCurrent;
    // 最大步骤标识
    int m_iOperateMax;
    // 是否保存过
    bool m_bSaved;
};

#endif // DBLOGIC_H
