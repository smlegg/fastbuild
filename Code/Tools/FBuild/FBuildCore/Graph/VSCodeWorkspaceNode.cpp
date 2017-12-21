// VSCodeWorkspaceNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "VSCodeWorkspaceNode.h"

#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeWorkspaceGenerator.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceNode::VSCodeWorkspaceNode( const AString & workspaceOutput,
										  const Array< VSCodeProjectNode * > & projects,
										  const Array< VSCodeWorkspaceFolder> & folders )
: FileNode( workspaceOutput, Node::FLAG_NONE )
, m_Folders( folders )
{
	m_LastBuildTimeMs = 100; // higher default than a file node
	m_Type = Node::VSCODEWORKSPACE_NODE;

	// depend on the input nodes
	for ( VSCodeProjectNode * project : projects )
	{
		m_StaticDependencies.Append( Dependency( project ) );
	}
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceNode::~VSCodeWorkspaceNode() = default;

// DoBuild
//------------------------------------------------------------------------------
/*virtual*/ Node::BuildResult VSCodeWorkspaceNode::DoBuild( Job * UNUSED( job ) )
{
	VSCodeWorkspaceGenerator g;

	Array< VSCodeProjectNode * > projects( m_StaticDependencies.GetSize(), false );
	const Dependency * const end = m_StaticDependencies.End();
	for ( const Dependency * it = m_StaticDependencies.Begin(); it != end; ++it )
	{
		projects.Append( it->GetNode()->CastTo< VSCodeProjectNode >() );
	}

	// Generate output file
	const AString & output = g.Generate( projects, m_Folders );
	if ( ProjectGeneratorBase::WriteIfDifferent( "VSCodeWorkspace", output, m_Name ) == false )
	{
		return Node::NODE_RESULT_FAILED; // WriteIfDifferent will have emitted an error
	}

	return NODE_RESULT_OK;
}

// Load
//------------------------------------------------------------------------------
/*static*/ Node * VSCodeWorkspaceNode::Load( NodeGraph & nodeGraph, IOStream & stream )
{
	NODE_LOAD( AStackString<>, name );
	NODE_LOAD_DEPS( 1, staticDeps );

	Array< VSCodeProjectNode * > projects( staticDeps.GetSize(), false );
	const Dependency * const end = staticDeps.End();
	for ( const Dependency * it = staticDeps.Begin(); it != end; ++it )
	{
		projects.Append( it->GetNode()->CastTo< VSCodeProjectNode >() );
	}

	Array< VSCodeWorkspaceFolder > folders;
	VSCodeWorkspaceFolder::Load( stream, folders );

	VSCodeWorkspaceNode * n = nodeGraph.CreateVSCodeWorkspaceNode( name,
																   projects,
																   folders );
	return n;
}

// Save
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeWorkspaceNode::Save( IOStream & stream ) const
{
	NODE_SAVE( m_Name );
	NODE_SAVE_DEPS( m_StaticDependencies );
	VSCodeWorkspaceFolder::Save( stream, m_Folders );
}