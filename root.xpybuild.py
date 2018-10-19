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

if isWindows():
	VSROOT=r'c:\Program Files (x86)\Microsoft Visual Studio 14.0'
	assert os.path.exists(VSROOT), 'Cannot find Visual Studio installed in: %s'%VSROOT
	setGlobalOption('native.compilers', VisualStudio(VSROOT+r'\VC\bin\amd64'))
	setGlobalOption('native.include', [r"%s\VC\ATLMFC\INCLUDE" % VSROOT, r"%s\VC\INCLUDE" % VSROOT, r"C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt"])
	setGlobalOption('native.libpaths', [r"%s\VC\ATLMFC\LIB\amd64" % VSROOT, r"%s\VC\LIB\amd64" % VSROOT, r"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x64", r"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64"])
	setGlobalOption('native.cxx.flags', ['/EHa', '/GR', '/O2', '/Ox', '/Ot', '/MD', '/nologo'])
else:
	setGlobalOption('native.compilers', GCC())
	setGlobalOption('native.cxx.flags', ['-fPIC', '-O3', '--std=c++14'])

Cpp(objectname('${BUILD_WORK_DIR}/FilePlugin'), 'plugin/file_plugin.cpp',
	includes=[
		'${APAMA_HOME}/include',
	])

# really we only want stdc++fs if using GCC, fix later
platform_libs = [] if isWindows() else ['stdc++fs']

Link(libname('${APAMA_WORK}/lib/FilePlugin'),
		[objectname('${BUILD_WORK_DIR}/FilePlugin')],
		libpaths=[
			'${APAMA_HOME}/lib'
		],
		libs=[
			'apclient',
		] + platform_libs,
		shared=True)

Copy('${APAMA_WORK}/monitors/FilePlugin.mon', 'plugin/FilePlugin.mon')
