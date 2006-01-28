
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
    TTMessageLogger();
~TTMessageLogger();

  public:
    static TTMessageLogger* getInstance();

    void infoMsg(QString caller, QString msgString);

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
   
