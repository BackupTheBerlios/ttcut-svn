
#include "ttmemorybuffer.h"

TTMemoryBuffer::TTMemoryBuffer( int size )
  : QByteArray( size )
{
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadWrite );
  buffer_size = byte_stream->device()->size();

  temp_buffer = NULL;
}

TTMemoryBuffer::TTMemoryBuffer( uint8_t* buffer )
  : QByteArray()
{
  byte_buffer = buffer;

  setRawData( (char*)byte_buffer, sizeof(byte_buffer) );
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadOnly );
  buffer_size = byte_stream->device()->size();

  temp_buffer = NULL;
}

TTMemoryBuffer::TTMemoryBuffer( uint8_t* buffer, int start, int size )
  : QByteArray()
{
  int i;

  temp_buffer = new uint8_t [size];

  for ( i = 0; i < size; i++ )
  {
    if ( start+i < sizeof(buffer) )
      temp_buffer[i] = buffer[start+i];
  }

  setRawData( (char*)temp_buffer, sizeof(temp_buffer) );
  byte_stream = new QDataStream( (QByteArray*)this, QIODevice::ReadOnly );
  buffer_size = byte_stream->device()->size();
}

TTMemoryBuffer::~TTMemoryBuffer()
{
  if ( temp_buffer != NULL )
  {
    resetRawData( (char*)temp_buffer, sizeof(temp_buffer) );
    delete [] temp_buffer;
  }
  else
    resetRawData( (char*)byte_buffer, sizeof(byte_buffer) );
}

int TTMemoryBuffer::currentPosition()
{
  return (int)0;  
}

bool TTMemoryBuffer::seek( int pos, SeekOrigin origin )
{
  if ( origin == begin )
    return byte_stream->device()->at( pos );
  else
    return byte_stream->device()->at( buffer_size-pos );
}


Q_UINT8 TTMemoryBuffer::readUInt8()
{
  Q_UINT8 byte1;

  *byte_stream >> byte1;

  return byte1;
}


Q_UINT16 TTMemoryBuffer::readUInt16()
{
  Q_UINT16 byte16;

  *byte_stream >> byte16;

  return byte16;
}


Q_INT16 TTMemoryBuffer::readInt16()
{
  Q_INT16 byte16;

  *byte_stream >> byte16;

  return byte16;
}


Q_UINT32 TTMemoryBuffer::readUInt32()
{
  Q_UINT32 byte32;

  *byte_stream >> byte32;

  return byte32;
}


Q_INT32 TTMemoryBuffer::readInt32()
{
  Q_INT32 byte32;

  *byte_stream >> byte32;

  return byte32;
}

Q_UINT64 TTMemoryBuffer::readUInt64()
{
  Q_UINT64 byte64;

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

bool TTMemoryBuffer::writeTo( TTFileBuffer* f_buffer )
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
