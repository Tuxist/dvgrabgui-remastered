################################################################################
# Building/Installing script for DVGrabGUI.
# Copyright (C) 2006, 2007, 2008 Bastien Montagne <montagne29@wanadoo.fr>
# Version: 0.9.3
################################################################################

import os
import glob

if 'debug' in COMMAND_LINE_TARGETS:
	debug=1
else:
	debug=0

wx='2.6'
if 'wx-2.6' in COMMAND_LINE_TARGETS:
	wx='2.6'
elif 'wx-2.8' in COMMAND_LINE_TARGETS:
	wx='2.8'
elif 'wx-3.0' in COMMAND_LINE_TARGETS:
	wx='3.0'

#################################################
# Environment creation.
#################################################
# Options, path of miscelleneaous files used by DVGrabGUI, and doc files
if debug:
	opt = Options('user_options_debug_wx-' + wx + '.py')
else:
	opt = Options('user_options_wx-' + wx + '.py')

#opt.Add('DESTDIR', 'the "DESTDIR" var for debian package building (empty by default!)', '')
opt.AddOptions(('overrideCCFLAGS', """the compiler options, set it to empty to use the """
                                   """default ones for each target:\n"""
                                   """          * target "default": '-O2 -g0 -Wall';\n"""
                                   """          * target "debug": '-O0 -g3 -Wall'.""", ''),
               PathOption('wxInclude', 'where are stored the wx includes files', '/usr/include/wx-' + wx),
               PathOption('wxLib', 'where are stored the wx lib files', '/usr/lib'),
               ('baseInstallDir', """the 'root' directory for real install target (should be empty,\n"""
                                  """unless building a package, DEB, RPM, ...).""", ''),
               ('binDir', 'the binary directory for installation', 'usr/local/bin'),
               ('desktopDir', 'the directory where to store *.desktop file', 'usr/local/share/applications'),               
               ('pixmapsDir', 'the directory where to store icon files', 'usr/local/share/pixmaps'),               
               ('dvgrabguiSDir', 'where to store langs and misc files used by DVGrabGUI', 'usr/local/share'),
               ('dvgrabguiDDir', 'where to store doc files about/used by DVGrabGUI,', 'usr/local/share/doc'),
               BoolOption('libjpeg', 'wether we can build with jpeg support (set automatically, should not be changed)', 'no'),
               BoolOption('checks', 'yes to make the "configure-like" checks', 'yes'))
if debug:
	opt.AddOptions(('buildDir', 'where to build this program, use it for multiple builds', 'build-dbg-wx-' + wx),
	               ('binName', 'the name of the program file', 'dvgrabgui-dbg-wx-' + wx),
	               PathOption('wxSetupInclude', 'where is stored the wx "setup.h" file',
                             '/usr/lib/wx/include/gtk2-unicode-debug-' + wx))
else:
	opt.AddOptions(('buildDir', 'where to build this program, use it for multiple builds', 'build-wx-' + wx),
	               ('binName', 'the name of the program file', 'dvgrabgui-wx-' + wx),
	               PathOption('wxSetupInclude', 'where is stored the wx "setup.h" file',
                      '/usr/lib/wx/include/gtk2-unicode-release-' + wx))

dvsd = '\'_T("$dvgrabguiSDir/dvgrabgui/")\''
dvdd = '\'_T("$dvgrabguiDDir/dvgrabgui/")\''
if debug:
	desktop_file = 'dvgrabgui-dbg-wx-' + wx + '.desktop'
else:
	desktop_file = 'dvgrabgui-wx-' + wx + '.desktop'

# Create the environments
env = Environment(options = opt)

#################################################
# "Configure-like" checks.
#################################################
# If the 'checks' option is set, do 'configure-like' checks; only done once if
# successful (the 'checks' state is stored in the 'user_options.py' file).

# Two special checks, for wxWidgets and libjpeg include files.
def CheckLibjpegHeader(context):
	context.Message('Checking for jpeglib.h (for libjpeg)...')
	result = context.TryLink('#include <stdio.h>\n#include <sys/types.h>\n#include "jpeglib.h"', '.h')
	context.Result(result)
	return result

#not used (very tricky to check wxWidgets headers!)
#def CheckWxHeaders(context):
#	context.Message('Checking for wx.h and setup.h files (for wxWidgets)...')
#	context.env.Append(CPPPATH   =['$wxInclude', '$wxSetupInclude'],
#	                   CPPDEFINES=['__GNUG__', '__WXGTK__', 'WX_PRECOMP', 'WXUSINGDLL'])
#	result = context.TryLink(FIND_A_CODE_WORKING, '.cpp')
#	context.Result(result)
#	return result

if env['checks']:
	# autoconf-like checks
	conf = Configure(env, custom_tests={'CheckLibjpegHeader':CheckLibjpegHeader})
#	                                    'CheckWxHeaders':CheckWxHeaders})
	exit = 0
	print ('Assuming includes files of wxWidgets are in "' + env['wxInclude']
	        + '/wx/",\nand in "' + env['wxSetupInclude'] + '/wx/" for setup.h,\n'
	        + 'because it is quite difficult to test it!\nThis just means you must'
	        + 'have installed the "-dev" packages for your wxWidgets version!')
#not used (very tricky to check wxWidegets headers!)
#	if not conf.CheckWxHeaders():
#		print ('wxWidgets must be installed with its dev- packages'
#		       + '(not found "' + env['wxInclude'] + '/wx/wx.h" '
#		       + 'or "' + env['wxSetupInclude'] + '/wx/setup.h")')
#		exit = 1
	if debug:
		if not conf.CheckLib('wx_baseud-' + wx, language='c++', autoadd=0):
			print 'wxWidgets (debug version) must be installed with its dev- packages (not found "libwx_baseud-' + wx + '.so")'
			exit = 1
		if not conf.CheckLib('wx_gtk2ud_core-' + wx, language='c++', autoadd=0):
			print 'wxWidgets (debug version) must be installed with its dev- packages (not found "libwx_gtk2ud_core-' + wx + '.so")'
			exit = 1
		if not conf.CheckLib('wx_gtk2ud_xrc-' + wx, language='c++', autoadd=0):
			print 'wxWidgets (debug version) must be installed with its dev- packages (not found "libwx_gtk2ud_xrc-' + wx + '.so")'
			exit = 1
	else:
		if not conf.CheckLib('wx_baseu-' + wx, language='c++', autoadd=0):
			print 'wxWidgets must be installed with its dev- packages (not found "libwx_baseu-' + wx + '.so")'
			exit = 1
		if not conf.CheckLib('wx_gtk3u_core-' + wx, language='c++', autoadd=0):
			print 'wxWidgets must be installed with its dev- packages (not found "libwx_gtk2u_core-' + wx + '.so")'
			exit = 1
		if not conf.CheckLib('wx_gtk3u_xrc-' + wx, language='c++', autoadd=0):
			print 'wxWidgets must be installed with its dev- packages (not found "libwx_gtk2u_xrc-' + wx + '.so")'
			exit = 1
	if not conf.CheckLibWithHeader('dv', 'libdv/dv.h', 'c++', autoadd=0):
		print 'libdv must be installed with its dev- packages (not found "libdv.so" or "libdv/dv.h")'
		exit = 1
	if (conf.CheckLibjpegHeader() and  conf.CheckLib('jpeg', language='c++', autoadd=0)):
		print 'libjpeg found, enabling jpeg file writing feature.'
		conf.env['libjpeg']=1
	if not conf.CheckLibWithHeader('iec61883', 'libiec61883/iec61883.h', 'c++', autoadd=0):
		print 'libiec61883 must be installed with its dev- packages (not found "libiec61883.so" or "libiec61883/iec61883.h")'
		exit = 1
	if not conf.CheckLibWithHeader('raw1394', 'libraw1394/raw1394.h', 'c++', autoadd=0):
		print 'libraw1394 must be installed with its dev- packages (not found "libraw1394.so" or "libraw1394/raw1394.h")'
		exit = 1
	if not conf.CheckLibWithHeader('avc1394', 'libavc1394/avc1394.h', 'c++', autoadd=0):
		print 'libavc1394 must be installed with its dev- packages (not found "libavc1394.so" or "libavc1394/avc1394.h")'
		exit = 1
	if not conf.CheckLibWithHeader('rom1394', 'libavc1394/rom1394.h', 'c++', autoadd=0):
		print 'libavc1394 must be installed with its dev- packages (not found "librom1394.so" or "libavc1394/rom1394.h")'
		exit = 1
	if not conf.CheckLibWithHeader('z', 'zlib.h', 'c++', autoadd=0):
		print 'libz must be installed with its dev- packages (not found "libz.so" or "zlib.h")'
		exit = 1
	if not conf.CheckLibWithHeader('dl', 'dlfcn.h', 'c++', autoadd=0):
		print 'libdl must be installed with its dev- packages (should be in a "libc6" package!) (not found "libdl.so" or "dlfnc.h")'
		exit = 1
	if not conf.CheckLibWithHeader('m', 'math.h', 'c++', autoadd=0):
		print 'libm must be installed with its dev- packages (should be in a "libc6" package!) (not found "libm.so" or "math.h")'
		exit = 1
	if exit:
		print 'Checks errors, aborting...'
		Exit(1)
	env = conf.Finish()
	# If successful, don't do it again.
	env['checks'] = 0

#################################################
# Build options.
#################################################
env.Append(CPPPATH   =['$wxInclude', '$wxSetupInclude'],
           CPPDEFINES=['__WXGTK__', 'WX_PRECOMP', 'WXUSINGDLL',
                       'HAVE_LIBDV', 'HAVE_LIBDV_1_0', 'WITH_LIBDV',
                       ('DVGRABGUI_SETTING_PATH', dvsd),
                       ('DVGRABGUI_DOC_PATH', dvdd),
                       'NO_GCC_PRAGMA', ('_FILE_OFFSET_BITS', 64), '_LARGE_FILES'],
           LIBPATH   =['/usr/lib', '$wxLib'],
           LIBS      =['dv', 'iec61883', 'raw1394', 'avc1394', 'rom1394', 'z', 'dl', 'm'],
           LINKFLAGS =['-pthread', '-Wl,-rpath,$wxLib'])

if env['libjpeg']:
	env.Append(CPPDEFINES=['HAVE_LIBJPEG'], LIBS=['jpeg'])

if debug:
	env.Append(CCFLAGS   =['-O0', '-g3', '-Wall'],
	           CPPDEFINES=['__WXDEBUG__'],
	           LIBS      =['wx_baseud-' + wx, 'wx_gtk2ud_core-' + wx, 'wx_gtk2ud_xrc-' + wx])
else:
	env.Append(CCFLAGS   =['-O2', '-g0', '-Wall'],
	           LIBS      =['wx_baseu-' + wx, 'wx_gtk2u_core-' + wx, 'wx_gtk2u_xrc-' + wx])

if env['overrideCCFLAGS'] != '':
	env.Replace(CCFLAGS = env['overrideCCFLAGS'].split())

#################################################
# i18n install stuff.
#################################################
# Finding all GUI languages available.
gui_langs=[]
temp=glob.glob('langs/[a-z][a-z]_*/')
for t in temp:
	gui_langs.append(t[6:-1])

# Finding all Help languages available.
help_langs=[]
temp=glob.glob('help/[a-z][a-z]_*/')
for t in temp:
	help_langs.append(t[5:-1])

# Listing (in a hash map, with [lang_code] as key) all help files.
help_files={}
help_picts={}
for h in help_langs:
	help_files[h]=glob.glob('help/' + h + '/*')
	help_picts[h]=glob.glob('help/' + h + '/*/*.*')
	
#################################################
# Default target (alias with 'debug' and 'wx-2.x' targets).
#################################################
# building the program from the SConscript in src/ directory, to the '$buildDir' directory.
prog = env.SConscript('src/SConscript', exports='env', build_dir='$buildDir', duplicate=0)
#symlink = env.Command('$buildDir/dvgrabgui', prog, "cd '$buildDir'; ln -s '$binName' './dvgrabgui'; cd ..")
env.Default(prog)
env.Alias('debug', prog)
env.Alias('wx-2.6', prog)
env.Alias('wx-2.8', prog)

#################################################
# 'install' target.
#################################################
aliases_bin=[env.Alias('install-bin', env.Install('$baseInstallDir/$binDir', [prog])),]
#env.Install('$baseInstallDir/$binDir', [prog])#, symlink])
aliases_desktop=[env.Alias('install-desktop', env.Install('$baseInstallDir/$desktopDir', desktop_file)),]
aliases_pixmaps=[env.Alias('install-pixmaps', env.Install('$baseInstallDir/$pixmapsDir', 'dvgrabgui32x32.xpm')),]
aliases_resources=[env.Alias('install-resources',
                             env.Install('$baseInstallDir/$dvgrabguiSDir/dvgrabgui',
                                         ['beep.wav', 'dvgrabgui16x16.xpm', 'dvgrabgui32x32.xpm'])),]
#env.Install('$baseInstallDir/$dvgrabguiSDir/dvgrabgui', ['beep.wav', 'dvgrabgui16x16.xpm', 'dvgrabgui32x32.xpm'])
# All languages messages files (using a list of aliases, to be "re-aliased" to "install" target).
aliases_gui_langs=[]
for g in gui_langs:
	aliases_gui_langs.append(env.Alias('install-guilangs_' + g,
	                                   env.Install('$baseInstallDir/$dvgrabguiSDir/dvgrabgui/langs/' + g,
	                                               ['langs/' + g + '/dvgrabgui.mo'])))
# All doc/help files in all languages (using a list of aliases, to be "re-aliased" to "install" target).
aliases_help_langs=[]
for h in help_langs:
	a=env.Install('$baseInstallDir/$dvgrabguiDDir/dvgrabgui/' + h, ['README_' + h + '.txt'])
	b=env.Install('$baseInstallDir/$dvgrabguiDDir/dvgrabgui/' + h + '/help', help_files[h])
	c=env.Install('$baseInstallDir/$dvgrabguiDDir/dvgrabgui/' + h + '/help/pict', help_picts[h])
	aliases_help_langs.append(env.Alias('install-help_' + h, [a, b, c]))
# Finally, regroup everything in top "install" alias.
env.Alias('install', aliases_bin + aliases_desktop + aliases_pixmaps
                     + aliases_resources + aliases_gui_langs + aliases_help_langs)
# For 'install' target, when doing cleanup, also erase this folders.
env.Clean('install', ['$baseInstallDir/$dvgrabguiSDir/dvgrabgui', '$baseInstallDir/$dvgrabguiDDir/dvgrabgui'])

#################################################
# Help message
#################################################
Help("""File SConstruct for compiling/installing DVGrabGUI\n"""
     """Usage: '$ scons' for building;\n"""
     """       '$ scons -c' for cleaning the previous build;\n"""
     """       '# scons install' for installing dvgrabgui;\n"""
     """       '# scons -c install' for uninstalling dvgrabgui.\n"""
     """Append 'debug' for a debug build/install (WARNING: using "debug"\n"""
     """version of wxWidgets!)\n"""
     """By default, built against wxWidgets 2.6.x: to use wxWidgets 2.8.x,\n"""
     """append 'wx-2.8' to use wxWidgets 2.8.x instead.\n"""
     """Note: You must have wxWidgets available at standard location,\n"""
     """      in 2.6.x / 2.8.x unicode shared version.\n"""
     """      You also need libdv, libiec61883, libraw1394, libz,\n"""
     """      (libdl and libm)=libc6, all with dev- packages installed.\n"""
     """Upgrade: You should not have any problems, provided you set the same\n"""
     """         install path (WARNING: by default, is now installing DVGrabGUI\n"""
     """         under the '/usr/local' dir - to be more "linux compliant"! - modify\n"""
     """         the new 'baseDir' option described below).\n"""
     """         The config file (under '/home/<your id>/.dvgrabgui' should be updated\n"""
     """         automatically at first restart of the new version 0.9.3!\n"""
     """Options:\n""" + opt.GenerateHelpText(env))

#################################################
# Saving options.
#################################################
if debug:
	opt.Save('user_options_debug_wx-' + wx + '.py', env)
else:
	opt.Save('user_options_wx-' + wx + '.py', env)
