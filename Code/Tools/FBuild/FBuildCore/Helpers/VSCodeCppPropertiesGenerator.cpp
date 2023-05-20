// VSCodeConfigGenerator
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
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeCppPropertiesGenerator.h"
#include "Tools/FBuild/FBuildCore/Helpers/ProjectGeneratorBase.h" // TODO:C Remove when VSProjectGenerator derives from ProjectGeneratorBase

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeCppPropertiesGenerator::VSCodeCppPropertiesGenerator()
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeCppPropertiesGenerator::~VSCodeCppPropertiesGenerator() = default;

// Generate
//------------------------------------------------------------------------------
void VSCodeCppPropertiesGenerator::GenerateCppConfigs( const Array< VSCodeCppConfig > & configs, const char *indent )
{
	Write( "%s\"configurations\":\n", indent );
	Write( "%s[\n", indent );

	const VSCodeCppConfig * const cEnd = configs.End();
	for ( const VSCodeCppConfig * cIt = configs.Begin(); cIt != cEnd; ++cIt )
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
		Write( "%s\t{\n", indent );

		Write( "%s\t\t\"name\": \"%s\",\n", indent, cIt->m_Config.Get() );

		if ( cIt->m_ConfigurationProvider.IsEmpty() == false )
		{
			Write( "%s\t\t\t\"configurationProvider\": \"%s\",\n", indent, cIt->m_ConfigurationProvider.Get() );
		}

		if ( cIt->m_Compiler.IsEmpty() == false )
		{
			AString compiler(cIt->m_Compiler);
			compiler.Replace( '\\', '/' );
			Write( "%s\t\t\"compilerPath\": \"%s\",\n", indent, compiler.Get() );
		}

		if ( cIt->m_CompilerArgs.IsEmpty() == false )
		{
			Write( "%s\t\t\"compilerArgs\":\n", indent );
			Write( "%s\t\t[\n", indent );

			AString compilerArgsIndent(indent);
			compilerArgsIndent += "\t\t\t";
			WriteStringList( cIt->m_CompilerArgs, compilerArgsIndent.Get() );

			Write( "%s\t\t],\n", indent );
		}

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

		Write( "%s\t\t\"includePath\":\n", indent );
		Write( "%s\t\t[\n", indent );

		AString includePathsIndent(indent);
		includePathsIndent += "\t\t\t";
		WritePathList( *includePaths, includePathsIndent.Get() );

		Write( "%s\t\t],\n", indent );

		Write( "%s\t\t\"defines\":\n", indent );
		Write( "%s\t\t[\n", indent );
		if ( cIt->m_Defines.IsEmpty() == false )
		{
			AString definesIndent(indent);
			definesIndent += "\t\t\t";
			WriteStringList( cIt->m_Defines, definesIndent.Get() );
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

				AString definesIndent(indent);
				definesIndent += "\t\t\t";
				WriteStringList( defines, definesIndent.Get() );
			}
		}
		Write( "\n%s\t\t],\n", indent );

		if ( cIt->m_ForcedInclude.IsEmpty() == false )
		{
			Array< AString > resolved;
			ResolveIncludeFiles( cIt->m_ForcedInclude, *includePaths, resolved );

			Write( "%s\t\t\"forcedInclude\":\n", indent );
			Write( "%s\t\t[\n", indent );


			AString includesIndent(indent);
			includesIndent += "\t\t\t\t";
			WritePathList( resolved, includesIndent.Get() );

			Write( "\n%s\t\t\t],\n", indent );
		}

		if ( cIt->m_IntellisenseMode.IsEmpty() == false )
		{
			Write( "%s\t\t\"intelliSenseMode\": \"%s\",\n", indent, cIt->m_IntellisenseMode.Get() );
		}

		if ( oln )
		{
			StackArray< AString, 2 > prefixes;
			prefixes.EmplaceBack( "-std=" );
			prefixes.EmplaceBack( "/std:" );

			Array< AString > standard;
			ProjectGeneratorBase::ExtractIntellisenseOptions( oln->GetCompilerOptions(), prefixes, standard, false, false );

			if ( standard.IsEmpty() == false )
			{
				Write( "%s\t\t\"cppStandard\": \"%s\",\n", indent, standard[0].Get() );
			}
		}

		Write( "%s\t\t\"browse\":\n", indent );
		Write( "%s\t\t{\n", indent );

		Write( "%s\t\t\t\"path\":\n", indent );
		Write( "%s\t\t\t[\n", indent );

		AString browseIncludesIndent(indent);
		browseIncludesIndent += "\t\t\t\t";
		WritePathList( *includePaths, browseIncludesIndent.Get() );

		Write( "\n%s\t\t\t],\n", indent );

		if ( cIt->m_DatabaseFilename.IsEmpty() == false )
		{
			Write( "%s\t\t\t\"databaseFilename\": \"%s\",\n", indent, cIt->m_DatabaseFilename.Get() );
		}

		Write( "%s\t\t\t\"limitSymbolsToIncludedHeaders\": %s,\n", indent, cIt->m_LimitSymbolsToIncludedHeaders ? "true" : "false" );

		Write( "%s\t\t}\n", indent );

		Write( "%s\t}", indent );
	}

	Write( "\n%s],\n", indent );
}

// Generate
//------------------------------------------------------------------------------
const AString & VSCodeCppPropertiesGenerator::Generate( const Array< VSCodeCppConfig > & configs )
{
	m_Tmp.SetReserved( MEGABYTE );
	m_Tmp.SetLength( 0 );

	Write( "{\n" );

	GenerateCppConfigs( configs, "\t" );
	Write( "\t\"version\": 3\n" );

	Write( "}\n" );

	return m_Tmp;
}

// WritePathList
//------------------------------------------------------------------------------
void VSCodeCppPropertiesGenerator::WritePathList( const Array< AString > & paths, const char * prefix )
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
void VSCodeCppPropertiesGenerator::WriteStringList( const Array< AString > & strings, const char * prefix )
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
void VSCodeCppPropertiesGenerator::ResolveIncludeFile( const AString & fileName, const Array< AString > & paths, AString & resolved )
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
void VSCodeCppPropertiesGenerator::ResolveIncludeFiles( const Array < AString > &fileNames, const Array< AString > & paths, Array< AString > & resolved )
{
	resolved.SetCapacity( fileNames.GetSize() );
	for ( const AString & fileName : fileNames )
	{
		AString resolvedFileName;
		ResolveIncludeFile( fileName, paths, resolvedFileName );
		resolved.Append( resolvedFileName );
	}
}
