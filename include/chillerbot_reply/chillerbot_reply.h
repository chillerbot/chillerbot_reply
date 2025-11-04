#pragma once

#include <cstddef>

enum
{
	TEE_MAIN,
	TEE_DUMMY,
	NUM_TEES,
};

class CChillerBotReplyTee
{
public:
	const char *m_pName = "nameless tee";
	const char *m_pClan = "";
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
};

class CChillerBotReplyChatMessage
{
public:
	const char *m_pMessage = "";
	const char *m_pAuthor = "";
	int m_Team = 0;
};

class CChillerBotReply
{
	bool LineShouldHighlight(const char *pLine, const char *pName);

public:
	CChillerBotReplyContext m_Context;

	// returns true when it found a reply to `pMsg`
	// and fills the reply into `pReplyBuf`
	bool Reply(const CChillerBotReplyChatMessage *pMsg, char *pReplyBuf, size_t ReplyBufLen);
};
