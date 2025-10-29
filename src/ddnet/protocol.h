namespace chillerbot_ddnet
{
	enum
	{
		SERVER_TICK_SPEED = 50,
		SERVER_FLAG_PASSWORD = 1 << 0,
		SERVER_FLAG_TIMESCORE = 1 << 1,
		SERVERINFO_LEVEL_MIN = 0,
		SERVERINFO_LEVEL_MAX = 2,

		MAX_SERVER_ADDRESSES = 16,
		SERVERINFO_MAX_CLIENTS = 128,
		MAX_CLIENTS = 128,
		VANILLA_MAX_CLIENTS = 16,
		SERVER_MAX_CLIENTS = 64,
		MAX_CHECKPOINTS = 25,
		MIN_TICK = 0,
		MAX_TICK = 0x6FFFFFFF,

		MAX_INPUT_SIZE = 128,
		MAX_SNAPSHOT_PACKSIZE = 900,

		MAX_NAME_LENGTH = 16,
		MAX_CLAN_LENGTH = 12,
		MAX_SKIN_LENGTH = 24,

		// message packing
		/**
	 * Guaranteed to be delivered, resent on packet loss.
	 */
		MSGFLAG_VITAL = 1 << 0,
		/**
	 * Makes the message be sent immediately. Without this flag the message will be delayed until the next flush.
	 */
		MSGFLAG_FLUSH = 1 << 1,
		/**
	 * Don't write message to demo recorders. This flag is server-side only, where sent messages are recorded by default.
	 */
		MSGFLAG_NORECORD = 1 << 2,
		/**
	 * Write message to demo recorders. This flag is client-side only, where sent messages are not recorded by default.
	 */
		MSGFLAG_RECORD = 1 << 3,
		/**
	 * Don't send the message to client/server. Useful combined with @link MSGFLAG_RECORD @endlink to record a message without sending it.
	 */
		MSGFLAG_NOSEND = 1 << 4,
	};

	enum
	{
		VERSION_NONE = -1,
		VERSION_VANILLA = 0,
		VERSION_DDRACE = 1,
		VERSION_DDNET_OLD = 2,
		VERSION_DDNET_WHISPER = 217,
		VERSION_DDNET_GOODHOOK = 221,
		VERSION_DDNET_RCONPROTECT = 408,
		VERSION_DDNET_ANTIPING_PROJECTILE = 604,
		VERSION_DDNET_UPDATER_FIXED = 707,
		VERSION_DDNET_GAMETICK = 10042,
		VERSION_DDNET_EARLY_VERSION = 13020,
		VERSION_DDNET_MSG_LEGACY = 15040,
		VERSION_DDNET_SWITCH = 15060,
		VERSION_DDNET_INDEPENDENT_SPECTATORS_TEAM = 16000,
		VERSION_DDNET_WEAPON_SHIELDS = 16010,
		VERSION_DDNET_NEW_HUD = 16020,
		VERSION_DDNET_MULTI_LASER = 16040,
		VERSION_DDNET_ENTITY_NETOBJS = 16200,
		VERSION_DDNET_REDIRECT = 17020,
		VERSION_DDNET_PLAYERFLAG_SPEC_CAM = 18090,
		VERSION_DDNET_RECONNECT = 18090,
		VERSION_DDNET_128_PLAYERS = 19000,
		VERSION_DDNET_PREINPUT = 19040,
		VERSION_DDNET_SAVE_CODE = 19060,
	};

	enum
	{
		NUM_TUNEZONES = 256,
	};

}
