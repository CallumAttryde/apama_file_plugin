#ifndef CORRELATOR_PLUGINS_FILE_TRANSPORT_H
#define CORRELATOR_PLUGINS_FILE_TRANSPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <filesystem>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <epl_plugin.hpp>

using com::apama::epl::EPLPlugin;
using com::apama::epl::data_t;
using com::apama::epl::list_t;
using com::apama::epl::get;

using std::ifstream;
using std::ofstream;
using std::string;

namespace fs = std::experimental::filesystem;

class FilePlugin : public EPLPlugin<FilePlugin>
{
public:
	FilePlugin() 
		: base_plugin_t("FilePluginPlugin"), root_dir(discover_root_dir())
	{}
	~FilePlugin() {}

	static void initialize(base_plugin_t::method_data_t &md)
	{
		md.registerMethod<decltype(&FilePlugin::get_root_dir), &FilePlugin::get_root_dir>("get_root_dir", "action<> returns string");
		md.registerMethod<decltype(&FilePlugin::read), &FilePlugin::read>("read", "action<string> returns sequence<string>");
		md.registerMethod<decltype(&FilePlugin::write), &FilePlugin::write>("write", "action<string, sequence<string> >");
		md.registerMethod<decltype(&FilePlugin::exists), &FilePlugin::exists>("exists", "action<string> returns boolean");
		md.registerMethod<decltype(&FilePlugin::copy), &FilePlugin::copy>("copy", "action<string, string>");
		md.registerMethod<decltype(&FilePlugin::move), &FilePlugin::move>("move", "action<string, string>");
		md.registerMethod<decltype(&FilePlugin::remove), &FilePlugin::remove>("remove", "action<string>");
		md.registerMethod<decltype(&FilePlugin::make_dir), &FilePlugin::make_dir>("mkdir", "action<string>");
		md.registerMethod<decltype(&FilePlugin::list_dir), &FilePlugin::list_dir>("ls", "action<string> returns sequence<string>");
		md.registerMethod<decltype(&FilePlugin::get_file_size_MB), &FilePlugin::get_file_size_MB>("get_file_size_MB", "action<string> returns float");
	}

	string get_root_dir() { return root_dir; }

	list_t read(const string &path)
	{
		check_exists(path);
		list_t contents;
		ifstream ifs(build_path(path));
		for(string line; std::getline(ifs, line);) {
			contents.push_back(line);
		}
		return contents;
	}

	void write(const string &path, const list_t &contents)
	{
		std::ofstream ofs;
		if (exists(path)) {
			ofs.open(build_path(path), std::ios_base::app);
		} else {
			ofs.open(build_path(path));
		}
		for(auto it = contents.begin(); it != contents.end(); ++it) {
			ofs << get<const char*>(*it) << std::endl;
		}
	}

	bool exists(const string &path)
	{
		return fs::exists(build_path(path));
	}

	void copy(const string &path, const string &target)
	{
		fs::copy(build_path(path), build_path(target));
	}

	// should this be atomic?
	void move(const string &path, const string &target)
	{
		copy(path, target);
		remove(path);
	}

	void remove(const string &path)
	{
		if (fs::remove(build_path(path)) != 0) { throw std::runtime_error("Unable to remove file: " + build_path(path)); }
	}

	void make_dir(const string &path)
	{
		if (fs::create_directory(build_path(path)) != 0) { throw std::runtime_error("Unable to make directory: " + build_path(path)); }
	}

	list_t list_dir(const string &path)
	{
		list_t entries;
		for (const auto &dir_ent : fs::directory_iterator(build_path(path)))
		{
			entries.push_back(dir_ent.path().c_str());
		}
		return entries;
	}

	double get_file_size_MB(const string &path)
	{
		check_exists(path);
		return fs::file_size(build_path(path));
	}

private:
	void check_exists(const string &path)
	{
		if (!fs::exists(path)) { throw std::runtime_error("Could not find: " + build_path(path)); }
	}

	fs::path build_path(const string &path)
	{
		return root_dir + path;
	}

	string discover_root_dir()
	{
		char *root_dir = getenv("APAMA_FILEPLUGIN_ROOT_DIR");
		string ret;
		if (root_dir) {
			ret = root_dir;
		} else {
			ret = getenv("APAMA_WORK");
		}
		return ret + "/";
	}

	string root_dir;
};

APAMA_DECLARE_EPL_PLUGIN(FilePlugin)

#endif

