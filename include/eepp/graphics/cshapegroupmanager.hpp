#ifndef EE_GRAPHICSCSHAPEGROUPMANAGER_HPP
#define EE_GRAPHICSCSHAPEGROUPMANAGER_HPP

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/cshape.hpp>
#include <eepp/graphics/cshapegroup.hpp>

namespace EE { namespace Graphics {

class EE_API cShapeGroupManager : public tResourceManager<cShapeGroup> {
	SINGLETON_DECLARE_HEADERS(cShapeGroupManager)

	public:
		cShapeGroupManager();

		virtual ~cShapeGroupManager();

		cShape * GetShapeByName( const std::string& Name );

		cShape * GetShapeById( const Uint32& Id );

		/** Search for a pattern name
		* @param name First part of the shape name
		* @param extension Extension of the shape name ( if have one, otherwise "" )
		* @param SearchInShapeGroup If you want only to search in a especific group ( NULL if you want to search in all groups )
		* @example Search for name "car" with extensions "png", i will try to find car00.png car01.png car02.png, and so on, it will continue if find something, otherwise it will stop ( it will always search at least for car00.png and car01.png )
		*/
		std::vector<cShape*> GetShapesByPattern( const std::string& name, const std::string& extension = "", cShapeGroup * SearchInShapeGroup = NULL );

		std::vector<cShape*> GetShapesByPatternId( const Uint32& ShapeId, const std::string& extension = "", cShapeGroup * SearchInShapeGroup = NULL );

		void PrintResources();

		void PrintWarnings( const bool& warn );

		const bool& PrintWarnings() const;
	protected:
		bool	mWarnings;
};

}}

#endif