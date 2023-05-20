// VSCodeCppPropertiesNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeCppPropertiesNode.h"

#include "Tools/FBuild/FBuildCore/Error.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_NODE_BEGIN( VSCodeCppPropertiesNode, Node, MetaName("Output") + MetaFile() )
	REFLECT_ARRAY_OF_STRUCT( m_Configs, "CppConfigs", VSCodeCppConfig, MetaNone() )
REFLECT_END( VSCodeCppPropertiesNode )

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeCppPropertiesNode::VSCodeCppPropertiesNode()
: FileNode( AString::GetEmpty(), Node::FLAG_NONE )
{
	m_LastBuildTimeMs = 100; // higher default than a file node
	m_Type = Node::VSCODE_CPP_PROPERTIES_NODE;
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeCppPropertiesNode::~VSCodeCppPropertiesNode() = default;

// Initialize
//------------------------------------------------------------------------------
/*virtual*/ bool VSCodeCppPropertiesNode::Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function )
{
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


// DoBuild
//------------------------------------------------------------------------------
/*virtual*/ Node::BuildResult VSCodeCppPropertiesNode::DoBuild( Job * )
{
	VSCodeCppPropertiesGenerator g;

	// Generate output file
	const AString & output = g.Generate( m_Configs );
	if ( ProjectGeneratorBase::WriteIfDifferent( "VSCodeCppProps", output, m_Name ) == false )
	{
		return Node::NODE_RESULT_FAILED; // WriteIfDifferent will have emitted an error
	}

	return NODE_RESULT_OK;
}

// PostLoad
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeCppPropertiesNode::PostLoad( NodeGraph & nodeGraph )
{
    for (VSCodeCppConfig &config : m_Configs)
    {
        config.ResolveTargets( nodeGraph );
    }
}
