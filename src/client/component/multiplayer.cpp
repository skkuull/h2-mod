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

		void reallocate_svs_clients()
		{
			utils::hook::inject(0x1406AE52D + 3, &svs_clients);
			utils::hook::inject(0x1406AE5F6 + 3, &svs_clients);
			utils::hook::inject(0x1406AE63C + 3, &svs_clients);
			utils::hook::inject(0x1406AE662 + 3, &svs_clients);
			utils::hook::inject(0x1406AFF3E + 3, &svs_clients);
			utils::hook::inject(0x1406B0332 + 3, &svs_clients);
			utils::hook::inject(0x1406B048F + 3, &svs_clients);
			utils::hook::inject(0x1406B070D + 3, &svs_clients);
			utils::hook::inject(0x1406B1030 + 3, &svs_clients);
			utils::hook::inject(0x1406B1BF2 + 3, &svs_clients);
			utils::hook::inject(0x1406B1E2A + 3, &svs_clients);
			utils::hook::inject(0x1406B2127 + 3, &svs_clients);
			utils::hook::inject(0x1406B2F61 + 3, &svs_clients);
			utils::hook::inject(0x1406B3120 + 3, &svs_clients);
			utils::hook::inject(0x1406B322D + 3, &svs_clients);
			utils::hook::inject(0x1406B3426 + 3, &svs_clients);
			utils::hook::inject(0x1406B398F + 3, &svs_clients);
			utils::hook::inject(0x1406B40BD + 3, &svs_clients);

			utils::hook::set(0x1406AE7AB + 3, &svs_clients);

			utils::hook::inject(0x1406B1D73 + 2, &svs_clients);
			utils::hook::inject(0x1406B3979 + 2, &svs_clients);
			utils::hook::inject(0x1406B5A61 + 2, &svs_clients);
			utils::hook::inject(0x1406AE64F + 2, &svs_clients);

			utils::hook::inject(0x1406B4D55 + 3, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B5F25 + 3, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B3481 + 3, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B492D + 3, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B4C3D + 3, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B4C4C + 3, &svs_clients[0].__pad0[4]);

			utils::hook::inject(0x1406B4C53 + 4, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B4985 + 2, &svs_clients[0].__pad0[4]);
			utils::hook::inject(0x1406B4B19 + 2, &svs_clients[0].__pad0[4]);

			utils::hook::inject(0x1406B4B00 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4942 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4978 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4C1A + 2, &svs_clients[0].__pad0[0xC]);

			utils::hook::inject(0x1406B4939 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B49C8 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B49E1 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4B06 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4C14 + 2, &svs_clients[0].__pad0[0xC]);
			utils::hook::inject(0x1406B4C46 + 2, &svs_clients[0].__pad0[0xC]);

			utils::hook::inject(0x1406B498B + 3, &svs_clients[0].__pad0[0xC + 4]);

			utils::hook::inject(0x1406B1037 + 3, &svs_clients[MAX_CLIENTS].__pad0[sizeof(game::client_t)]);
			utils::hook::inject(0x1406B3BC7 + 2, &svs_clients[MAX_CLIENTS].__pad0[sizeof(game::client_t)]);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			//reallocate_svs_clients();

			// Assign level.clients to our g_clients
			utils::hook::call(0x1404C8CB3, clear_clients);
		}
	};
}

REGISTER_COMPONENT(multiplayer::component)
