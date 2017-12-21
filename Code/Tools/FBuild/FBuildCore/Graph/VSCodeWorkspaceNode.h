// VSCodeWorkspaceNode.h - a node that build a Visual Studio Code workspace file
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

// FBuildCore
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeWorkspaceGenerator.h"

// Forward Declarations
//------------------------------------------------------------------------------
class VSCodeProjectNode;

// VSCodeWorkspaceNode
//------------------------------------------------------------------------------
class VSCodeWorkspaceNode : public FileNode
{
public:
	explicit VSCodeWorkspaceNode( const AString & workspaceOutput,
								  const Array< VSCodeProjectNode * > & projects,
								  const Array< VSCodeWorkspaceFolder > & folders );
	virtual ~VSCodeWorkspaceNode();

	static inline Node::Type GetTypeS() { return Node::VSCODEWORKSPACE_NODE; }

	static Node * Load( NodeGraph & nodeGraph, IOStream & stream );
	virtual void Save( IOStream & stream ) const override;
private:
	virtual BuildResult DoBuild( Job * job ) override;

	Array< VSCodeWorkspaceFolder > m_Folders;
};