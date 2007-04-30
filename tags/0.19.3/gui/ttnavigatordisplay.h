/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttnavigatordisplay.h                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 03/03/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTNAVIGATORDISPLAY
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

#ifndef TTNAVIGATORDISPLAY_H
#define TTNAVIGATORDISPLAY_H


#include "ui_navigatordisplaywidget.h"

class TTCutListData;

class TTNavigatorDisplay : public QFrame, public Ui::TTNavigatorDisplayWidget
{
  Q_OBJECT

  public:
    TTNavigatorDisplay(QWidget* parent);

    void controlEnabled(bool enabled);
    void setCutListData(TTCutListData* data);
    void setMinValue(int val);
    void setMaxValue(int val);

    public slots:
      void onRefreshDisplay();

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent* event);
    void drawCutList();

  private:
    TTCutListData* cutListData;
    bool   isControlEnabled;
    int    minValue;
    int    maxValue;
    double scaleFactor;
};

#endif //TTNAVIGATORDISPLAY_H
