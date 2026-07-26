// VSCodeWorkspaceGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeWorkspaceGenerator.h"

#include "Core/Containers/UnorderedMap.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeWorkspaceNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeClangDConfig.h"


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
													const Array< VSCodeWorkspaceFolder > & folders,
													const AString & clangDPath,
													const Array< AString > & clangDArgs,
													const AString & compileCommandsPath,
													const Array< VSCodeClangDConfig > & clangDConfigs )
{
	m_Tmp.SetReserved( MEGABYTE );
	m_Tmp.SetLength( 0 );

	Write( "{\n" );

	Write( "\t\"folders\":\n" );
	Write( "\t[\n" );

	bool haveMultiRootCppConfigs = false;
	bool haveMultiRootSlangConfigs = false;

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

		if (!project->GetConfigs().IsEmpty())
		{
			haveMultiRootCppConfigs = true;
		}
		if (!project->GetSlangIncludePath().IsEmpty())
		{
			haveMultiRootSlangConfigs = true;
		}
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

		if (!folder.m_Configs.IsEmpty())
		{
			haveMultiRootCppConfigs = true;
		}
		if (!folder.m_SlangIncludePath.IsEmpty())
		{
			haveMultiRootSlangConfigs = true;
		}
	}

	Write( "\n\t],\n" );

	if (haveMultiRootCppConfigs || !clangDConfigs.IsEmpty() || haveMultiRootSlangConfigs)
	{
		Write( "\t\"settings\":\n" );
		Write( "\t{\n" );

		if (haveMultiRootCppConfigs)
		{
			Write( "\t\t\"C_Cpp.intellisenseEngine\n\": \"enabled\",\n" );
			Write( "\t\t\"clangd.enable\": false,\n" );
			Write( "\t\t\"C_Cpp.default.configurationProvider\": \"multi-root-cpp-config-provider\",\n" );
			Write( "\t\t\"multiRootCppConfig.folders\":\n" );
			Write( "\t\t[\n" );

			first = true;
			for ( const VSCodeProjectNode * project : projects )
			{
				if (project->GetConfigs().IsEmpty())
				{
					continue;
				}

				if ( !first )
				{
					Write( ",\n" );
				}
				first = false;

				Write( "\t\t\t{\n");
				Write( "\t\t\t\t\"name\": \"%s\",\n", project->GetName().Get() );

				GenerateCppConfigs( project->GetConfigs(), "\t\t\t\t" );

				Write( "\t\t\t}" );
			}

			for ( const VSCodeWorkspaceFolder & folder : folders )
			{
				if (folder.m_Configs.IsEmpty())
				{
					continue;
				}

				if ( !first )
				{
					Write( ",\n" );
				}
				first = false;

				Write( "\t\t\t{\n");
				Write( "\t\t\t\t\"name\": \"%s\",\n", folder.m_Name.Get() );

				GenerateCppConfigs( folder.m_Configs, "\t\t\t\t" );

				Write( "\t\t\t}" );
			}

			Write( "\n\t\t]" );
		} else if ( !clangDConfigs.IsEmpty() )
		{
			Write( "\t\t\"C_Cpp.intelliSenseEngine\": \"disabled\",\n" );
			Write( "\t\t\"clangd.enable\": true,\n" );

			if ( !clangDPath.IsEmpty() )
			{
				AString path( clangDPath );
				path.Replace( '\\', '/' );

				Write( "\t\t\"clangd.path\": \"%s\",\n", path.Get() );
			}

			if ( !compileCommandsPath.IsEmpty() )
			{
				AString path( compileCommandsPath );
				path.Replace( '\\', '/' );

				Write( "\t\t\"clangd.arguments\":\n" );
				Write( "\t\t[\n" );
				Write( "\t\t\t\"--compile-commands-dir=%s\"", path.Get() );
				for ( const AString &arg : clangDArgs )
				{
					Write( ",\n\t\t\t\"%s\"", arg.Get() );
				}
				Write( "\n\t\t],\n" );
			}

			Write( "\t\t\"multiRootCppConfig.clangd\":\n" );
			Write( "\t\t{\n" );
			Write( "\t\t\t\"configurations\":\n" );
			Write( "\t\t\t[\n" );

			first = true;
			for ( const VSCodeClangDConfig & config : clangDConfigs )
			{
				if ( !first )
				{
					Write( ",\n" );
				}
				first = false;

				Write( "\t\t\t\t{\n");
				Write( "\t\t\t\t\t\"name\": \"%s\",\n", config.m_Name.Get() );
				Write( "\t\t\t\t\t\"command\": \"%s\",\n", config.m_Command.Get() );
				if ( !config.m_CWD.IsEmpty() )
				{
					AString path( config.m_CWD );
					path.Replace( '\\', '/' );
					Write( "\t\t\t\t\t\"cwd\": \"%s\",\n", path.Get() );
				}
				Write( "\t\t\t\t}");
			}

			Write( "\n\t\t\t]\n" );
			Write( "\t\t}" );

		}

		if (haveMultiRootSlangConfigs)
		{
			UnorderedMap<AString, bool> includePaths;

			if (haveMultiRootCppConfigs || !clangDConfigs.IsEmpty() )
			{
				Write(",\n");
			}
			Write( "\t\t\"slang.additionalSearchPaths\":\n" );
			Write( "\t\t[\n" );

			first = true;

			for ( const VSCodeProjectNode * project : projects )
			{
				for ( const AString &includePath : project->GetSlangIncludePath() )
				{
					AString path( includePath );
					path.Replace( '\\', '/' );

					if ( !includePaths.Find(path) )
					{
						if ( !first )
						{
							Write( ",\n" );
						}
						first = false;

						Write( "\t\t\t\t\"%s\"", path.Get() );

						includePaths.Insert(path, true);
					}
				}
			}
			for ( const VSCodeWorkspaceFolder & folder : folders )
			{
				for ( const AString &includePath : folder.m_SlangIncludePath )
				{
					AString path( includePath );
					path.Replace( '\\', '/' );

					if ( !includePaths.Find(path) )
					{
						if ( !first )
						{
							Write( ",\n" );
						}
						first = false;

						Write( "\t\t\t\t\"%s\"", path.Get() );

						includePaths.Insert(path, true);
					}
				}
			}

			Write( "\n\t\t]" );
		}

		Write( "\n\t}\n" );
	}

	Write( "}\n" );

	return m_Tmp;
}
