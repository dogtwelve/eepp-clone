#include <eepp/audio/soundfiledefault.hpp>

#ifdef EE_LIBSNDFILE_ENABLED

namespace EE { namespace Audio {

SoundFileDefault::SoundFileDefault() :
	mFile(NULL)
{
}

SoundFileDefault::~SoundFileDefault() {
	if ( NULL != mFile )
		sf_close( mFile );
}

bool SoundFileDefault::IsFileSupported( const std::string& Filename, bool Read ) {
	if ( Read ) {
		// Open the sound file
		SF_INFO fileInfos;
		fileInfos.format = 0;
		SNDFILE * File = sf_open( Filename.c_str(), SFM_READ, &fileInfos );

		if ( NULL != File ) {
			sf_close( File );
			return true;
		}

		return false;
	} else {
		// Check the extension
		return GetFormatFromFilename(Filename) != -1;
	}
}

bool SoundFileDefault::IsFileSupported( const char* Data, std::size_t SizeInBytes ) {
	// Define the I/O custom functions for reading from memory
	MemoryIO tMemoryIO;
	SF_VIRTUAL_IO io = tMemoryIO.Prepare( Data, SizeInBytes );

	// Open the sound file
	SF_INFO fileInfos;
	fileInfos.format = 0;
	SNDFILE * File = sf_open_virtual( &io, SFM_READ, &fileInfos, &tMemoryIO );

	if ( NULL != File ) {
		sf_close( File );
		return true;
	}

	return false;
}

bool SoundFileDefault::OpenRead( const std::string& Filename, std::size_t& SamplesCount, unsigned int& ChannelCount, unsigned int& SampleRate ) {
	// If the file is already opened, first close it
	if ( NULL != mFile )
		sf_close( mFile );

	// Open the sound file
	SF_INFO fileInfos;
	fileInfos.format = 0;
	mFile = sf_open(Filename.c_str(), SFM_READ, &fileInfos);

	if ( NULL == mFile ) {
		eePRINTL( "Failed to read sound file %s", Filename.c_str() );
		return false;
	}

	// Set the sound parameters
    mChannelCount	= fileInfos.channels;
    mSampleRate		= fileInfos.samplerate;
    mSamplesCount		= static_cast<std::size_t>(fileInfos.frames) * mChannelCount;

	ChannelCount	= mChannelCount;
	SampleRate		= mSampleRate;
	SamplesCount		= mSamplesCount;

	return true;
}

bool SoundFileDefault::OpenRead( const char* Data, std::size_t SizeInBytes, std::size_t& SamplesCount, unsigned int& ChannelCount, unsigned int& SampleRate ) {
	// If the file is already opened, first close it
	if ( NULL != mFile )
		sf_close( mFile );

	// Prepare the memory I/O structure
	SF_VIRTUAL_IO io = mMemoryIO.Prepare( Data, SizeInBytes );

	// Open the sound file
	SF_INFO fileInfos;
	fileInfos.format = 0;
	mFile = sf_open_virtual( &io, SFM_READ, &fileInfos, &mMemoryIO );

	if ( !mFile ) {
		eePRINTL( "Failed to read sound file from memory ( %s )", sf_strerror( mFile ) );
		return false;
	}

	// Set the sound parameters
	mChannelCount	= fileInfos.channels;
	mSampleRate		= fileInfos.samplerate;
	mSamplesCount		= static_cast<std::size_t>(fileInfos.frames) * mChannelCount;

	ChannelCount	= mChannelCount;
	SampleRate		= mSampleRate;
	SamplesCount		= mSamplesCount;

    return true;
}

bool SoundFileDefault::OpenWrite( const std::string& Filename, unsigned int ChannelCount, unsigned int SampleRate ) {
	// If the file is already opened, first close it
	if ( NULL != mFile )
		sf_close( mFile );

	// Find the right format according to the file extension
	int Format = GetFormatFromFilename( Filename );
	if (Format == -1) {
		// Error : unrecognized extension
		eePRINTL( "Failed to create sound file %s : unknown format", Filename.c_str() );
		return false;
	}

	// Fill the sound infos with parameters
	SF_INFO fileInfos;
	fileInfos.channels   = ChannelCount;
	fileInfos.samplerate = SampleRate;
	fileInfos.format	 = Format | (Format == SF_FORMAT_OGG ? SF_FORMAT_VORBIS : SF_FORMAT_PCM_16);

	// Open the sound file for writing
	mFile = sf_open( Filename.c_str(), SFM_WRITE, &fileInfos );

	if ( NULL == mFile ) {
		eePRINTL( "Failed to create sound file %s", Filename.c_str() );
		return false;
	}

	return true;
}

std::size_t SoundFileDefault::Read( Int16 * Data, std::size_t SamplesCount ) {
	if ( NULL != mFile && Data && SamplesCount )
		return static_cast<std::size_t>( sf_read_short( mFile, Data, SamplesCount ) );
	else
		return 0;
}

void SoundFileDefault::Write( const Int16 * Data, std::size_t SamplesCount ) {
	if ( NULL != mFile && Data && SamplesCount )
		sf_write_short( mFile, Data, SamplesCount );
}

void SoundFileDefault::Seek( Time timeOffset ) {
	if ( NULL != mFile ) {
		sf_count_t frameOffset = static_cast<sf_count_t>( timeOffset.AsSeconds() * mSampleRate / 1000 );
		sf_seek( mFile, frameOffset, SEEK_SET );
	}
}

int SoundFileDefault::GetFormatFromFilename(const std::string& Filename) {
	// Extract the extension
	std::string ext = "wav";
	std::string::size_type Pos = Filename.find_last_of(".");
	if (Pos != std::string::npos)
		ext = Filename.substr(Pos + 1);

	// Match every supported extension with its format constant
	if (ext == "ogg"   || ext == "OGG")   return SF_FORMAT_OGG;
	if (ext == "wav"   || ext == "WAV" )  return SF_FORMAT_WAV;
	if (ext == "aif"   || ext == "AIF" )  return SF_FORMAT_AIFF;
	if (ext == "aiff"  || ext == "AIFF")  return SF_FORMAT_AIFF;
	if (ext == "au"    || ext == "AU"  )  return SF_FORMAT_AU;
	if (ext == "raw"   || ext == "RAW" )  return SF_FORMAT_RAW;
	if (ext == "paf"   || ext == "PAF" )  return SF_FORMAT_PAF;
	if (ext == "svx"   || ext == "SVX" )  return SF_FORMAT_SVX;
	if (ext == "nist"  || ext == "NIST")  return SF_FORMAT_NIST;
	if (ext == "voc"   || ext == "VOC" )  return SF_FORMAT_VOC;
	if (ext == "sf"    || ext == "SF"  )  return SF_FORMAT_IRCAM;
	if (ext == "w64"   || ext == "W64" )  return SF_FORMAT_W64;
	if (ext == "mat4"  || ext == "MAT4")  return SF_FORMAT_MAT4;
	if (ext == "mat5"  || ext == "MAT5")  return SF_FORMAT_MAT5;
	if (ext == "pvf"   || ext == "PVF" )  return SF_FORMAT_PVF;
	if (ext == "xi"    || ext == "XI" )   return SF_FORMAT_XI;
	if (ext == "htk"   || ext == "HTK" )  return SF_FORMAT_HTK;
	if (ext == "sds"   || ext == "SDS" )  return SF_FORMAT_SDS;
	if (ext == "avr"   || ext == "AVR" )  return SF_FORMAT_AVR;
	if (ext == "sd2"   || ext == "SD2" )  return SF_FORMAT_SD2;
	if (ext == "flac"  || ext == "FLAC")  return SF_FORMAT_FLAC;
	if (ext == "caf"   || ext == "CAF" )  return SF_FORMAT_CAF;
	if (ext == "wve"   || ext == "WVE" )  return SF_FORMAT_WVE;
	if (ext == "mpc2k" || ext == "MPC2K") return SF_FORMAT_MPC2K;
	if (ext == "rf64"  || ext == "RF64")  return SF_FORMAT_RF64;

	return -1;
}

SF_VIRTUAL_IO SoundFileDefault::MemoryIO::Prepare( const void * data, std::size_t sizeInBytes ) {
    // Setup the I/O functions
    SF_VIRTUAL_IO io;
    io.get_filelen = &SoundFileDefault::MemoryIO::GetLength;
    io.read        = &SoundFileDefault::MemoryIO::Read;
    io.seek        = &SoundFileDefault::MemoryIO::Seek;
    io.tell        = &SoundFileDefault::MemoryIO::Tell;
    io.write       = &SoundFileDefault::MemoryIO::Write;

    // Initialize the memory data
    mDataStart		= static_cast<const char*>(data);
    mDataPtr		= mDataStart;
    mTotalSize		= sizeInBytes;

    return io;
}

sf_count_t SoundFileDefault::MemoryIO::GetLength( void* UserData ) {
	MemoryIO * self = static_cast<MemoryIO*>(UserData);

	return self->mTotalSize;
}

sf_count_t SoundFileDefault::MemoryIO::Read( void* Ptr, sf_count_t Count, void* UserData ) {
	MemoryIO * self = static_cast<MemoryIO*>(UserData);

	sf_count_t Position = self->mDataPtr - self->mDataStart;

	if ( Position + Count >= self->mTotalSize )
		Count = self->mTotalSize - Position;

	memcpy( Ptr, self->mDataPtr, static_cast<std::size_t>(Count) );

	self->mDataPtr += Count;

	return Count;
}

sf_count_t SoundFileDefault::MemoryIO::Seek( sf_count_t Offset, int Whence, void* UserData ) {
	MemoryIO * self = static_cast<MemoryIO*>(UserData);

	sf_count_t Position = 0;

	switch (Whence) {
		case SEEK_SET :
			Position = Offset;
			break;
		case SEEK_CUR :
			Position = self->mDataPtr - self->mDataStart + Offset;
			break;
		case SEEK_END :
			Position = self->mTotalSize - Offset;
			break;
		default :
			Position = 0;
			break;
	}

	if (Position >= self->mTotalSize)
		Position = self->mTotalSize - 1;
	else if (Position < 0)
		Position = 0;

	self->mDataPtr = self->mDataStart + Position;

	return Position;
}

sf_count_t SoundFileDefault::MemoryIO::Tell( void* UserData ) {
	MemoryIO * self = static_cast<MemoryIO*>(UserData);

	return self->mDataPtr - self->mDataStart;
}

sf_count_t SoundFileDefault::MemoryIO::Write( const void*, sf_count_t, void* ) {
	return 0;
}

}}

#endif

