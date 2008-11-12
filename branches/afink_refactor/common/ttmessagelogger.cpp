/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2006 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2006                                                      */
/* FILE     : ttmessagelogger.cpp                                             */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 01/28/2006 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// TTMESSAGELOGGER
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

/*!
 * The TTMessageLogger class provide a tool for writing a logfile during
 * runtime. Theire are four logging-level options:
 * INFO    for informational messages
 * WARNING for generell warnings about possible problems
 * ERROR   for realy program errors during runtime
 * DEBUG   for debug messages
 * The TTMessageLogger object is a singleton. The first call to getInstance()
 * instantiate the statical TTMessageLogger object. 
 */

#include "ttmessagelogger.h"
//#include "ttmessagewindow.h"
//#include <sys/stdarg.h>


const int   TTMessageLogger::STD_LOG_MODE      = TTMessageLogger::SUMMARIZE;
int         TTMessageLogger::logMode           = TTMessageLogger::STD_LOG_MODE;
const char* TTMessageLogger::INFO_FILE_NAME    = "info.log";
const char* TTMessageLogger::WARNING_FILE_NAME = "warning.log";
const char* TTMessageLogger::ERROR_FILE_NAME   = "error.log";
const char* TTMessageLogger::DEBUG_FILE_NAME   = "debug.log";
const char* TTMessageLogger::SUM_FILE_NAME     = "logfile.log";

TTMessageLogger* TTMessageLogger::loggerInstance = NULL;
QString TTMessageLogger::stdLogFilePath = "./logfile.log";

/*!
 * The private constructor for the TTMessageLogger object. This constructor
 * is only called from the public getInstance() method.
 */
TTMessageLogger::TTMessageLogger(int mode)
{
  logfile     = new QFile(stdLogFilePath);
  logEnabled  = true;
  logMode     = mode;
  logConsole  = false;
  logExtended = false;
  

  QFile file(stdLogFilePath);
  
  if(file.exists())
    file.remove();

  logfile->open(QIODevice::WriteOnly | QIODevice::Text);
}

/*!
 * The public destructor. The destructor removes the statical TTMessageLogger
 * object from memory an closes the logfile.
 */
TTMessageLogger::~TTMessageLogger()
{
  if (logfile != NULL) {
    logfile->close();
    delete logfile;
  }
}

/*!
 * This public method is used to get an instance of the TTMessageLogger
 * object.
 */
TTMessageLogger* TTMessageLogger::getInstance(int mode)
{
  if(loggerInstance == NULL)
    loggerInstance = new TTMessageLogger(mode);

  return loggerInstance;
}

void TTMessageLogger::enableLogFile(bool enable)
{
  logEnabled = enable;
}

void TTMessageLogger::setLogModeConsole(bool console)
{
  if (console) {
    logMode = SUMMARIZE | CONSOLE;
  } else {
    logMode = SUMMARIZE;
  }
  logConsole = console;
}

void TTMessageLogger::setLogModeExtended(bool extended)
{
  logExtended = extended;
}
 
void TTMessageLogger::setLogMode(int mode)
{
  logMode = mode;
}

/*!
 * Writes an logfile message from type INFO
 */
void TTMessageLogger::infoMsg(QString caller, QString msgString)
{
  logMsg(INFO, caller, 0, msgString);
}

/*!
 * Writes an logfile message from type WARNING
 */
void TTMessageLogger::warningMsg(QString caller, QString msgString)
{
  logMsg(WARNING, caller, 0, msgString);
}


/*!
 * Writes an logfile messages from type ERROR
 */
void TTMessageLogger::errorMsg(QString caller, QString msgString)
{
  logMsg(ERROR, caller, 0, msgString);
}


/*!
 * Writes an logfile messages from type DEBUG
 */
void TTMessageLogger::debugMsg(QString caller, QString msgString)
{
  logMsg(DEBUG, caller, 0, msgString); 
}

void TTMessageLogger::debugMsg(QString caller, int line, QString msgString)
{
  logMsg(DEBUG, caller, line, msgString);
}

/*!
 * Writes an logfile message from type INFO
 */
void TTMessageLogger::infoMsg(QString caller, const char* msg, ...)
{
  char buf[1024];
  va_list ap;

  va_start( ap, msg );
  vsprintf( buf, msg, ap );
  va_end( ap );

  logMsg(INFO, caller, 0, buf);
}

/*!
 * Writes an logfile message from type WARNING
 */
void TTMessageLogger::warningMsg(QString caller, const char* msg, ...)
{
  char buf[1024];
  va_list ap;

  va_start( ap, msg );
  vsprintf( buf, msg, ap );
  va_end( ap );

  logMsg(WARNING, caller, 0, buf);
}

/*!
 * Writes an logfile message from type ERROR
 */
void TTMessageLogger::errorMsg(QString caller, const char* msg, ...)
{
  char buf[1024];
  va_list ap;

  va_start( ap, msg );
  vsprintf( buf, msg, ap );
  va_end( ap );

  logMsg(ERROR, caller, 0, buf);
}

void TTMessageLogger::showErrorMsg(QString caller, const char* msg, ...)
{
  char buf[1024];
  va_list ap;

  va_start( ap, msg );
  vsprintf( buf, msg, ap );
  va_end( ap );

  logMsg(ERROR, caller, 0, buf, true);
}

void TTMessageLogger::debugMsg(QString caller, const char* msg, ...)
{
  char buf[1024];
  va_list ap;

  va_start( ap, msg );
  vsprintf( buf, msg, ap );
  va_end( ap );

  logMsg(DEBUG, caller, 0, buf);
}



/*!
 * This method finally writes the message to the logfile.
 * You must set the message type.  
 */
void TTMessageLogger::logMsg(MsgType msgType, QString caller, int line, QString msgString, bool show)
{
  QString write;
  
  if (!logEnabled && msgType != ERROR)
    return;
  
  if(msgType == INFO)
    write = "[info]";

  if(msgType == WARNING)
    write = "[warning]";

  if(msgType == ERROR)
   write = "[error]";

  if(msgType == DEBUG)
    write = "[debug]";

  write.append("[");
  write.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
  write.append("][");
  write.append(caller);
  //write.append(":");
  //write.append(line);
  write.append("] ");
  write.append(msgString);

  // show message window
  if (show)
    ;

  if (logMode & CONSOLE || msgType == ERROR)
    qDebug(write.toUtf8().data());

  writeMsg(write);
}

/*!
 * This method finally writes the message to the logfile.
 */
void TTMessageLogger::writeMsg(QString msgString)
{
  QTextStream out(logfile);

  out << msgString << "\n"; 

  logfile->flush();
}
