#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "scheduler.hpp"
#include "updater.hpp"
#include "game/ui_scripting/execution.hpp"

#include "version.h"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/nt.hpp>
#include <utils/concurrency.hpp>
#include <utils/http.hpp>
#include <utils/cryptography.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

#define MASTER "https://master.fed0001.xyz/"

#define FILES_PATH "files.json"
#define FILES_PATH_DEV "files-dev.json"

#define DATA_PATH "data/"
#define DATA_PATH_DEV "data-dev/"

#define ERR_UPDATE_CHECK_FAIL "Failed to check for updates"
#define ERR_DOWNLOAD_FAIL "Failed to download file "
#define ERR_WRITE_FAIL "Failed to write file "

namespace updater
{
	namespace
	{
		game::dvar_t* cl_auto_update;
		bool has_tried_update = false;

		struct status
		{
			bool done;
			bool success;
		};

		struct file_data
		{
			std::string name;
			std::string data;
		};

		struct update_data_t
		{
			bool restart_required{};
			bool cancelled{};
			status check{};
			status download{};
			std::string error{};
			std::string current_file{};
			std::vector<std::string> required_files{};
		};

		utils::concurrency::container<update_data_t> update_data;

		std::string select(const std::string& main, const std::string& develop)
		{
			if (GIT_BRANCH == "develop"s)
			{
				return develop;
			}

			return main;
		}

		void notify(const std::string& name)
		{
			scheduler::once([=]()
			{
				ui_scripting::notify(name, {});
			}, scheduler::pipeline::lui);
		}

		void set_update_check_status(bool done, bool success, const std::string& error = {})
		{
			update_data.access([done, success, error](update_data_t& data_)
			{
				data_.check.done = done;
				data_.check.success = success;
				data_.error = error;

				notify("update_check_done");
			});
		}

		void set_update_download_status(bool done, bool success, const std::string& error = {})
		{
			update_data.access([done, success, error](update_data_t& data_)
			{
				data_.download.done = done;
				data_.download.success = success;
				data_.error = error;
				notify("update_done");
			});
		}

		bool check_file(const std::string& name, const std::string& sha)
		{
			std::string data;
			if (!utils::io::read_file(name, &data))
			{
				return false;
			}

			if (utils::cryptography::sha1::compute(data, true) != sha)
			{
				return false;
			}

			return true;
		}

		std::string load_binary_name()
		{
			utils::nt::library self;
			return self.get_name();
		}

		std::string get_binary_name()
		{
			static const auto name = load_binary_name();
			return name;
		}

		std::string get_time_str()
		{
			return utils::string::va("%i", uint32_t(time(nullptr)));
		}

		std::optional<std::string> download_file(const std::string& name)
		{
			return utils::http::get_data(MASTER + select(DATA_PATH, DATA_PATH_DEV) + name + "?" + get_time_str());
		}

		bool is_update_cancelled()
		{
			return update_data.access<bool>([](update_data_t& data_)
			{
				return data_.cancelled;
			});
		}

		bool write_file(const std::string& name, const std::string& data)
		{
			if (get_binary_name() == name && 
				utils::io::file_exists(name) && 
				!utils::io::move_file(name, name + ".old"))
			{
				return false;
			}

			return utils::io::write_file(name, data);
		}

		void delete_old_file()
		{
			utils::io::remove_file(get_binary_name() + ".old");
		}

		void reset_data()
		{
			update_data.access([](update_data_t& data_)
			{
				data_ = {};
			});
		}
	}

	void relaunch()
	{
		utils::nt::relaunch_self("-singleplayer");
		utils::nt::terminate();
	}

	void set_has_tried_update(bool tried)
	{
		has_tried_update = tried;
	}

	bool get_has_tried_update()
	{
		return has_tried_update;
	}

	bool auto_updates_enabled()
	{
		return cl_auto_update->current.enabled;
	}

	bool is_update_check_done()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.check.done;
		});
	}

	bool is_update_download_done()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.download.done;
		});
	}

	bool get_update_check_status()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.check.success;
		});
	}

	bool get_update_download_status()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.download.success;
		});
	}

	bool is_update_available()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.required_files.size() > 0;
		});
	}

	bool is_restart_required()
	{
		return update_data.access<bool>([](update_data_t& data_)
		{
			return data_.restart_required;
		});
	}

	std::string get_last_error()
	{
		return update_data.access<std::string>([](update_data_t& data_)
		{
			return data_.error;
		});
	}

	std::string get_current_file()
	{
		return update_data.access<std::string>([](update_data_t& data_)
		{
			return data_.current_file;
		});
	}

	void cancel_update()
	{
#ifdef DEBUG
		printf("[Updater] Cancelling update\n");
#endif

		return update_data.access([](update_data_t& data_)
		{
			data_.cancelled = true;
		});
	}

	void start_update_check()
	{
		cancel_update();
		reset_data();

#ifdef DEBUG
		printf("[Updater] starting update check\n");
#endif

		scheduler::once([]()
		{
			const auto files_data = utils::http::get_data(MASTER + select(FILES_PATH, FILES_PATH_DEV) + "?" + get_time_str());

			if (is_update_cancelled())
			{
				reset_data();
				return;
			}

			if (!files_data.has_value())
			{
				set_update_check_status(true, false, ERR_UPDATE_CHECK_FAIL);
				return;
			}

			rapidjson::Document j;
			j.Parse(files_data.value().data());

			if (!j.IsArray())
			{
				set_update_check_status(true, false, ERR_UPDATE_CHECK_FAIL);
				return;
			}

			std::vector<std::string> required_files;

			const auto files = j.GetArray();
			for (const auto& file : files)
			{
				if (!file.IsArray() || file.Size() != 3 || !file[0].IsString() || !file[2].IsString())
				{
					continue;
				}

				const auto name = file[0].GetString();
				const auto sha = file[2].GetString();

				if (!check_file(name, sha))
				{
					if (get_binary_name() == name)
					{
						update_data.access([](update_data_t& data_)
						{
							data_.restart_required = true;
						});
					}

#ifdef DEBUG
					printf("[Updater] need file %s\n", name);
#endif

					required_files.push_back(name);
				}
			}

			update_data.access([&required_files](update_data_t& data_)
			{
				data_.check.done = true;
				data_.check.success = true;
				data_.required_files = required_files;
			});

			notify("update_check_done");
		}, scheduler::pipeline::async);
	}

	void start_update_download()
	{
#ifdef DEBUG
		printf("[Updater] starting update download\n");
#endif

		if (!is_update_check_done() || !get_update_check_status() || is_update_cancelled())
		{
			return;
		}

		scheduler::once([]()
		{
			const auto required_files = update_data.access<std::vector<std::string>>([](update_data_t& data_)
			{
				return data_.required_files;
			});

			std::vector<file_data> downloads;

			for (const auto& file : required_files)
			{
				update_data.access([file](update_data_t& data_)
				{
					data_.current_file = file;
				});

#ifdef DEBUG
				printf("[Updater] downloading file %s\n", file.data());
#endif

				const auto data = download_file(file);

				if (is_update_cancelled())
				{
					reset_data();
					return;
				}

				if (!data.has_value())
				{
					set_update_download_status(true, false, ERR_DOWNLOAD_FAIL + file);
					return;
				}

				downloads.push_back({file, data.value()});
			}

			for (const auto& download : downloads)
			{
				if (!write_file(download.name, download.data))
				{
					set_update_download_status(true, false, ERR_WRITE_FAIL + download.name);
					return;
				}
			}

			set_update_download_status(true, true);
		}, scheduler::pipeline::async);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			delete_old_file();
			cl_auto_update = dvars::register_bool("cg_auto_update", true, game::DVAR_FLAG_SAVED);
		}
	};
}

REGISTER_COMPONENT(updater::component)
