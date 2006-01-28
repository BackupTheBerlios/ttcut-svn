#include "ttmessagelogger.h"

const int     TTMessageLogger::STD_LOG_MODE = TTMessageLogger::SUMMARIZE | CONSOLE;
const char* TTMessageLogger::INFO_FILE_NAME    = "info.log";
const char* TTMessageLogger::WARNING_FILE_NAME = "warning.log";
const char* TTMessageLogger::ERROR_FILE_NAME   = "error.log";
const char* TTMessageLogger::DEBUG_FILE_NAME   = "debug.log";
const char* TTMessageLogger::SUM_FILE_NAME     = "logfile.log";

TTMessageLogger* TTMessageLogger::loggerInstance = NULL;
QString TTMessageLogger::stdLogFilePath = "./logfile.log";


TTMessageLogger::TTMessageLogger()
{
  logfile = new QFile(stdLogFilePath);
  logMode = STD_LOG_MODE;

  QFile file(stdLogFilePath);
  
  if(file.exists())
    file.remove();

  logfile->open(QIODevice::WriteOnly | QIODevice::Text);
}

TTMessageLogger::~TTMessageLogger()
{
  if(logfile != NULL)
  {
    logfile->close();
    delete logfile;
  }
  
}

TTMessageLogger* TTMessageLogger::getInstance()
{
  if(loggerInstance == NULL)
    loggerInstance = new TTMessageLogger();

  return loggerInstance;
}

void TTMessageLogger::infoMsg(QString caller, QString msgString)
{
logMsg(INFO, caller, msgString);
}

void TTMessageLogger::logMsg(MsgType msgType, QString caller, QString msgString)
{
  QString write;
  
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
  write.append("] ");
  write.append(msgString);

  if(logMode & CONSOLE)
    qDebug(write);

  writeMsg(write);
}

void TTMessageLogger::writeMsg(QString msgString)
{
  QTextStream out(logfile);

  out << msgString << "\n"; 

  logfile->flush();
}
