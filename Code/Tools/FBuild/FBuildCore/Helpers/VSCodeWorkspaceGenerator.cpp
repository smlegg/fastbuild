// VSCodeWorkspaceGenerator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeWorkspaceGenerator.h"

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

	bool haveMultiRootConfigs = false;

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
			haveMultiRootConfigs = true;
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
			haveMultiRootConfigs = true;
		}
	}

	Write( "\n\t],\n" );

	if (haveMultiRootConfigs)
	{
		Write( "\t\"settings\":\n" );
		Write( "\t{\n" );
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

		Write( "\n\t\t]\n" );
		Write( "\t}\n" );
	}

	Write( "}\n" );

	return m_Tmp;
}