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
PROJECT     = TTCUT
CONFIG      += qt warn_on debug
QT          += opengl
DEFINES     += _FILE_OFFSET_BITS=64
DIST        = TTCut
TARGET      = TTCut
LIBS        = -lmpeg2 -lmpeg2convert

#FIXME: find out why these defines wont work
#macx {
#  LIBS     += -framework QuickTime -framework Carbon
#  DEFINES  += MACX
#}

#unix {
 DEFINES   += UNIX
#}

OBJECTS_DIR = obj
MOC_DIR     = moc
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
              ui/ttmessagewindow.qrc\
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
              ui/mplexparamdlg.ui\
              ui/previewwidget.ui\
              ui/processviewwidget.ui\
              ui/avcutdialog.ui\
              ui/aboutdlg.ui\
              ui/ttmessagewindow.ui\
              ui/mainwindow.ui
HEADERS     = common/ttcut.h\
              common/ttmessagelogger.h\
              common/ttmessagewindow.h\
              common/ttexception.h\
              data/ttaudiolistdata.h\
              data/ttavdata.h\
              data/ttcutlistdata.h\
              data/ttcutparameter.h\
              data/ttmuxlistdata.h\
              avstream/ttcommon.h\
              avstream/ttac3audioheader.h\
              avstream/ttac3audiostream.h\
              avstream/ttaudioheaderlist.h\
              avstream/ttavheader.h\
              avstream/ttavstream.h\
              avstream/ttavtypes.h\
              avstream/ttfilebuffer.h\ 
              avstream/ttheaderlist.h\ 
              avstream/ttmpeg2videoheader.h\
              avstream/ttmpeg2videostream.h\   
              avstream/ttmpegaudioheader.h\    
              avstream/ttmpegaudiostream.h\    
              avstream/ttvideoheaderlist.h\    
              avstream/ttvideoindexlist.h\
              avstream/ttaviwriter.h\
              mpeg2decoder/ttmpeg2decoder.h\
              mpeg2window/ttmpeg2window.h \
              avilib/avilib.h\
              extern/tttranscode.h\
              extern/ttmplexprovider.h\
              gui/ttcutsettings.h\
              gui/ttcutsettingschapter.h\
              gui/ttcutsettingsmuxer.h\
              gui/ttcutsettingsencoder.h\
              gui/ttcutsettingsfiles.h\
              gui/ttcutsettingscommon.h\
              gui/ttcutsettingsdlg.h\
              gui/ttvideofileinfo.h\
              gui/ttaudiofilelist.h\
              gui/ttcutoutframe.h\
              gui/ttcurrentframe.h\
              gui/ttnavigation.h\
              gui/ttnavigatordisplay.h \
              gui/ttstreamnavigator.h \
              gui/ttcutlist.h\
              gui/ttprogressbar.h\
              gui/ttprocessform.h\
              gui/ttcutpreview.h\
              gui/ttmoviewidget.h\
              gui/ttcutproject.h\
              gui/ttcutaboutdlg.h\
              gui/ttsearchframe.h\
              gui/ttcutavcutdlg.h\
              gui/ttcutmainwindow.h
#macx {
#  HEADERS += gui/tthimoviewidget.h
#}

#unix {
  HEADERS += gui/ttmplayerwidget.h
#}
              
SOURCES     = common/ttcut.cpp\
              common/ttmessagelogger.cpp\
              common/ttmessagewindow.cpp\
              common/ttexception.cpp\
              data/ttaudiolistdata.cpp \
              data/ttavdata.cpp\
              data/ttcutlistdata.cpp\
              data/ttcutparameter.cpp\
              data/ttmuxlistdata.cpp\
              avstream/ttcommon.cpp\
              avstream/ttac3audioheader.cpp\
              avstream/ttac3audiostream.cpp\
              avstream/ttaudioheaderlist.cpp\
              avstream/ttavheader.cpp\
              avstream/ttavstream.cpp\
              avstream/ttavtypes.cpp\
              avstream/ttfilebuffer.cpp\ 
              avstream/ttheaderlist.cpp\ 
              avstream/ttmpeg2videoheader.cpp\
              avstream/ttmpeg2videostream.cpp\   
              avstream/ttmpegaudioheader.cpp\    
              avstream/ttmpegaudiostream.cpp\    
              avstream/ttvideoheaderlist.cpp\    
              avstream/ttvideoindexlist.cpp\
              avstream/ttaviwriter.cpp\
              mpeg2decoder/ttmpeg2decoder.cpp\
              mpeg2window/ttmpeg2window.cpp\
              avilib/avilib.c\
              extern/tttranscode.cpp\
              extern/ttmplexprovider.cpp\
              gui/ttcutsettings.cpp\
              gui/ttcutsettingschapter.cpp\
              gui/ttcutsettingsmuxer.cpp\
              gui/ttcutsettingsencoder.cpp\
              gui/ttcutsettingsfiles.cpp\
              gui/ttcutsettingscommon.cpp\
              gui/ttcutsettingsdlg.cpp\
              gui/ttvideofileinfo.cpp\
              gui/ttaudiofilelist.cpp\
              gui/ttcutoutframe.cpp\
              gui/ttcurrentframe.cpp\
              gui/ttnavigation.cpp\
              gui/ttnavigatordisplay.cpp\
              gui/ttstreamnavigator.cpp\
              gui/ttcutlist.cpp\
              gui/ttprogressbar.cpp\
              gui/ttprocessform.cpp\
              gui/ttcutpreview.cpp\
              gui/ttmoviewidget.cpp\
              gui/ttcutproject.cpp\
              gui/ttcutaboutdlg.cpp\
              gui/ttsearchframe.cpp\
              gui/ttcutavcutdlg.cpp\
              gui/ttcutmainwindow.cpp\
              gui/ttcutmain.cpp
#macx {
#  SOURCES += gui/tthimoviewidget.cpp
#}
#unix {
  SOURCES += gui/ttmplayerwidget.cpp
#}
#
# -------------------------------------------------------------------------------
# TTCUT.PRO ENDE
# -------------------------------------------------------------------------------
