#pragma once

#include <cstddef>
#include <optional>

enum
{
	TEE_MAIN,
	TEE_DUMMY,
	NUM_TEES,
};

class CChillerBotReplyTee
{
public:
	bool m_Active = true;

	int m_ClientId = 0;
	const char *m_pName = "(invalid)";
	const char *m_pClan = "";
	std::optional<float> m_PosX = std::nullopt;
	std::optional<float> m_PosY = std::nullopt;
};

class CChillerBotReplyContext
{
public:
	CChillerBotReplyContext();
	void Reset();

	// main tee and dummy of the client user
	CChillerBotReplyTee m_aOwnTees[NUM_TEES];

	// indexes into m_aOwnTees deciding if dummy is used or not
	int m_ActiveTee = 0;

	bool m_IsDummyConnected = false;

	void *m_pUser = nullptr;

	const char *(*m_pfnGetClientName)(int ClientId, void *pUser) = nullptr;
	const char *(*m_pfnGetClientClan)(int ClientId, void *pUser) = nullptr;
	CChillerBotReplyTee (*m_pfnGetClient)(int ClientId, void *pUser) = nullptr;

	void (*m_pfnGetWarReason)(const char *pName, char *pReason, int ReasonSize, void *pUser) = nullptr;
	void (*m_pfnGetWarClansStr)(char *pBuf, size_t BufLen, void *pUser) = nullptr;
	bool (*m_pfnIsWar)(const char *pName, const char *pClan, void *pUser) = nullptr;
	bool (*m_pfnIsWarlist)(const char *pName, void *pUser) = nullptr;
	bool (*m_pfnIsTeamlist)(const char *pName, void *pUser) = nullptr;
	bool (*m_pfnIsTraitorlist)(const char *pName, void *pUser) = nullptr;
	bool (*m_pfnIsWarClanlist)(const char *pClan, void *pUser) = nullptr;
	bool (*m_pfnIsTeamClanlist)(const char *pClan, void *pUser) = nullptr;
	bool (*m_pfnIsWarClanmate)(const char *pClan, void *pUser) = nullptr;
	bool (*m_pfnIsWarClanmateId)(int ClientId, void *pUser) = nullptr;
	int (*m_pfnNumEnemies)(void *pUser) = nullptr;
	int (*m_pfnNumTeam)(void *pUser) = nullptr;
};

class CChillerBotReplyChatMessage
{
public:
	const char *m_pMessage = "";
	const char *m_pAuthor = "";
	const char *m_pAuthorClan = "";
	int m_Team = 0;
};

class CWarListWrapper
{
	const CChillerBotReplyContext *m_pContext = nullptr;

public:
	void OnInit(const CChillerBotReplyContext *pContext);

	void GetWarReason(const char *pName, char *pReason, int ReasonSize);
	void GetWarClansStr(char *pBuf, size_t BufLen);
	bool IsWar(const char *pName, const char *pClan);
	bool IsWarlist(const char *pName);
	bool IsTeamlist(const char *pName);
	bool IsTraitorlist(const char *pName);
	bool IsWarClanlist(const char *pClan);
	bool IsTeamClanlist(const char *pClan);
	bool IsWarClanmate(const char *pClan);
	bool IsWarClanmate(int ClientId);
	int NumEnemies();
	int NumTeam();
};

class CChillerBotReply
{
	const char *m_pMessage = "";
	const char *m_pMessageAuthor = "";
	const char *m_pMessageAuthorClan = "";
	char *m_pReplyBuf = nullptr;
	size_t m_ReplyBufLen = 0;

	CWarListWrapper m_WarList;
	CWarListWrapper &WarList() { return m_WarList; }

	const char *Name();
	const char *DummyName();
	bool IsDummyConnected() const;
	CChillerBotReplyTee GetClient(int ClientId) const;

	void WriteReplyBuf(const char *pMessage);
	void WriteReplyBufWithPing(const char *pMessage);
	[[gnu::format(printf, 2, 3)]] void WriteReplyBufFormat(const char *pFormat, ...);

	bool LineShouldHighlight(const char *pLine, const char *pName);

	bool CanIJoinYourClan();
	bool ListClanWars();
	bool IsWarName();

	// list_wars.cpp
	bool ListWars();

	// check_war_others.cpp
	bool WhyWar(const char *pVictim, bool IsCheck = false);

	/*
		IsWarCheckSuffix

		Check if the given string could be a message checking
		if another player has war. By only looking at the suffix.
		And only looking at the format "name is war?" not "is war name?"

		Returns the suffix length that was found
	*/
	int IsWarCheckSuffix(const char *pStr);
	bool NameIsWar();

	// check_war_self.cpp

	/*
		WhyDoYouKillMe

		Covering all kinds of questions like only "?"
		But only triggering an response if its coming from an enemy.
	*/
	bool WhyDoYouKillMe(int NameLen, int MsgLen);

	/*
		DoYouWarMe

		intentionally check for being on warlist
		also expecting an no if not
	*/
	bool DoYouWarMe(int NameLen, int MsgLen);

public:
	CChillerBotReplyContext m_Context;

	// returns true when it found a reply to `pMsg`
	// and fills the reply into `pReplyBuf`
	bool Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen);
};
