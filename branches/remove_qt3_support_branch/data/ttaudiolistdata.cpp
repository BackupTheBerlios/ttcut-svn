#include "ttaudiolistdata.h"

#include <QFileInfo>
#include <QString>

/*!
 * This class represent an item in the list
 */
TTAudioListDataItem::TTAudioListDataItem()
{
  audioStream = NULL;
}

TTAudioListDataItem::TTAudioListDataItem(QFileInfo fInfo, TTAudioStream* aStream)
{
  audioFileInfo = fInfo;
  audioStream   = aStream;
}

QFileInfo TTAudioListDataItem::getFileInfo()
{
  return audioFileInfo;
}

TTAudioStream* TTAudioListDataItem::getAudioStream()
{
  return audioStream;
}


/*!
 * This class represents the audio list object
 */
TTAudioListData::TTAudioListData()
{
}

TTAudioListData::~TTAudioListData()
{
}

int TTAudioListData::addItem(QString fName, TTAudioStream* aStream)
{
  QFileInfo fInfo(fName);

  TTAudioListDataItem item(fInfo, aStream);

  data.append(item);

  return data.count()-1;
}

int TTAudioListData::count()
{
  return data.count();
}

TTAudioStream* TTAudioListData::audioStream(int index)
{
  return data[index].audioStream;
}
void TTAudioListData::writeToProject(TTCutProject* prj)
{
}

