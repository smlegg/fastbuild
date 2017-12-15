// VSCodeProjectGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "VSCodeProjectGenerator.h"

// FBuildCore
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/ObjectListNode.h"
#include "Tools/FBuild/FBuildCore/Helpers/ProjectGeneratorBase.h" // TODO:C Remove when VSProjectGenerator derives from ProjectGeneratorBase

// CONSTRUCTOR (VSCodeProjectConfig)
//------------------------------------------------------------------------------
VSCodeProjectConfig::VSCodeProjectConfig()
	: m_Target(nullptr)
	, m_LimitSymbolsToIncludedHeaders(true)
{
}

// DESTRUCTOR (VSCodeProjectConfig)
//------------------------------------------------------------------------------
VSCodeProjectConfig::~VSCodeProjectConfig() = default;

// VSCodeProjectConfig::Save
//------------------------------------------------------------------------------
/*static*/ void VSCodeProjectConfig::Save( IOStream & stream, const Array< VSCodeProjectConfig > & configs )
{
	uint32_t numConfigs = (uint32_t)configs.GetSize();
	stream.Write( numConfigs );
	for ( uint32_t i = 0; i < numConfigs; ++i )
	{
		const VSCodeProjectConfig & cfg = configs[i];

		stream.Write( cfg.m_Config );
		stream.Write( cfg.m_Defines );
		stream.Write( cfg.m_IncludePath );
		stream.Write( cfg.m_IntellisenseMode );
		stream.Write( cfg.m_LimitSymbolsToIncludedHeaders );
		stream.Write( cfg.m_DatabaseFilename );

		Node::SaveNodeLink( stream, cfg.m_Target );
	}
}

// VSCodeProjectConfig::Load
//------------------------------------------------------------------------------
/*static*/ bool VSCodeProjectConfig::Load( NodeGraph & nodeGraph , IOStream & stream, Array< VSCodeProjectConfig > & configs )
{
	ASSERT( configs.IsEmpty() );

	uint32_t numConfigs( 0 );
	if ( !stream.Read( numConfigs ) )
	{
		return false;
	}
	configs.SetSize( numConfigs );
	for ( uint32_t i = 0; i < numConfigs; ++i )
	{
		VSCodeProjectConfig & cfg = configs[i];

		if ( stream.Read( cfg.m_Config ) == false ) { return false; }
		if ( stream.Read( cfg.m_Defines ) == false ) { return false; }
		if ( stream.Read( cfg.m_IncludePath ) == false ) { return false; }
		if ( stream.Read( cfg.m_IntellisenseMode ) == false ) { return false;  }
		if ( stream.Read( cfg.m_LimitSymbolsToIncludedHeaders ) == false ) { return false; }
		if ( stream.Read( cfg.m_DatabaseFilename ) == false ) { return false; }

		if ( !Node::LoadNodeLink( nodeGraph, stream, cfg.m_Target ) ) { return false; }
	}
	return true;
}

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectGenerator::VSCodeProjectGenerator()
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectGenerator::~VSCodeProjectGenerator() = default;

// Generate
//------------------------------------------------------------------------------
const AString & VSCodeProjectGenerator::Generate( const Array< VSCodeProjectConfig > & configs )
{
	m_Tmp.SetReserved( MEGABYTE );
	m_Tmp.SetLength( 0 );

	Write( "{\n" );

	Write( "\t\"configurations\":\n" );
	Write( "\t[\n" );

	const VSCodeProjectConfig * const cEnd = configs.End();
	for ( const VSCodeProjectConfig * cIt = configs.Begin(); cIt != cEnd; ++cIt )
	{
		const ObjectListNode * oln = nullptr;
		if ( cIt->m_IncludePath.IsEmpty() || cIt->m_Defines.IsEmpty() )
		{
			oln = ProjectGeneratorBase::FindTargetForIntellisenseInfo( cIt->m_Target );
		}

		if ( cIt != configs.Begin() )
		{
			Write( ",\n" );
		}
		Write( "\t\t{\n" );

		Write( "\t\t\t\"name\": \"%s\",\n", cIt->m_Config.Get() );

		const Array< AString >* includePaths;
		Array< AString > extractedIncludePaths;
		if ( cIt->m_IncludePath.IsEmpty() == false )
		{
			includePaths = &cIt->m_IncludePath;
		}
		else
		{
			if ( oln )
			{
				ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), "/I", "-I", extractedIncludePaths, false );
			}
			includePaths = &extractedIncludePaths;
		}

		Write( "\t\t\t\"includePath\":\n" );
		Write( "\t\t\t[\n" );
		WritePathList( *includePaths, "\t\t\t\t" );
		Write( "\t\t\t],\n" );

		Write( "\t\t\t\"defines\":\n" );
		Write( "\t\t\t[\n" );
		if ( cIt->m_Defines.IsEmpty() == false )
		{
			WriteStringList( cIt->m_Defines, "\t\t\t\t" );
		}
		else
		{
			if ( oln )
			{
				Array< AString > defines;
				ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), "/D", "-D", defines, false );
				WriteStringList( defines, "\t\t\t\t" );
			}
		}
		Write( "\n\t\t\t],\n" );

		if ( cIt->m_IntellisenseMode.IsEmpty() == false )
		{
			Write( "\t\t\t\"intellisenseMode\": \"%s\",\n", cIt->m_IntellisenseMode.Get() );
		}

		Write( "\t\t\t\"browse\":\n" );
		Write( "\t\t\t{\n" );

		Write( "\t\t\t\t\"path\":\n" );
		Write( "\t\t\t\t[\n" );
		WritePathList( *includePaths, "\t\t\t\t\t" );
		Write( "\n\t\t\t\t],\n" );

		if ( cIt->m_DatabaseFilename.IsEmpty() == false )
		{
			Write( "\t\t\t\t\"databaseFilename\": \"%s\",\n", cIt->m_DatabaseFilename.Get() );
		}

		Write( "\t\t\t\t\"limitSymbolsToIncludedHeaders\": %s\n", cIt->m_LimitSymbolsToIncludedHeaders ? "true" : "false" );

		Write( "\t\t\t}\n" );

		Write( "\t\t}" );
	}

	Write( "\n\t],\n" );

	Write( "\t\"version\": 2\n" );
	Write( "}\n" );

	return m_Tmp;
}

// WritePathList
//------------------------------------------------------------------------------
void VSCodeProjectGenerator::WritePathList( const Array< AString > & paths, const char * prefix )
{
	bool first = true;
	for ( const AString & path : paths )
	{
		if ( !first )
		{
			Write( ",\n" );
		}
		first = false;

		AStackString<> fullPath;
		NodeGraph::CleanPath( path, fullPath );
		fullPath.Replace( '\\', '/' );

		Write( "%s\"%s\"", prefix, fullPath.Get() );
	}
	Write( "\n" );
}

// WriteStringList
//------------------------------------------------------------------------------
void VSCodeProjectGenerator::WriteStringList( const Array< AString > & strings, const char * prefix )
{
	bool first = true;
	for ( const AString & string : strings )
	{
		if ( !first )
		{
			Write( ",\n" );
		}
		first = false;

		Write( "%s\"%s\"", prefix, string.Get() );
	}
	Write( "\n" );
}
