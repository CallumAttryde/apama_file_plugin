# Apama FilePlugin [![Build Status](https://travis-ci.org/CallumAttryde/apama_file_transport.svg?branch=master)](https://travis-ci.org/CallumAttryde/apama_file_transport)

Apama EPL Plugin for interacting with the filesystem from within Apama. Linux only at present

## Building the plugin

To build the plugin, first ensure you have sourced the `apama_env` script in your current shell. This script can be found inside the `bin` directory of your Apama installation. Next, navigate to the `plugins` directory and execute the following commands:

    make;
    make install;
    
This will build the plugin and copy the resulting library to your `APAMA_WORK/lib` directory.

### Building the doc

To build the documentation, execute the following commands:

    java -jar $APAMA_HOME/lib/ap-generate-apamadoc.jar plugin/doc plugin/

## Running the tests

To run the the tests, first ensure you have sourced the `apama_env` script in your current shell. This script can be found inside the `bin` directory of your Apama installation. Next, navigate to the `tests` directory and execute the following commands:

    pysys run;
    
## Using the plugin

To use the plugin, first inject `FilePlugin.mon`, then use the static actions on the `com.apamax.plugins.Filesystem` event to perform various filesystem operations. All actions throw an exception upon error.

For more details on the full range of functionality provided, please consult the API documentation.
