

#ifndef TTAUDIOLISTDATA_H
#define TTAUDIOLISTDATA_H

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

    QFileInfo getFileInfo();
    TTAudioStream* getAudioStream();
    
  private:
    QFileInfo audioFileInfo;
    TTAudioStream* audioStream;
};


class TTAudioListData
{
  public:
    TTAudioListData();
    ~TTAudioListData();

    int  addItem(QString fName, TTAudioStream* aStream);
    void writeToProject(TTCutProject* prj);
    int  count();
    TTAudioStream* audioStream(int index);
    
  private:
    QList<TTAudioListDataItem>data;

};
#endif
