Name: dvgrabgui
Summary: Capture dv video with a GUI app.
Version: 0.9.4
Release: 1
License: GPL
Group: Applications/Multimedia
URL: http://dvgrabgui.sourceforge.net/
Source0: %{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: wxGTK >= 2.8, libdv, libiec61883, libraw1394, libavc1394, zlib, glibc
BuildPrereq: scons, wxGTK-devel >= 2.8, libdv-devel, libiec61883-devel, libraw1394-devel, libavc1394-devel, zlib-devel, glibc-devel

%description
DVGrabGUI is based on the dvgrab code. It adds a graphical user interface, based
on wxWidgets, the visualization of the dv stream, and the possibility of
begining/pausing record just with the mouse buttons (i.e. without controlling
its position on screen). This is desingned to 'forget' the laptop in a backpack,
and just use the mouse to record/pause while shooting with the scope in the wilde!
Sould be the last version of this app!

%prep
%setup -q

%build
scons checks=yes baseInstallDir=%{buildroot} binDir=%{_bindir} \
      desktopDir='%{_datadir}/applications' pixmapsDir='%{_datadir}/pixmaps' \
      dvgrabguiSDir=%{_datadir} dvgrabguiDDir=%{_defaultdocdir} wx-2.8

%install
rm -fr %{buildroot}
mkdir %{buildroot}
# using same params as "build" call...
scons install wx-2.8
# ugly trick to create a nice shortcut...
# Help! how to make an install-time shortcut with rpm???
#ln -s %{buildroot}/%{_bindir}/dvgrabgui* %{buildroot}/%{_bindir}/dvgrabgui
# Remove all .sconsign files (rpmbuild doesn't like them...)
# No more needed, it seems...
#rm -f $(find %{buildroot} -name .sconsign)

%clean
scons -c checks='no' wx-2.8
rm -fr %{buildroot}/*

%post

%preun

%postun
# ugly trick to remove the nice shortcut...
# Help! how to make an install-time shortcut with rpm???
#rm -f %{buildroot}/%{_bindir}/dvgrabgui

%files
%defattr(-,root,root,-)
#%doc README_en.html, README_en.txt, README_es.html, README_es.txt, README_fr.html, README_fr.txt
%{_bindir}/*
%{_datadir}/applications/*
%{_datadir}/pixmaps/*
%{_datadir}/dvgrabgui
%{_defaultdocdir}/dvgrabgui

%changelog

* Sat Aug 22 2008 Bastien Montagne <montagne29@wanadoo.fr>
- Second RPM release, from new "upstream" version (should be the last).
- New improved GUI.
- Fixed the bug with the "beep" sound (wxWidgets seems to have problems looping sounds!).

* Sat Feb 10 2007 Bastien Montagne <montagne29@wanadoo.fr>
- First RPM release.

