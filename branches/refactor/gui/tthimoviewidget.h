/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : tthimoviewidget.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/05/2006 */
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

#include <QtGui/QWidget>
#include <QResizeEvent>

#include "ttmoviewidget.h"

#include <Carbon/Carbon.h>
#include <Quicktime/Quicktime.h>
#include <Quicktime/HIMovieView.h>
#include <Quicktime/Movies.h>


#ifndef TTHIMOVIEWIDGET_H
#define TTHIMOVIEWIDGET_H

typedef struct MovieType** Movie;

class TTHiMovieWidget : public TTMovieWidget
{
    Q_OBJECT
    Q_PROPERTY(bool getControlsVisible READ getControlsVisible WRITE setControlsVisible)

public:
    TTHiMovieWidget(QWidget *parent);
    ~TTHiMovieWidget();

    void resizeEvent(QResizeEvent* event);
    //QSize sizeHint() const;

    static Boolean CustomActionFilter(MovieController mc, short action, void* params, long refCon);

    void toggleIsPlaying();
    bool getControlsVisible() const;
    void setControlsVisible(bool visible);

    void playMovie();
    void stopMovie();

signals:
    void optimalSizeChange();

public slots:
    void loadMovie(const QString &fileName);

private:
    Movie currentMovie;
};

#endif
