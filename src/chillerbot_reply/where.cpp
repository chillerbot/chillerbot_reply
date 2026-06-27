#include <chillerbot_reply/chillerbot_reply.h>
#include <ddnet_base/base/str.h>
#include <polynet/protocol.h>
#include <polynet/vmath.h>

#include <cstdlib>

using namespace ddnet_base;
using namespace polynet;

bool CChillerBotReply::Where()
{
	// TODO: dont get current dummy but the pinged dummy, but if it differs the reply chat message will be confusing??
	//       if that is done it has to say "my dummy xx is at pos yy"
	if(!Pos().has_value())
	{
		WriteReplyBufFormat("%s no idea. Where are you?", m_pMessageAuthor);
		return true;
	}
	vec2 Self = Pos().value();

	if(str_find_nocase(m_pMessage, "where are you") || str_find_nocase(m_pMessage, "where r u") || str_find_nocase(m_pMessage, "where r yo") ||
		str_find_nocase(m_pMessage, "where u") ||
		str_find_nocase(m_pMessage, "wo bist"))
	{
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			CChillerBotReplyTee Client = GetClient(i);
			if(!Client.m_Active)
				continue;
			if(str_comp(Client.m_pName, m_pMessageAuthor))
				continue;

			if(!Client.m_PosX.has_value() || !Client.m_PosY.has_value())
			{
				// TODO: this is not ideal, also say we are at finish or start if we are close to that
				WriteReplyBufFormat("%s no idea where you are but I am at x: %.2f, y: %.2f", m_pMessageAuthor, Self.x, Self.y);
				return true;
			}

			vec2 Other = vec2(Client.m_PosX.value(), Client.m_PosY.value());
			float DistY = std::abs(Self.y - Other.y);
			float DistX = std::abs(Self.x - Other.x);
			// instead of some unimaginable long distance number in tiles
			// prefer saying "finish" or "end" without any relation to the other player
			// since it does not matter in this case
			if(DistX > 50 * 32 && DistY > 50 * 32)
			{
				if(RaceHelper().IsClusterRangeFinish(Self.x, Self.y, 32))
				{
					WriteReplyBufFormat("%s I am at the finish line", m_pMessageAuthor);
					return true;
				}
				else if(RaceHelper().IsClusterRangeStart(Self.x, Self.y, 32))
				{
					WriteReplyBufFormat("%s I am at start", m_pMessageAuthor);
					return true;
				}
			}
			if(DistX < 5 * 32 && DistY < 5 * 32)
			{
				if(DistX > DistY)
				{
					if(Self.x > Other.x)
					{
						WriteReplyBufFormat("%s i am literally next to you (on your right)", m_pMessageAuthor);
						return true;
					}
					else
					{
						WriteReplyBufFormat("%s i am literally next to you (on your left)", m_pMessageAuthor);
						return true;
					}
				}
				else
				{
					if(Self.y > Other.y)
					{
						WriteReplyBufFormat("%s i am literally next to you (below you)", m_pMessageAuthor);
						return true;
					}
					else
					{
						WriteReplyBufFormat("%s i am literally next to you (above you)", m_pMessageAuthor);
						return true;
					}
				}
			}
			else if(DistY > DistX * 8)
			{
				if(Self.y > Other.y)
				{
					WriteReplyBufFormat("%s below you (%d tiles)", m_pMessageAuthor, (int)(DistY / 32));
					return true;
				}
				else
				{
					WriteReplyBufFormat("%s above you (%d tiles)", m_pMessageAuthor, (int)(DistY / 32));
					return true;
				}
			}
			else if(Self.x > Other.x)
			{
				if(DistY < 15 * 32)
				{
					WriteReplyBufFormat("%s i am on your right (%d tiles away)", m_pMessageAuthor, (int)(DistX / 32));
					return true;
				}
				else if(Self.y > Other.y)
				{
					WriteReplyBufFormat("%s i am on your right (%d tiles away) and %d tiles below you", m_pMessageAuthor, (int)(DistX / 32), (int)(DistY / 32));
					return true;
				}
				else
				{
					WriteReplyBufFormat("%s i am on your right (%d tiles away) and %d tiles above you", m_pMessageAuthor, (int)(DistX / 32), (int)(DistY / 32));
					return true;
				}
			}
			else
			{
				if(DistY < 15 * 32)
				{
					WriteReplyBufFormat("%s i am on your left (%d tiles away)", m_pMessageAuthor, (int)(DistX / 32));
					return true;
				}
				else if(Self.y > Other.y)
				{
					WriteReplyBufFormat("%s i am on your left (%d tiles away) and %d tiles below you", m_pMessageAuthor, (int)(DistX / 32), (int)(DistY / 32));
					return true;
				}
				else
				{
					WriteReplyBufFormat("%s i am on your left (%d tiles away) and %d tiles above you", m_pMessageAuthor, (int)(DistX / 32), (int)(DistY / 32));
					return true;
				}
			}
		}
		if(RaceHelper().IsNearFinish(Self.x, Self.y, 32))
			WriteReplyBufFormat("%s I am at the finish line", m_pMessageAuthor);
		else if(RaceHelper().IsNearStart(Self.x, Self.y, 32))
			WriteReplyBufFormat("%s I am at start", m_pMessageAuthor);
		else
			WriteReplyBufFormat("%s I am really lost haha, where am i?", m_pMessageAuthor);
		return true;
	}
	return false;
}
