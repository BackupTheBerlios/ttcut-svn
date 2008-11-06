/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttprogressbar.h                                                 */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/11/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/24/2007 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTPROGRESSBAR
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

#ifndef TTPROGRESSBAR_H
#define TTPROGRESSBAR_H

#include <QDialog>
#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "../common/ttcut.h"

#ifndef _WIN32
#include <inttypes.h>
#endif

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QProgressBar;
class QPushButton;

class TTProgressBar : public QDialog
{ 
    Q_OBJECT

public:
  enum State
  {
    Init,
    Start,
    Step,
    Finished,
    Canceled,
    Error
  };

public:
    TTProgressBar( QWidget* parent = 0, Qt::WFlags f = 0 );
    ~TTProgressBar();

    // public methods
    void setActionText( QString action );
    void setElapsedTime( QTime time );
    void setPercentages( float percent );

    void setTotalSteps( uint64_t  t_steps, int r_int=0 );
    bool setProgress( uint64_t progress );
    void setComplete();
    void resetProgress();

    void showBar();
    void hideBar();

    bool isCanceled();

    public slots:
    void slotCancel();
    void setProgress2(TTProgressBar::State state, const QString& msg = "", quint64 value = 0);


    signals:
      void cancel();

protected:
    void updateProgressBar();

 private:
    QGridLayout*  TTProgressBarLayout;
    QHBoxLayout*  Layout1;
    QHBoxLayout*  Layout2;
    QLabel*       laAction;
    QLabel*       actionString;
    QLabel*       laElapsedTime;
    QLabel*       elapsedTimeString;
    QLabel*       laPercentage;
    QLabel*       percentageString;
    QProgressBar* progressBar;
    QPushButton*  pbCancel;

    bool          userCancel;
    QTime         elapsedTime;
    int           elapsedMsec;
    QString       strPercentage;
    uint64_t      totalSteps;
    int           refresh_intervall;
    int           refresh;
    int           normTotalSteps;
    int           normProgress;
    double        progressPercent;

};

#endif // TTPROGRESSBAR_H
