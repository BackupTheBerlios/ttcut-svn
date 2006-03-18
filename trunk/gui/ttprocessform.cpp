/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttprocessform.cpp                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 08/07/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTPROCESSFORM
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

#include "ttprocessform.h"


TTProcessForm::TTProcessForm(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  btnCancel->hide();

  // signal and slot connection
  connect(btnCancel, SIGNAL(clicked()), SIGNAL(btnCancelClicked()));
}

TTProcessForm::~TTProcessForm()
{
  for (int i=0; i < procOutputList->count(); i++) {
    delete procOutputList->takeItem(i);
  }
}

void TTProcessForm::setFrameCaption(QString& caption)
{
  gbProcessView->setTitle(caption);
}

void TTProcessForm::showCancelButton(bool show)
{
  btnCancel->setVisible(show);
}

void TTProcessForm::addLine(QString& str_line)
{
  QListWidgetItem *item = new QListWidgetItem(procOutputList);
  item->setText(str_line);
  procOutputList->scrollToItem(item, QAbstractItemView::EnsureVisible);

  qApp->processEvents();
}
