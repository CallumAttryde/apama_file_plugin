#ifndef CORRELATOR_PLUGINS_FILE_TRANSPORT_H
#define CORRELATOR_PLUGINS_FILE_TRANSPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <experimental/filesystem>

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
		: base_plugin_t("FilePluginPlugin"), root_dir(discover_root_dir()), last_ec()
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
		md.registerMethod<decltype(&FilePlugin::build_path), &FilePlugin::build_path>("build_path", "action<sequence<string> > returns string");
	}

	string get_root_dir() { return root_dir.string(); }

	list_t read(const string &path)
	{
		if (!exists(path)) {
			throw fs::filesystem_error("cannot read", append_path(path), last_ec);
		}
		list_t contents;
		ifstream ifs(append_path(path));
		for(string line; std::getline(ifs, line);) {
			contents.push_back(line);
		}
		return contents;
	}

	void write(const string &path, const list_t &contents)
	{
		std::ofstream ofs;
		if (exists(path)) {
			ofs.open(append_path(path), std::ios_base::app);
		} else {
			ofs.open(append_path(path));
		}
		for (auto it = contents.begin(); it != contents.end(); ++it) {
			ofs << get<const char*>(*it) << std::endl;
		}
	}

	bool exists(const string &path)
	{
		return fs::exists(append_path(path), last_ec);
	}

	void copy(const string &path, const string &target)
	{
		fs::copy(append_path(path), append_path(target));
	}

	// should this be atomic?
	void move(const string &path, const string &target)
	{
		copy(path, target);
		remove(path);
	}

	void remove(const string &path)
	{
		if (!fs::remove(append_path(path))) {
			throw fs::filesystem_error("cannot remove", append_path(path), last_ec);
		}
	}

	void make_dir(const string &path)
	{
		fs::create_directory(append_path(path));
	}

	list_t list_dir(const string &path)
	{
		list_t entries;
		for (const auto &dir_ent : fs::directory_iterator(append_path(path))) {
			entries.push_back(dir_ent.path().string());
		}
		return entries;
	}

	double get_file_size_MB(const string &path)
	{
		return fs::file_size(append_path(path));
	}

	string build_path(const list_t &paths)
	{
		fs::path ret;
		for (auto it = paths.begin(); it != paths.end(); ++it) {
			ret /= get<const char*>(*it);
		}
		return ret.string();
	}

private:
	fs::path append_path(const string &path)
	{
		return root_dir / path;
	}

	fs::path discover_root_dir()
	{
		char *root = getenv("APAMA_FILEPLUGIN_ROOT_DIR");
		string ret;
		if (root) {
			ret = root;
		} else {
			ret = getenv("APAMA_WORK");
		}
		return ret;
	}

	std::error_code last_ec;
	fs::path root_dir;
};

APAMA_DECLARE_EPL_PLUGIN(FilePlugin)

#endif

