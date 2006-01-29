/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2006 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttmessagelogger.h                                               */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 01/28/2006 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// TTMESSAGELOGGER
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------------
// *
// -----------------------------------------------------------------------------

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


#ifndef TTMESSAGELOGGER_H
#define TTMESSAGELOGGER_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>

class TTMessageLogger
{
  private:
    TTMessageLogger(int mode = STD_LOG_MODE);
~TTMessageLogger();

  public:
    static TTMessageLogger* getInstance(int mode = STD_LOG_MODE);
    void infoMsg(QString caller, QString msgString);
    void warningMsg(QString caller, QString msgString);
    void errorMsg(QString caller, QString msgString);
    void debugMsg(QString caller, QString msgString);;

void testMsg(QString caller, const char* msg, ...);
enum MsgType
{
  INFO,
  WARNING,
  ERROR,
  DEBUG
};

enum LogMode
{
  INDIVIDUAL = 0x01,
  SUMMARIZE  = 0x02,
  CONSOLE    = 0x04
};

void logMsg( MsgType type, QString caller, QString msgString);
void writeMsg(QString msgString);
  private:
    QFile* logfile;
    static TTMessageLogger* loggerInstance;
    static QString stdLogFilePath;
    int    logMode;

    static const int   STD_LOG_MODE;
    static const char* INFO_FILE_NAME;
    static const char* WARNING_FILE_NAME;
    static const char* ERROR_FILE_NAME;
    static const char* DEBUG_FILE_NAME;
    static const char* SUM_FILE_NAME;
};
#endif //TTMESSAGELOGGER_H
   
