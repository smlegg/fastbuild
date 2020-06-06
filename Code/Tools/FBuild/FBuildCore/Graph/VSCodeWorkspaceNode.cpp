// VSCodeWorkspaceNode.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "VSCodeWorkspaceNode.h"

#include "Tools/FBuild/FBuildCore/Error.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeWorkspaceGenerator.h"
#include "Tools/FBuild/FBuildCore/Graph/AliasNode.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_STRUCT_BEGIN_BASE( VSCodeWorkspaceFolder )
	REFLECT( m_Path, "Path", MetaPath() )
	REFLECT( m_Name, "Name", MetaOptional() )
REFLECT_END( VSCodeWorkspaceFolder )

REFLECT_NODE_BEGIN( VSCodeWorkspaceNode, Node, MetaName("WorkspaceOutput") + MetaFile() )
	REFLECT_ARRAY( m_Projects, "WorkspaceProjects", MetaNone() )
	REFLECT_ARRAY_OF_STRUCT( m_Folders, "WorkspaceFolders", VSCodeWorkspaceFolder, MetaNone() )
REFLECT_END( VSCodeWorkspaceNode )


// ResolveVSCodeProjectRecurse
//------------------------------------------------------------------------------
static VSCodeProjectNode * ResolveVSCodeProjectRecurse( Node * node )
{
	if ( node == nullptr )
	{
		return nullptr;
	}

	// search inside targets if this is an alias
	if ( node->GetType() == Node::ALIAS_NODE )
	{
		AliasNode * const alias = node->CastTo< AliasNode >();
		const Dependencies & targets = alias->GetAliasedNodes();

		const Dependency * const end = targets.End();
		for ( const Dependency * it = targets.Begin(); it != end; ++it )
		{
			VSCodeProjectNode *result = ResolveVSCodeProjectRecurse( it->GetNode() );
			if ( result != nullptr )
			{
				return result;
			}
		}
	}

	// check that this a vcxproject
	if ( node->GetType() != Node::VSCODEPROJECT_NODE )
	{
		// don't know how to handle this type of node
		return nullptr;
	}

	return node->CastTo< VSCodeProjectNode >();
}

// CONSTRUCTOR (VSCodeWorkspaceFolder)
//------------------------------------------------------------------------------
VSCodeWorkspaceFolder::VSCodeWorkspaceFolder()
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceFolder::~VSCodeWorkspaceFolder() = default;

// CONSTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceNode::VSCodeWorkspaceNode()
: FileNode( AString::GetEmpty(), Node::FLAG_NONE )
{
	m_LastBuildTimeMs = 100; // higher default than a file node
	m_Type = Node::VSCODEWORKSPACE_NODE;
}

// DESTRUCTOR
//------------------------------------------------------------------------------
VSCodeWorkspaceNode::~VSCodeWorkspaceNode() = default;

// Initialize
//------------------------------------------------------------------------------
/*virtual*/ bool VSCodeWorkspaceNode::Initialize( NodeGraph& nodeGraph, const BFFToken * iter, const Function * function )
{
	ResolveProjects( nodeGraph, iter, function );

	// depend on the input nodes
	for ( VSCodeProjectNode * project : m_ProjectNodes )
	{
		m_StaticDependencies.Append( Dependency( project ) );
	}

	return true;
}



// DoBuild
//------------------------------------------------------------------------------
/*virtual*/ Node::BuildResult VSCodeWorkspaceNode::DoBuild( Job * )
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

// PostLoad
//------------------------------------------------------------------------------
/*virtual*/ void VSCodeWorkspaceNode::PostLoad( NodeGraph & nodeGraph )
{
    ResolveProjects( nodeGraph );
}

// VSCodeWorkspaceNode::ResolveTargets
//------------------------------------------------------------------------------
/*static*/ bool VSCodeWorkspaceNode::ResolveProjects( NodeGraph & nodeGraph,
                                                      const BFFToken * iter,
                                                      const Function * function )
{
    // Must provide iter and function, or neither
    ASSERT( ( ( iter == nullptr ) && ( function == nullptr ) ) ||
            ( iter && function ) );

	m_ProjectNodes.Clear();
    for ( auto & projectName : m_Projects )
    {
		Node * node = nodeGraph.FindNode( projectName );
		if ( node == nullptr )
		{
			if (iter)
			{
				Error::Error_1104_TargetNotDefined( iter, function, ".WorkspaceProjects", projectName );
			}
		}

		VSCodeProjectNode * project = ResolveVSCodeProjectRecurse( node );
		if ( project )
		{
			m_ProjectNodes.Append( project );
    	} else
		{
			Error::Error_1005_UnsupportedNodeType( iter, function, ".WorkspaceProjects", node->GetName(), node->GetType() );
		}
}
    return true;
}
