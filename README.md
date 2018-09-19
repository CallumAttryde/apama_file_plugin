# Apama FilePlugin [![Build Status](https://travis-ci.org/CallumAttryde/apama_file_transport.svg?branch=master)](https://travis-ci.org/CallumAttryde/apama_file_transport)

Apama EPL Plugin for interacting with the filesystem from within Apama. Linux only at present

## Building the plugin

To build the plugin, first ensure you have sourced the `apama_env` script in your current shell. This script can be found inside the `bin` directory of your Apama installation. Next, navigate to the `plugins` directory and execute the following commands:

    make;
    make install;
    
This will build the plugin and copy the resulting library to your `APAMA_WORK/lib` directory
