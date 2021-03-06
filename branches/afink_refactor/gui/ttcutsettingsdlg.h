/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutsettingsdlg.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTSETTINGSDLG
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

#ifndef TTCUTSETTINGSDLG_H 
#define TTCUTSETTINGSDLG_H

#include "ui_ttsettingsdialog.h"

#include "../common/ttcut.h"


class TTCutSettingsDlg : public QDialog, Ui::TTSettingsDialog
{
  Q_OBJECT

  public:
    TTCutSettingsDlg(QWidget* parent=0);

    void setGlobalData();

    public slots:
      void onDlgOk();
    void onDlgCancel();
};

#endif
