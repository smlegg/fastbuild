// VSCodeProjectNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeProjectNode.h"

#include "Tools/FBuild/FBuildCore/Error.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_NODE_BEGIN( VSCodeProjectNode, Node, MetaNone() )
	REFLECT( m_ProjectPath, "ProjectPath", MetaPath() )
	REFLECT( m_ProjectName, "ProjectName", MetaOptional() )
	REFLECT_ARRAY_OF_STRUCT( m_Configs, "CppConfigs", VSCodeCppConfig, MetaOptional() )
    REFLECT_ARRAY( m_SlangIncludePath, "SlangIncludePath", MetaOptional() + MetaPath() )
    REFLECT_ARRAY( m_PreBuildDependencyNames,  "PreBuildDependencies", MetaOptional() + MetaFile() + MetaAllowNonFile() )
REFLECT_END( VSCodeProjectNode )

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectNode::VSCodeProjectNode()
: Node( AString::GetEmpty(), Node::VSCODEPROJECT_NODE, Node::FLAG_NONE )
{
}

// Initialize
//------------------------------------------------------------------------------
/*virtual*/ bool VSCodeProjectNode::Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function )
{
    // .PreBuildDependencies
    if ( !InitializePreBuildDependencies( nodeGraph, iter, function, m_PreBuildDependencyNames ) )
    {
        return false; // InitializePreBuildDependencies will have emitted an error
    }

    // Resolve Target names to Node pointers for later use
    for (VSCodeCppConfig &config : m_Configs)
    {
        if ( config.ResolveTargets( nodeGraph, iter, function ) == false )
        {
            return false; // Initialize will have emitted an error
        }
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
    return NODE_RESULT_OK;
}

// PostLoad
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeProjectNode::PostLoad( NodeGraph & nodeGraph )
{
    for (VSCodeCppConfig &config : m_Configs)
    {
        config.ResolveTargets( nodeGraph );
    }
}
