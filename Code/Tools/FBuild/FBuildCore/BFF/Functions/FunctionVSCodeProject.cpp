// FunctionVSCodeProject
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

// FBuild
#include "FunctionVSCodeProject.h"
#include "Tools/FBuild/FBuildCore/FBuild.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFStackFrame.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFVariable.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"

// Core
#include "Core/Strings/AStackString.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionVSCodeProject::FunctionVSCodeProject()
: Function( "VSCodeProject" )
{
}

// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeProject::AcceptsHeader() const
{
	return true;
}

// Commit
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeProject::Commit( NodeGraph & nodeGraph, const BFFIterator & funcStartIter ) const
{
	AStackString<> projectOutput;
	if ( !GetString( funcStartIter, projectOutput, ".ProjectOutput", true ) )
	{
		return false;
	}

	// create configs
	Array< VSCodeProjectConfig > configs( 16, true );

	AStackString<> includePathName( ".IncludePath" );
	AStackString<> definesName( ".Defines" );
	AStackString<> limitSymbolsToIncludedHeadersName( ".LimitSymbolsToIncludedHeaders" );

	const BFFVariable * projectConfigs = BFFStackFrame::GetVar( ".ProjectConfigs" );
	if ( projectConfigs )
	{
		if ( projectConfigs->IsArrayOfStructs() == false )
		{
			Error::Error_1050_PropertyMustBeOfType( funcStartIter, this, ".ProjectConfigs", projectConfigs->GetType(), BFFVariable::VAR_ARRAY_OF_STRUCTS );
			return false;
		}

		const Array< const BFFVariable * > & structs = projectConfigs->GetArrayOfStructs();
		const BFFVariable * const * end = structs.End();
		for ( const BFFVariable ** it = structs.Begin(); it != end; ++it )
		{
			const BFFVariable * s = *it;

			VSCodeProjectConfig newConfig;

			// .Config must be provided
			if ( !GetStringFromStruct( s, ".Config", newConfig.m_Config ) )
			{
				// TODO:B custom error
				Error::Error_1101_MissingProperty( funcStartIter, this, AStackString<>( ".Config" ) );
				return false;
			}

			// .Target is optional
			AStackString<> target;
			if ( GetStringFromStruct( s, ".Target", target ) )
			{
				if ( target.IsEmpty() == false )
				{
					newConfig.m_Target = nodeGraph.FindNode( target );
					if ( !newConfig.m_Target )
					{
						Error::Error_1104_TargetNotDefined( funcStartIter, this, ".Target", target );
						return false;
					}
				}
			}

			const BFFVariable ** includePathFound = BFFVariable::GetMemberByName( includePathName, s->GetStructMembers() );
			const BFFVariable * includePath = includePathFound ? *includePathFound : nullptr;
			if ( includePath )
			{
				if ( includePath->IsArrayOfStrings() == false )
				{
					Error::Error_1050_PropertyMustBeOfType( funcStartIter, this, ".IncludePath", includePath->GetType(), BFFVariable::VAR_ARRAY_OF_STRINGS );
					return false;
				}

				newConfig.m_IncludePath = includePath->GetArrayOfStrings();
			}

			const BFFVariable ** definesFound = BFFVariable::GetMemberByName( definesName, s->GetStructMembers() );
			const BFFVariable * defines = definesFound ? *definesFound : nullptr;
			if ( defines )
			{
				if ( defines->IsArrayOfStrings() == false )
				{
					Error::Error_1050_PropertyMustBeOfType( funcStartIter, this, ".Defines", defines->GetType(), BFFVariable::VAR_ARRAY_OF_STRINGS );
					return false;
				}

				newConfig.m_Defines = defines->GetArrayOfStrings();
			}

			GetStringFromStruct( s, ".IntellisenseMode", newConfig.m_IntellisenseMode );
		
			const BFFVariable ** limitSymbolsToIncludedHeadersFound = BFFVariable::GetMemberByName( limitSymbolsToIncludedHeadersName, s->GetStructMembers() );
			const BFFVariable * limitSymbolsToIncludedHeaders = limitSymbolsToIncludedHeadersFound ? *limitSymbolsToIncludedHeadersFound : nullptr;
			if ( limitSymbolsToIncludedHeaders )
			{
				if ( limitSymbolsToIncludedHeaders->IsBool() == false )
				{
					Error::Error_1050_PropertyMustBeOfType( funcStartIter, this, ".LimitSymbolsToIncludedHeaders", limitSymbolsToIncludedHeaders->GetType(), BFFVariable::VAR_BOOL );
					return false;
				}

				newConfig.m_LimitSymbolsToIncludedHeaders = limitSymbolsToIncludedHeaders->GetBool();
			}

			GetStringFromStruct( s, ".DatabaseFilename", newConfig.m_DatabaseFilename );

			configs.Append( newConfig );
		}
	}

	// Check for existing node
	if ( nodeGraph.FindNode( projectOutput ) )
	{
		Error::Error_1100_AlreadyDefined( funcStartIter, this, projectOutput );
		return false;
	}

	VSCodeProjectNode * pn = nodeGraph.CreateVSCodeProjectNode( projectOutput, configs );
	ASSERT( pn );

	return ProcessAlias( nodeGraph, funcStartIter, pn );
}

// GetStringFromStruct
//------------------------------------------------------------------------------
bool FunctionVSCodeProject::GetStringFromStruct( const BFFVariable * s, const char * name, AString & result ) const
{
	const Array< const BFFVariable * > & members = s->GetStructMembers();
	const BFFVariable * const * end = members.End();
	for ( const BFFVariable ** it = members.Begin(); it != end; ++it )
	{
		const BFFVariable * v = *it;
		if ( v->IsString() == false )
		{
			continue; // ignore non-strings
		}
		if ( v->GetName() == name )
		{
			result = v->GetString();
			return true; // found
		}
	}
	return false; // not found - caller decides if this is an error
}
