#ifndef CORRELATOR_PLUGINS_FILE_TRANSPORT_H
#define CORRELATOR_PLUGINS_FILE_TRANSPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>

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

class FileTransport : public EPLPlugin<FileTransport>
{
public:
	FileTransport() 
		: base_plugin_t("FilePluginPlugin"), root_dir(discover_root_dir())
	{}
	~FileTransport() {}

	static void initialize(base_plugin_t::method_data_t &md)
	{
		md.registerMethod<decltype(&FileTransport::get_root_dir), &FileTransport::get_root_dir>("get_root_dir", "action<> returns string");
		md.registerMethod<decltype(&FileTransport::read), &FileTransport::read>("read", "action<string> returns sequence<string>");
		md.registerMethod<decltype(&FileTransport::write), &FileTransport::write>("write", "action<string, sequence<string> >");
		md.registerMethod<decltype(&FileTransport::exists), &FileTransport::exists>("exists", "action<string> returns boolean");
		md.registerMethod<decltype(&FileTransport::copy), &FileTransport::copy>("copy", "action<string, string>");
		md.registerMethod<decltype(&FileTransport::move), &FileTransport::move>("move", "action<string, string>");
		md.registerMethod<decltype(&FileTransport::remove), &FileTransport::remove>("remove", "action<string, string>");
		md.registerMethod<decltype(&FileTransport::make_dir), &FileTransport::make_dir>("mkdir", "action<string>");
		md.registerMethod<decltype(&FileTransport::remove_dir), &FileTransport::remove_dir>("rmdir", "action<string>");
		md.registerMethod<decltype(&FileTransport::list_dir), &FileTransport::list_dir>("ls", "action<string> returns sequence<string>");
		md.registerMethod<decltype(&FileTransport::get_file_size_MB), &FileTransport::get_file_size_MB>("get_file_size_MB", "action<string> returns float");
	}

	string get_root_dir() { return root_dir; }

	list_t read(const string &path)
	{
		list_t contents;
		ifstream ifs(build_path(path));
		for(string line; std::getline(ifs, line);) {
			contents.push_back(line);
		}
		return contents;
	}

	void write(const string &path, const list_t &contents)
	{
		std::ofstream ofs(build_path(path));
		for(auto it = contents.begin(); it != contents.end(); ++it) {
			ofs << get<const char*>(*it) << std::endl;
		}
	}

	bool exists(const string &path)
	{
		struct stat buffer;
		return (stat (build_path(path).c_str(), &buffer) == 0);
	}

	void copy(const string &path, const string &target)
	{
		ifstream source(build_path(path), std::ios::binary);
		ofstream dest(build_path(target), std::ios::binary);
		dest << source.rdbuf();
	}

	// should this be atomic?
	void move(const string &path, const string &target)
	{
		copy(path, target);
		remove(path);
	}

	void remove(const string &path)
	{
		if (std::remove(build_path(path).c_str()) != 0) { throw std::runtime_error("Unable to remove file: " + build_path(path)); }
	}

	void make_dir(const string &path)
	{
		if (mkdir(build_path(path).c_str(), 0777) != 0) { throw std::runtime_error("Unable to make directory: " + build_path(path)); }
	}

	void remove_dir(const string &path)
	{
		if (rmdir(build_path(path).c_str()) != 0) { throw std::runtime_error("Unable to remove directory: " + build_path(path)); }
	}

	list_t list_dir(const string &path)
	{
		list_t entries;
		DIR *dir;
		struct dirent *ent;
		
		if ((dir = opendir(build_path(path).c_str())) == nullptr) { throw std::runtime_error("Could not find directory: " + build_path(path)); }
		while ((ent = readdir(dir)) != nullptr)
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) { continue; }
			entries.push_back(ent->d_name);
		}
		closedir(dir);

		return entries;
	}

	double get_file_size_MB(const string &path)
	{
		ifstream ifs(build_path(path), std::ios::binary | std::ios::ate);
		return ifs.tellg() / (1024.0 * 1024.0);
	}

private:
	string build_path(const string &path)
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

APAMA_DECLARE_EPL_PLUGIN(FileTransport)

#endif

