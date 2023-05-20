#include "VSCodeCppConfig.h"

#include "Tools/FBuild/FBuildCore/Error.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_STRUCT_BEGIN_BASE( VSCodeCppConfig )
	REFLECT( m_Config, "Config", MetaNone() )
	REFLECT( m_Target, "Target", MetaOptional() )
	REFLECT_ARRAY( m_IncludePath, "IncludePath", MetaOptional() + MetaPath() )
	REFLECT_ARRAY( m_Defines, "Defines", MetaOptional() )
    REFLECT_ARRAY( m_ForcedInclude, "ForcedInclude", MetaOptional() )
    REFLECT( m_Compiler, "Compiler", MetaOptional() + MetaFile() + MetaAllowNonFile() )
    REFLECT_ARRAY( m_CompilerArgs, "CompilerArgs", MetaOptional() )
	REFLECT( m_IntellisenseMode, "IntellisenseMode", MetaOptional() )
	REFLECT( m_LimitSymbolsToIncludedHeaders, "LimitSymbolsToIncludedHeaders", MetaOptional() )
	REFLECT( m_DatabaseFilename, "DatabaseFilename", MetaOptional() + MetaFile() )
    REFLECT( m_ConfigurationProvider, "ConfigurationProvider", MetaOptional() )
REFLECT_END( VSCodeCppConfig )

// CONSTRUCTOR (VSCodeCppConfig)
//------------------------------------------------------------------------------
VSCodeCppConfig::VSCodeCppConfig()
	: m_LimitSymbolsToIncludedHeaders(true)
	, m_TargetNode(nullptr)
{
}

// DESTRUCTOR (VSCodeProjectConfig)
//------------------------------------------------------------------------------
VSCodeCppConfig::~VSCodeCppConfig() = default;

// VSCodeProjectConfig::ResolveTargets
//------------------------------------------------------------------------------
/*static*/ bool VSCodeCppConfig::ResolveTargets( NodeGraph & nodeGraph,
                                                 const BFFToken * iter,
                                                 const Function * function )
{
    // Must provide iter and function, or neither
    ASSERT( ( ( iter == nullptr ) && ( function == nullptr ) ) ||
            ( iter && function ) );

	// Target is allowed to be empty (perhaps this project represents
	// something that cannot be built, like header browsing information
	// for a 3rd party library for example)
	if ( m_Target.IsEmpty() )
	{
		return true;
	}

	// Find the node
	Node * node = nodeGraph.FindNode( m_Target );
	if ( node == nullptr )
	{
		if ( iter && function )
		{
			Error::Error_1104_TargetNotDefined( iter, function, ".Target", m_Target );
			return false;
		}
		ASSERT( false ); // Should not be possible to fail when restoring from serialized DB
	}

	m_TargetNode = node;

    return true;
}
