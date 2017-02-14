#ifndef EE_SYSTEM_FILESYSTEM_HPP
#define EE_SYSTEM_FILESYSTEM_HPP

#include <eepp/system/base.hpp>
#include <eepp/system/safedatapointer.hpp>

namespace EE { namespace System {

class EE_API FileSystem {
	public:
		/** @return The default slash path code of the current OS */
		static std::string getOSlash();

		/** @return True if the file exists ( false if is a directory, to know if directory exists use IsDirectory ) */
		static bool fileExists( const std::string& Filepath );

		/** Copy a file to memory
		* @param path The file path
		* @param data The vector to allocate the file in memory
		* @return True if returned the file to the vector.
		*/
		static bool fileGet( const std::string& path, std::vector<Uint8>& data );

		/** Copy a file to memory
		* @param path The file path
		* @param data A SafeDataPointer to allocate the data to memory
		* @return True if returned the file to the SafeDataPointer
		*/
		static bool fileGet( const std::string& path, SafeDataPointer& data );

		/** Copy a file to location.
		* @param src Source File Path
		* @param dst Destination File Path
		* @return If success.
		*/
		static bool fileCopy( const std::string& src, const std::string& dst );

		/** @return The file extension
		* @param filepath The file path or name
		* @param lowerExt Lowercase the extension obtained? ( true by default )
		*/
		static std::string fileExtension( const std::string& filepath, const bool& lowerExt = true );

		/** @return The file name of a file path */
		static std::string fileNameFromPath( const std::string& filepath );

		/** @return Removes the file name from a path, and return the path. */
		static std::string fileRemoveFileName( const std::string& filepath );

		/** @return Removes the extension of a filepath */
		static std::string fileRemoveExtension( const std::string& filepath );

		/** Removes the process path to a file path */
		static void filePathRemoveProcessPath( std::string& path );

		/** Write a file in binary mode and close it. */
		static bool fileWrite( const std::string& filepath, const Uint8* data, const Uint32& dataSize );

		/** Write a file in binary mode and close it. */
		static bool fileWrite( const std::string& filepath, const std::vector<Uint8>& data );

		/** Deletes a file from the file system. */
		static bool fileRemove( const std::string& filepath );

		/** @return The modification date of the file */
		static Uint32 fileGetModificationDate( const std::string& Filepath );

		/** If the directory path not end with a slash, it will add it. */
		static void dirPathAddSlashAtEnd( std::string& path );

		/** Move up from directory tree */
		static std::string removeLastFolderFromPath( std::string path );

		/** @return The files and sub directories contained by a directory */
		static std::vector<std::string> filesGetInPath( const std::string& path, const bool& sortByName = false, const bool& foldersFirst = false );

		/** @return The files and sub directories contained by a directory */
		static std::vector<String> filesGetInPath( const String& path, const bool& sortByName = false, const bool& foldersFirst = false );

		/** @return The size of a file */
		static Uint64 fileSize( const std::string& Filepath );
		
		/** @return If directory exists, and is a directory */
		static bool isDirectory( const std::string& path );

		/** @return If directory exists, and is a directory */
		static bool isDirectory( const String& path );

		/** Creates a new directory */
		static bool makeDir( const std::string& path, const Uint16& mode = 0770 );

		/** Convert a size represented in bytes, to a string converted in byes/kb/mb/tb.
		* For example 10485760 -> "10.0 MB"
		*/
		static std::string sizeToString(const Int64& Size );

};

}}

#endif
