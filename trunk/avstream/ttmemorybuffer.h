

#ifndef TTMEMORYBUFFER_H
#define TTMEMORYBUFFER_H

#include "ttcommon.h"

#include <QByteArray>
#include <QDataStream>

#ifdef __WIN32
#include "ttwfilebuffer.h"
#else
#include "ttfilebuffer.h"
#endif

class TTMemoryBuffer : public QByteArray
{
 public:
  TTMemoryBuffer( int size=64 );
  TTMemoryBuffer( uint8_t* buffer );
  TTMemoryBuffer( uint8_t* buffer, int start, int size );
  ~TTMemoryBuffer();

  quint8  readUInt8();
  quint16 readUInt16();
  qint16  readInt16();
  quint32 readUInt32();
  qint32  readInt32();
  quint64 readUInt64();

  bool write( uint8_t byte1 );
  bool write( uint16_t byte2 );
  bool write( uint32_t byte4 );
  bool write( int32_t byte4 );
  bool writeByte( uint8_t byte );

  bool writeTo( TTFileBuffer* f_buffer );

  int currentPosition();
  enum SeekOrigin { begin=0, current=1, end=2 };
  bool seek( int pos, SeekOrigin origin=begin );

  uint8_t* toArray();

 private:
  QDataStream* byte_stream;
  int          buffer_size;
  uint8_t*     byte_buffer;
  uint8_t*     temp_buffer;
};

#endif //TTMEMORYBUFFER_H


