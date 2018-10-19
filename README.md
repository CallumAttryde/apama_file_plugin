# Apama FilePlugin [![Build Status](https://travis-ci.org/CallumAttryde/apama_file_plugin.svg?branch=master)](https://travis-ci.org/CallumAttryde/apama_file_plugin)

Apama EPL Plugin for interacting with the filesystem from within Apama. Linux & Windows

## Building the plugin

To build the plugin, first ensure you have sourced the `apama_env` script in your current shell. This script can be found inside the `bin` directory of your Apama installation. 

### Using xpybuild

[xpybuild](https://github.com/xpybuild/xpybuild) is a python2 based cross platform build system. If you do not already have xpybuild installed, a submodule is provided in this repository which can be initialised with the following command:

    git submodule init && git submodule update
    
Then, run the xpybuild script from the top level:

    xpybuild/xpybuild.py
    
This will build the plugin and copy the resulting library to your `APAMA_WORK/lib` directory. Many options are available for customising builds; please see the xpybuild doc or --help text for further information.

### Using make

A makefile is also provided for users without python. To build, navigate to the `plugin` directory and execute the following commands:

    make;
    make install;
    
This will build the plugin and copy the resulting library to your `APAMA_WORK/lib` directory.

### Building the doc

To build the documentation, execute the following commands from the top level:

    java -jar $APAMA_HOME/lib/ap-generate-apamadoc.jar plugin/doc plugin/

## Running the tests

To run the the tests, first ensure you have sourced the `apama_env` script in your current shell. This script can be found inside the `bin` directory of your Apama installation. Next, navigate to the `tests` directory and execute the following commands:

    pysys run;
    
## Using the plugin

To use the plugin, first inject `FilePlugin.mon`, then use the static actions on the `com.apamax.plugins.Filesystem` event to perform various filesystem operations. All actions throw an exception upon error.

For more details on the full range of functionality provided, please consult the API documentation.
