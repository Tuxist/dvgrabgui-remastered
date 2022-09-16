project(dvgrabgui)

cmake_minimum_required(VERSION 3.22)                                      

find_package(wxWidgets REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS})

set(CMAKE_WARN_DEPRECATED FALSE)

add_definitions(
	-DDVGRABGUI_SETTING_PATH="${CMAKE_INSTALL_PREFIX}/share/dvgrabgui/"
	-DDVGRABGUI_DOC_PATH="${CMAKE_INSTALL_PREFIX}/share/doc/dvgrabgui/"
	-DHAVE_LIBDV
)

include(
	${wxWidgets_USE_FILE}
)

include_directories(
	${wxWidgets_INCLUDE_DIRS}
)

set(uiSrcs
	src/avi.cpp          
	src/mainframe.cpp       
	src/smiltime.cpp
	src/dvgrab.cpp       
	src/mainwindow.cpp      
	src/stringutils.cpp
	src/filehandler.cpp  
	src/raw1394util.cpp     
	src/theapp.cpp
	src/frame.cpp        
	src/riff.cpp            
	src/vidwin.cpp
	src/ieee1394io.cpp   
	src/settingswindow.cpp
)

add_executable(
	dvgrabgui ${uiSrcs}
)

target_link_libraries(
	dvgrabgui
	${wxWidgets_LIBRARIES}
	pthread
	dv
	iec61883
	raw1394
	avc1394
	rom1394
	z
	dl
	m
)

install(
	FILES
		beep.wav
		dvgrabgui16x16.xpm
		dvgrabgui32x32.xpm
	DESTINATION
		share/dvgrabgui
)

install(
	DIRECTORY
		help
		langs
	DESTINATION
		share/dvgrabgui
)

configure_file(dvgrabgui.desktop.in dvgrabgui.desktop)

install(
	FILES
		${CMAKE_BINARY_DIR}/dvgrabgui.desktop
	DESTINATION
		share/applications
)

install(
	TARGETS
		dvgrabgui
	DESTINATION
		bin
)
