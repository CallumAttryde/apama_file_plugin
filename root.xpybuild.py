# xpyBuild - eXtensible Python-based Build System
#
# Copyright (c) 2013 - 2018 Software AG, Darmstadt, Germany and/or its licensors
# Copyright (c) 2013 - 2018 Ben Spiller and Matthew Johnson
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
# $Id: root.xpybuild.py 301527 2017-02-06 15:31:43Z matj $
# Requires: Python 2.7


# xpybuild release build file. Creates pydoc API docs and versioned zip file for releases.

from propertysupport import *
from buildcommon import *
from pathsets import *

from targets.copy import Copy
from targets.custom import CustomCommand
from targets.native import Cpp, Link, objectname, libname

from utils.process import call
from utils.compilers import GCC, VisualStudio

requireXpyBuildVersion('1.12')

defineOutputDirProperty('OUTPUT_DIR', 'release-output')
definePathProperty('APAMA_HOME', os.getenv('APAMA_HOME', None), mustExist=True)
definePathProperty('APAMA_WORK', os.getenv('APAMA_WORK', None), mustExist=True)

setGlobalOption('native.compilers', GCC())
setGlobalOption('native.cxx.flags', ['-fPIC', '-O3', '--std=c++14'])

Cpp(objectname('${BUILD_WORK_DIR}/FilePlugin'), 'plugin/file_plugin.cpp',
	includes=[
		'${APAMA_HOME}/include',
	])

Link(libname('${APAMA_WORK}/lib/FilePlugin'),
		[objectname('${BUILD_WORK_DIR}/FilePlugin')],
		libpaths=[
			'$(APAMA_HOME)/lib'
		],
		libs=[
			'apclient',
			'stdc++fs'
		],
		shared=True)

