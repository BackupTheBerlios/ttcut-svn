

#ifndef TTAUDIOLISTDATA_H
#define TTAUDIOLISTDATA_H

#include "../common/ttmessagelogger.h"
#include "../avstream/ttavheader.h"
#include "../avstream/ttavstream.h"
#include "../gui/ttcutproject.h"

#include <QList>

class TTAudiostream;
class TTCutProject;
class QString;
class QFileInfo;

class TTAudioListDataItem
{
  friend class TTAudioListData;
  
  public:
    TTAudioListDataItem();
    TTAudioListDataItem(QFileInfo fInfo, TTAudioStream* aStream);

    QFileInfo      getFileInfo();
    TTAudioStream* getAudioStream();
    QString        getFileName();
    QString        getLength();
    QString        getVersion();
    QString        getMode();
    QString        getBitrate();
    QString        getSamplerate();
    QString        getDelay();
    
  private:
    QFileInfo      audioFileInfo;
    TTAudioStream* audioStream;
    QString        audioLength;
    QString        audioVersion;
    QString        audioMode;
    QString        audioSamplerate;
    QString        audioBitrate;
    QString        audioDelay;    
};


class TTAudioListData
{
  public:
    TTAudioListData();
    ~TTAudioListData();

    int  addItem(QString fName, TTAudioStream* aStream);
    TTAudioListDataItem& itemAt(int index);
    TTAudioStream* audioStreamAt(int index);
    int  count();
    void deleteAll();
    void removeAt(int index);
    void swap(int a, int b);
    void print();
    void writeToProject(TTCutProject* prj);

  private:
    TTMessageLogger* log;
    QList<TTAudioListDataItem>data;

};

#endif //TTAUDIOLISTDATA_H
