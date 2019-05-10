#ifndef DATAOBJECTBASE_H
#define DATAOBJECTBASE_H

#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <utility>
#include <algorithm>
#include "DbInterface.hpp"
#include "RecordsetMgr.hpp"
// 定义对象构造函数
#define CONSTRUCT_OBJECT_MODEL(class_name)										\
    public:																		\
    class_name (CDbInterface *pdb = nullptr)	: CEntityBase ( pdb) { init (); }	\ ( pdb) { init (); }

// 定义对象析构函数
#define DESTRUCT_OBJECT_MODEL(class_name)																						\
    public:																														\
    virtual ~class_name () {}

// 定义对象公共方法
#define VIRTUAL_OBJECT_FUNCTION(class_name, class_string)																		\
    private:																													\
    virtual std::string get_table_name () { return (class_string); }															\
    virtual CEntityBase* get_entity (CDbInterface *pdb = nullptr)									\
{ return new class_name ( (pdb ? pdb : m_pdb)); }

// 简化定义
#define ALL_OBJECT_FUNCTION(class_name, class_string)																			\
    CONSTRUCT_OBJECT_MODEL(class_name)																						\
    DESTRUCT_OBJECT_MODEL(class_name)																						\
    VIRTUAL_OBJECT_FUNCTION((class_name), (class_string))


// 定义UUID主键字段名称
#define DEFINE_UUID_COLUMN(uuid_col_name)																			\
    public:																											\
    virtual LPCTSTR get_uuid_column_name () { return uuid_col_name; }

// 定义有序列号
#define DEFINE_SEQUENCE(class_name)																					\
    protected:																										\
    virtual SequenceType get_sequence () { return SequenceType (&class_name::m_nSequence); }

// 定义序列号排序字段
#define DEFINE_SEQUENCE_ORDER(class_name, order_col_name, order_col_value)											\
    protected:																										\
    virtual LPCTSTR get_sequence_order_column_name () { return order_col_name; }								\
    virtual SequenceOrderType get_sequence_order () { return SequenceOrderType (&class_name::order_col_value); }

#define DEFINE_DB_CREATER(class_name )													\
    public:																			\
    static void * creater(){ return new class_name; }							\
    virtual class_name* GetPointerType(){return nullptr ; }


//罗志彬 2017年11月6日
//增加操作以供应通用复制
////////////【begin】///////////////////////////////////////////////////////////////////////////////////////////////
// 简化定义
#define ALL_OBJECT_FUNCTION_WITH_COPYTYPE(class_name, class_string,copytype)												\
    CONSTRUCT_OBJECT_MODEL_WITH_COPYTYPE((class_name),(copytype))															\
    DESTRUCT_OBJECT_MODEL(class_name)																						\
    VIRTUAL_OBJECT_FUNCTION((class_name), (class_string))																	\
    DEFINE_DB_CREATER(class_name)

// 定义对象构造函数
#define CONSTRUCT_OBJECT_MODEL_WITH_COPYTYPE(class_name, copytype)																		\
    public:																																\
    class_name (CDbInterface *pdb = nullptr)	: CEntityBase (pdb,(copytype) ) { init (); m_iId = -1; }

typedef void*(* DbObjectCreaterFunc)(void);

#define DB_ASSIGNMENT_OPERATER_FIELD(value, right)	value = right->value

#ifndef DB_COPYTYPE_DECLARE
#define DB_COPYTYPE_DECLARE

#define DB_COPYTYPE_NONE								("NONE")

#endif

class CEntityBase
{

public:
    CEntityBase(CDbInterface *m_db = nullptr, std::string strCopyType = "")
        :m_pdb(m_db), m_strCopyType(strCopyType)
    {
    }

    virtual ~CEntityBase(){}

    ///////////////////////////////纯虚接口函数，派生类实现
public:
    // 生成实体时，为各个属性初始化值，构造函数中调用
    virtual void init () = 0;
    // 查询的时候 使用查询的结果填充实体对象
    virtual void Produce( CRecordsetMgr* pRecordSet ) = 0;
    // 获取表名
    virtual std::string				get_table_name () = 0;
    // 获得该表对应内存实体
    virtual CEntityBase		*get_entity (CDbInterface *pdb) = 0;
    // 根据表字段生成数据信息相关的sql语句
    virtual std::string GenDataSql(bool bWithFieldName = false ) =0;

    //////////////////////////////功能函数
protected:
    // 生成插入语句
    inline std::string GenInsertSQL()
    {
        // 插入语句的实例 insert into TABLE_NAME values( val1, val2, val3 );
        // 所以这里GenDataSql函数参数为false ，不带字段名称
        std::string strSql = "insert into "+ get_table_name()
                +" values(null," + GenDataSql(false) + ")";
        return strSql;
    }
    // 生成更新语句
    inline std::string GenUpdateSQL()
    {
        // 更新语句的实例 update TABLE_NAME　set col1=val1, col2=val2 where id = 1;
        // 所以这里GenDataSql函数参数为true， 带字段名称
        std::string strSql = "update "+ get_table_name() +
                " set "+ GenDataSql(true) +
                " where id = " + std::to_string(m_iId);
        return strSql;
    }
    // 生成删除语句
    inline std::string GenDeleteSQL()
    {
        std::string strSql = "delete from " + get_table_name() +
                " where id = "  + std::to_string(m_iId);
        return strSql;
    }
    // 初始化值
    void init_value(std::string &val)
    {
        val = "";
    }
    void init_value(int &val)
    {
        val = 0;
    }
    void init_value(double &val)
    {
        val = 0.0;
    }
    void init_value(bool &val)
    {
        val =false;
    }


    template< typename T>
    std::string init_sql (std::string col_name, const T &val,bool with_field_name = false )
    {
        std::string strTmp;
        if( with_field_name )
        {
            strTmp = " " + col_name + " = "+std::to_string(val)+" ";
        }
        else
        {
            strTmp = " "+std::to_string(val)+" ";
        }
        return strTmp;
    }
    std::string init_sql(std::string col_name, const std::string &val, bool with_field_name = false)
    {
        std::string strTmp;
        if( with_field_name )
        {
            strTmp = " " + col_name + " = "+val+" ";
        }
        else
        {
            strTmp = " "+val+" ";
        }
        return strTmp;
    }
    std::string init_sql (std::string col_name, bool &val,bool with_field_name = false)
    {
        return init_sql(col_name,(val? 1:0),with_field_name);
    }


public:
    // 获得该条记录的id主键
    inline int GetRecordId()
    {
        return m_iId;
    }

    // 设置数据库
    inline void set_db (CDbInterface *pdb)
    {
        m_pdb = pdb;
    }

    // 通用赋值操作符
    virtual CEntityBase& operator=(CEntityBase &o)
    {
        m_pdb = o.m_pdb;
        return *this;
    }


    //////////////////////////////私有属性
protected:
    // 数据库连接指针
    CDbInterface* m_pdb;

    // 复制类型，保留使用，
    std::string m_strCopyType;

    // 数据表的主键，id
    int m_iId;

    /////////////////////////////数据操作
public:

    // 构建查询字符串
    // 模板函数，完成 int double float long等内置类型的拼接
    template< typename second_type>
    std::string init_query_sql( std::pair<std::string, second_type >& pair )
    {
        return pair.first +" = " + std::to_string(pair.second);
    }

    template< typename second_type>
    std::string init_query_sql( std::pair<std::string, second_type >&& pair )
    {
        return pair.first +" = " + std::to_string(pair.second);
    }

    std::string init_query_sql( std::pair<std::string, const char* >& pair )
    {
        return pair.first +" = " + std::string(pair.second);
    }

    // string 类型特化版本，完成string类型的拼接
    std:: string init_query_sql(std::pair<std::string, std::string >& pair )
    {
        return pair.first +" = " + pair.second;
    }

    // 可变参数查询函数
    template<typename ...Condition>
    bool getTable(std::vector<CEntityBase*> &v, Condition&&... args )
    {
        // 判断数据库是否可用
        if(!m_pdb || !m_pdb->isDbOpen() )
        {
            return false;
        }

        // 构建查询字符串
        std::string strSql = "";

        // 初始化列表构造查询条件
        std::initializer_list<std::string> initlstSql
                = {(init_query_sql(std::forward<Condition>(args)))...};

        // 开始生成sql语句
        std::string sql = "select * from " + get_table_name();

        // 遍历变参列表
        for( auto lit = initlstSql.begin();
             lit != initlstSql.end();
             lit ++ )
        {
            if( lit == initlstSql.begin() )
            {// 如果是第一个条件，加上where
                sql += " where ";
            }
            else
            {// 如果不是，加上and
                sql += " and " ;
            }
            sql += (*lit);
        }

        // 调用数据库操作执行查询
        return m_pdb->select(sql, std::bind(&CEntityBase::get_entity,this,m_pdb), v);
    }


    bool fullfillData( int iId )
    {
        return fullfillData(std::make_pair(std::string("id"),iId) );
    }

    template<typename ...Condition>
    bool fullfillData( Condition&&... args )
    {
        // 判断数据库是否可用
        if(!m_pdb || !m_pdb->isDbOpen() )
        {
            return false;
        }

        // 构建查询字符串
        std::string strSql = "";

        // 初始化列表构造查询条件
        std::initializer_list<std::string> initlstSql
                = {(init_query_sql(std::forward<Condition>(args)))...};

        // 开始生成sql语句
        std::string sql = "select * from " + get_table_name();

        // 遍历变参列表
        for( auto lit = initlstSql.begin();
             lit != initlstSql.end();
             lit ++ )
        {
            if( lit == initlstSql.begin() )
            {// 如果是第一个条件，加上where
                sql += " where ";
            }
            else
            {// 如果不是，加上and
                sql += " and " ;
            }
            sql += (*lit);
        }
        return  m_pdb->select_single(sql,this);
    }

    //    // 读取整张表，追加到存储结构体
    //    bool get_table_append (std::vector<CEntityBase*> &v, std::string col_name = "", std::string col_value = "", std::string col_name2 = "", std::string col_value2 ="" );


    //************************************************************************************************************
    // 函数名称	: modify_self
    // 访问权限	: public
    // 功能说明	: 将自身记录的数据同步到数据表中
    // 返回值	: bool
    // 参数列表	:
    // 创建日期	: 2019年5月7日
    // 备注		:
    //************************************************************************************************************
    bool modify_self()
    {
        // 检测一下数据库连接是否有效
        if( !m_pdb )
        {
            return false;
        }

        // 获得更新语句
        std::string strUpdateSql = GenUpdateSQL() ;
        // 将语句转换成UTF-8编码
        // 调用接口语句执行
        return m_pdb->Update( strUpdateSql );
    }


    //************************************************************************************************************
    // 函数名称	: delete_self
    // 访问权限	: public
    // 功能说明	: 删除自身所代表的数据
    // 返回值	: bool
    // 参数列表	:
    // 创建日期	: 2019/02/20
    // 备注		:
    //************************************************************************************************************
    bool delete_self()
    {
        // 检测一下数据库连接是否有效
        if( !m_pdb )
        {
            return false;
        }

        // 获得删除语句
        std::string strUpdateSql  = GenDeleteSQL() ;
        // 调用接口执行
        return m_pdb->Delete( strUpdateSql );
    }



    // 数据库表记录的id被强行设置为必带和自增长，所以这里可以获得某张表下一个id的值
    // 这个值由sqlite自己维护，查询使用就行
    int get_next_auto_id()
    {
        if( !m_pdb )
        {
            return -1;
        }
        std::string strSql;
        // sqlite3 中设置了sqlite_sequence表记录各张表的自增长字段的下一个值，直接从这张表中就能查出来

        // 构造语句
        strSql = " select seq from sqlite_sequence where name = '" + get_table_name()+"' ";

        // 调用数据库接口函数查询
        std::string strValue = m_pdb->select(strSql);
        if( strValue.empty())
        {// 如果没查到，就返回-1
            return -1;
        }
        else
        {// 查到了，转成整型返回去
            return atoi(strValue.c_str());
        }
    }


    //************************************************************************************************************
    // 函数名称	: add_item
    // 访问权限	: public
    // 功能说明	: 将自身所记录的数据插入到数据库中
    // 返回值	: int 新纪录的id值
    // 参数列表	:
    // 创建日期	: 2019/02/20
    // 备注		:
    //************************************************************************************************************
    int add_item()
    {
        // 检测数据库连接
        if( !m_pdb )
        {
            return -1;
        }

        // 获得对应表的下一个id值，这个值将被新增记录所使用
        int iNextId = get_next_auto_id();

        if( iNextId <= 0 )
        {// 如果这个id比0小了，说明获得的不对
            return -1;
        }

        // 调用数据库连接插入数据
        if( m_pdb->Insert(GenInsertSQL()) )
        {//如果成功了
            // 记录id值
            m_iId = iNextId;
        }
        else
        {// 失败了
            // id值记录为-1
            m_iId = -1;
        }

        // 将id值返回，所以这个函数如果返回-1，说明插入失败
        return m_iId;
    }


};



#endif // DATAOBJECTBASE_H
