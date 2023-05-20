// VSCodeCppPropertiesNode.h - a node that builds Visual Studio Code C/C++ properties
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

#include "Tools/FBuild/FBuildCore/Graph/VSCodeCppConfig.h"

// VSCodeCppPropertiesNode
//------------------------------------------------------------------------------
class VSCodeCppPropertiesNode : public FileNode
{
	REFLECT_NODE_DECLARE( VSCodeCppPropertiesNode )

public:
	explicit VSCodeCppPropertiesNode();
	virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
	virtual ~VSCodeCppPropertiesNode();

	static inline Node::Type GetTypeS() { return Node::VSCODE_CPP_PROPERTIES_NODE; }

private:
	virtual BuildResult DoBuild( Job * job ) override;
    virtual void PostLoad( NodeGraph & nodeGraph ) override;

	Array< VSCodeCppConfig > m_Configs;
};
