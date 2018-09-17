#ifndef CORRELATOR_PLUGINS_FILE_TRANSPORT_H
#define CORRELATOR_PLUGINS_FILE_TRANSPORT_H

#include <epl_plugin.hpp>


class FileTransport : public com::apama::epl::EPLPlugin<FileTransport>
{
public:
	FileTransport() : base_plugin_t("FileTransportPlugin") {}
	~FileTransport() {}

	static void initialize(base_plugin_t::method_data_t &md)
	{

	}

};

APAMA_DECLARE_EPL_PLUGIN(FileTransport)

#endif

