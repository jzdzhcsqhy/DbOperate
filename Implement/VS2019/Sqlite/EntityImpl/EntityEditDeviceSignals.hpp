﻿////////////////////////////////////////////////////////////////////////////////
//
// File Name:	EntityEditDeviceSignals.hpp
// Class Name:	CEntityEditDeviceSignals
// Description:	用于数据库对象的示例
// Author:		罗志彬
// Date:		2019年2月20日
//
////////////////////////////////////////////////////////////////////////////////

#ifndef EDIT_DEVICE_SIGNAL_TABLE_HPP
#define EDIT_DEVICE_SIGNAL_TABLE_HPP

#pragma once
#include "../Common/DbCommonHeader.h"
#include "../Common/EntityBase.h"

class CEntityEditDeviceSignals : public CEntityBase
{
////////////////////////////第一部分 用一个宏来写这个类的基础函数支持，具体如下
	// 必带宏， 生成构造析构函数，必要的虚函数实现，等等
	// 第一个参数是这个类名，
	// 第二个参数是对应的数据库中的表明，大小写不敏感
	// 第三个参数是如果支持复制的话，此数据对应的复制类型，现在可以不用，所以暂时写成NONE
    ALL_OBJECT_FUNCTION_WITH_COPYTYPE (CEntityEditDeviceSignals, ("EDIT_DEVICE_SIGNAL"),DB_COPYTYPE_NONE)

///////////////////////////第二部分，声明本表字段，这些字段和数据库字段顺序要对应，不然可能出错
public:
    std::string		m_strName;				// 测试动作的UUID，唯一标识一个测试动作
    std::string		m_strType;				// 测试动作名称
	bool			m_bIsChecked;			// ATML_ACTION使用的ID

//////////////////////////第三部分，必须实现的几个虚函数
protected:
	// 初始化类成员，构造函数中调用，顺序可以随意，可以不初始化m_iId,但是写了也无妨
	virtual void init ()
	{
        init_value (m_iId);
        init_value (m_strName);
        init_value (m_strType);
        init_value (m_bIsChecked);
    }


    // 这个函数用来将数据从结果集中读出，并且存入对应的变量里，
	// 注意顺序必须与数据库表中的一致，必须获得m_iId
    virtual void Produce( CRecordsetMgr* pRecordSet )
    {
        if( !pRecordSet )
        {
            return ;
        }
        int iIndex = 0;
        pRecordSet->get_value(iIndex++,m_iId);
        pRecordSet->get_value(iIndex++, m_strName);
        pRecordSet->get_value(iIndex++, m_strType);
        pRecordSet->get_value(iIndex++, m_bIsChecked);
    }

	// 将数据组织成字符串，供各种语句使用，
	// 用这张表举例子，插入语句要写成 insert into EDIT_DEVICE_SIGNAL values( null, 'name1', 'type1', true);
	// 更新语句要写成 update EDIT_DEVICE_SIGNAL set name='name2', type='type2', is_checked=false where id = 1;
	// 所以参数为true时，会带有字段名，用逗号分隔，供update使用 name='name2', type='type2', is_checked=false
	// 参数为false是，不带字段名，逗号分隔，供insert使用 'name1', 'type1', true
    virtual std::string GenDataSql(bool bWithFieldName = false )
	{
		// 调用连接字符串，将各个数据连接起来
		// 这是一个变参函数，使用时注意，
        return CCommonString::joinString<std::string>(
            ",",// 分隔符，使用逗号， 不要修改
			false,// 如果为空是否要连接， 不要修改
			//////////////////////////////////////////////////////新增的表从这里开始改
			3, //需要连接的字符串个数，不包括id， 有几个就写几个，这个有三个，所以是3
			// 上一行写的是几，从这里开始，就要有几个，不然就会出错
			// 这里的顺序必须按照数据库中字段顺序来写，不然insert语句会出错
            init_sql(("NAME"),m_strName,bWithFieldName),
            init_sql(("TYPE"),m_strType,bWithFieldName),
            init_sql(("IS_CHECKED"),m_bIsChecked,bWithFieldName)
			);
	}

/////////////////////////////////第四部分，赋值操作符，
public:
	// 复制记录
	virtual CEntityBase &operator= (CEntityEditDeviceSignals &o)
	{
		CEntityEditDeviceSignals *po = dynamic_cast<CEntityEditDeviceSignals*>(&o);
		if (po)
		{
			//////////////////////这个函数只在这里修改就行了，顺序无所谓
			DB_ASSIGNMENT_OPERATER_FIELD(m_strName, po);
			DB_ASSIGNMENT_OPERATER_FIELD(m_strType, po);
			DB_ASSIGNMENT_OPERATER_FIELD(m_bIsChecked, po);
		}

		return CEntityBase::operator= (o);
	}
};

#endif // __EDIT_DEVICE_SIGNAL_TABLE_HPP__

