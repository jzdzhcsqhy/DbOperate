#ifndef RECORDSETMGR_H
#define RECORDSETMGR_H
#include <string>
class CRecordsetMgr
{
public:
    CRecordsetMgr(){}
    virtual ~CRecordsetMgr(){}
public:

    virtual void get_value( int iColIndex, int& val ) = 0;
    virtual void get_value( int iColIndex, double& val ) = 0;
    virtual void get_value( int iColIndex, bool& val ) = 0;
    virtual void get_value( int iColIndex, std::string& val ) = 0;
};



#endif // RECORDSETMGR_H
