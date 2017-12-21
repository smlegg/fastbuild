// VSCodeWorkspaceNode.h - a node that build a Visual Studio Code workspace file
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

// Forward Declarations
//------------------------------------------------------------------------------
class VSCodeProjectNode;

// VSCodeWorkspaceNode
//------------------------------------------------------------------------------
class VSCodeWorkspaceNode : public FileNode
{
public:
	explicit VSCodeWorkspaceNode( const AString & workspaceOutput,
								  const Array< VSCodeProjectNode * > & projects );
	virtual ~VSCodeWorkspaceNode();


	static inline Node::Type GetTypeS() { return Node::VSCODEWORKSPACE_NODE; }

	static Node * Load( NodeGraph & nodeGraph, IOStream & stream );
	virtual void Save( IOStream & stream ) const override;
private:
	virtual BuildResult DoBuild( Job * job ) override;
};