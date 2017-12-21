// FunctionVSCodeWorkspace
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

// FBuild
#include "FunctionVSCodeWorkspace.h"
#include "Tools/FBuild/FBuildCore/Graph/AliasNode.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeWorkspaceNode.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionVSCodeWorkspace::FunctionVSCodeWorkspace()
: Function("VSCodeWorkspace")
{
}


// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeWorkspace::AcceptsHeader() const
{
	return true;
}

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

// ResolveVSCodeProject
//------------------------------------------------------------------------------
VSCodeProjectNode * FunctionVSCodeWorkspace::ResolveVSCodeProject( NodeGraph & nodeGraph, const BFFIterator & iter, const AString & projectName ) const
{
	Node * node = nodeGraph.FindNode( projectName );
	if ( node == nullptr )
	{
		Error::Error_1104_TargetNotDefined( iter, this, ".WorkspaceProjects", projectName );
		return nullptr;
	}

	VSCodeProjectNode * project = ResolveVSCodeProjectRecurse( node );
	if ( project )
	{
		return project;
	}

	// don't know how to handle this type of node
	Error::Error_1005_UnsupportedNodeType( iter, this, ".WorkspaceProjects", node->GetName(), node->GetType() );
	return nullptr;
}

// Commit
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeWorkspace::Commit(NodeGraph & nodeGraph, const BFFIterator & funcStartIter) const
{
	AStackString<> workspaceOutput;
	if (!GetString(funcStartIter, workspaceOutput, ".WorkspaceOutput", true))
	{
		return false;
	}

	Array< AString > workspaceProjects( 8, true );
	if ( !GetStrings( funcStartIter, workspaceProjects, ".WorkspaceProjects", false ) )
	{
		return false;
	}

	Array< VSCodeProjectNode* > projects( workspaceProjects.GetSize(), false );
	for ( const AString & projectName : workspaceProjects )
	{
		VSCodeProjectNode * project = ResolveVSCodeProject( nodeGraph, funcStartIter, projectName );
		if ( project == nullptr )
		{
			return false;
		}
		projects.Append( project );
	}

	VSCodeWorkspaceNode * wn = nodeGraph.CreateVSCodeWorkspaceNode( workspaceOutput, projects );
	ASSERT( wn );

	return ProcessAlias( nodeGraph, funcStartIter, wn );
}
