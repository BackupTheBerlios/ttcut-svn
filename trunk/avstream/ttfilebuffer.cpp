/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2005 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttfilebuffer.cpp                                                */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/23/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/01/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/13/2005 */
/* MODIFIED: b. altendorf                                    DATE: 04/21/2005 */
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

// -----------------------------------------------------------------------------
// Buffered file I/O
// -----------------------------------------------------------------------------

#include "ttfilebuffer.h"

#include <stdio.h>

#define MAX_BUFFER_SIZE 1048576

const char c_name [] = "TTFILEBUFFER  : ";

uint8_t start_code [] = {0x00, 0x00, 0x01};

// Construct object
// -----------------------------------------------------------------------------
TTFileBuffer::TTFileBuffer( )
{
  file_handle      = -1;         /*..no file descriptor.......................*/
  mem_buffer       = NULL;       /*..no memory buffer.........................*/
  buffer_size      = 1048576;    /*..memory buffer size.......................*/

  buffer_read_size = 0;
  buffer_start     = 0;
  buffer_end       = 0;
  buffer_pos       = 0;
  stream_pos       = 0;
  stream_length    = 0;

  last_buffer_read = false;
  stream_eof       = false;

  file_name[0]     = '\0';
  file_mode        = -1;

  fill_count       = 0;
  read_count       = 0;

  initTSearch();
}


/* Construct object with file name*/
/* -----------------------------------------------------------------------------*/
TTFileBuffer::TTFileBuffer( const char* f_name, int f_mode )
{
  file_handle      = -1;         /*..no file descriptor.......................*/
  mem_buffer       = NULL;       /*..no memory buffer.........................*/
  buffer_size      = 1048576;    /*..memory buffer size.......................*/

  buffer_read_size = 0;
  buffer_start     = 0;
  buffer_end       = 0;
  buffer_pos       = 0;
  stream_pos       = 0;
  stream_length    = 0;

  last_buffer_read = false;
  stream_eof       = false;

  fill_count       = 0;
  read_count       = 0;

  openFile( f_name, f_mode );

  initTSearch();
}


/* Destruct object*/
/* -----------------------------------------------------------------------------*/
TTFileBuffer::~TTFileBuffer()
{
  releaseBuffer();

  if ( file_handle > -1 )
  {
    //printf("%sclose file: %s\n",c_name,file_name);
    close( file_handle );
  }
}
  
/* Open file*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::openFile( const char* f_name, int f_mode)
{
  bool     b_result;
  // UNUSED: uint64_t offset;

  buffer_read_size = 0;
  buffer_start     = 0;
  buffer_end       = 0;
  buffer_pos       = 0;
  stream_pos       = 0;
  stream_length    = 0;

  last_buffer_read = false;
  stream_eof       = false;

  file_name[0]     = '\0';
  file_mode        = f_mode;

  /* get the file name*/
  strcpy( file_name, f_name );

  //printf("%sopen file: %s\n",c_name,file_name);

  /* if a file already open close the open file*/
  if ( file_handle > -1 )
    close( file_handle );

  /* select open statement according to file_mode*/
  switch ( file_mode )
  {
   case fm_create :
     file_handle   = open( file_name, O_RDWR | O_CREAT, 0644);
     stream_length = 0;
     break;

   case fm_open_write :
     file_handle = open( file_name, O_WRONLY | O_CREAT, 0644);
     if ( file_handle > -1  )
       stream_length = streamLength();
     else
       stream_length = 0;
     break;

   case fm_open_read :
     file_handle = open( file_name, O_RDONLY);
     if ( file_handle > -1  )
       stream_length = streamLength();
     else
       stream_length = 0;
     initBuffer();
     break;
   }

   if ( file_handle > -1 )
     b_result    = true;
   else
   {
     stream_eof = true;
     b_result   = false;
     file_mode  = -1;
   }
   return b_result;
}


/* Close file*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::closeFile()
{
  //printf("%sclose file: %s\n",c_name,file_name);

  buffer_read_size = 0;
  buffer_start     = 0;
  buffer_end       = 0;
  buffer_pos       = 0;
  stream_pos       = 0;
  stream_length    = 0;

  last_buffer_read = true;
  stream_eof       = true;

  file_name[0]     = '\0';
  file_mode        = -1;

  if ( file_handle > -1 )
    close( file_handle );

  file_handle = -1;
}


/* Get file size in bytes*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::streamLength()
{
  off64_t offset;
  off64_t o_result;

  if ( file_handle > -1 )
  {
     /*save current value of the file position pointer*/
    offset = lseek64( file_handle, (off64_t)0, SEEK_CUR );
    /*set file pointer at end of file*/
    lseek64( file_handle, (off64_t)0, SEEK_END );
    /*gets size of file*/
    o_result = lseek64( file_handle, (off64_t)0, SEEK_CUR );
    /*set the pointer back to previous position*/
    lseek64( file_handle, offset, SEEK_SET );
  }
  else
    o_result = (off64_t)-1;

  /*printf("fileSize: %lld\n",o_result);*/
  return o_result;
}


/* Set memory buffer size*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::setBufferSize( int size )
{
  if ( buffer_size != size )
  {
    releaseBuffer();

    if ( size > MAX_BUFFER_SIZE )
      buffer_size = 1048576;
    else
      buffer_size = size;

    initBuffer();
  }
}


/* Get buffer size*/
/* -----------------------------------------------------------------------------*/
int TTFileBuffer::bufferSize()
{
  return  buffer_size;
}


bool TTFileBuffer::readUInt8( uint8_t &byte1 )
{
  return readArray( (uint8_t*)&byte1, 1 );
}

bool TTFileBuffer::readUInt16( uint16_t &byte2 )
{
  return readArray( (uint8_t*)&byte2, 2 );
}

bool TTFileBuffer::readUInt32( uint32_t &byte4 )
{
  return readArray( (uint8_t*)&byte4, 4 );
}

bool TTFileBuffer::readUInt64( uint64_t &byte8 )
{
  return readArray( (uint8_t*)&byte8, 8 );
}

/* Read x bytes from stream*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::readArray( uint8_t* read_buffer, int read_length )
{
  int  i_read   = 0;
  bool b_result = true;

  while ( b_result && i_read < read_length )
  {
    b_result = readByte( read_buffer[i_read] );
    i_read++;
  }

  if ( i_read < read_length )
    b_result = false;

  return b_result;
}

bool TTFileBuffer::readArray( uint8_t* read_buffer, __attribute__ ((unused))int start_pos, int read_length )
{
  // TODO: read array with start pos
  return readArray( read_buffer, read_length );
}

int TTFileBuffer::readCount2( uint8_t* read_buffer, __attribute__ ((unused))int start_pos, int read_length )
{
  long current_read_count = read_count;

  readArray( read_buffer, read_length );

  return (int)read_count-current_read_count;
}

/* Read x bytes from stream*/
/* -----------------------------------------------------------------------------*/
int TTFileBuffer::readBuffer( uint8_t* read_buffer, int read_length )
{
  int  i_read   = 0;
  bool b_result = true;
  
  try
  {
    while ( b_result && i_read < read_length )
    {
      b_result = readByte( read_buffer[i_read] );
      i_read++;
    }
  }
  catch ( TTStreamEOFException )
  {
    printf("%sreadBuffer: EOF: %d\n",c_name,i_read);
    return i_read;
  }
    
  return i_read;
}


/* "Brute force" search for next start-code in the stream*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::nextStartCodeBF()
{
  byte4 = 0xFFFFFFFF;

  while ( !stream_eof && ((byte4 & 0xFFFFFF)) != 1 )
  {
   /* buffer position inside memory buffer area;read from memory buffer*/
    if ( buffer_pos < buffer_read_size )
    {
      byte4 = (byte4 << 8) | mem_buffer[buffer_pos];

      /* increment bufferPosition and streamPosition*/
      buffer_pos++;
      stream_pos++;
      read_count++;
    }

    /* new buffer pos outside memory buffer; fill the buffer*/
    if ( buffer_pos >= buffer_read_size )
    {
      /* we are in the last stream buffer near the stream end*/
      if ( last_buffer_read )
      {
        stream_eof = true;
      }
      /* fill the memory buffer*/
      else
      {
        fillBuffer();
      }
    }
  }
}

/* Initialize "T-Search"*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::initTSearch()
{
  int i;

  t_delta = 3;

  for ( i = 0; i < 256; i++ )
    shift[i] = t_delta+1;

  for ( i = 0; i < t_delta; i++ )
    shift[start_code[i]] = t_delta - i;
  
  look_at = 0;

  while ( look_at < t_delta-1 )
  {
    if (start_code[t_delta-1] == start_code[t_delta-(look_at+2)])
      break;
    look_at++;
  }
}

/* "T-Search" for next start-code sequence (0x000001) in the stream           */
/* ---------------------------------------------------------------------------*/
void TTFileBuffer::nextStartCodeTS()
{
  do
  {
    t_index = t_delta-1;

    while ( mem_buffer[buffer_pos+t_index] != start_code[t_index] )
    {
      if( !seekRelative(shift[mem_buffer[buffer_pos+t_delta]]) )
        return;
    }
    --t_index;
    while ( mem_buffer[buffer_pos+t_index] == start_code[t_index] )
    {
      if ( --t_index < 0 )
      {
        seekRelative(t_delta);
        return; 
      }
    }
    bool seek = seekRelative(look_at+shift[mem_buffer[buffer_pos+t_delta+look_at]]);
    if(!seek)
    {
      //printf("next start code: %d\n", seek);
      //printf("t-search index: %d / %d\n", t_index, look_at+shift[mem_buffer[buffer_pos+t_delta+look_at]]);
      return;
    }
 
  }while(-1);
}



/* Read x bytes from stream and fill read_buffer in reverse order*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::readArrayRev( uint8_t* read_buffer, int read_length )
{
  int  i_read   = read_length-1;
  bool b_result = true;

  while ( b_result && i_read >= 0)
  {
    b_result = readByte( read_buffer[i_read] );
    i_read--;
  }

  if ( i_read > - 1 )
    b_result = false;

  return b_result;
}


/* Unbuffered read*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::directRead( uint8_t* read_buffer, off64_t read_length )
{
  off64_t o_result;

  if ( file_mode == fm_open_read && file_handle > -1 )
  {
    stream_pos = lseek64( file_handle,(off64_t)0,SEEK_CUR );
    o_result   = read( file_handle, read_buffer, read_length );
    o_result  += stream_pos;
  }
  else
    o_result = (off64_t)-1;

  return o_result;
}

off64_t TTFileBuffer::directReadUInt8( uint8_t &byte1 )
{
  return directRead( (uint8_t*)byte1, 1 );
}

off64_t TTFileBuffer::directReadUInt16( uint16_t &byte2 )
{
  return directRead( (uint8_t*)byte2, 2 );
}

off64_t TTFileBuffer::directReadUInt32( uint32_t &byte4 )
{
  return directRead( (uint8_t*)byte4, 4 );
}

off64_t TTFileBuffer::directReadUInt64( uint64_t &byte8 )
{
  return directRead( (uint8_t*)byte8, 8 );
}

/* Unbuffered write*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::directWrite( const uint8_t* write_buf, int write_len )
{
  off64_t o_result;

  if ( ((file_mode == fm_open_write) || (file_mode == fm_create)) &&
       (file_handle > -1) )
  {
    stream_pos  = lseek64( file_handle,(off64_t)0,SEEK_CUR );
    o_result    = write( file_handle, write_buf, write_len );
    o_result   += stream_pos;
    stream_pos  = o_result;
    if ( o_result > stream_length )
      stream_length = o_result;
  }
  else
    o_result = (off64_t)-1;

  //printf("directWrite: %lld, len: %lld\n",o_result,write_len);
  return o_result;
}

off64_t TTFileBuffer::directWriteUInt8( uint8_t byte1 )
{
  return directWrite( (uint8_t*)&byte1, 1 );
}

off64_t TTFileBuffer::directWriteUInt16( uint16_t byte2 )
{
  return directWrite( (uint8_t*)&byte2, 2 );
}

off64_t TTFileBuffer::directWriteUInt32( uint32_t byte4 )
{
  return directWrite( (uint8_t*)&byte4, 4 );
}

off64_t TTFileBuffer::directWriteUInt64( uint64_t byte8 )
{
  return directWrite( (uint8_t*)&byte8, 8 );
}

/* stream end reached*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::streamEOF()
{
  return stream_eof;
}


/* last memory buffer before stream end*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::lastBufferRead()
{
  return last_buffer_read;
}


/* Rewind*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::rewindFile()
{
  lseek64( file_handle, 0, SEEK_SET );
  initBuffer();
}


/* Seek forward x bytes*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::seekForward( off64_t step_bytes )
{
  return newPosition( stream_pos + step_bytes );
}


/* Seek backward x bytes*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::seekBackward( off64_t step_bytes )
{
  return newPosition( stream_pos - step_bytes );
}


/* Seek x bytes relative to current position*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::seekRelative( off64_t step_bytes )
{
  /*printf("seekRelative: %lld - %lld\n",stream_pos,step_bytes);*/

  return newPosition( stream_pos + step_bytes );
}


/* Seek x bytes relative to stream beginning*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::seekAbsolute( off64_t step_bytes )
{
  return newPosition( step_bytes );
}


/* Current stream offset*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::currentOffset()
{
  return stream_pos;
}

char* TTFileBuffer::fileName()
{
  return file_name;
}


/* Current buffer start offset*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::bufferStart()
{
  return buffer_start;
}


/* Current buffer end offset*/
/* -----------------------------------------------------------------------------*/
off64_t TTFileBuffer::bufferEnd()
{
  return buffer_end;
}


/* Current buffer relative position*/
/* -----------------------------------------------------------------------------*/
int TTFileBuffer::bufferPos()
{
  return buffer_pos;
}


/* Pufferreigeben*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::releaseBuffer()
{
  if ( mem_buffer != NULL )
  {
    delete [] mem_buffer;
    mem_buffer = NULL;
  }
}


/* PufferInitialisieren*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::initBuffer()
{
  /* release buffer */
  releaseBuffer();

  /* Ist die Datei zum lesen geoeffnet und die Puffergroesse*/
  /* groesser 0 wird Speicher besorgt und der Puffer mit dem*/
  /* Inhalt der Datei ab der aktuellen Position neu gefuellt*/
  /* Ansonsten ist der Puffer als leer zu betrachten*/
  if ( file_mode == fm_open_read || file_mode == fm_create )
  {
    if ( buffer_size > 0 )
      mem_buffer = new uint8_t[buffer_size];

    if ( file_handle > -1 )
    {
      stream_pos = lseek64( file_handle, buffer_start, SEEK_SET );
    }
    else
    {
      stream_pos     = 0;
      buffer_start   = 0;
      buffer_end     = 0;
    }

    buffer_pos       = 0;
    last_buffer_read = false;
    stream_eof       = false;

    fillBuffer();
  }
  else
  {
    printf("%swrong file mode (!)\n",c_name);   
  }
}


/* Fill the memory buffer (private)*/
/* -----------------------------------------------------------------------------*/
void TTFileBuffer::fillBuffer()
{
  /* buffer start position in file stream*/
  buffer_start     = stream_pos;
  buffer_pos       = 0;
  buffer_read_size = 0;
  fill_count++;

  /* try to read buffer_size bytes from the stream*/
  buffer_read_size = read( file_handle, mem_buffer, buffer_size );

  /* set bufferEnd position*/
  buffer_end = buffer_start + buffer_read_size;

  /* less bytes read than bufferSize; we are near the stream end*/
  if ( buffer_read_size < buffer_size )
    last_buffer_read = true;
}


/* Read one byte from stream (private)*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::readByte( uint8_t &byte1 )
{
  b_result = false;

  /* buffer position inside memory buffer area;read from memory buffer*/
  if ( buffer_pos < buffer_read_size )
  {
    byte1    = mem_buffer[buffer_pos];
    b_result = true;
	
    /* increment bufferPosition and streamPosition*/
    buffer_pos++;
    stream_pos++;
    read_count++;
  }

  /* new buffer pos outside memory buffer; fill the buffer*/
  if ( buffer_pos >= buffer_read_size )
  {
    /* we are in the last stream buffer near the stream end*/
    if ( last_buffer_read )
    {
      stream_eof = true;
      b_result   = false;
      //printf("EOF: %ld / %ld\n",buffer_pos,buffer_read_size);
      throw TTStreamEOFException();
    }
    /* fill the memory buffer*/
    else
    {
      fillBuffer();
      b_result = true;
    }
  }
  return b_result;
}


/* Seek to new position in stream (private)*/
/* -----------------------------------------------------------------------------*/
bool TTFileBuffer::newPosition( off64_t new_pos )
{
  /* new position inside memory buffer*/
  if ( new_pos >= buffer_start  &&
      new_pos <  buffer_end    
     )
  {
    buffer_pos += new_pos - stream_pos;
    stream_pos += new_pos - stream_pos;
    b_result    = true;
    stream_eof  = false;
  }
  /* new position outside buffer*/
  else
  {
    if ( new_pos > -1             &&
        (file_mode  == fm_open_read && new_pos < stream_length) ||
        ((file_mode == fm_open_write || file_mode == fm_create) &&
         new_pos < stream_length + 1) )
    {
      /* seek to new Position*/
      stream_pos = lseek64(file_handle, (off64_t)new_pos, SEEK_SET);

      stream_eof       = false;
      last_buffer_read = false;

      fillBuffer();

      if ( stream_pos == new_pos )
        b_result = true;
      else
        b_result = false;
    }
    else
      b_result = false;
  }
  return b_result;
}


// statistic data
// -----------------------------------------------------------------------------
long TTFileBuffer::fillCount()
{
  return fill_count;
}

long TTFileBuffer::readCount()
{
  return read_count;
}








