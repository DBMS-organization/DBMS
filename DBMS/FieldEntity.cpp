#include "pch.h"
#include "FieldEntity.h"

//初始化字段实体
CFieldEntity::CFieldEntity(CString fieldname)
{
    this->fieldName = fieldName;
    SYSTEMTIME time;
    ::GetLocalTime(&time);
    CTime t(time);
    this->modifyTime = t.Format("%Y-%m-%d %H:%M:%S");
    //SetFieldName(fieldName);
    /*this->fieldOrder = field_order;
    this->fieldName = field_name;
    this->fieldType = field_type;
    this->fieldParam = field_param;
    this->modifyTime = modifytime;
    this->Integrities = integrities;*/
}

CFieldEntity::~CFieldEntity(void)
{
}

//设置约束条件（主键，非空，唯一）
void CFieldEntity::SetIntegrities(bool primary, bool unique, bool notnull)
{
    this->Primary = primary;
    this->Unique = unique;
    this->NotNull = notnull;
}



