#include <eepp/system/cpackmanager.hpp>
#include <eepp/system/filesystem.hpp>
#include <eepp/system/clog.hpp>

namespace EE { namespace System {

SINGLETON_DECLARE_IMPLEMENTATION(cPackManager)

cPackManager::cPackManager() :
	mFallback( true )
{
}

cPackManager::~cPackManager() {
}

cPack * cPackManager::Exists( std::string& path ) {
	std::string tpath( path );

	FileSystem::FilePathRemoveProcessPath( tpath );

	std::list<cPack*>::iterator it;

	for ( it = mResources.begin(); it != mResources.end(); it++ ) {
		if ( -1 != (*it)->Exists( tpath ) ) {
			if ( path.size() != tpath.size() ) {
				path = tpath;
			}

			return (*it);
		}
	}

	return NULL;
}

cPack * cPackManager::GetPackByPath( std::string path ) {
	std::list<cPack*>::iterator it;

	for ( it = mResources.begin(); it != mResources.end(); it++ ) {
		if ( path == (*it)->GetPackPath() ) {
			return (*it);
		}
	}

	return NULL;
}

const bool& cPackManager::FallbackToPacks() const {
	return mFallback;
}

void cPackManager::FallbackToPacks( const bool& fallback ) {
	mFallback = fallback;
}

}}
