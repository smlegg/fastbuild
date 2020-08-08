// VSCodeProjectGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------

// Core
#include "Core/FileIO/FileIO.h"
#include "Core/FileIO/PathUtils.h"

// FBuildCore
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/ObjectListNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Helpers/ProjectGeneratorBase.h" // TODO:C Remove when VSProjectGenerator derives from ProjectGeneratorBase

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
			oln = ProjectGeneratorBase::FindTargetForIntellisenseInfo( cIt->m_TargetNode );
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
				StackArray< AString, 3 > prefixes;
				prefixes.EmplaceBack( "-isystem" );
				prefixes.EmplaceBack( "/I" );
				prefixes.EmplaceBack( "-I" );
				ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), prefixes, extractedIncludePaths, false, false );
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
				StackArray< AString, 2 > prefixes;
				prefixes.EmplaceBack( "/D" );
				prefixes.EmplaceBack( "-D" );

				Array< AString > defines;
				ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), prefixes, defines, false, false );
				WriteStringList( defines, "\t\t\t\t" );
			}
		}
		Write( "\n\t\t\t],\n" );

		if ( cIt->m_ForcedInclude.IsEmpty() == false )
		{
			Array< AString > resolved;
			ResolveIncludeFiles( cIt->m_ForcedInclude, *includePaths, resolved );

			Write( "\t\t\t\"forcedInclude\":\n" );
			Write( "\t\t\t[\n" );
			WritePathList( resolved, "\t\t\t\t\t" );
			Write( "\n\t\t\t\t],\n" );
		}

		if ( cIt->m_IntellisenseMode.IsEmpty() == false )
		{
			Write( "\t\t\t\"intelliSenseMode\": \"%s\",\n", cIt->m_IntellisenseMode.Get() );
		}

		{
			StackArray< AString, 2 > prefixes;
			prefixes.EmplaceBack( "-std=" );
			prefixes.EmplaceBack( "/std:" );

			Array< AString > standard;
			ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), prefixes, standard, false, false );

			if ( standard.IsEmpty() == false )
			{
				Write( "\t\t\t\"cppStandard\": \"%s\",\n", standard[0].Get() );
			}
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

	Write( "\t\"version\": 3\n" );
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

		AString fullPath;
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


// ResolveIncludeFile
//------------------------------------------------------------------------------
void VSCodeProjectGenerator::ResolveIncludeFile( const AString & fileName, const Array< AString > & paths, AString & resolved )
{
	AString cleanFileName( fileName );
	NodeGraph::CleanPath( cleanFileName, false );

	resolved = cleanFileName;
	if (!PathUtils::IsFullPath( cleanFileName ))
	{
		for (const AString & path : paths)
		{
			AString fullPath(path);
			NodeGraph::CleanPath( fullPath );
			fullPath += NATIVE_SLASH;
			fullPath += cleanFileName;

			if (FileIO::FileExists( fullPath.Get() ))
			{
				resolved = fullPath;
				break;
			}
		}
	}
}

// ResolveIncludeFiles
//------------------------------------------------------------------------------
void VSCodeProjectGenerator::ResolveIncludeFiles( const Array < AString > &fileNames, const Array< AString > & paths, Array< AString > & resolved )
{
	resolved.SetCapacity( fileNames.GetSize() );
	for ( const AString & fileName : fileNames )
	{
		AString resolvedFileName;
		ResolveIncludeFile( fileName, paths, resolvedFileName );
		resolved.Append( resolvedFileName );
	}
}
