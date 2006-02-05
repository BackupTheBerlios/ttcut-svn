/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttfilebuffer.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/21/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED: b. altendorf                                    DATE: 06/05/2005 */
/* MODIFIED:                                                 DATE:            */
/*----------------------------------------------------------------------------*/

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

#ifndef TTFILEBUFFER_H
#define TTFILEBUFFER_H

// C header
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

extern "C" bool    ttAssigned( const void* pointer );

// constants for file open mode
#define fm_create      2
#define fm_open_write  4
#define fm_open_read   8

// -----------------------------------------------------------------------------
// TTFileBuffer: class declaration
// -----------------------------------------------------------------------------
class TTFileBuffer
{
public:
  // create / delete
  TTFileBuffer();
  TTFileBuffer( const char* f_name, int f_mode );
  ~TTFileBuffer();

  // file stream
  bool    openFile( const char* f_name, int f_mode );
  void    closeFile();
  off64_t streamLength();
  bool    streamEOF();
  off64_t currentOffset();
  char*   fileName();

  // memory buffer
  void    setBufferSize( int size );
  int     bufferSize();
  bool    lastBufferRead();
  off64_t bufferStart();
  off64_t bufferEnd();
  int     bufferPos();

  // read / write
  bool    readByte( uint8_t &byte1 );
  bool    readUInt8( uint8_t &byte1 );
  bool    readUInt16( uint16_t &byte2 );
  bool    readUInt32( uint32_t &byte4 );
  bool    readUInt64( uint64_t &byte8 );
  bool    readArray( uint8_t* read_buffer, int read_length );
  bool    readArray( uint8_t* read_buffer, int start_pos, int read_length );
  int     readCount2( uint8_t* read_buffer, int start_pos, int read_length );
  bool    readArrayRev( uint8_t* read_buffer, int read_length );
  int     readBuffer( uint8_t* read_buffer, int read_length );
  off64_t directRead( uint8_t* read_buffer, off64_t read_length );
  off64_t directReadUInt8( uint8_t &byte1 );
  off64_t directReadUInt16( uint16_t &byte2 );
  off64_t directReadUInt32( uint32_t &byte4 );
  off64_t directReadUInt64( uint64_t &byte8 );
  off64_t directWrite( const uint8_t* write_buf, int write_len );
  off64_t directWriteUInt8( uint8_t byte1 );
  off64_t directWriteUInt16( uint16_t byte2 );
  off64_t directWriteUInt32( uint32_t byte4 );
  off64_t directWriteUInt64( uint64_t byte8 );

  // search
  void    nextStartCodeBF();
  void    initTSearch();
  void    nextStartCodeTS();

  // positioning
  void    rewindFile();
  bool    seekForward( off64_t step_bytes );
  bool    seekBackward( off64_t step_bytes );
  bool    seekRelative( off64_t step_bytes );
  bool    seekAbsolute( off64_t step_bytes );

  // statistic
  long    readCount();
  long    fillCount();

 protected:
  void    releaseBuffer();
  void    initBuffer();
  void    fillBuffer();
  bool    newPosition( off64_t new_pos );


 private:
  bool     ts_debug;
  char     file_name[256];     /*..name of opened stream......................*/
  int      file_mode;          /*..open mode: read, write, create.............*/
  int      file_handle;        /*..handle to open stream......................*/
  uint8_t* mem_buffer;         /*..memory buffer..............................*/
  bool     last_buffer_read;   /*..last mem buffer filled with data...........*/
  bool     stream_eof;         /*..end of stream..............................*/
  long     buffer_size;        /*..memory buffer size.........................*/
  long     buffer_read_size;   /*..current read bytes from stream into buffer.*/
  off64_t  buffer_start;       /*..buffer start offset from stream beginning..*/
  off64_t  buffer_end;         /*..buffer end offset from stream beginning....*/
  long     buffer_pos;         /*..current pointer position in buffer.........*/
  off64_t  stream_pos;         /*..current pointer position in stream.........*/
  off64_t  stream_length;      /*..actual stream length in bytes..............*/

  long     byte4;
  int      t_delta;
  int      t_index;
  uint8_t  shift[256];
  int      look_at;
  
  bool     b_result;
  long     fill_count;
  long     read_count;
};

class TTStreamException
{
};

class TTStreamEOFException : public TTStreamException
{
};

class TTStreamOpenException : public TTStreamException
{
};

class TTStreamReadException : public TTStreamException
{
};

class TTStreamWriteException : public TTStreamException
{
};

class TTStreamSeekException : public TTStreamException
{
};

class TTStreamMiscException : public TTStreamException
{
};


#endif //TTFILEBUFFER_H
