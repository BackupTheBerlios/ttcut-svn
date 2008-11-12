/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutpreview.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 03/12/2006 */
/* MODIFIED: b. altendorf                                    DATE: 04/19/2007 */
/* MODIFIED: b. altendorf                                    DATE: 05/06/2008 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTPREVIEW
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* This program is free software; you can redistribute it and/or modify it    */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation;                                                       */
/* either version 2 of the License, or (at your option) any later version.    */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but WITHOUT*/
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.                                          */
/* See the GNU General Public License for more details.                       */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation,          */
/* Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.              */
/*----------------------------------------------------------------------------*/

#include "ttcutpreview.h"
#include "data/ttavdata.h"
#ifdef MACX
#include "tthimoviewidget.h"
#endif

#ifdef UNIX
#include "ttmplayerwidget.h"
#endif

#include <QApplication>
#include <QDir>

const char c_name[] = "TTCutPreview";

/* /////////////////////////////////////////////////////////////////////////////
 * TTCutPreview constructor
 */
TTCutPreview::TTCutPreview(QWidget* parent, int prevW, int prevH)
  : QDialog(parent)
{
  setupUi(this);

#ifdef UNIX
  movieWidget = new TTMplayerWidget(videoFrame);
#endif

#ifdef MACX
  movieWidget = new TTHiMovieWidget(videoFrame);
#endif

  // set the objects name
  setObjectName(c_name);

  // message logger instance
  log = TTMessageLogger::getInstance();

  // set desired video width x height
  previewWidth  = prevW;
  previewHeight = prevH;

  cbCutPreview->setEditable( false );
  cbCutPreview->setMinimumSize( 160, 20 );
  cbCutPreview->setInsertPolicy( QComboBox::InsertAfterCurrent );

  // connect signals and slots
  connect(movieWidget,  SIGNAL(optimalSizeChange()),      SLOT(updateSizes()));
  connect(movieWidget,  SIGNAL(isPlayingEvent(bool)),     SLOT(isPlayingSlot(bool)));
  connect(cbCutPreview, SIGNAL(currentIndexChanged(int)), SLOT(selectCut(int)));
  connect(pbPlay,       SIGNAL(clicked()),                SLOT(playPreview()));
  connect(pbExit,       SIGNAL(clicked()),                SLOT(exitPreview()));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Destroys the object and frees any allocated resources
 */
TTCutPreview::~TTCutPreview()
{
  if (preview_cut_list != 0)
    delete preview_cut_list;
}

void TTCutPreview::resizeEvent(QResizeEvent* event)
{
  movieWidget->resize(videoFrame->width()-2, videoFrame->height()-2);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Event handler to receive widgets close events
 */
void TTCutPreview::closeEvent(QCloseEvent* event)
{
  cleanUp();

  event->accept();
}

void TTCutPreview::updateSizes()
{
  adjustSize();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Initialize preview parameter
 */
void TTCutPreview::initPreview( TTVideoStream* v_stream, TTAudioStream* a_stream, TTCutListData* c_list )
{
  video_stream = v_stream;
  audio_stream = a_stream;
  avcut_list   = c_list;
  isPlaying    = false;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Create the preview clips
 */
void TTCutPreview::createPreview( int c_index )
{
  TTVideoStream* video_stream= avcut_list->avData(0)->videoStream();

  int            i, iPos;
  QString        preview_video_name;
  QString        preview_audio_name;
  QString        preview_mplex_name;
  QFileInfo      preview_video_info;
  QFileInfo      preview_audio_info;
  QFileInfo      preview_mplex_info;
  QString        selectionString;
  QString        mplex_command;
  QTime          preview_time;
  uint           preview_frames;
  uint           start_index;
  uint           end_index;
  TTCutListData* temp_cut_list;

  preview_time.setHMS(0,0,0);
  preview_time   = preview_time.addSecs( TTCut::cutPreviewSeconds );
  preview_frames = ttTimeToFrames( preview_time, video_stream->frameRate() ) / 2;

#if defined PREVIEW_DEBUG
  log->debugMsg(c_name, "------------------------------------------------");
  log->debugMsg(c_name, "create cut preview");
  log->debugMsg(c_name, "cut index  : %d", c_index);
#endif

  // create the cut list for the preview clips
  createCutPreviewList();

  // number of list entrys
  num_cut_list = preview_cut_list->count();
  num_preview  = num_cut_list/2+1;

#if defined PREVIEW_DEBUG
  log->debugMsg(c_name, "num preview: %d", num_preview);
#endif

  // ---------------------------------------------------------------------
  // Create video and audio preview clips
  // ---------------------------------------------------------------------
  for ( i = 0; i < num_preview; i++ )
  {
#if defined PREVIEW_DEBUG
    log->debugMsg(c_name, "cut index: %d / %d", i, cut_index);
#endif

    temp_cut_list = new TTCutListData();

    int index = (c_index > 0) ? i+1-c_index : i+1;

    preview_video_name.sprintf("preview_%03d.m2v", index);
    preview_audio_name.sprintf("preview_%03d.mpa", index);

    // add temporary path information
    preview_video_info.setFile(QDir(TTCut::tempDirPath), preview_video_name);
    preview_audio_info.setFile(QDir(TTCut::tempDirPath), preview_audio_name);

    preview_video_name = preview_video_info.absoluteFilePath();
    preview_audio_name = preview_audio_info.absoluteFilePath();

    // first cut-in
    if ( i == 0 )
    {
      temp_cut_list->deleteAll();

      temp_cut_list->addItem( preview_cut_list->cutInPosAt(i), preview_cut_list->cutOutPosAt(i),
                              preview_cut_list->avData(i) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "Start: %s", qPrintable(ttFramesToTime(preview_cut_list->cutInPosAt(i), 
                                                         video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    // cut i-i
    if ( num_preview > 1 && i > 0 && i < num_preview-1 )
    {
      iPos = (i-1)*2+1;

      temp_cut_list->deleteAll();

      temp_cut_list->addItem( preview_cut_list->cutInPosAt(iPos), preview_cut_list->cutOutPosAt(iPos),
                              preview_cut_list->avData(iPos) );
      temp_cut_list->addItem( preview_cut_list->cutInPosAt(iPos+1), preview_cut_list->cutOutPosAt(iPos+1),
                              preview_cut_list->avData(iPos+1) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        start_index = preview_cut_list->cutOutPosAt( iPos );
        end_index   = preview_cut_list->cutInPosAt( iPos+1 );
        selectionString.sprintf( "Cut %d-%d: %s - %s",i,i+1,
            qPrintable(ttFramesToTime( start_index, video_stream->frameRate()).toString("hh:mm:ss")),
            qPrintable(ttFramesToTime( end_index, video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    //last cut out
    if ( i == num_preview-1 )
    {
      iPos = (i-1)*2+1;

      temp_cut_list->deleteAll();

      temp_cut_list->addItem( preview_cut_list->cutInPosAt(iPos), preview_cut_list->cutOutPosAt(iPos),
                              preview_cut_list->avData(iPos) );

      if ( c_index == i || c_index+1 == i || c_index < 0 )
      {
        selectionString.sprintf( "End: %s", 
            qPrintable(ttFramesToTime(preview_cut_list->cutOutPosAt(iPos), 
                video_stream->frameRate()).toString("hh:mm:ss")));
        cbCutPreview->addItem( selectionString );
      }
    }

    // create cut
    if ( c_index == i || c_index+1 == i || c_index < 0 )
    {
      qDebug( "%screate preview cut: %d - %s", c_name, i, qPrintable(preview_video_name));
      TTAudioStream* audio_stream = temp_cut_list->avData(0)->audioStream(0);
      progress_bar = new TTProgressBar( this );
      progress_bar->show();
      qApp->processEvents();

      connect(video_stream, SIGNAL(progressChanged(TTProgressBar::State, const QString&, quint64)),
              progress_bar, SLOT(setProgress2(TTProgressBar::State, const QString&, quint64)));
      video_cut_stream = new TTFileBuffer(preview_video_name, QIODevice::WriteOnly );

      if ( audio_stream != 0 )
      {
        connect(audio_stream, SIGNAL(progressChanged(TTProgressBar::State, const QString&, quint64)),
                progress_bar, SLOT(setProgress2(TTProgressBar::State, const QString&, quint64)));
         audio_cut_stream = new TTFileBuffer(preview_audio_name, QIODevice::WriteOnly );
      }

      video_stream->cut( video_cut_stream, temp_cut_list );

      if ( audio_stream != 0 )
        audio_stream->cut( audio_cut_stream, temp_cut_list );

      video_stream->disconnect();
      delete video_cut_stream;  

      if ( audio_stream != 0 )
      {
        audio_stream->disconnect();
        delete audio_cut_stream;
      }
      delete progress_bar;
      progress_bar = 0;

    }
    delete temp_cut_list;
    // next video preview clip
  }
  // end cut video/audio preview clips

  // multiplex video and audio files give a better result because
  // video and audio synchronisation is much better
  progress_bar = new TTProgressBar( this );
  progress_bar->show();
  progress_bar->setActionText( "Mplexing preview cuts..." );
  progress_bar->setTotalSteps( num_preview, 1 );

  for ( i = 0; i < num_preview; i++ )
  {
    preview_video_name.sprintf("preview_%03d.m2v",i+1);
    preview_audio_name.sprintf("preview_%03d.mpa",i+1);
    preview_mplex_name.sprintf("preview_%03d.mpg",i+1);

    // add temporary path information
    preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );
    preview_audio_info.setFile( QDir(TTCut::tempDirPath), preview_audio_name );
    preview_mplex_info.setFile( QDir(TTCut::tempDirPath), preview_mplex_name );

    preview_video_name = preview_video_info.absoluteFilePath();
    preview_audio_name = preview_audio_info.absoluteFilePath();
    preview_mplex_name = preview_mplex_info.absoluteFilePath();

    // have we audio tracks
    if ( avcut_list->avData(0)->audioStream(0) != 0 )
    {
      mplex_command  = "mplex -f 8 -o ";
      mplex_command += preview_mplex_name;
      mplex_command += " ";
      mplex_command += preview_video_name;
      mplex_command += " ";
      mplex_command += preview_audio_name;
      mplex_command += " ";
      mplex_command += "2>/dev/null";

      system( qPrintable(mplex_command) );
    }
    else
    {
      mplex_command  = "mv ";
      mplex_command += preview_video_name;
      mplex_command += " ";
      mplex_command += preview_mplex_name;
      mplex_command += " ";
      mplex_command += "2>/dev/null";

      system( qPrintable(mplex_command) );
    }

#if defined PREVIEW_DEBUG
    log->infoMsg(c_name, "mplex command: %s", mplex_command.toLatin1().constData());
#endif

    progress_bar->setProgress( i+1 );
  }

  progress_bar->setComplete();
  delete progress_bar;
  progress_bar = 0;
  delete preview_cut_list;
  preview_cut_list = 0;

  // set the current cut preview to the first cut clip
  preview_video_name.sprintf("preview_001.mpg");
  preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );

  current_video_file = preview_video_info.absoluteFilePath();
  selectCut(0);
}

void TTCutPreview::createCutPreviewList( )
{
  int    list_count;
  int    current_entry;
  int    i;
  QTime  preview_time;
  long   preview_frames;
  int    frame_type;
  long   start_index;
  long   end_index;

  // create new cut list for the preview clips
  preview_cut_list = new TTCutListData();

  current_entry = 0;
  list_count    = 0;

  preview_time.setHMS(0,0,0);
  preview_time   = preview_time.addSecs( TTCut::cutPreviewSeconds );
  preview_frames = ttTimeToFrames( preview_time, avcut_list->avData(0)->videoStream()->frameRate() ) / 2;

#if defined PREVIEW_DEBUG
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%s>>> create cut preview list",c_name );
  qDebug( "%s-----------------------------------------------",c_name );
  qDebug( "%spreview frames : %d",c_name,preview_frames);
  qDebug( "%savcutlist count: %d",c_name,avcut_list->count());
#endif

  for ( i = 0; i < avcut_list->count(); i++ )
  {
    TTVideoStream* pVideoStream = avcut_list->avData(i)->videoStream();
    start_index = avcut_list->cutInPosAt( i );
    end_index   = start_index + preview_frames;

    if ( end_index >= video_stream->frameCount() )
      end_index = video_stream->frameCount()-1;

    // cut should end at an I-frame or P-frame
    frame_type = pVideoStream->frameType( end_index );
    //qDebug("%sframe type at end: %ld - %d",c_name,end_index,frame_type);
    while ( frame_type == 3 &&
            end_index < pVideoStream->frameCount()-1 )
    {
      end_index++;
      frame_type = pVideoStream->frameType( end_index );
    }

    //qDebug("%screate cut list entry: %ld - %d: %d",c_name,start_index,end_index,preview_frames);

    preview_cut_list->addItem( start_index, end_index, avcut_list->avData(i) );

    list_count++;

    end_index   = avcut_list->cutOutPosAt( i );
    start_index = end_index - preview_frames;

    if ( start_index < 0 )
      start_index = 0;

    // cut should start at an I-frame
    frame_type = video_stream->frameType( start_index );
    while ( frame_type != 1 && start_index > 0 )
    {
      start_index--;
      frame_type = video_stream->frameType( start_index );
    }

    //qDebug("%screate cut list entry: %ld - %d: %d",c_name,start_index,end_index,preview_frames);

    preview_cut_list->addItem( start_index, end_index, avcut_list->avData(i) );

    list_count++;
    current_entry++;
  }
}

void TTCutPreview::isPlayingSlot(bool value)
{
  isPlaying = value;

  if (isPlaying) {
   pbPlay->setText(tr("Stop"));
   pbPlay->setIcon(QIcon(":/pixmaps/pixmaps/stop_18.xpm"));
   return;
  }

  pbPlay->setText(tr("Play"));
  pbPlay->setIcon(QIcon(":/pixmaps/pixmaps/play_18.xpm"));
}

/* /////////////////////////////////////////////////////////////////////////////
 * ComboBox selectionChanged event handler: load the selected movie
 */
void TTCutPreview::selectCut( int iCut )
{
  QString   preview_video_name;
  QFileInfo preview_video_info;

  preview_video_name.sprintf("preview_%03d.mpg",iCut+1);
  preview_video_info.setFile( QDir(TTCut::tempDirPath), preview_video_name );
  current_video_file = preview_video_info.absoluteFilePath();

   movieWidget->loadMovie(current_video_file);

   movieWidget->stopMovie();
   pbPlay->setText(tr("Play"));
   pbPlay->setIcon(QIcon(":/pixmaps/pixmaps/play_18.xpm"));
   isPlaying = false;
 }

/* /////////////////////////////////////////////////////////////////////////////
 * Play/Pause the selected preview clip
 */
void TTCutPreview::playPreview()
{
  if (isPlaying)
  {
    movieWidget->stopMovie();
    //pbPlay->setText(tr("Play"));
    //pbPlay->setIcon(QIcon(":/pixmaps/pixmaps/play_18.xpm"));
    //isPlaying = false;
    return;
  }

  printf("play preview....\n");
  //pbPlay->setText(tr("Stop"));
  //pbPlay->setIcon(QIcon(":/pixmaps/pixmaps/stop_18.xpm"));
  movieWidget->playMovie();
  //isPlaying = true;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Exit the preview window
 */
void TTCutPreview::exitPreview()
{
  close();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Housekeeping: Remove the temporary created preview clips
 */
void TTCutPreview::cleanUp()
{
  QString   rmCommand = "rm ";
  QString   fileName  = "preview*";
  QFileInfo fileInfo;

  if (isPlaying)
  {
    movieWidget->stopMovie();
    isPlaying = false;
  }

  // clean up preview* files in temp directory
  fileInfo.setFile(QDir(TTCut::tempDirPath), fileName);
  rmCommand += fileInfo.absoluteFilePath();
  rmCommand += " 2>/dev/null";

  //qDebug("rm: %s", rmCommand.toAscii().data());
  system(rmCommand.toAscii().data());
}

