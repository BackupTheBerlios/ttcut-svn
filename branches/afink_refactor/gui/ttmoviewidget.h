/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttmoviewidget.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 05/08/2008 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTMOVIEWIDGET
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

#include <QObject>
#include <QtGui/QWidget>

#ifndef TTMOVIEWIDGET_H
#define TTMOVIEWIDGET_H

class TTMovieWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool  getControlsVisible READ getControlsVisible WRITE setControlsVisible)
    Q_PROPERTY(QSize getMovieSize       READ getMovieSize       WRITE setMovieSize)

public:
    TTMovieWidget(QWidget *parent);
    virtual ~TTMovieWidget() {};
 
    virtual QSize getMovieSize() const {return movieSize;};
    virtual void  setMovieSize(const QSize& size) {movieSize = size;};
    virtual bool  getControlsVisible() const {return areControlsVisible;};
    virtual void  setControlsVisible(bool visible) {areControlsVisible = visible;};

    virtual void playMovie() {};
    virtual void stopMovie() {};

signals:
    virtual void optimalSizeChange();
    virtual void isPlayingEvent(bool);


public slots:
    virtual void loadMovie(const QString& fileName) {currentMovie = fileName;};

protected:
    QString currentMovie;
    QSize   movieSize;
    bool    isPlaying;
    bool    areControlsVisible;
};

#endif
