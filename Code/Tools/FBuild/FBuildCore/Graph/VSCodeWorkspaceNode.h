// VSCodeWorkspaceNode.h - a node that build a Visual Studio Code workspace file
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

// FBuildCore
#include "Tools/FBuild/FBuildCore/Helpers/VSCodeWorkspaceGenerator.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeCppConfig.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeClangDConfig.h"

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
	Array< VSCodeCppConfig > m_Configs;
	Array< AString > m_SlangIncludePath;
};

// VSCodeWorkspaceNode
//------------------------------------------------------------------------------
class VSCodeWorkspaceNode : public FileNode
{
	REFLECT_NODE_DECLARE( VSCodeWorkspaceNode )
public:
	explicit VSCodeWorkspaceNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
	virtual ~VSCodeWorkspaceNode() override;

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
	AString m_ClangDPath;
	Array< AString > m_ClangDArgs;
	AString m_CompileCommandsPath;
	Array< VSCodeClangDConfig > m_ClangDConfigs;
};
