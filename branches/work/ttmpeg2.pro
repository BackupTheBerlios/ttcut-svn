#/*.............................................................................*/
#/*  (c) 2003/2008 TriTime (ttcut.tritime.org)                                  */
#/*-----------------------------------------------------------------------------*/
#/*  FILE:   QMake Projektfile for ttmpeg2                                      */
#/*-----------------------------------------------------------------------------*/
#/*  ABOUT:                                                                     */
#/*-----------------------------------------------------------------------------*/
#/*  AUTHOR:    b. altendorf (ttcut@tritime.org)               DATE: 03/01/2007 */
#/*  MODIFIED:  b. altendorf                                   DATE:            */
#/*-----------------------------------------------------------------------------*/
#
#
#REQUIRES   = opengl
PROJECT     = TTMPEG2
CONFIG     += qt-mt opengl warn_on release
QT         += opengl
DEFINES    += _FILE_OFFSET_BITS=64 __TTMPEG2 
DIST        = TTMpeg2
TARGET      = ttmpeg2
LIBS        = -lmpeg2 -lmpeg2convert
OBJECTS_DIR = ttmpeg2_obj
MOC_DIR     = ttmpeg2_moc
UI_DIR      = ui_h 
RCC_DIR     = res
RESOURCES   = ui/videofileinfowidget.qrc\
              ui/processviewwidget.qrc\
              ttmpeg2_src/ui/stepbywidget.qrc\
              ttmpeg2_src/ui/frameinfowidget.qrc\
              ttmpeg2_src/ui/ttmpeg2_mainwnd.qrc
FORMS       = ui/videofileinfowidget.ui\
              ui/processviewwidget.ui\
              ttmpeg2_src/ui/stepbywidget.ui\
              ttmpeg2_src/ui/frameinfowidget.ui\
              ttmpeg2_src/ui/ttmpeg2_mainwnd.ui
#
SOURCES     = \
#
              ttmpeg2_src/gui/ttmpeg2main.cpp\
#              
              common/ttmessagelogger.cpp\
              common/ttcut.cpp\
#
              data/ttcutlistdata.cpp\
              data/ttaudiolistdata.cpp\
              data/ttcutparameter.cpp\
              data/ttmuxlistdata.cpp\
#             ------------------------------------------------------------
#             AVStream / AVHeader
#             ------------------------------------------------------------
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
#             ------------------------------------------------------------
#             MPEG2
#             ------------------------------------------------------------
              mpeg2decoder/ttmpeg2decoder.cpp \
              mpeg2window/ttmpeg2window.cpp \
#             ------------------------------------------------------------
#             GUI (TTCut)
#             ------------------------------------------------------------
              gui/ttprogressbar.cpp \
              gui/ttprocessform.cpp \
              gui/ttcutproject.cpp\
              gui/ttvideofileinfo.cpp\
#
#             ------------------------------------------------------------
#             GUI (TTMpeg2)
#             ------------------------------------------------------------
              ttmpeg2_src/gui/ttmpeg2mainwnd.cpp \
              ttmpeg2_src/gui/ttframeinfo.cpp \
              ttmpeg2_src/gui/ttstepby.cpp \
#
#             -------------------------------------------------------------
#             EXTERN
#             -------------------------------------------------------------
              extern/tttranscode.cpp \
#
#             -------------------------------------------------------------
#             TTMPEG2-TOOLS
#             -------------------------------------------------------------
              ttmpeg2_src/tools/ttheaderwriter.cpp
#
HEADERS     = \
#
              common/ttmessagelogger.h \
              common/ttcut.h\
              data/ttcutlistdata.h\
              data/ttaudiolistdata.h\
              data/ttcutparameter.h\
              data/ttmuxlistdata.h\
#                    
#             ------------------------------------------------------------
#             AVStream / AVHeader
#             ------------------------------------------------------------
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
#             ------------------------------------------------------------
#             MPEG2
#             ------------------------------------------------------------
              mpeg2decoder/ttmpeg2decoder.h \
              mpeg2window/ttmpeg2window.h \
#             ------------------------------------------------------------
#             GUI (TTCut)
#             ------------------------------------------------------------
              gui/ttprogressbar.h \
              gui/ttprocessform.h \
              gui/ttcutproject.h\
              gui/ttvideofileinfo.h\
#
#             ------------------------------------------------------------
#             GUI (TTMpeg2)
#             ------------------------------------------------------------
              ttmpeg2_src/gui/ttmpeg2mainwnd.h \
              ttmpeg2_src/gui/ttframeinfo.h \
              ttmpeg2_src/gui/ttstepby.h \
##
#             -------------------------------------------------------------
#             EXTERN
#             -------------------------------------------------------------
              extern/tttranscode.h \
#
#             -------------------------------------------------------------
#             TTMPEG2-TOOLS
#             -------------------------------------------------------------
              ttmpeg2_src/tools/ttheaderwriter.h
