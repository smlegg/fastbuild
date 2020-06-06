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

// VSCodeFolder
//-----------------------------------------------------------------------------
struct VSCodeWorkspaceFolder
{
	REFLECT_STRUCT_DECLARE( VSCodeWorkspaceFolder )
public:
	VSCodeWorkspaceFolder();
	~VSCodeWorkspaceFolder();

	AString m_Path;
	AString m_Name;
};

// VSCodeWorkspaceNode
//------------------------------------------------------------------------------
class VSCodeWorkspaceNode : public FileNode
{
	REFLECT_NODE_DECLARE( VSCodeWorkspaceNode )
public:
	explicit VSCodeWorkspaceNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
	virtual ~VSCodeWorkspaceNode();

	static inline Node::Type GetTypeS() { return Node::VSCODEWORKSPACE_NODE; }
private:
	virtual BuildResult DoBuild( Job * job ) override;
    virtual void PostLoad( NodeGraph & nodeGraph ) override;

	bool ResolveProjects( NodeGraph &nodeGraph,
						  const BFFToken * iter = nullptr,
						  const Function * function = nullptr);

	Array< AString > m_Projects;
	Array< VSCodeProjectNode * > m_ProjectNodes;
	Array< VSCodeWorkspaceFolder > m_Folders;
};