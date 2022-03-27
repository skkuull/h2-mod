#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

#define MAX_CLIENTS 18

namespace multiplayer
{
	game::gclient_s g_clients[MAX_CLIENTS];
	game::client_t svs_clients[MAX_CLIENTS];

	// must also reallocate svs_clients which is a painful thing to do (need to replace every reference)
	// dont really know what to do after that

	namespace
	{
		void clear_clients()
		{
			game::level->maxclients = MAX_CLIENTS;
			*game::dyn_ent_anchor_count = MAX_CLIENTS - 1; // num of extra clients ?
			game::Com_Memset(g_clients, 0, sizeof(g_clients));
			game::level->clients = g_clients;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Assign level.clients to our g_clients
			utils::hook::call(0x1404C8CB3, clear_clients);
		}
	};
}

REGISTER_COMPONENT(multiplayer::component)
