#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "filesystem.hpp"

#include <utils/io.hpp>

namespace filesystem
{
	std::unordered_set<std::string>& get_search_paths()
	{
		static std::unordered_set<std::string> search_paths{};
		return search_paths;
	}

	std::string read_file(const std::string& path)
	{
		for (const auto& search_path : get_search_paths())
		{
			const auto path_ = search_path + "/" + path;
			if (utils::io::file_exists(path_))
			{
				return utils::io::read_file(path_);
			}
		}

		return {};
	}

	bool read_file(const std::string& path, std::string* data, std::string* real_path)
	{
		for (const auto& search_path : get_search_paths())
		{
			const auto path_ = search_path + "/" + path;
			if (utils::io::read_file(path_, data))
			{
				if (real_path != nullptr)
				{
					*real_path = path_;
				}

				return true;
			}
		}

		return false;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			get_search_paths().insert(".");
			get_search_paths().insert("h2-mod");
			get_search_paths().insert("data");
		}
	};
}

REGISTER_COMPONENT(filesystem::component)
