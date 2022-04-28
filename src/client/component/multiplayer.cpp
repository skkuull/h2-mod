#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace multiplayer
{
	namespace
	{

	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{

		}
	};
}

REGISTER_COMPONENT(multiplayer::component)
