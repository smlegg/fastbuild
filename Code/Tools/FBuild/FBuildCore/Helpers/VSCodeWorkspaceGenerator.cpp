// VSCodeWorkspaceGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "VSCodeWorkspaceGenerator.h"

#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceGenerator::VSCodeWorkspaceGenerator()
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceGenerator::~VSCodeWorkspaceGenerator() = default;

// Generate
//------------------------------------------------------------------------------
const AString & VSCodeWorkspaceGenerator::Generate( const Array< VSCodeProjectNode * > & projects )
{
	m_Tmp.SetReserved( MEGABYTE );
	m_Tmp.SetLength( 0 );

	Write( "{\n" );
	
	Write( "\t\"folders\":\n" );
	Write( "\t[\n" );

	bool first = true;
	for ( const VSCodeProjectNode * project : projects )
	{
		if ( !first )
		{
			Write( ",\n" );
		}
		first = false;

		Write( "\t\t{\n" );

		const AString & name = project->GetName();
		if ( !name.IsEmpty() )
		{
			Write( "\t\t\t\"name\": \"%s\",\n", project->GetName().Get() );
		}

		AString path( project->GetPath() );
		path.Replace( '\\', '/' );

		Write( "\t\t\t\"path\": \"%s\"\n", path.Get() );

		Write( "\t\t}" );
	}

	Write( "\n\t]\n" );
	Write( "}\n" );

	return m_Tmp;
}