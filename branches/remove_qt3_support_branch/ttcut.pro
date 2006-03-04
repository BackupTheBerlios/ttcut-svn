#/*.............................................................................*/
#/*  (c) 2003/2008 TriTime  (ttcut.tritime.org)                                 */
#/*-----------------------------------------------------------------------------*/
#/*  FILE:   QMake Projektfile for ttcut                                        */
#/*-----------------------------------------------------------------------------*/
#/*  ABOUT:                                                                     */
#/*-----------------------------------------------------------------------------*/
#/*  AUTHOR:    b. altendorf (ttcut@tritime.org)              DATE: 02/18/2006  */
#/*-----------------------------------------------------------------------------*/
#
#
PROJECT           = TTCUT
CONFIG            += qt warn_on release
QT                += opengl qt3support
DEFINES           += _FILE_OFFSET_BITS=64
DIST              = TTCut
TARGET            = ttcut
LIBS              = -lmpeg2 -lmpeg2convert
OBJECTS_DIR       = obj
MOC_DIR           = moc
UI_DIR            = ui_h 
RCC_DIR           = res
RESOURCES = ui/videofileinfowidget.qrc\
            ui/audiofilelistwidget.qrc\
            ui/cutoutframewidget.qrc\
            ui/currentframewidget.qrc\
            ui/framenavigationwidget.qrc \
            ui/cutlistwidget.qrc\
            ui/mainwindow.qrc
FORMS     = ui/videofileinfowidget.ui\
            ui/audiofilelistwidget.ui\
            ui/cutoutframewidget.ui\
            ui/currentframewidget.ui\
            ui/framenavigationwidget.ui\
            ui/streamnavigationwidget.ui\
            ui/navigatordisplaywidget.ui\
            ui/cutlistwidget.ui\
            ui/aboutdlg.ui\
            ui/mainwindow.ui
HEADERS   = common/ttcut.h\
            common/ttmessagelogger.h \
            data/ttaudiolistdata.h\
            data/ttcutlistdata.h\
            data/ttcutparameter.h\
            avstream/ttcommon.h\
            avstream/ttmemorybuffer.h\
            avstream/ttac3audioheader.h\
            avstream/ttac3audiostream.h\
            avstream/ttaudioheaderlist.h\
            avstream/ttaudioindexlist.h\
            avstream/ttavheader.h\
            avstream/ttavstream.h\
            avstream/ttavtypes.h\
            avstream/ttdtsaudioheader.h\
            avstream/ttdtsaudiostream.h\     
            avstream/ttfilebuffer.h\ 
            avstream/ttheaderlist.h\ 
            avstream/ttmpeg2videoheader.h\
            avstream/ttmpeg2videostream.h\   
            avstream/ttmpegaudioheader.h\    
            avstream/ttmpegaudiostream.h\    
            avstream/ttpcmaudioheader.h\     
            avstream/ttpcmaudiostream.h\     
            avstream/ttvideoheaderlist.h\    
            avstream/ttvideoindexlist.h\
            avstream/ttaviwriter.h\
            mpeg2decoder/ttmpeg2decoder.h\
            mpeg2window/ttmpeg2window.h \
            avilib/avilib.h\
            extern/tttranscode.h\
            gui/ttcutsettings.h\
            gui/ttvideofileinfo.h\
            gui/ttaudiofilelist.h\
            gui/ttcutoutframe.h\
            gui/ttcurrentframe.h\
            gui/ttnavigation.h\
            gui/ttstreamnavigator.h \
            gui/ttnavigatordisplay.h \
            gui/ttcutlist.h\
            gui/ttprogressbar.h\
            gui/ttprocessform.h\
            gui/ttcutpreview.h\
            gui/ttcutproject.h\
            gui/ttcutaboutdlg.h\
            gui/ttsearchframe.h\
            gui/ttcutavcutdlg.h\
            gui/ttcutmainwindow.h
SOURCES   = common/ttcut.cpp\
            common/ttmessagelogger.cpp\
            data/ttaudiolistdata.cpp \
            data/ttcutlistdata.cpp\
            data/ttcutparameter.cpp\
            avstream/ttcommon.cpp\
            avstream/ttmemorybuffer.cpp\
            avstream/ttac3audioheader.cpp\
            avstream/ttac3audiostream.cpp\
            avstream/ttaudioheaderlist.cpp\
            avstream/ttaudioindexlist.cpp\
            avstream/ttavheader.cpp\
            avstream/ttavstream.cpp\
            avstream/ttavtypes.cpp\
            avstream/ttdtsaudioheader.cpp\
            avstream/ttdtsaudiostream.cpp\     
            avstream/ttfilebuffer.cpp\ 
            avstream/ttheaderlist.cpp\ 
            avstream/ttmpeg2videoheader.cpp\
            avstream/ttmpeg2videostream.cpp\   
            avstream/ttmpegaudioheader.cpp\    
            avstream/ttmpegaudiostream.cpp\    
            avstream/ttpcmaudioheader.cpp\     
            avstream/ttpcmaudiostream.cpp\     
            avstream/ttvideoheaderlist.cpp\    
            avstream/ttvideoindexlist.cpp\
            avstream/ttaviwriter.cpp\
            mpeg2decoder/ttmpeg2decoder.cpp\
            mpeg2window/ttmpeg2window.cpp\
            avilib/avilib.c\
            extern/tttranscode.cpp\
            gui/ttcutsettings.cpp\
            gui/ttvideofileinfo.cpp\
            gui/ttaudiofilelist.cpp\
            gui/ttcutoutframe.cpp\
            gui/ttcurrentframe.cpp\
            gui/ttnavigation.cpp\
            gui/ttstreamnavigator.cpp\
            gui/ttnavigatordisplay.cpp\
            gui/ttcutlist.cpp\
            gui/ttprogressbar.cpp\
            gui/ttprocessform.cpp\
            gui/ttcutpreview.cpp\
            gui/ttcutproject.cpp\
            gui/ttcutaboutdlg.cpp\
            gui/ttsearchframe.cpp\
            gui/ttcutavcutdlg.cpp\
            gui/ttcutmainwindow.cpp\
            gui/ttcutmain.cpp
#
# -------------------------------------------------------------------------------
# TTCUT.PRO ENDE
# -------------------------------------------------------------------------------
