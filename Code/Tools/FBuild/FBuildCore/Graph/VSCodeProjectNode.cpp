// VSCodeProjectNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "VSCodeProjectNode.h"

#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeProjectGenerator.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectNode::VSCodeProjectNode( const AString & projectOutput,
									  const Array< VSCodeProjectConfig > & configs )
: FileNode( projectOutput, Node::FLAG_NONE )
, m_Configs( configs )
{
	m_LastBuildTimeMs = 100; // higher default than a file node
	m_Type = Node::VSCODEPROJECT_NODE;
}


// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeProjectNode::~VSCodeProjectNode() = default;

// DoBuild
//------------------------------------------------------------------------------
/*virtual*/ Node::BuildResult VSCodeProjectNode::DoBuild( Job * UNUSED( job ) )
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

// Load
//------------------------------------------------------------------------------
/*static*/ Node * VSCodeProjectNode::Load( NodeGraph & nodeGraph, IOStream & stream )
{
	NODE_LOAD( AStackString<>, name );

	Array< VSCodeProjectConfig > configs;
	VSCodeProjectConfig::Load( nodeGraph, stream, configs );

	VSCodeProjectNode * n = nodeGraph.CreateVSCodeProjectNode( name, configs );
	return n;
}

// Save
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeProjectNode::Save( IOStream & stream ) const
{
	NODE_SAVE( m_Name );
	VSCodeProjectConfig::Save( stream, m_Configs );
}
	