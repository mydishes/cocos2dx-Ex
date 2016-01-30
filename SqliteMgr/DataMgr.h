//******************************************************************
// FILENAME:	DataMgr.h
// AUTHOR  :	Steaphan
// VERSION :	1.0.0
// DESCRIBE:	sqlite数据管理
//******************************************************************
#ifndef __DATAMGR_H_
#define __DATAMGR_H_
#include <map>
#include <vector>
#include "sqlite3/include/sqlite3.h"
enum class enDataType
{
	SOUND,				// 声音数据    0x01 音乐  0x02 音效	(1:开启,0：关闭)
	PLAYERID,			// 上次GAMECENTER玩家数据
	ISGUIDED,			// 是否已经引导过
	LASTTIME,			// 上次时间


	PLAYER_INDEX,		// 每个的索引 
	MONEY,				// 金币
	RECORD,				// 记录
};

// 数据表格类型
enum class enSqlTableType
{
	DEFAULT,	// 默认数据存储 声音等
	PLAYER,		// 玩家数据存储 金币
};

// 
enum class enSqlDataType
{
	NONE,		// 没有数据
	INT,		// int类型
	LONG,		// long类型
	BOOL,		// bool 类型
	STRING,		// string 类型
};


struct SSqlData
{
public:
	SSqlData()
	{
		memset(&udata, 0, sizeof(udata));
	}
	SSqlData(enSqlTableType ntable, enSqlDataType ntype, std::string name)
		:nSqlTableType(ntable), nSqlDataType(ntype), sSqlDataName(name)
	{
		memset(&udata, 0, sizeof(udata));
	}

	// 实际数据
	union UData
	{
		int			_intData;
		long		_longData;
		bool		_boolData;
	};
	UData		udata;
	std::string _strData;

	enSqlTableType	nSqlTableType;	// 数据表格
	enSqlDataType	nSqlDataType;   // 数据库中数据类型
	std::string		sSqlDataName;	// 数据库中名字
};


class CDataMgr
{
public:
	static CDataMgr* getInstance();
	
	static void destroyInstance();

	const SSqlData&	GetData(enDataType nType){ return m_mKeyValue[nType]; }

	void				SetData(enDataType nType, SSqlData& data);
private:
	void				GetSqlData(enSqlTableType nSqlType, std::vector<enDataType>& vDataType);

	void				SaveSqlTable(enSqlTableType nSqlType);

	void				OpenSql(enSqlTableType nType);		// 打开数据库操作

	void				CloseSql();		// 关闭数据库操作
	 
	  
	CDataMgr();
	~CDataMgr();
	 
	 
	std::map<enDataType, SSqlData>		m_mKeyValue;			// 程序当前的值
	sqlite3 *							 m_pSqlData;			// 数据库指针
	 
}; 
#endif