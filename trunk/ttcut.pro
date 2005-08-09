#/*.............................................................................*/
#/*..(c) 2003/2005 TriTime......................................................*/
#/*-----------------------------------------------------------------------------*/
#/*..FILE:   QMake Projektfile for ttcut                                      ..*/
#/*-----------------------------------------------------------------------------*/
#/*..ABOUT:                                                                   ..*/
#/*-----------------------------------------------------------------------------*/
#/*..AUTHOR:    b. altendorf              DATE: 21.10.2003         TIME: 20:26..*/
#/*..MODIFIED:  b. altendorf              DATE: 25.10.2003         TIME: 17:30..*/
#/*..MODIFIED:  b. altendorf              DATE: 14.12.2003         TIME: 09:30..*/
#/*..MODIFIED:  b. altendorf              DATE: 23.02.2005         TIME: 11:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 01.03.2005         TIME: 16:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 23.03.2005         TIME: 16:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 13.06.2005         TIME: 09:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 02.07.2005         TIME: 08:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 17.07.2005         TIME: 13:30..*/
#/*..MODIFIED:  b. altendorf              DATE: 05.08.2005         TIME: 12:00..*/
#/*-----------------------------------------------------------------------------*/
#
#
#REQUIRES         = opengl
PROJECT           = TTCUT
CONFIG            += opengl warn_on debug
QT                += qt3support	opengl
DEFINES           += _FILE_OFFSET_BITS=64
DIST              = TTCut
TARGET            = ttcut
LIBS              = -lmpeg2 -lmpeg2convert
# WIN LIBS        = -lglaux -lglu32 -Llibmpeg2/win32/lib -llibmpeg2 -llibmpeg2convert
OBJECTS_DIR       = obj
MOC_DIR           = moc
#
SOURCES           = \
#
#                   -------------------------------------------------------------
#                   compat
#                   -------------------------------------------------------------
#
#                   -------------------------------------------------------------
#                   AVSTREAM
#                   -------------------------------------------------------------
                    avstream/ttcommon.cpp \
                    avstream/ttmemorybuffer.cpp \
                    avstream/ttac3audioheader.cpp \
                    avstream/ttac3audiostream.cpp \
                    avstream/ttaudioheaderlist.cpp \
                    avstream/ttaudioindexlist.cpp \
                    avstream/ttavheader.cpp \
                    avstream/ttavstream.cpp \
                    avstream/ttavtypes.cpp \
                    avstream/ttdtsaudioheader.cpp \
                    avstream/ttdtsaudiostream.cpp \     
                    avstream/ttfilebuffer.cpp \ 
# WIN32             avstream/ttwfilebuffer.cpp \ 
                    avstream/ttheaderlist.cpp \ 
                    avstream/ttmpeg2videoheader.cpp \
                    avstream/ttmpeg2videostream.cpp \   
                    avstream/ttmpegaudioheader.cpp \    
                    avstream/ttmpegaudiostream.cpp \    
                    avstream/ttpcmaudioheader.cpp \     
                    avstream/ttpcmaudiostream.cpp \     
                    avstream/ttvideoheaderlist.cpp \    
                    avstream/ttvideoindexlist.cpp \
	            avstream/ttaviwriter.cpp \
#
#                   -------------------------------------------------------------
#                   GUI
#                   -------------------------------------------------------------
                    gui/ttcutmainwnd.cpp \
                    gui/ttcutsettings.cpp \
                    gui/ttcutavcutdlg.cpp \
                    gui/ttcutquickjump.cpp \
                    gui/ttprogressbar.cpp \
                    gui/ttcutpreview.cpp \
                    gui/ttsearchframe.cpp \
                    gui/ttcutaboutdlg.cpp \
                    gui/ttcutlistview.cpp \
                    gui/ttcutccrwnd.cpp \
                    gui/ttaudiolistview.cpp \
                    gui/ttprocessform.cpp \
                    gui/ttcut.cpp \
                    gui/main.cpp \
#
#                   -------------------------------------------------------------
#                   MPEG2DECODER
#                   -------------------------------------------------------------
                    mpeg2decoder/ttmpeg2decoder.cpp \
#
#                   -------------------------------------------------------------
#                   MPEG2WINDOW
#                   -------------------------------------------------------------
                    mpeg2window/ttmpeg2window.cpp \
#
#                   -------------------------------------------------------------
#                   AVCUT
#                   -------------------------------------------------------------
#                   avcut/tschnittsuche.cpp \
                    avcut/ttavcutposition.cpp \
#
#                   -------------------------------------------------------------
#                   AVILIB
#                   -------------------------------------------------------------
	            avilib/avilib.c \
#
#                   -------------------------------------------------------------
#                   EXTERN
#                   -------------------------------------------------------------
	            extern/tttranscode.cpp
#
HEADERS           = \
#                   -------------------------------------------------------------
#                   COMPAT
#                   -------------------------------------------------------------
#
#                   -------------------------------------------------------------
#                   AVSTREAM
#                   -------------------------------------------------------------
                    avstream/ttcommon.h \
                    avstream/ttmemorybuffer.h \
                    avstream/ttac3audioheader.h \
                    avstream/ttac3audiostream.h \
                    avstream/ttaudioheaderlist.h \
                    avstream/ttaudioindexlist.h \
                    avstream/ttavheader.h \
                    avstream/ttavstream.h \
                    avstream/ttavtypes.h \
                    avstream/ttdtsaudioheader.h \
                    avstream/ttdtsaudiostream.h \       
                    avstream/ttfilebuffer.h \   
# WIN32             avstream/ttwfilebuffer.h \ 
                    avstream/ttheaderlist.h \   
                    avstream/ttmpeg2videoheader.h \
                    avstream/ttmpeg2videostream.h \     
                    avstream/ttmpegaudioheader.h \      
                    avstream/ttmpegaudiostream.h \      
                    avstream/ttpcmaudioheader.h \       
                    avstream/ttpcmaudiostream.h \       
                    avstream/ttvideoheaderlist.h \      
                    avstream/ttvideoindexlist.h \
                    avstream/ttaviwriter.h \
#
#                   -------------------------------------------------------------
#                   GUI
#                   -------------------------------------------------------------
                    gui/ttcutmainwnd.h \
                    gui/ttcutsettings.h \
                    gui/ttcutavcutdlg.h \
                    gui/ttcutquickjump.h \
                    gui/ttprogressbar.h \
                    gui/ttcutpreview.h \
                    gui/ttsearchframe.h \
                    gui/ttcutaboutdlg.h \
                    gui/ttcutlistview.h \
                    gui/ttcutccrwnd.h \
                    gui/ttaudiolistview.h \
                    gui/ttprocessform.h \
                    gui/ttcut.h \
#
#                   -------------------------------------------------------------
#                   MPEG2DECODER
#                   -------------------------------------------------------------
                    mpeg2decoder/ttmpeg2decoder.h \
#
#                   -------------------------------------------------------------
#                   MPEG2WINDOW
#                   -------------------------------------------------------------
                    mpeg2window/ttmpeg2window.h \
#
#                   -------------------------------------------------------------
#                   AVCUT
#                   -------------------------------------------------------------
#                   avcut/tschnittsuche.h \
                    avcut/ttavcutposition.h \
#
#                   -------------------------------------------------------------
#                   AVILIB
#                   -------------------------------------------------------------
	            avilib/avilib.h \
#
#                   -------------------------------------------------------------
#                   EXTERN
#                   -------------------------------------------------------------
	            extern/tttranscode.h

# -------------------------------------------------------------------------------
# TTCUT.PRO ENDE
# -------------------------------------------------------------------------------

#The following line was inserted by qt3to4
QT +=  opengl 
