
#include "ttmemorybuffer.h"

TTMemoryBuffer::TTMemoryBuffer( int size )
  : QByteArray( size, ' ' )
{
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadWrite );
  buffer_size = byte_stream->device()->size();

  temp_buffer = 0;
}

TTMemoryBuffer::TTMemoryBuffer( uint8_t* buffer )
  : QByteArray()
{
  byte_buffer = buffer;

  fromRawData( (char*)byte_buffer, sizeof(byte_buffer) );
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadOnly );
  buffer_size = byte_stream->device()->size();

  temp_buffer = 0;
}

TTMemoryBuffer::TTMemoryBuffer( uint8_t* buffer, int start, int size )
  : QByteArray()
{
  int i;

  temp_buffer = new uint8_t [size];

  for ( i = 0; i < size; i++ )
  {
    if ( start+i < (int)sizeof(buffer) )
      temp_buffer[i] = buffer[start+i];
  }

  fromRawData( (char*)temp_buffer, sizeof(temp_buffer) );
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadOnly );
  buffer_size = byte_stream->device()->size();
}

TTMemoryBuffer::~TTMemoryBuffer()
{
  //Wie geht das ????
// clear();

  //if (byte_stream != 0)
    //delete byte_stream;

   //if (temp_buffer != 0)
    //delete [] temp_buffer;
}

int TTMemoryBuffer::currentPosition()
{
  return (int)0;  
}

bool TTMemoryBuffer::seek( int pos, SeekOrigin origin )
{
  if ( origin == begin )
    return byte_stream->device()->seek( pos );
  else
    return byte_stream->device()->seek( buffer_size-pos );
}


quint8 TTMemoryBuffer::readUInt8()
{
  quint8 byte1;

  *byte_stream >> byte1;

  return byte1;
}


quint16 TTMemoryBuffer::readUInt16()
{
  quint16 byte16;

  *byte_stream >> byte16;

  return byte16;
}


qint16 TTMemoryBuffer::readInt16()
{
  qint16 byte16;

  *byte_stream >> byte16;

  return byte16;
}

quint32 TTMemoryBuffer::readUInt32()
{
  quint32 byte32;

  *byte_stream >> byte32;

  return byte32;
}


qint32 TTMemoryBuffer::readInt32()
{
  qint32 byte32;

  *byte_stream >> byte32;

  return byte32;
}

quint64 TTMemoryBuffer::readUInt64()
{
  quint64 byte64;

  *byte_stream >> byte64;

  return byte64;
}

bool TTMemoryBuffer::write( uint8_t byte1 )
{
  *byte_stream << byte1;

  return true;
}

bool TTMemoryBuffer::write( uint16_t byte2 )
{
  *byte_stream << byte2;
  return true;
}

bool TTMemoryBuffer::write( uint32_t byte4 )
{
  *byte_stream << byte4;
  return true;
}

bool TTMemoryBuffer::write( int32_t byte4 )
{
  *byte_stream << byte4;
  return true;
}

bool TTMemoryBuffer::writeByte( uint8_t byte )
{
  *byte_stream << byte;
  return true;
}

bool TTMemoryBuffer::writeTo( __attribute__ ((unused))TTFileBuffer* f_buffer )
{
  return true;

}

uint8_t* TTMemoryBuffer::toArray()
{
  int i;
  uint8_t* ret_buffer = new uint8_t [size()];

  for ( i = 0; i < size(); i++ )
  {
    qDebug( "to array: %x",(uint8_t)at(i) );
    ret_buffer[i] = (uint8_t)at(i);
  }

  return ret_buffer;
}
