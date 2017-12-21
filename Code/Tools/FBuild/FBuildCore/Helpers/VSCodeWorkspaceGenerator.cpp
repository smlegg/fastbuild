// VSCodeWorkspaceGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "VSCodeWorkspaceGenerator.h"

#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"

// CONSTRUCTOR (VSCodeWorkspaceFolder)
//------------------------------------------------------------------------------
VSCodeWorkspaceFolder::VSCodeWorkspaceFolder()
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceFolder::~VSCodeWorkspaceFolder() = default;

// VSCodeWorkspaceFolder::Save
/*static*/ void VSCodeWorkspaceFolder::Save( IOStream & stream, const Array< VSCodeWorkspaceFolder > & folders )
{
	uint32_t numFolders = (uint32_t)folders.GetSize();
	stream.Write( numFolders );
	for ( uint32_t i = 0; i < numFolders; ++i )
	{
		const VSCodeWorkspaceFolder & folder = folders[i];

		stream.Write( folder.m_Path );
		stream.Write( folder.m_Name );
	}
}

// VSCodeWorkspaceFolder::Load
//------------------------------------------------------------------------------
/*static*/ bool VSCodeWorkspaceFolder::Load( IOStream & stream, Array< VSCodeWorkspaceFolder > & folders )
{
	ASSERT( folders.IsEmpty() );

	uint32_t numFolders( 0 );
	if ( !stream.Read( numFolders ) )
	{
		return false;
	}
	folders.SetSize( numFolders );
	for ( uint32_t i = 0; i < numFolders; ++i )
	{
		VSCodeWorkspaceFolder & folder = folders[i];

		if ( stream.Read( folder.m_Path ) == false ) { return false; }
		if ( stream.Read( folder.m_Name ) == false ) { return false; }
	}
	return true;
}

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
const AString & VSCodeWorkspaceGenerator::Generate( const Array< VSCodeProjectNode * > & projects,
													const Array< VSCodeWorkspaceFolder > & folders )
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

	for ( const VSCodeWorkspaceFolder & folder : folders )
	{
		if ( !first )
		{
			Write( ",\n" );
		}
		first = false;

		Write( "\t\t{\n" );

		if ( !folder.m_Name.IsEmpty() )
		{
			Write( "\t\t\t\"name\": \"%s\",\n", folder.m_Name.Get() );
		}

		AString path( folder.m_Path );
		path.Replace( '\\', '/' );

		Write( "\t\t\t\"path\": \"%s\"\n", path.Get() );

		Write( "\t\t}" );
	}

	Write( "\n\t]\n" );
	Write( "}\n" );

	return m_Tmp;
}