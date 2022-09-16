/*
* filehandler.cc -- saving DV data into different file formats
* Copyright (C) 2000 Arne Schirmacher <arne@schirmacher.de>
* Raw DV, JPEG, and Quicktime portions Copyright (C) 2003 Dan Dennedy <dan@dennedy.org>
* Portions of Quicktime code borrowed from Arthur Peters' dv_utils.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using std::cerr;
using std::cout;
using std::endl;
using std::ostringstream;
using std::setw;
using std::setfill;

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>

#include "filehandler.h"
//#include "error.h"
#include "riff.h"
#include "avi.h"
#include "frame.h"
#include "affine.h"

FileTracker *FileTracker::instance = NULL;

FileTracker::FileTracker( ) : mode( CAPTURE_MOVIE_APPEND )
{
	return ;
	cerr << ">> Constructing File Capture tracker" << endl;
}

FileTracker::~FileTracker( )
{
	return ;
	cerr << ">> Destroying File Capture tracker" << endl;
}

FileTracker &FileTracker::GetInstance( )
{
	if ( instance == NULL )
		instance = new FileTracker();

	return *instance;
}

void FileTracker::SetMode( FileCaptureMode mode )
{
	this->mode = mode;
}

FileCaptureMode FileTracker::GetMode( )
{
	return this->mode;
}

char *FileTracker::Get( int index )
{
	return list[ index ];
}

void FileTracker::Add( const char *file )
{
	return ;
	if ( this->mode != CAPTURE_IGNORE )
	{
		cerr << ">>>> Registering " << file << " with the tracker" << endl;
		list.push_back( strdup( file ) );
	}
}

unsigned int FileTracker::Size( )
{
	return list.size();
}

void FileTracker::Clear( )
{
	while ( Size() > 0 )
	{
		free( list[ Size() - 1 ] );
		list.pop_back( );
	}
	this->mode = CAPTURE_MOVIE_APPEND;
}

FileHandler::FileHandler()
{
	done = false;
	autoSplit = false;
	maxFrameCount = 999999;
	lastCollectionFreeSpace = 0;
	currentCollectionSize = 0;
	framesWritten = 0;
	filename = "";
	/* empty body */
}


FileHandler::~FileHandler()
{
	/* empty body */
}


void FileHandler::CollectionCounterUpdate()
{
	// Called when a split occurs!
	//cerr << ">> Updating Collection Counter" << endl;
	if ( GetSizeSplitMode() == 1) 
	{
		currentCollectionSize += GetFileSize();

		// Shove this cut into the last collection if there's room
		if (currentCollectionSize <= lastCollectionFreeSpace) 
		{
			currentCollectionSize = 0;
			lastCollectionFreeSpace = lastCollectionFreeSpace - currentCollectionSize;
		}
		else
			lastCollectionFreeSpace = 0;

		// Start a new collection if we've gone over the Minimum Collection Size
		if (currentCollectionSize >= GetMinColSize())
		{
			if (currentCollectionSize < GetMaxColSize())
				lastCollectionFreeSpace = GetMaxColSize() - currentCollectionSize;
			else
				lastCollectionFreeSpace = 0;
			currentCollectionSize = 0; 
		}
	}
}


bool FileHandler::GetAutoSplit() const
{
	return autoSplit;
}


bool FileHandler::GetTimeStamp() const
{
	return timeStamp;
}


bool FileHandler::GetTimeSys() const
{
	return timeSys;
}


bool FileHandler::GetTimeCode() const
{
	return timeCode;
}


string FileHandler::GetBaseName() const
{
	return base;
}


string FileHandler::GetExtension() const
{
	return extension;
}


int FileHandler::GetMaxFrameCount() const
{
	return maxFrameCount;
}

off_t FileHandler::GetMaxFileSize() const
{
	return maxFileSize;
}

int FileHandler::GetSizeSplitMode() const
{
	return sizeSplitMode;
}

off_t FileHandler::GetMaxColSize() const
{
	return maxColSize;
}

off_t FileHandler::GetMinColSize() const
{
	return minColSize;
}


void FileHandler::SetAutoSplit( bool flag )
{
	autoSplit = flag;
}


void FileHandler::SetTimeStamp( bool flag )
{
	timeStamp = flag;
}


void FileHandler::SetTimeSys( bool flag )
{
	timeSys = flag;
}


void FileHandler::SetTimeCode( bool flag )
{
	timeCode = flag;
}


void FileHandler::SetBaseName( const string& s )
{
	base = s;
}


void FileHandler::SetMaxFrameCount( int count )
{
	assert( count >= 0 );
	maxFrameCount = count;
}


void FileHandler::SetEveryNthFrame( int every )
{
	assert ( every > 0 );

	everyNthFrame = every;
}


void FileHandler::SetMaxFileSize( off_t size )
{
	assert ( size >= 0 );
	maxFileSize = size;
}

void FileHandler::SetSizeSplitMode( int mode )
{
	sizeSplitMode  = mode;
}

void FileHandler::SetMaxColSize( off_t size )
{
	maxColSize  = size;
}


void FileHandler::SetMinColSize( off_t size )
{
	minColSize  = size;
}

void FileHandler::SetSampleFrame( const Frame& sample )
{
	/* empty body */
}


bool FileHandler::Done()
{
	return done;
}


bool FileHandler::WriteFrame( Frame& frame )
{
	/* If the user wants autosplit, start a new file if a
	   new recording is detected. */
	if ( FileIsOpen() && GetAutoSplit() && frame.IsNewRecording() )
	{
		// Note: This doesn't really do anything, because the code 
		//       at (dvgrab.cc:around m_writer->CollectionCounterUpdate)
		//       actually preceeds this...
		CollectionCounterUpdate();
		Close();
	}

	if ( ! FileIsOpen() )
	{

		static int counter = 0;

		if ( GetTimeStamp() )
		{
			ostringstream	sb, sb2;
			struct tm	date;
			string	recDate;

			if ( frame.GetRecordingDate( date ) )
			{
				sb << setfill( '0' )
				<< setw( 4 ) << date.tm_year + 1900 << '.'
				<< setw( 2 ) << date.tm_mon + 1 << '.'
				<< setw( 2 ) << date.tm_mday << '_'
				<< setw( 2 ) << date.tm_hour << '-'
				<< setw( 2 ) << date.tm_min << '-'
				<< setw( 2 ) << date.tm_sec;
				recDate = sb.str();
			}
			else
			{
				goto TimeSys;
			}
			sb2 << GetBaseName() << recDate << GetExtension();
			filename = sb2.str();
			//cerr << ">>> Trying " << filename << endl;
		}
		else if ( GetTimeSys() )
		{
TimeSys:
			ostringstream sb;
			time_t timesys;
			struct tm date;
			
			time( &timesys );
			localtime_r( &timesys, &date );
			
			sb << GetBaseName() 
			<< setfill( '0' )
			<< setw( 4 ) << date.tm_year + 1900 << '.'
			<< setw( 2 ) << date.tm_mon + 1 << '.'
			<< setw( 2 ) << date.tm_mday << '_'
			<< setw( 2 ) << date.tm_hour << '-'
			<< setw( 2 ) << date.tm_min << '-'
			<< setw( 2 ) << date.tm_sec
			<< GetExtension();
			filename = sb.str();
		}
		else if ( GetTimeCode() )
		{
			ostringstream sb;
			TimeCode tc;
			
			if ( frame.GetTimeCode( tc ) )
			{	
				sb << GetBaseName()
				<< setfill( '0' )
				<< setw( 2 ) << tc.hour << ':'
				<< setw( 2 ) << tc.min << ':'
				<< setw( 2 ) << tc.sec << ':'
				<< setw( 2 ) << tc.frame
				<< GetExtension();
			}
			else
			{
				sb << GetBaseName()
				<< "EE:EE:EE:EE"
				<< GetExtension();
			}
			filename = sb.str();
		}
		else
		{
			struct stat stats;
			do
			{
				ostringstream sb;
				sb << GetBaseName() << setfill( '0' ) << setw( 3 ) << ++ counter << GetExtension();
				filename = sb.str();
				//cerr << ">>> Trying " << filename << endl;
			}
			while ( stat( filename.c_str(), &stats ) == 0 );
		}

		if ( ! Create( filename ) )
		{
			cerr << ">>> Error creating file!" << endl;
			return false;
		}
		framesWritten = 0;
		framesToSkip = 0;
	}

	/* write frame */

	if ( framesToSkip == 0 )
	{
		if ( 0 > Write( frame ) )
		{
			cerr << ">>> Error writing frame!" << endl;
			return false;
		}
		framesToSkip = everyNthFrame;
		++framesWritten;
	}
	framesToSkip--;

	/* If the frame count is exceeded, close the current file.
	   If the autosplit flag is set, a new file will be created in the next iteration.
	   If the flag is not set, we are done. */

	if ( ( GetMaxFrameCount() > 0 ) &&
	        ( framesWritten >= GetMaxFrameCount() ) )
	{
		CollectionCounterUpdate();
		Close();
		done = !GetAutoSplit();
	}

	/* If the file size could be exceeded by another frame, close the current file.
	   If the autosplit flag is set, a new file will be created on the next iteration.
	   If the flag is not set, we are done. */
	/* not exact, but should be good enough to prevent going over. */
	if ( FileIsOpen() )
	{
		AudioInfo info;
		frame.GetAudioInfo( info );
		if
		((
			( GetFileSize() > 0 ) && ( GetMaxFileSize() > 0 ) &&
			(
				( GetFileSize() + frame.GetFrameSize() + info.samples * 4 + 12 )
				   >= GetMaxFileSize() 
			)
		)
		||
		(
			( GetFileSize() > 0 ) && ( GetMaxColSize() > 0 ) && ( GetSizeSplitMode() == 1 ) &&
			(
				( GetCurrentCollectionSize() + GetFileSize() + frame.GetFrameSize() + info.samples * 4 + 12 )
				  >= GetMaxColSize()
			)
		))
		{                     // 12 = sizeof chunk metadata
			CollectionCounterUpdate();
			Close();
			done = !GetAutoSplit();
		}
	}

	return true;
}


/***************************************************************************/


RawHandler::RawHandler( const string& ext ) : fd( -1 )
{
	extension = ext;
}


RawHandler::~RawHandler()
{
	Close();
}


bool RawHandler::FileIsOpen()
{
	return fd != -1;
}


bool RawHandler::Create( const string& filename )
{
	if ( GetBaseName() == "-" )
		fd = fileno( stdout );
	else
		fd = open( filename.c_str(), O_CREAT | O_TRUNC | O_RDWR | O_NONBLOCK, 0644 );
	if ( fd != -1 )
	{
		FileTracker::GetInstance().Add( filename.c_str() );
		this->filename = filename;
	}
	return ( fd != -1 );
}


int RawHandler::Write( Frame& frame )
{
	int result = write( fd, frame.data, frame.GetFrameSize() );
	return result;
}


int RawHandler::Close()
{
	if ( fd != -1 && fd != fileno( stdin ) && fd != fileno( stdout ) )
	{
		close( fd );
		fd = -1;
	}
	return 0;
}


off_t RawHandler::GetFileSize()
{
	struct stat file_status;
	fstat( fd, &file_status );
	return file_status.st_size;
}

int RawHandler::GetTotalFrames()
{
	return GetFileSize() / ( 480 * numBlocks );
}


bool RawHandler::Open( const char *s )
{
	unsigned char data[ 4 ];
	assert( fd == -1 );
	if ( strcmp( s, "-" ) == 0 )
	{
		fd = fileno( stdin );
		filename = "stdin";
	}
	else
	{
		fd = open( s, O_RDWR | O_NONBLOCK );
		if ( fd < 0 )
			return false;
		filename = s;
	}
	if ( read( fd, data, 4 ) < 0 )
		return false;
	lseek( fd, 0, SEEK_SET );
	numBlocks = ( ( data[ 3 ] & 0x80 ) == 0 ) ? 250 : 300;
	return true;

}

int RawHandler::GetFrame( Frame &frame, int frameNum )
{
	assert( fd != -1 );
	int size = 480 * numBlocks;
	if ( frameNum < 0 )
		return -1;
	off_t offset = ( ( off_t ) frameNum * ( off_t ) size );
	if( lseek( fd, offset, SEEK_SET ) == ( off_t ) - 1 )
		return -1;
	if ( read( fd, frame.data, size ) > 0 )
	{
		frame.ExtractHeader();
		return 0;
	}
	else
		return -1;
}


/***************************************************************************/


AVIHandler::AVIHandler( int format ) : avi( NULL ), aviFormat( format ), isOpenDML( false ),
		fccHandler( make_fourcc( (char*) "dvsd" ) )
{
	extension = ".avi";
}


AVIHandler::~AVIHandler()
{
	Close();
}


void AVIHandler::SetSampleFrame( const Frame& sample )
{
	Pack pack;

	sample.GetAudioInfo( audioInfo );
	sample.GetVideoInfo( videoInfo );

	sample.GetAAUXPack( 0x50, pack );
	dvinfo.dwDVAAuxSrc = *( DWORD* ) ( pack.data + 1 );
	sample.GetAAUXPack( 0x51, pack );
	dvinfo.dwDVAAuxCtl = *( DWORD* ) ( pack.data + 1 );

	sample.GetAAUXPack( 0x52, pack );
	dvinfo.dwDVAAuxSrc1 = *( DWORD* ) ( pack.data + 1 );
	sample.GetAAUXPack( 0x53, pack );
	dvinfo.dwDVAAuxCtl1 = *( DWORD* ) ( pack.data + 1 );

	sample.GetVAUXPack( 0x60, pack );
	dvinfo.dwDVVAuxSrc = *( DWORD* ) ( pack.data + 1 );
	sample.GetVAUXPack( 0x61, pack );
	dvinfo.dwDVVAuxCtl = *( DWORD* ) ( pack.data + 1 );

#ifdef WITH_LIBDV

	if ( sample.decoder->std == e_dv_std_smpte_314m )
		fccHandler = make_fourcc( (char*) "dv25" );
#endif

}


bool AVIHandler::FileIsOpen()
{
	return avi != NULL;
}

bool AVIHandler::Create( const string& filename )
{
	assert( avi == NULL );

	switch ( aviFormat )
	{

	case AVI_DV1_FORMAT:
		if( !(avi = new AVI1File) )
			return false;
		if ( avi->Create( filename.c_str() ) == false )
			return false;
		avi->Init( videoInfo.isPAL ? AVI_PAL : AVI_NTSC, audioInfo.frequency,
		           ( AVI_SMALL_INDEX | AVI_LARGE_INDEX ) );
		break;

	case AVI_DV2_FORMAT:
		if( !(avi = new AVI2File) )
			return false;
		if ( avi->Create( filename.c_str() ) == false )
			return false;
		if ( GetOpenDML() )
			avi->Init( videoInfo.isPAL ? AVI_PAL : AVI_NTSC, audioInfo.frequency,
			           ( AVI_SMALL_INDEX | AVI_LARGE_INDEX ) );
		else
			avi->Init( videoInfo.isPAL ? AVI_PAL : AVI_NTSC, audioInfo.frequency,
			           ( AVI_SMALL_INDEX ) );
		break;

	default:
		assert( aviFormat == AVI_DV1_FORMAT || aviFormat == AVI_DV2_FORMAT );
	}

	avi->setDVINFO( dvinfo );
	avi->setFccHandler( make_fourcc( (char*) "iavs" ), fccHandler );
	avi->setFccHandler( make_fourcc( (char*) "vids" ), fccHandler );
	this->filename = filename;

	FileTracker::GetInstance().Add( filename.c_str() );
	return ( avi != NULL );
}


int AVIHandler::Write( Frame& frame )
{
	assert( avi != NULL );
	return ( avi->WriteFrame( frame ) ? 0 : -1 );
}


int AVIHandler::Close()
{
	if ( avi != NULL )
	{
		avi->WriteRIFF();
		delete avi;
		avi = NULL;
	}
	return 0;
}

off_t AVIHandler::GetFileSize()
{
	if ( avi )
		return avi->GetFileSize();
	else
		return 0;
}

int AVIHandler::GetTotalFrames()
{
	return avi->GetTotalFrames();
}


bool AVIHandler::Open( const char *s )
{
	assert( avi == NULL );
	if( !(avi = new AVI1File) )
		return false;
	if ( avi->Open( s ) )
	{
		avi->ParseRIFF();
		if ( !( avi->verifyStreamFormat( make_fourcc( (char*) "dvsd" ) ) ||
		        avi->verifyStreamFormat( make_fourcc( (char*) "dv25" ) ) ) )
			return false;
		avi->ReadIndex();
		if ( avi->verifyStream( make_fourcc( (char*) "auds" ) ) )
			aviFormat = AVI_DV2_FORMAT;
		else
			aviFormat = AVI_DV1_FORMAT;
		isOpenDML = avi->isOpenDML();
		filename = s;
		return true;
	}
	else
		return false;

}

int AVIHandler::GetFrame( Frame &frame, int frameNum )
{
	int result = avi->GetDVFrame( frame, frameNum );
	frame.ExtractHeader();
	return result;
}


void AVIHandler::SetOpenDML( bool flag )
{
	isOpenDML = flag;
}


bool AVIHandler::GetOpenDML() const
{
	return isOpenDML;
}


/***************************************************************************/

#ifdef HAVE_LIBQUICKTIME

#ifndef HAVE_LIBDV_DV_H
#define DV_AUDIO_MAX_SAMPLES 1944
#endif

QtHandler::QtHandler() : fd( NULL ), audioBufferSize( 0 )
{
	extension = ".mov";
	Init();
}


QtHandler::~QtHandler()
{
	Close();
}

void QtHandler::Init()
{
	if ( fd != NULL )
		Close();

	fd = NULL;
	samplingRate = 0;
	samplesPerBuffer = 0;
	channels = 2;
	audioBuffer = NULL;
	audioChannelBuffer = NULL;
	isFullyInitialized = false;
}


bool QtHandler::FileIsOpen()
{
	return fd != NULL;
}


bool QtHandler::Create( const string& filename )
{
	Init();

	fd = quicktime_open( const_cast<char*>( filename.c_str() ), 0, 1 );

	//if (fd != NULL) FileTracker::GetInstance().Add(filename.c_str());
	return ( fd != NULL );
}


inline void QtHandler::DeinterlaceStereo16( void* pInput, int iBytes,
        void* pLOutput, void* pROutput )
{
	short int * piSampleInput = ( short int* ) pInput;
	short int* piSampleLOutput = ( short int* ) pLOutput;
	short int* piSampleROutput = ( short int* ) pROutput;

	while ( ( char* ) piSampleInput < ( ( char* ) pInput + iBytes ) )
	{
		*piSampleLOutput++ = *piSampleInput++;
		*piSampleROutput++ = *piSampleInput++;
	}
}

int QtHandler::Write( Frame& frame )
{
	if ( ! isFullyInitialized )
	{
		AudioInfo audio;

		if ( frame.GetAudioInfo( audio ) )
		{
			/* TODO: handle 12-bit, non-linear audio */
			channels = 2;
			quicktime_set_audio( fd, channels, audio.frequency, 16,
			                     QUICKTIME_TWOS );
		}
		else
		{
			channels = 0;
		}

		quicktime_set_video( fd, 1, 720, frame.IsPAL() ? 576 : 480,
		                     frame.GetFrameRate(), QUICKTIME_DV );

		if ( channels > 0 )
		{
			audioBuffer = new int16_t[ DV_AUDIO_MAX_SAMPLES * channels ];
			audioBufferSize = DV_AUDIO_MAX_SAMPLES;

			audioChannelBuffer = new short int * [ channels ];
			for ( int c = 0; c < channels; c++ )
				audioChannelBuffer[ c ] = new short int[ 3000 ];

			assert( channels <= 4 );

			for ( int c = 0; c < channels; c++ )
				audioChannelBuffers[ c ] = audioChannelBuffer[ c ];
		}
		else
		{
			audioChannelBuffer = NULL;

			for ( int c = 0; c < 4; c++ )
				audioChannelBuffers[ c ] = NULL;
		}

		isFullyInitialized = true;
	}

	int result = quicktime_write_frame( fd, const_cast<unsigned char*>( frame.data ),
	                                    frame.GetFrameSize(), 0 );

	if ( channels > 0 )
	{
		AudioInfo audio;
		frame.ExtractHeader();
		if ( frame.GetAudioInfo( audio ) && ( unsigned int ) audio.samples < audioBufferSize )
		{
			long bytesRead = frame.ExtractAudio( audioBuffer );

			DeinterlaceStereo16( audioBuffer, bytesRead,
			                     audioChannelBuffer[ 0 ],
			                     audioChannelBuffer[ 1 ] );

			quicktime_encode_audio( fd, audioChannelBuffers,
			                        NULL, bytesRead / 4 );
		}
	}
	return result;
}


int QtHandler::Close()
{
	if ( fd != NULL )
	{
		quicktime_close( fd );
		fd = NULL;
	}
	if ( audioBuffer != NULL )
	{
		delete audioBuffer;
		audioBuffer = NULL;
	}
	if ( audioChannelBuffer != NULL )
	{
		for ( int c = 0; c < channels; c++ )
			delete audioChannelBuffer[ c ];
		delete audioChannelBuffer;
		audioChannelBuffer = NULL;
	}
	return 0;
}


off_t QtHandler::GetFileSize()
{
	if ( fd )
	{
		struct stat file_status;
		fstat( fileno( fd->stream ), &file_status );
		return file_status.st_size;
	}
	else
		return 0;
}


int QtHandler::GetTotalFrames()
{
	return ( int ) quicktime_video_length( fd, 0 );
}


bool QtHandler::Open( const char *s )
{
	Init();

	fd = quicktime_open( ( char * ) s, 1, 1 );
	if ( fd == NULL )
	{
		fprintf( stderr, "Error opening: %s\n", s );
		return false;
	}

	if ( quicktime_has_video( fd ) <= 0 )
	{
		fprintf( stderr, "There must be at least one video track in the input file (%s).\n",
		         s );
		Close();
		return false;
	}
	if ( strncmp( quicktime_video_compressor( fd, 0 ), QUICKTIME_DV, 4 ) != 0 )
	{
		fprintf( stderr, "Video in input file (%s) must be in DV format.\n", s );
		Close();
		return false;
	}

	return true;
}

int QtHandler::GetFrame( Frame &frame, int frameNum )
{
	assert( fd != NULL );
	quicktime_set_video_position( fd, frameNum, 0 );
	frame.bytesInFrame = quicktime_read_frame( fd, frame.data, 0 );
	frame.ExtractHeader();
	return 0;
}
#endif

/********************************************************************************/

#if defined(HAVE_LIBJPEG) && defined(HAVE_LIBDV)

JPEGHandler::JPEGHandler( int quality, bool deinterlace, int width, int height,
                          bool overwrite ) :
		isOpen( false ), count( 0 ), deinterlace( deinterlace ), overwrite( overwrite )
{
	extension = ".jpg";
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress( &cinfo );
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	jpeg_set_defaults( &cinfo );
	jpeg_set_quality( &cinfo, quality, TRUE /* limit to baseline-JPEG values */ );
	new_height = CLAMP( height, -1, 2048 );
	new_width = CLAMP( width, -1, 2048 );
}


JPEGHandler::~JPEGHandler()
{
	Close();
	jpeg_destroy_compress( &cinfo );
}

bool JPEGHandler::Create( const string& filename )
{
	this->filename = filename;
	isOpen = true;
	count = 0;
	return true;
}

/* this must be called before scaling */
/* height is fixed, returns new width */
int JPEGHandler::fixAspect( Frame &frame )
{
	int width = frame.GetWidth( );
	int height = frame.GetHeight( );
	static JSAMPLE image[ 2048 * 2048 * 3 ];
	register JSAMPLE *dest = image_buffer, *src = image;
	int new_width = frame.IsPAL() ? 337 : 320;
	int n = width / 2 - new_width;
	int d = width / 2;
	int a = n;

	memcpy( src, dest, width * height * 3 );

	for ( register int j = 0; j < height; j += 2 )
	{
		src = image + j * ( width * 3 );
		for ( register int i = 0; i < new_width ; i++ )
		{
			if ( a > d )
			{
				a -= d;
				src += 3;
			}
			else
				a += n;

			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = *src++;
			src += 3;
		}
	}
	return new_width;
}

bool JPEGHandler::scale( Frame &frame )
{
	int width = frame.GetWidth( );
	int height = frame.GetHeight( );
	static JSAMPLE image[ 2048 * 2048 * 3 ];
	register JSAMPLE *dest = image_buffer, *src = image;
	AffineTransform affine;
	double scale_x = ( double ) new_width / ( double ) width;
	double scale_y = ( double ) new_height / ( double ) height;

	memcpy( src, dest, width * height * 3 );

	register int i, j, x, y;
	if ( scale_x <= 1.0 && scale_y <= 1.0 )
	{
		affine.Scale( scale_x, scale_y );

		for ( j = 0; j < height; j++ )
			for ( i = 0; i < width; i++ )
			{
				x = ( int ) ( affine.MapX( i - width / 2, j - height / 2 ) );
				y = ( int ) ( affine.MapY( i - width / 2, j - height / 2 ) );
				x += new_width / 2;
				x = CLAMP( x, 0, new_width );
				y += new_height / 2;
				y = CLAMP( y, 0, new_height );
				//cout << "i = " << i << " j = " << j << " x = " << x << " y = " << y << endl;
				src = image + ( j * width * 3 ) + i * 3;
				dest = image_buffer + y * ( int ) ( new_width ) * 3 + ( int ) ( x * 3 );
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
			}
	}
	else if ( scale_x >= 1.0 && scale_y >= 1.0 )
	{
		affine.Scale( 1.0 / scale_x, 1.0 / scale_y );

		for ( y = 0; y < new_height; y++ )
			for ( x = 0; x < new_width; x++ )
			{
				i = ( int ) ( affine.MapX( x - new_width / 2, y - new_height / 2 ) );
				j = ( int ) ( affine.MapY( x - new_width / 2, y - new_height / 2 ) );
				i += width / 2;
				i = CLAMP( i, 0, new_width );
				j += height / 2;
				j = CLAMP( j, 0, new_height );
				//cout << "i = " << i << " j = " << j << " x = " << x << " y = " << y << endl;
				src = image + ( j * width * 3 ) + i * 3;
				dest = image_buffer + y * ( int ) ( new_width ) * 3 + ( int ) ( x * 3 );
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
			}
	}
	else
		return false;

	return true;
}


int JPEGHandler::Write( Frame &frame )
{
	JSAMPROW row_pointer[ 1 ];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	JDIMENSION width = frame.GetWidth();
	JDIMENSION height = frame.GetHeight();
	string file;

	frame.ExtractHeader();
	if ( frame.IsNewRecording() && GetAutoSplit() )
		count = 0;
	frame.ExtractRGB( image_buffer );
	if ( deinterlace )
		frame.Deinterlace( image_buffer, 3 );
	if ( new_width != -1 || new_height != -1 )
	{
		if ( new_width == -1 )
			new_width = width;
		if ( new_height == -1 )
			new_height = height;
		if ( !scale( frame ) )
		{
			new_width = width;
			new_height = height;
		}
	}
	else
	{
		if ( new_width == -1 )
			new_width = width;
		if ( new_height == -1 )
			new_height = height;
	}

	if ( overwrite )
	{
		ostringstream sb;
		sb << GetBaseName() << GetExtension();
		file = sb.str();
	}
	else
	{
		ostringstream sb;
		sb << filename.substr( 0, filename.find_last_of( '.' ) ) << "-" << setfill( '0' ) << setw( 8 ) << ++count << GetExtension();
		file = sb.str();
	}
	FILE *outfile = fopen( const_cast<char*>( file.c_str() ), "wb" );
	if ( outfile != NULL )
		jpeg_stdio_dest( &cinfo, outfile );
	cinfo.image_width = new_width;
	cinfo.image_height = new_height;
	row_stride = cinfo.image_width * cinfo.input_components;
	jpeg_start_compress( &cinfo, TRUE );
	while ( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[ 0 ] = &image_buffer[ cinfo.next_scanline * row_stride ];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	jpeg_finish_compress( &cinfo );
	fclose( outfile );
	return 0;
}

int JPEGHandler::Close( void )
{
	isOpen = false;
	return 0;
}

#endif
