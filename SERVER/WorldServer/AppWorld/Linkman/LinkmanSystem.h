


/*
*	file:		LinkmanSystem.h
*	Brief:		����ϵͳ������
*	detail:		������ϵ����Ľ�������������ص�DBͨѶ������DB����
*
*				DB�����ᱻ�����ڶ����У���˱�����DBS������Чʱ�ͷŶ��󣬲ſɱ�֤���и������ݱ�д�뵽DB��
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-20
*/

#include "../../public/LinkmanGroupSetup.h"
#include "../../public/ConvenientQueue.h"
#include "LinkmanGroup.h"


#pragma once

//!				�����ϵ����Ŀ��������
const	ULONG	MAX_LINKMAN_BUF_NUM = 1024 * 512;

class CEntityGroup;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	����ϵͳ������
class LinkmanSystem
{
public:
	LinkmanSystem(void);
	~LinkmanSystem(void);

public:
	//!								�õ�ȫ��Ψһʵ��
	static		LinkmanSystem&		GetInstance						(void);


	//! ����
	//!-----------------------------------------------------------------------------------------------
public:
	//!								�õ����ò��ҳ�ʼ������
	bool							LoadSetupAndInit				(const char* pFilename);
	//!								��������Ϣ���뵽����
	void							SetupAddToByteArray				(vector<BYTE>& veData);

private:
	//!								����ϵͳ�������
	LGSManage						m_LGSManage;

	//!-----------------------------------------------------------------------------------------------
public:
	//!								��һ����ҵ����ݱ��뵽���飬����DBS����
	void							AddToByteArray_ForDBS			(const CGUID &PlayerGuid, CEntityGroup* linkmanGroupOwner);
	//!								����DB����
	void							DecodeDataFromDBS				(const CGUID &PlayerGuid, CEntityGroup* linkmanGroupOwner, CEntityGroup* linkmanGroupAim);
	//!								��Ӧ�������
	void							OnPlayerOnline					(const CGUID &PlayerGuid);
	//!								��Ӧһ��������ݶ�ȡ���
	void							OnLoadPlayerDataFinish			(const CGUID &PlayerGuid);
	//!								��Ӧ���תGS
	void							OnPlayerChangeGs				(CPlayer *pPlayer);
	//!								��Ӧ�������
	void							OnPlayerOffline					(CPlayer *pPlayer);
	//!
	void							SavePlayerData					(const CGUID &PlayerGuid);

	//!								��Ӧ��ҵȼ��ı�
	void							OnPlayerLevelChange				(CPlayer *pPlayer);
	//!								��Ӧ���ְҵ�ı�
	void							OnPlayerOccuChange				(CPlayer *pPlayer);
	//!								��Ӧ���ɾ��״̬�ı�
	void							OnPlayerDelStateChange			(const CGUID &PlayerGuid, BOOL bDeleted);

	//!								�ܷ����ԣ��������Ե����ȼ���eLWPRI_Cant��ʾ��������
	ULONG							CanLeaveWord					(const CGUID &SenderGuid, const char *pReceiverName, CGUID &reReceiverGuid);
	//!								�ܷ��ţ������߱������ߣ�
	BOOL							CanMail							(CPlayer *pReceiver, const char *pSenderName);

	//!								��Ӧϵͳ����������Ϣ
	void							OnSystemRequestAddMsg			(CMessage *pMsg);						
	//!								��Ӧ�������������Ϣ
	void							OnPlayerRequestAddMsg			(CMessage *pMsg);
	//!								��Ӧ��һظ�������Ϣ
	void							OnPlayerReAddMsg				(CMessage *pMsg);
	//!								��Ӧ�������ɾ����Ϣ
	void							OnPlayerRequestDelMsg			(CMessage *pMsg);
	//!								��Ӧ��������ƶ���Ϣ
	void							OnPlayerRequestMoveMsg			(CMessage *pMsg);


	//!								��ӦGS��ͼ�ı���Ϣ
	void							OnGsMapChange					(const CGUID &PlayerGuid, const char* pMapName);
	//!								��ӦGS���ݷ�����Ϣ
	void							OnGsDataSend					(BYTE *pData, LONG &lPos);

	//!								ɾ��һ����ҵ���Ϣ
	void							DelPlayerLinkman				(const CGUID &PlayerGuid);


	void							OutLinkmanInfo(VOID);


private:
	//! ��������
	enum tagLinkmanOprType
	{
		eLOT_Add,
		eLOT_Del,
		eLot_Move,
	};
	//! �Ự���ݶ���
	struct tagOptSession
	{
		ULONG _uEndTime;	//! �Ự����ʱ��
		ULONG _uOptType;	//! ��������
		ULONG _uSrcGroupID;	//! ��������ʼ��ID
		ULONG _uDesGroupID;	//! ��������ֹ��ID
	};

	//!								����
	BOOL							CreateAddSession				(const CGUID& OwnerGuid, const CGUID& FlagGuid, tagOptSession &OptSession);
	//!								�Ƴ�һ����ҵ����лỰ
	void							EndAddSession					(const CGUID& OwnerGuid);
	//!								�Ƴ��������֮��ĻỰ
	BOOL							EndAddSession					(const CGUID& OwnerGuid, const CGUID& FlagGuid, tagOptSession &OptSession);
	//!								���һ����ҵĳ�ʱ�Ự
	void							ClearOutTimeSession				(map<CGUID, tagOptSession> &PlayerSession);
	


	//! ������ϵ���б�
	//!-----------------------------------------------------------------------------------------------
private:
	//!								���뻺��
	void							PushCache(const CGUID &PlayerGuid);
	//!								�����ʱ����
	void							ClearTimeoutData(void);

private:
	struct  tagQueueValue
	{
		CGUID PlayerGuid;
		DWORD Timeout;

		tagQueueValue(void){memset(this, 0, sizeof(tagQueueValue));}
		tagQueueValue(CGUID _Guid, DWORD _Time):PlayerGuid(_Guid),Timeout(_Time){}
	};

	//!											��ʱ����
	ConvenientQueue<CGUID, tagQueueValue>		m_TimeoutQueue;

private:
	//!											���к�����Ϣ�ڴ˴��
	map<CGUID, VecPlayerLinkMan>				m_mapPlayerLinkMan;

	//!											�Ự����map<Ӧ����ID, map<�ȴ�Ӧ����ID, pair<��ʱʱ���, ��ʶ��>>>
	map<CGUID, map<CGUID, tagOptSession>>		m_Session;

	
};

//! ��ϵ��DB����
class LinkmanDbCache
{
	
};