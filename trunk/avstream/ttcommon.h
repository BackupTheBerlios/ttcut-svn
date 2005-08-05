
//#define __WIN32

//#pragma warning(disable:4190)
//#pragma warning(disable:4100) //Unreferenzierter formaler Parameter

#include <qstring.h>
#include <qdatetime.h>
class QString;
class QTime;
class TTTimeCode;

extern "C" bool    ttAssigned( const void* pointer );
extern "C" bool    ttFileExists( QString fName );
extern "C" bool    ttDeleteFile( QString fName );
extern "C" QString ttAddFileExt( QString fName, const char* cExt );
extern "C" QString ttChangeFileExt( QString fName, const char* cExt );
extern "C" QTime   ttMsecToTime( int msec );
extern "C" QTime   ttMsecToTimeD( double msec );
extern "C" QTime   ttFramesToTime(long lFrames, float fps);
extern "C" long    ttTimeToFrames(QTime timeCode, float fps);
extern "C" TTTimeCode* ttFrameToTimeCode( int FrameNr, float fps);

typedef Q_INT64 int64;
#ifdef __WIN32
extern "C"  int trunc( double f );
extern "C"  int round( double f );
typedef long off64_t;
typedef Q_UINT64 uint64_t;
typedef Q_UINT32 uint32_t;
typedef Q_UINT16 uint16_t;
typedef Q_UINT8  uint8_t;
typedef Q_INT8   int8_t;
typedef Q_INT16  int16_t;
typedef Q_INT32  int32_t;
typedef Q_INT64  int64_t;
typedef Q_INT8   int8;
typedef Q_INT16  int16;
typedef Q_INT32  int32;
typedef Q_INT64  int64;
#endif


#ifndef TTTIMECODE_H
#define TTTIMECODE_H

class TTTimeCode
{
 public:
  TTTimeCode();
  QString toString();

  bool drop_frame_flag;
  short hours;
  short minutes;
  short seconds;
  short pictures;
  bool marker_bit;
  
};
#endif
