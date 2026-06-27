#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>
#include <polynet/protocol.h>

using namespace polynet;
using namespace ddnet_base;

bool CChillerBotReply::WhyWar(const char *pVictim, bool IsCheck)
{
	if(!pVictim)
		return false;

	if(!str_comp(pVictim, "me"))
		pVictim = m_pMessageAuthor;
	bool HasWar = true;
	// aVictim also has to hold the full own name to match the chop off
	char aVictim[MAX_NAME_LENGTH + 3 + MAX_NAME_LENGTH];
	str_copy(aVictim, pVictim, sizeof(aVictim));
	if(!m_WarList.IsWarlist(aVictim) && !m_WarList.IsTraitorlist(aVictim))
	{
		HasWar = false;
		while(str_endswith(aVictim, "?")) // cut off the question marks from the victim name
			aVictim[str_length(aVictim) - 1] = '\0';
		while(str_endswith(aVictim, " ")) // cut off spaces from victim name 'why war foo ?' -> 'foo ?' -> 'foo ' -> 'foo'
			aVictim[str_length(aVictim) - 1] = '\0';
		// cut off own name from the victime name if question in this format "why do you war foo (your name)"
		char aOwnName[MAX_NAME_LENGTH + 3];
		// main tee
		str_format(aOwnName, sizeof(aOwnName), " %s", Name());
		if(str_endswith_nocase(aVictim, aOwnName))
			aVictim[str_length(aVictim) - str_length(aOwnName)] = '\0';
		if(IsDummyConnected())
		{
			str_format(aOwnName, sizeof(aOwnName), " %s", DummyName());
			if(str_endswith_nocase(aVictim, aOwnName))
				aVictim[str_length(aVictim) - str_length(aOwnName)] = '\0';
		}

		// cut off descriptions like this
		// why do you block foo he is new here!
		// why do you block foo she is my friend!!
		for(int i = 0; i < str_length(aVictim); i++)
		{
			// c++ be like...
			if(i < 2)
				continue;
			if(aVictim[i - 1] != ' ')
				continue;
			if((aVictim[i] != 'h' || !aVictim[i + 1] || aVictim[i + 1] != 'e' || !aVictim[i + 2] || aVictim[i + 2] != ' ') &&
				(aVictim[i] != 's' || !aVictim[i + 1] || aVictim[i + 1] != 'h' || !aVictim[i + 2] || aVictim[i + 2] != 'e' || !aVictim[i + 3] || aVictim[i + 3] != ' '))
				continue;

			aVictim[i - 1] = '\0';
			break;
		}

		// do not kill my friend foo
		const char *pFriend = nullptr;
		if((pFriend = str_find_nocase(aVictim, " friend ")))
			pFriend += str_length(" friend ");
		else if((pFriend = str_find_nocase(aVictim, " frint ")))
			pFriend += str_length(" frint ");
		else if((pFriend = str_find_nocase(aVictim, " mate ")))
			pFriend += str_length(" mate ");
		else if((pFriend = str_find_nocase(aVictim, " bff ")))
			pFriend += str_length(" bff ");
		else if((pFriend = str_find_nocase(aVictim, " girlfriend ")))
			pFriend += str_length(" girlfriend ");
		else if((pFriend = str_find_nocase(aVictim, " boyfriend ")))
			pFriend += str_length(" boyfriend ");
		else if((pFriend = str_find_nocase(aVictim, " dog ")))
			pFriend += str_length(" dog ");
		else if((pFriend = str_find_nocase(aVictim, " gf ")))
			pFriend += str_length(" gf ");
		else if((pFriend = str_find_nocase(aVictim, " bf ")))
			pFriend += str_length(" bf ");

		if(pFriend)
			str_copy(aVictim, pFriend, sizeof(aVictim));
	}

	if(!str_comp(aVictim, "me"))
		str_copy(aVictim, m_pMessageAuthor, sizeof(aVictim));

	char aWarReason[128];
	if(HasWar || m_WarList.IsWarlist(aVictim) || m_WarList.IsTraitorlist(aVictim))
	{
		m_WarList.GetWarReason(aVictim, aWarReason, sizeof(aWarReason));
		if(aWarReason[0])
			WriteReplyBufFormat("%s: %s has war because: %s", m_pMessageAuthor, aVictim, aWarReason);
		else
			WriteReplyBufFormat("%s: the name %s is on my warlist.", m_pMessageAuthor, aVictim);
		return true;
	}
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		const char *pLoopName = m_Context.m_pfnGetClientName(i, m_Context.m_pUser);
		const char *pLoopClan = m_Context.m_pfnGetClientName(i, m_Context.m_pUser);
		if(str_comp(pLoopName, aVictim))
			continue;

		if(m_WarList.IsWarClanlist(pLoopClan))
		{
			WriteReplyBufFormat("%s i war %s because his clan %s is on my warlist.", m_pMessageAuthor, aVictim, pLoopClan);
			return true;
		}
		if(m_WarList.IsWarClanmate(i))
		{
			WriteReplyBufFormat("%s i might kill %s because I war member from his clan %s", m_pMessageAuthor, aVictim, pLoopClan);
			return true;
		}
	}
	if(IsCheck && str_comp_nocase(aVictim, "me"))
	{
		WriteReplyBufFormat("%s: '%s' is not on my warlist.", m_pMessageAuthor, aVictim);
		return true;
	}
	return false;
}
