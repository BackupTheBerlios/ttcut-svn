#/*.............................................................................*/
#/*..(c) 2003/2005 TriTime......................................................*/
#/*-----------------------------------------------------------------------------*/
#/*..FILE:   QMake Projektfile for ttmpeg2                                    ..*/
#/*-----------------------------------------------------------------------------*/
#/*..ABOUT:                                                                   ..*/
#/*-----------------------------------------------------------------------------*/
#/*..AUTHOR:    b. altendorf              DATE: 21.10.2003         TIME: 20:26..*/
#/*..MODIFIED:  b. altendorf              DATE: 25.10.2003         TIME: 17:30..*/
#/*..MODIFIED:  b. altendorf              DATE: 14.12.2003         TIME: 09:30..*/
#/*..MODIFIED:  b. altendorf              DATE: 23.02.2005         TIME: 11:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 01.03.2005         TIME: 16:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 23.03.2005         TIME: 16:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 05.08.2005         TIME: 12:00..*/
#/*..MODIFIED:  b. altendorf              DATE: 17.03.2006         TIME: 21:30..*/
#/*-----------------------------------------------------------------------------*/
#
#
#REQUIRES         = opengl
PROJECT           = TTMPEG2
CONFIG            += qt-mt opengl warn_on release
QT                += qt3support	opengl
DEFINES           += _FILE_OFFSET_BITS=64 __TTMPEG2
DIST              = TTMpeg2
TARGET            = ttmpeg2
LIBS              = -lmpeg2 -lmpeg2convert
OBJECTS_DIR       = ttmpeg2_obj
MOC_DIR           = ttmpeg2_moc
UI_DIR      = ui_h 
RCC_DIR     = res
RESOURCES   = ui/videofileinfowidget.qrc\
              ui/audiofilelistwidget.qrc\
              ui/cutoutframewidget.qrc\
              ui/currentframewidget.qrc\
              ui/framenavigationwidget.qrc \
              ui/cutlistwidget.qrc\
              ui/ttcutsettingscommon.qrc\
              ui/ttcutsettingsmuxer.qrc\
              ui/ttsettingsdialog.qrc\
              ui/previewwidget.qrc\
              ui/avcutdialog.qrc\
              ui/processviewwidget.qrc\
              ui/mainwindow.qrc
FORMS       = ui/videofileinfowidget.ui\
              ui/audiofilelistwidget.ui\
              ui/cutoutframewidget.ui\
              ui/currentframewidget.ui\
              ui/framenavigationwidget.ui\
              ui/navigatordisplaywidget.ui\
              ui/streamnavigationwidget.ui\
              ui/cutlistwidget.ui\
              ui/ttcutsettingschapter.ui\
              ui/ttcutsettingsmuxer.ui\
              ui/ttcutsettingsencoder.ui\
              ui/ttcutsettingsfiles.ui\
              ui/ttcutsettingscommon.ui\
              ui/ttsettingsdialog.ui\
              ui/previewwidget.ui\
              ui/processviewwidget.ui\
              ui/avcutdialog.ui\
              ui/aboutdlg.ui\
              ui/mainwindow.ui
#
SOURCES           = \
#
                    common/ttmessagelogger.cpp\
                    common/ttcut.cpp\
#
                    data/ttcutlistdata.cpp\
                    data/ttaudiolistdata.cpp\
                    data/ttcutparameter.cpp\
                    data/ttmuxlistdata.cpp\
#                   ------------------------------------------------------------
#                   AVStream / AVHeader
#                   ------------------------------------------------------------
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
                    avilib/avilib.c \
#
#                   ------------------------------------------------------------
#                   MPEG2
#                   ------------------------------------------------------------
                    mpeg2decoder/ttmpeg2decoder.cpp \
                    mpeg2window/ttmpeg2window.cpp \
#                   ------------------------------------------------------------
#                   GUI (TTCut)
#                   ------------------------------------------------------------
                    gui/ttprogressbar.cpp \
                    gui/ttprocessform.cpp \
                    gui/ttcutproject.cpp\
#
#                   ------------------------------------------------------------
#                   GUI (TTMpeg2)
#                   ------------------------------------------------------------
#                   ttmpeg2_src/ttmpeg2test.cpp \
#	                  ttmpeg2_src/ttslidetest.cpp \
                    ttmpeg2_src/slidemain.cpp \
                    ttmpeg2_src/ttmpeg2infoview.cpp \
                    ttmpeg2_src/ttframechart.cpp \
                    ttmpeg2_src/ttmpeg2mainwnd.cpp \
#
#                   -------------------------------------------------------------
#                   EXTERN
#                   -------------------------------------------------------------
                    extern/tttranscode.cpp
#
HEADERS           = \
#
                    common/ttmessagelogger.h \
                    common/ttcut.h\
                    data/ttcutlistdata.h\
                    data/ttaudiolistdata.h\
                    data/ttcutparameter.h\
                    data/ttmuxlistdata.h\
#                    
#                   ------------------------------------------------------------
#                   AVStream / AVHeader
#                   ------------------------------------------------------------
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
#                   ------------------------------------------------------------
#                   MPEG2
#                   ------------------------------------------------------------
                    mpeg2decoder/ttmpeg2decoder.h \
                    mpeg2window/ttmpeg2window.h \
#                   ------------------------------------------------------------
#                   GUI (TTCut)
#                   ------------------------------------------------------------
                    gui/ttprogressbar.h \
                    gui/ttprocessform.h \
                    gui/ttcutproject.h\
#
#                   ------------------------------------------------------------
#                   GUI (TTMpeg2)
#                   ------------------------------------------------------------
#                   ttmpeg2_src/ttmpeg2test.h \
                    ttmpeg2_src/ttmpeg2infoview.h \
                    ttmpeg2_src/ttframechart.h \
                    ttmpeg2_src/ttmpeg2mainwnd.h \
#
#                   -------------------------------------------------------------
#                   EXTERN
#                   -------------------------------------------------------------
                    extern/tttranscode.h
