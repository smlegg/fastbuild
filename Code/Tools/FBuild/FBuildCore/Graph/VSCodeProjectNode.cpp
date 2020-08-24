// VSCodeProjectNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeProjectNode.h"

#include "Tools/FBuild/FBuildCore/Error.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeProjectGenerator.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_STRUCT_BEGIN_BASE( VSCodeProjectConfig )
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
REFLECT_END( VSCodeProjectConfig )

REFLECT_NODE_BEGIN( VSCodeProjectNode, Node, MetaName("ProjectOutput") + MetaFile() )
	REFLECT( m_ProjectPath, "ProjectPath", MetaPath() )
	REFLECT( m_ProjectName, "ProjectName", MetaOptional() )
	REFLECT_ARRAY_OF_STRUCT( m_Configs, "ProjectConfigs", VSCodeProjectConfig, MetaNone() )
REFLECT_END( VSCodeProjectNode )

// CONSTRUCTOR (VSCodeProjectConfig)
//------------------------------------------------------------------------------
VSCodeProjectConfig::VSCodeProjectConfig()
	: m_LimitSymbolsToIncludedHeaders(true)
	, m_TargetNode(nullptr)
{
}

// DESTRUCTOR (VSCodeProjectConfig)
//------------------------------------------------------------------------------
VSCodeProjectConfig::~VSCodeProjectConfig() = default;

// VSCodeProjectConfig::ResolveTargets
//------------------------------------------------------------------------------
/*static*/ bool VSCodeProjectConfig::ResolveTargets( NodeGraph & nodeGraph,
                                                     Array< VSCodeProjectConfig > & configs,
                                                     const BFFToken * iter,
                                                     const Function * function )
{
    // Must provide iter and function, or neither
    ASSERT( ( ( iter == nullptr ) && ( function == nullptr ) ) ||
            ( iter && function ) );

    for ( auto & config : configs )
    {
        // Target is allowed to be empty (perhaps this project represents
        // something that cannot be built, like header browsing information
        // for a 3rd party library for example)
        if ( config.m_Target.IsEmpty() )
        {
            continue;
        }

        // Find the node
        Node * node = nodeGraph.FindNode( config.m_Target );
        if ( node == nullptr )
        {
            if ( iter && function )
            {
                Error::Error_1104_TargetNotDefined( iter, function, ".Target", config.m_Target );
                return false;
            }
            ASSERT( false ); // Should not be possible to fail when restoring from serialized DB
            continue;
        }

        config.m_TargetNode = node;
    }
    return true;
}

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectNode::VSCodeProjectNode()
: FileNode( AString::GetEmpty(), Node::FLAG_NONE )
{
	m_LastBuildTimeMs = 100; // higher default than a file node
	m_Type = Node::VSCODEPROJECT_NODE;
}

// Initialize
//------------------------------------------------------------------------------
/*virtual*/ bool VSCodeProjectNode::Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function )
{
    // Resolve Target names to Node pointers for later use
    if ( VSCodeProjectConfig::ResolveTargets( nodeGraph, m_Configs, iter, function ) == false )
    {
        return false; // Initialize will have emitted an error
    }

	return true;
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectNode::~VSCodeProjectNode() = default;

// DoBuild
//------------------------------------------------------------------------------
/*virtual*/ Node::BuildResult VSCodeProjectNode::DoBuild( Job * )
{
	VSCodeProjectGenerator g;

	// Generate output file
	const AString & output = g.Generate( m_Configs );
	if ( ProjectGeneratorBase::WriteIfDifferent( "VSCodeProj", output, m_Name ) == false )
	{
		return Node::NODE_RESULT_FAILED; // WriteIfDifferent will have emitted an error
	}

	return NODE_RESULT_OK;
}

// PostLoad
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeProjectNode::PostLoad( NodeGraph & nodeGraph )
{
    VSCodeProjectConfig::ResolveTargets( nodeGraph, m_Configs );
}
