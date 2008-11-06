/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : tthimoviewidget.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/03/2008 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTHIMOVIEWIDGET
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

#include "tthimoviewidget.h"

#include <QtGui/QLayout>
Boolean TTHiMovieWidget::CustomActionFilter(MovieController mc, short action, void* params, long refCon)
{
  if (action == mcActionPlay) {
    TTHiMovieWidget* obj = (TTHiMovieWidget*)refCon;
    obj->toggleIsPlaying();
  }

  return FALSE;
}

void TTHiMovieWidget::toggleIsPlaying()
{
  isPlaying = (isPlaying) ? FALSE : TRUE;
  emit isPlayingEvent(isPlaying);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Constructor for HIMovieWidget
 */
TTHiMovieWidget::TTHiMovieWidget(QWidget *parent)
    : TTMovieWidget(parent)
{
  EnterMovies();

    HIViewRef movieView;

    HIObjectCreate(kHIMovieViewClassID, 0, reinterpret_cast<HIObjectRef*>(&movieView));

    HIMovieViewChangeAttributes(movieView, 
            kHIMovieViewAutoIdlingAttribute |
            kHIMovieViewControllerVisibleAttribute, NULL);

    create(WId(movieView));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Destructor: clean up used resources
 */
TTHiMovieWidget::~TTHiMovieWidget()
{
    if (currentMovie)
        DisposeMovie(currentMovie);

  ExitMovies();
}

void TTHiMovieWidget::resizeEvent(QResizeEvent* e)
{
  EventRef event;
  HIRect   optimalBounds;

  CreateEvent(0, 
      kEventClassControl, 
      kEventControlGetOptimalBounds, 
      GetCurrentEventTime(), 
      kEventAttributeUserEvent, 
      &event);
  SendEventToEventTargetWithOptions(event,
      HIObjectGetEventTarget(HIObjectRef(winId())),
      kEventTargetDontPropagate);
  GetEventParameter(event, kEventParamControlOptimalBounds, typeHIRect,
      0, sizeof(HIRect), 0, &optimalBounds);
  ReleaseEvent(event);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Play the current movie; You have to load the current movie first!
 */
void TTHiMovieWidget::playMovie()
{
  //endCallback = NewCallBack(GetMovieTimeBase(currentMovie), callBackAtExtremes);
  //CallMeWhen(endCallback, NewQTCallBackUPP(QTFinishedPlayingProc), NULL, triggerAtStop, 0, 0);

  HIMovieViewPlay(HIViewRef(winId()));
}


/* /////////////////////////////////////////////////////////////////////////////
 * Stop playin the movie
 */
void TTHiMovieWidget::stopMovie()
{
  if (currentMovie == NULL)
    return;

  HIMovieViewPause(HIViewRef(winId()));
}

/* /////////////////////////////////////////////////////////////////////////////
 * Load the current movie from file
 */
void TTHiMovieWidget::loadMovie(const QString &fileName)
{
    QTVisualContextRef visualContext = 0;
    Boolean active = true;
    DataReferenceRecord dataRef;
    Movie oldMovie = 0;

    resize(width(), height()-1);

    // create an array of properties for NewMovieFromProperties
    // these properties describe how to create the new movie
    // see Movies.h for the full list of available properties
    QTNewMoviePropertyElement newMovieProperties[] = {
        {kQTPropertyClass_DataLocation,     kQTDataLocationPropertyID_DataReference, sizeof(dataRef),       &dataRef,       0},
        {kQTPropertyClass_NewMovieProperty, kQTNewMoviePropertyID_Active,            sizeof(active),        &active,        0},
        {kQTPropertyClass_Context,          kQTContextPropertyID_VisualContext,      sizeof(visualContext), &visualContext, 0},
    };

    CFStringRef cfString = CFStringCreateWithCharacters(0,
                                    reinterpret_cast<const UniChar *>(fileName.unicode()),
                                    fileName.length());
    QTNewDataReferenceFromFullPathCFString(cfString, kQTPOSIXPathStyle, 0, &dataRef.dataRef, &dataRef.dataRefType);
    CFRelease(cfString);

    // if there's an old movie save it so we can dispose of it if
    // we load up a new movie successfully
    oldMovie = currentMovie;
    currentMovie = 0;

    // create a new movie using movie properties
    // when calling this function, you supply a set of input properties that describe the information
    // required to instantiate the movie (its data reference, audio context, visual context, and so on)
    NewMovieFromProperties(sizeof(newMovieProperties) / sizeof(newMovieProperties[0]), // the number of properties in the array passed in inputProperties
            newMovieProperties,    // a pointer to a property array describing how to instantiate the movie
            0,                        // number of properties in the array passed in outputProperties
            0,                    // pointer to a property array to receive output parameters - 0 if you don’t want this information
            &currentMovie);  // pointer to a variable that receives the new movie
    // make sure to dispose of the dataRef we no longer need it
    DisposeHandle(dataRef.dataRef);

    // set the HIMovieView's current movie
    HIMovieViewSetMovie(HIViewRef(winId()), currentMovie);

    if (oldMovie != 0)
        DisposeMovie(oldMovie);

    resize(width(), height()+1);

    isPlaying = true;
    MovieController mvController = HIMovieViewGetMovieController(HIViewRef(winId()));
    MCActionFilterWithRefConUPP myActionFilter;
    myActionFilter = NewMCActionFilterWithRefConUPP(TTHiMovieWidget::CustomActionFilter);
    MCSetActionFilterWithRefCon(mvController, myActionFilter, (long)this);
}

/*QSize TTHiMovieWidget::sizeHint() const
{
  qDebug("HIMovieView->sizeHint()...");
    EventRef event;
    HIRect   optimalBounds;

    CreateEvent(0, 
            kEventClassControl, 
            kEventControlGetOptimalBounds, 
            GetCurrentEventTime(), 
            kEventAttributeUserEvent, 
            &event);
    SendEventToEventTargetWithOptions(event,
            HIObjectGetEventTarget(HIObjectRef(winId())),
            kEventTargetDontPropagate);
    GetEventParameter(event, kEventParamControlOptimalBounds, typeHIRect,
                      0, sizeof(HIRect), 0, &optimalBounds);
    ReleaseEvent(event);


    return QSize(optimalBounds.size.width, optimalBounds.size.height);
}*/

bool TTHiMovieWidget::getControlsVisible() const
{
    OptionBits currentBits = HIMovieViewGetAttributes(HIViewRef(winId()));
    return currentBits & kHIMovieViewControllerVisibleAttribute;
}

void TTHiMovieWidget::setControlsVisible(bool visible)
{
    if (visible != getControlsVisible()) {
        HIMovieViewChangeAttributes(HIViewRef(winId()),
                visible ? kHIMovieViewControllerVisibleAttribute : 0,
                !visible ? kHIMovieViewControllerVisibleAttribute : 0);
        updateGeometry();
        emit optimalSizeChange();
    }
}
