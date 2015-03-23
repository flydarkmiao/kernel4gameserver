#include "ObjectMgr.h"
#include "Object.h"
#include "AttributeMgr.h"
#include "Tools.h"
#include "CData.h"
#include <algorithm>
#include "CArgs.h"

string GenerateRandomString(size_t nLen) {
	static char szStr[1024];
	memset(szStr, 0, sizeof(szStr));
	for (size_t i = 0; i < nLen; ++i) {
		char cIndex = rand() % 52;
		char c = cIndex + 'a';
		if (cIndex >= 26)
			c = cIndex - 26 + 'A';

		szStr[i] = c;
	}
	return string(szStr, nLen);
}

void* GenerateRandomBlob(size_t nBlobLen) {
	char* pBlob = NEW char[nBlobLen];
	memset(pBlob, 0, nBlobLen);
	for (size_t i = 0; i < nBlobLen; ++i) {
		pBlob[i] = rand() % 256;
	}

	return pBlob;
}

bool ObjectMgr::Initialize(IKernel * pKernel) {
	bool res = CAttributeMgr::Instance()->LoadAllConfig(string(tools::GetAppPath()) + "/logic_config/ObjectMgr/AttribConfig.xml");
	TASSERT(res, "Load logic_config/AttribConfig.xml error");
	
	return res;
}

bool ObjectMgr::Launched(IKernel * pKernel) {

	return true;
}

bool ObjectMgr::Destroy(IKernel * pKernel) {

	return true;
}

objecthandle ObjectMgr::CreateObject(const string& strObjectType) {
	Object* pObj = NEW Object;
	if (!pObj || !pObj->SetType(strObjectType.c_str())) {
		if (pObj)
			delete pObj;

		return objecthandle(0, 0);
	}

	s16 nType = GetObjType(strObjectType);

	objecthandle objId = ObjectIDFactory::getInstance()->FetchID(nType);
	if (!pObj->SetAttr("object_id", objId.objectID)) {
		delete pObj;
		return objecthandle(0, 0);
	}

	m_mapObject[objId.objectID] = pObj;

	return objId;
}

bool ObjectMgr::CreateObject(const string& strObjectType, const objecthandle& objId) {
	Object* pObj = NEW Object;
	if (!pObj || !pObj->SetType(strObjectType.c_str()) || !pObj->SetAttr("object_id", objId.objectID)) {
		if (pObj)
			delete pObj;

		return false;
	}
	m_mapObject[objId.objectID] = pObj;
	return true;
}

bool ObjectMgr::DestroyObject(const objecthandle& hd)
{
	map<s64, Object*>::iterator itr = m_mapObject.find(hd.objectID);
	if (itr == m_mapObject.end())
		return false;

	delete itr->second;

	m_mapObject.erase(itr);
	return true;
}

bool ObjectMgr::Exists(const objecthandle& hd) {
	return FindObject(hd) != NULL;
}

s16 ObjectMgr::GetObjType(const string& strObjType) {
	return CAttributeMgr::Instance()->GetObjType(strObjType);
}

s8 ObjectMgr::GetAttrType(const objecthandle& objhd, const char* szAttrName) {
	Object* pObj = FindObject(objhd);
	if (pObj)
		return pObj->GetAttrType(szAttrName);

	return DATA_TYPE_INVALID;
}

size_t ObjectMgr::GetAttrLength(const objecthandle& objhd, const char* szAttrName) {
	Object* pObj = FindObject(objhd);
	if (pObj)
		return pObj->GetAttrLength(szAttrName);
	return 0;
}

void ObjectMgr::GetAllAttrs(const objecthandle& objhd, vector<string>& vAttrs) {
	Object* pObj = FindObject(objhd);
	if (pObj)
		pObj->GetAllAttrs(vAttrs);
}

void ObjectMgr::GetSelfAttrs(const objecthandle& objhd, vector<string>& vAttrs) {
	Object* pObj = FindObject(objhd);
	if (pObj)
		pObj->GetSelfAttrs(vAttrs);
}

Object* ObjectMgr::FindObject(const objecthandle& objh) {
	map<s64, Object*>::iterator itr = m_mapObject.find(objh.objectID);

	if (itr != m_mapObject.end())
		return itr->second;

	return NULL;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, s8 nValue) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, nValue)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, s16 nValue) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, nValue)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, s32 nValue) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, nValue)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, s64 nValue) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, nValue)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, double dValue) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, dValue)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, const char* szAttrValue, size_t nLen) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, szAttrValue, nLen)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::SetAttr(const objecthandle& hd, const char* szAttrName, const void* pBlob, size_t nBlobLen) {
	Object* obj = FindObject(hd);

	if (obj && obj->SetAttr(szAttrName, pBlob, nBlobLen)) {
		call_back(obj->GetType(), szAttrName);
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, s8& nValue)
{
	Object* obj = FindObject(hd);

	if (obj && obj->GetAttr(szAttrName, nValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, s16& nValue)
{
	Object* obj = FindObject(hd);

	if (obj && obj->GetAttr(szAttrName, nValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, s32& nValue)
{
	Object* obj = FindObject(hd);

	if (obj && obj->GetAttr(szAttrName, nValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, s64& nValue)
{
	Object* obj = FindObject(hd);

	if (obj && obj->GetAttr(szAttrName, nValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, double& dValue)
{
	Object* obj = FindObject(hd);

	if (obj && obj->GetAttr(szAttrName, dValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, string& strValue) {
	Object* obj = FindObject(hd);
	if (obj && obj->GetAttr(szAttrName, strValue)) {
		return true;
	}

	return false;
}

bool ObjectMgr::GetAttr(const objecthandle& hd, const char* szAttrName, void* pBlob, size_t nBlobLen) {
	Object* obj = FindObject(hd);
	if (obj && obj->GetAttr(szAttrName, pBlob, nBlobLen)) {
		return true;
	}

	return false;
}

bool ObjectMgr::CreateTable(const objecthandle& hd, const char* szTableName, const TABLE_COLUMN_TYPES& coltypes) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->CreateTable(szTableName, coltypes);
}

bool ObjectMgr::DestroyTable(const objecthandle& hd, const char* szTableName) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->DestroyTable(szTableName);
}

bool ObjectMgr::InsertTableRow(const objecthandle& hd, const char* szTableName, CArgs& args) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->InsertRow(szTableName, args);
}

bool ObjectMgr::DeleteTableRow(const objecthandle& hd, const char* szTableName, size_t nRow) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->DeleteRow(szTableName, nRow);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s8 nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s8& nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s16 nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s16& nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s32 nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s32& nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s64 nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, s64& nValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, nValue);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, double dValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, dValue);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, double& dValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, dValue);
}

bool ObjectMgr::SetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, const char* szValue, size_t nLen) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->SetTableValue(szTableName, nRow, nCol, szValue, nLen);
}

bool ObjectMgr::GetTableValue(const objecthandle& hd, const char* szTableName, size_t nRow, size_t nCol, string& strValue) {
	Object* obj = FindObject(hd);
	if (!obj) {
		return false;
	}
	return obj->GetTableValue(szTableName, nRow, nCol, strValue);
}

void ObjectMgr::call_back(const char* szObjType, const char* szAttrName) {
	map<string, map<string, SET_ATTR_CALLBACK> >::iterator itrObjType = m_mapSetAttrCallBack.find(szObjType);
	if (itrObjType != m_mapSetAttrCallBack.end()) {
		map<string, SET_ATTR_CALLBACK>::iterator itrAttrName = itrObjType->second.find(szAttrName);
		if (itrAttrName != itrObjType->second.end()) {
			(*itrAttrName->second)();
		}
	}
}

void ObjectMgr::RegisterSetAttrCallback(const char* szObjType, const char* szAttrName, SET_ATTR_CALLBACK callback)
{
	m_mapSetAttrCallBack[szObjType][szAttrName] = callback;
}
