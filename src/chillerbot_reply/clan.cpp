#include <chillerbot_reply/clan.h>
#include <ddnet_base/base/str.h>

using namespace ddnet_base;

namespace ChillerBotReply
{

	bool HowToJoinClan(const char *pClan, char *pResponse, int SizeOfResponse)
	{
		if(!pResponse)
			return false;
		pResponse[0] = '\0';
		if(!str_comp(pClan, "Chilli.*"))
			str_copy(pResponse, "Chilli.* is a fun clan everybody that uses the skin greensward can join", SizeOfResponse);
		else if(!str_comp(pClan, "|*KoG*|"))
			str_copy(pResponse, "If you want to join the gores clan |*KoG*| visit their website kog.tw", SizeOfResponse);
		else if(!str_comp(pClan, "χron"))
			str_copy(pResponse, "If you want to join the vanilla clan χron visit their website aeon.teewars.com", SizeOfResponse);
		else if(!str_comp(pClan, "ÆON"))
			str_copy(pResponse, "If you want to join the vanilla clan ÆON visit their website aeon.teewars.com", SizeOfResponse);
		else
			return false;
		return true;
	}

}
