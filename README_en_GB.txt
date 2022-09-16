--------------------------------------------------------------------------------
               DVGrabGUI v0.9.4 (c) 2006, 2007, 2008 Bastien Montagne.

Author:  Bastien Montagne <montagne29@wanadoo.fr>,
         based on dvgrab, by Dan Dennedy and the Kino team.
License: Released under the GNU GPL license, either v2 or any later version.

Write me for any remark/suggestion... Be kind, it's my first program!
--------------------------------------------------------------------------------

I   - Quickstart help
      a) installation
      b) update notes
      c) usage

II  - Todo list / Known bugs

III - Release history

---------------------

I - Quickstart help:

  a) Installation:
    I just write a little scons script for building/installing this program, so
    you'll need:
        - the Scons tool;
        - the wxWidgets library, v.2.6.x / 2.8.x, in a shared unicode (and
          non-monolithic) build, with the includes files (dev packages);
        - the libdv, libiec61883, libraw1394, libavc1394, libz, (libdl, libm) = libc6,
          with there includes files (dev package).
    If any of the above library is installed in an non-standard directory,
    you'll have to modify it's path in the SConstruct script, except for wxWidgets:
        you have the 'wxLib' option for its libpath,
                 the 'wxInclude' option for its common includes path,
            	  the 'wxSetupInclude' option for its 'wx/setup.h' file path.

    Go in the directory where you unpacked the archive, and type:
        $ scons
        $ su
        # scons install
        
    The bin name is something like 'dvgrabgui(-dbg)-wx-2.x', depending the
    wxWidgets version and the normal or debug build (see below).

    Note: In recent systems (e.g. Debian Etch, Fedora 7, 8, etc.), you have to
          run this app as root (standard users can't use firewire, it seems...).
    
    For uninstalling, type:
        $ su
        # scons -c install
    Note that (config) files created for each user that used this program
    (like '/root/.dvgrabgui/*' and '/home/<user>/.dvgrabgui/*') are not removed:
    you must delete these directories yourself.
    
    By default, dvgrabgui is built against wxWidgets 2.6.x: if you want to use
    wx-2.8.x, use the 'wx-2.8' target:
        $ scons wx-2.8
        $ su
        # scons install wx-2.8
    and for uninstalling it,
        $ su
        # scons -c install wx-2.8

    If you want to build a non-optimised dvgrabgui with debug symbols, use the
    'debug' target:
        $ scons debug [wx-2.x]
        $ su
        # scons install debug [wx-2.x]
    and for uninstalling it,
        $ su
        # scons -c install debug [wx-2.x]
    
    Note that you can build and install both against wx-2.6 and wx-2.8,
    and both normal and debug version of dvgrabgui (each target being built
    in a separate dir by default, like 'build(-dbg)-wx-2.x', with a different
    name, like 'dvgrabgui(-dbg)-wx-2.x')!

    You can make different builds by giving different directories via the
    'buildDir' option.

    The 'configure-like' checks (checking for the libraries needed) are by
    default only done once if successful; you can force (or disable) these
    checks with the 'checks' option.

    For other (build, install) options, type:
        $ scons --help

    Note that build options are stored in a file, so if you have to build
    multiple times, you won't have to retype all options each time! To reset
    the build options, just delete the file(s) 'user_options_wx-2.6.py' and/or
    'user_options_wx-2.8.py' and/or 'user_options_debug_wx-2.6.py' and/or
    'user_options_debug_wx-2.8.py', depending the target(s) you used/want to clean.

    For those interested by the code, before reading it, you can build the
    doxygen generated doc (you need the 'doxygen' tool installed!), just type:
        $ doxygen
    and open ./doxydoc/html/index.html!

  b) Update notes:
    You should not have any problems: the installation should just override the
    existing files! Note, however, that dvgrabgui is now INSTALLED BY DEFAULT
    UNDER '/USR/LOCAL/', and no more under '/usr/', to be more 'linux-compliant'!
    You should therefore set the 'baseDir' option to '/usr' if you want to
    override older (default) install.
    The config files in '~/.dvgrabgui' will be automatically updated at first
    launch of dvgrabgui 0.9.4.

  c) Usage:
    I coded this program because I needed a dv recording software witch I could
    control Rec/Pause command with the mouse buttons; the typical use is on a
    laptop:
        - Plug in the dv camera and the external mouse (put a piece of tape on
          the LED);
        - Launch DVGrabGUI, adjust the settings and start the capture thread;
        - With the touch-pad, place the mouse pointer on the video window, close
          the laptop screen (for saving batteries! Do not forget to uncheck the
          "show video" control... saving you cpu usage!);
        - Go to stand-by mode (right button + left button);
        - To record, press left button (there is a 'beep' sound as long as you
          are recording), press the right button to pause;
        - A new video file is created each time you press left button;
        - To leave stand-by mode, repress right button + left button;
    This was designed to "forget" the laptop in a backpack (for example) while
    shooting, and to keep a certain mobility.
    You can store your preferred settings in files, or make them defaults.
    This new version is also designed to be used with a small touchscreen (there
    is a special GUI mode for small screens - 800x600).
    
    See the help (F1 from the GUI) for more details!

---------------------

II - Todo list:

    * This should be the last version of this app: I plan to develop a new one,
      based on GStreamer - should be easier to maintain & upgrade!

   - Known bugs:

    * On recent systems, you must use dvrabgui as "root" (firewire devices seem
      unusable by regular users?!?).
    * If you are using recent version of your OS (like Fedora 7 or 8), this
      program might not work at all (like dvgrab): there are problems with the
      firewire system!
      It seems it had been resolved in Fedora 9 (kernel 2.6.25).
      Note that this version was developed under Debian Etch (4.0).
    * If, while the capture thread is starting, it halt immediately, and there are
      the following messages:
      "ERROR: invalid DV source specified" and
      "ERROR: DV reader couldn't be initialised",
      there is a problem with your libavc1394: you have to select the dv device
      in the dialog box "Options". ("GUID" choice box)
    * If, while the capture thread is starting, the program shuts down without
      any warning, try to unset the "Show video" check box (there is a problem
      with your libdv!).

---------------------

III - Release history:

    * v.0.9 (11/2006):
      First release.

    * v.0.9.1 (12/2006):
      Added support for multi-languages, and immediate effect on shortcuts when
      loading a new settings file.
      Corrected the continuous small processor usage due to periodical GUI
      refreshing.
      Shorcuts tab codded in the Options Dialog box.

    * v.0.9.2 (12/2006):
      No more problems with dv_decode_full_frame in the new libdv version
      (new Fedora Core, v6).
      Fixed problem of the "max frame to save" option.
      Added control on GUI fonts.
      Added control on the "beep" sound when recording.

    * v.0.9.2b (01/2007):
      Fixed build problem with new wxWidgets 2.8.

    * v.0.9.2c (02/2007):
      Small fixes, better handle of lack of def conf file.
      First version available in RPM package.

    * v.0.9.3 (04/2008):
      Major changes for the GUI.
      Buids with both wxWidgets 2.6.x and 2.8.x.
      Fixed the bug with the "beep" sound (wxWidgets seems to have problems
      looping sounds!).
      Note that the capture code hasn't changed (i.e. no update from dvgrab 3).
      DEB package.
      Should be the last release.
      
    * v.0.9.4 (08/2008):
      Minor changes in the build system.
      Bug correction in the GUI (under Fedora 9 / wxWidgets 2.8.7): the controls
      created before the wxStaticBoxSizer in witch they are to be included are
      not drawn at all (though they take room, and react as expected when
      clicked!).
