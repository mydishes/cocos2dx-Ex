#include "DataMgr.h"
#include "cocos2d.h"
#include <string>
#include <vector>
static CDataMgr *s_SharedDataMgr = nullptr;

#define SQL_NAME			"local_sql_1.db"		// 数据库
#define DEFAULT_TABLE_NAME  "default_db"		// 默认表
#define PLAYER_TABLE_NAME   "player_db"			// 玩家表
static char FORMAT_BUFFER[1024] = { 0 };		// 输出格式缓存区


CDataMgr* CDataMgr::getInstance()
{
	if (!s_SharedDataMgr)
	{
		s_SharedDataMgr = new CDataMgr();
	}
	return s_SharedDataMgr;
}
void CDataMgr::destroyInstance()
{
	if (s_SharedDataMgr)
	{
		delete s_SharedDataMgr;
		s_SharedDataMgr = nullptr;
	}
}

CDataMgr::CDataMgr() :m_pSqlData(0)
{
	// 初始化key-value
	m_mKeyValue[enDataType::SOUND] = SSqlData(enSqlTableType::DEFAULT, enSqlDataType::INT, "sound");
	m_mKeyValue[enDataType::PLAYERID] = SSqlData(enSqlTableType::DEFAULT, enSqlDataType::STRING, "playerId");
	m_mKeyValue[enDataType::ISGUIDED] = SSqlData(enSqlTableType::DEFAULT, enSqlDataType::BOOL, "isGuided");
	m_mKeyValue[enDataType::LASTTIME] = SSqlData(enSqlTableType::DEFAULT, enSqlDataType::LONG, "lastTime");

	m_mKeyValue[enDataType::MONEY] = SSqlData(enSqlTableType::PLAYER, enSqlDataType::LONG, "money");
	m_mKeyValue[enDataType::RECORD] = SSqlData(enSqlTableType::PLAYER, enSqlDataType::LONG, "record");
	m_mKeyValue[enDataType::PLAYER_INDEX] = SSqlData(enSqlTableType::PLAYER, enSqlDataType::STRING, "playerId");


	std::vector<enDataType> vDataType;
	vDataType.push_back(enDataType::PLAYERID);
	vDataType.push_back(enDataType::SOUND);
	vDataType.push_back(enDataType::LASTTIME);
	vDataType.push_back(enDataType::ISGUIDED);
	GetSqlData(enSqlTableType::DEFAULT, vDataType); // 获取数据库记录

	vDataType.clear();
	vDataType.push_back(enDataType::MONEY);
	vDataType.push_back(enDataType::RECORD);
	GetSqlData(enSqlTableType::PLAYER, vDataType); // 获取数据库记录
}


CDataMgr::~CDataMgr()
{
	SaveSqlTable(enSqlTableType::DEFAULT);
	SaveSqlTable(enSqlTableType::PLAYER);
}
 
void CDataMgr::OpenSql(enSqlTableType nType)
{
	if (m_pSqlData)
	{
		CloseSql();
	}
	std::string fullPath = cocos2d::FileUtils::getInstance()->getWritablePath() + SQL_NAME;
	if (SQLITE_OK != sqlite3_open(fullPath.c_str(), &m_pSqlData))
	{
		CCLOG("Open the Game Data Table falied\n");
		m_pSqlData = 0;
		return;
	}
	// 输入数据类型定义
	std::string sql_param_defs = "";
	memset(&FORMAT_BUFFER, 0, sizeof(FORMAT_BUFFER));
	auto it = m_mKeyValue.begin();
	while (it != m_mKeyValue.end())
	{
		SSqlData& data = it->second;
		if (nType == data.nSqlTableType)
		{
			std::string sql_type = "";
			switch (data.nSqlDataType)
			{
			case enSqlDataType::INT:
				sql_type = "int unsigned";
				break;
			case enSqlDataType::LONG:
				sql_type = "long unsigned";
				break;
			case enSqlDataType::BOOL:
				sql_type = "int unsigned";
				break;
			case enSqlDataType::STRING:
				sql_type = "varchar(50)";
				break;
			case enSqlDataType::NONE:
			default:
				continue;
			}
			sprintf(FORMAT_BUFFER, ",%s %s", data.sSqlDataName.c_str(), sql_type.c_str());
			sql_param_defs += FORMAT_BUFFER;
		}
		++it;
	}

	switch (nType)
	{
	case enSqlTableType::DEFAULT:
		sprintf(FORMAT_BUFFER, "create table if not exists %s (id integer primary key autoincrement  %s)", DEFAULT_TABLE_NAME, sql_param_defs.c_str());
		break;
	case enSqlTableType::PLAYER:
		sprintf(FORMAT_BUFFER, "create table if not exists %s (id integer primary key autoincrement  %s)", PLAYER_TABLE_NAME, sql_param_defs.c_str());
		break;
	default:
		return;
	}
	char *errMsg = 0;
	int info = sqlite3_exec(m_pSqlData, FORMAT_BUFFER, NULL, NULL, &errMsg); // create a table
	if (info != SQLITE_OK)
	{
		CCLOG("Create the Data table failed %s\n", errMsg);
		sqlite3_free(errMsg);
	}
}

void CDataMgr::CloseSql()
{
	if (m_pSqlData)
	{
		sqlite3_close(m_pSqlData);
		m_pSqlData = 0;
	}
}
 
void CDataMgr::GetSqlData(enSqlTableType nSqlType, std::vector<enDataType>& vDataType)
{
	if (vDataType.size() <= 0)
	{
		return;
	}
	// 输入查询数据
	std::string sql_params = "";
	bool isFirst = true;
	for (size_t i = 0; i < vDataType.size(); i++)
	{
		if (m_mKeyValue.find(vDataType[i]) != m_mKeyValue.end())
		{
			SSqlData& data = m_mKeyValue[vDataType[i]];
			sql_params += isFirst ? data.sSqlDataName : ("," + data.sSqlDataName);
			isFirst = false;
		}
	}
	
	switch (nSqlType)
	{
	case enSqlTableType::DEFAULT:
		sprintf(FORMAT_BUFFER, "select %s from %s", sql_params.c_str(), DEFAULT_TABLE_NAME);
		break;
	case enSqlTableType::PLAYER:
		sprintf(FORMAT_BUFFER, "select %s from %s where  %s = '%s'  ", sql_params.c_str(), PLAYER_TABLE_NAME, m_mKeyValue[enDataType::PLAYER_INDEX].sSqlDataName.c_str(), m_mKeyValue[enDataType::PLAYERID]._strData.c_str());
		break;
	default:
		return;
	}
	std::string sql_search = FORMAT_BUFFER;
	OpenSql(nSqlType);
	if (!m_pSqlData)
	{
		return;
	}
	sqlite3_stmt* pStmt = NULL;
	int info = sqlite3_prepare(m_pSqlData, sql_search.c_str(), -1, &pStmt, NULL);
	if (info == SQLITE_OK)
	{
		info = sqlite3_step(pStmt);
		bool res = info == SQLITE_ROW;
		{
			int nIndex = 0;
			for (size_t i = 0; i < vDataType.size(); i++)
			{
				if (m_mKeyValue.find(vDataType[i]) != m_mKeyValue.end())
				{
					SSqlData& data = m_mKeyValue[vDataType[i]];
					switch (data.nSqlDataType)
					{
					case enSqlDataType::INT:
						data.udata._intData = res?sqlite3_column_int(pStmt, nIndex):0;
						break;
					case enSqlDataType::LONG:
						data.udata._longData = res?sqlite3_column_int64(pStmt, nIndex):0;
						break;
					case enSqlDataType::BOOL:
						data.udata._boolData = res?sqlite3_column_int(pStmt, nIndex)>0:false;
						break;
					case enSqlDataType::STRING:
					{
						data._strData = "";
						if (res)
						{
							const unsigned char* c = sqlite3_column_text(pStmt, nIndex);
							if (c)
							{
								data._strData = (char*)(c);
							}
						}
					}
						break;
					case enSqlDataType::NONE:
					default:
						break;
					}
					++nIndex;
				} 
			}
		}
		sqlite3_finalize(pStmt);
	} 
	CloseSql();
}

void CDataMgr::SaveSqlTable(enSqlTableType nSqlType)
{
	std::string sTalbeName = "";
	switch (nSqlType)
	{
	case enSqlTableType::DEFAULT:
		sTalbeName = DEFAULT_TABLE_NAME;
		sprintf(FORMAT_BUFFER, "select * from %s", sTalbeName.c_str());
		break;
	case enSqlTableType::PLAYER:
	{
		if (m_mKeyValue.find(enDataType::PLAYER_INDEX) == m_mKeyValue.end())
		{
			return;
		}
		sTalbeName = PLAYER_TABLE_NAME;
		sprintf(FORMAT_BUFFER, "select * from %s where  %s = '%s' ", sTalbeName.c_str(), m_mKeyValue[enDataType::PLAYER_INDEX].sSqlDataName.c_str(), m_mKeyValue[enDataType::PLAYERID]._strData.c_str());
	}
		break;
	default:
		return;
	}
	std::string save_sql = FORMAT_BUFFER;
	OpenSql(nSqlType);
	if (!m_pSqlData)
	{
		return;
	}
	sqlite3_stmt* pStmt = NULL;
	int info = sqlite3_prepare(m_pSqlData, save_sql.c_str(), -1, &pStmt, NULL);
	if (info == SQLITE_OK)
	{
		info = sqlite3_step(pStmt);
		sqlite3_finalize(pStmt);
		memset(&FORMAT_BUFFER, 0, sizeof(FORMAT_BUFFER));
		if (info == SQLITE_ROW)
		{
			// 输入数据类型定义
			bool isFirstData = true;
			std::string sql_params = ""; 
			auto it = m_mKeyValue.begin();
			while (it != m_mKeyValue.end())
			{
				SSqlData& data = it->second;
				if (nSqlType == data.nSqlTableType)
				{
					switch (data.nSqlDataType)
					{
					case enSqlDataType::INT:
						sprintf(FORMAT_BUFFER, "%d", data.udata._intData); 
						break;
					case enSqlDataType::LONG:
						sprintf(FORMAT_BUFFER, "%ld", data.udata._longData);
						break;
					case enSqlDataType::BOOL:
						sprintf(FORMAT_BUFFER, "%d", data.udata._boolData ? 1 : 0);
						break;
					case enSqlDataType::STRING:
						sprintf(FORMAT_BUFFER, "'%s'", data._strData.c_str());
						break;
					case enSqlDataType::NONE:
					default:
						continue;
					}
					if (isFirstData)
					{
						sql_params += data.sSqlDataName + " = " +  std::string(FORMAT_BUFFER);
						isFirstData = false;
					}
					else
					{
						sql_params += "," + data.sSqlDataName + " = " + std::string(FORMAT_BUFFER);
					}
				}
				++it;
			}

			switch (nSqlType)
			{
			case enSqlTableType::DEFAULT:
				sprintf(FORMAT_BUFFER, "update %s set %s ", sTalbeName.c_str(), sql_params.c_str());
				break;
			case enSqlTableType::PLAYER:
				sprintf(FORMAT_BUFFER, "update %s set %s where %s = '%s'", sTalbeName.c_str(), sql_params.c_str(), m_mKeyValue[enDataType::PLAYER_INDEX].sSqlDataName.c_str(), m_mKeyValue[enDataType::PLAYERID]._strData.c_str());
				break;
			default:
				return;
			}
		}
		else
		{
			m_mKeyValue[enDataType::PLAYER_INDEX]._strData = m_mKeyValue[enDataType::PLAYERID]._strData;
			// 输入数据类型定义
			bool isFirstData = true;
			std::string sql_params = "";
			std::string sql_types = "";
			auto it = m_mKeyValue.begin();
			while (it != m_mKeyValue.end())
			{
				SSqlData& data = it->second;
				if (nSqlType == data.nSqlTableType)
				{
					switch (data.nSqlDataType)
					{
					case enSqlDataType::INT:
						sprintf(FORMAT_BUFFER, "%d",data.udata._intData);
						break;
					case enSqlDataType::LONG:
						sprintf(FORMAT_BUFFER, "%ld", data.udata._longData);
						break;
					case enSqlDataType::BOOL:
						sprintf(FORMAT_BUFFER, "%d", data.udata._boolData ? 1 : 0);
						break;
					case enSqlDataType::STRING:
						sprintf(FORMAT_BUFFER, "'%s'", data._strData.c_str());
						break;
					case enSqlDataType::NONE:
					default:
						continue;
					}
					if (isFirstData)
					{
						sql_params += data.sSqlDataName;
						sql_types += FORMAT_BUFFER;
						isFirstData = false;
					}
					else
					{
						sql_params += "," + data.sSqlDataName;
						sql_types += "," + std::string(FORMAT_BUFFER);
					}
				}
				++it;
			}
			sprintf(FORMAT_BUFFER, "insert into %s (%s)values (%s)",
				sTalbeName.c_str(), sql_params.c_str(), sql_types.c_str());
		}
		char *errMsg = 0;
		int info = sqlite3_exec(m_pSqlData, FORMAT_BUFFER, NULL, NULL, &errMsg);
		if (info != SQLITE_OK)
		{
			CCLOG("update the Data table failed %s\n", errMsg);
			sqlite3_free(errMsg);
		}
	}
	CloseSql();
}


void CDataMgr::SetData(enDataType nType, SSqlData& data)
{
	// 玩家角色变化更新玩家数据
	if (enDataType::PLAYERID == nType)
	{
		SaveSqlTable(enSqlTableType::PLAYER);
		m_mKeyValue[nType] = data;
		std::vector<enDataType> vDataType;
		vDataType.push_back(enDataType::MONEY);
		vDataType.push_back(enDataType::RECORD);
		GetSqlData(enSqlTableType::PLAYER, vDataType);
	}
	else
	{
		m_mKeyValue[nType] = data;
	}
}
