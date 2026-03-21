// VSCodeWorkspaceGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeWorkspaceGenerator.h"

#include "Core/Containers/UnorderedMap.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeWorkspaceNode.h"


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

	if (haveMultiRootCppConfigs || haveMultiRootSlangConfigs)
	{
		Write( "\t\"settings\":\n" );
		Write( "\t{\n" );

		if (haveMultiRootCppConfigs)
		{
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
		}

		if (haveMultiRootSlangConfigs)
		{
			UnorderedMap<AString, bool> includePaths;

			if (haveMultiRootCppConfigs)
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
