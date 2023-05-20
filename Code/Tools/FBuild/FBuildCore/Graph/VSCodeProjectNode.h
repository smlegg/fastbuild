// VSCodeProjectNode.h - a node that represents Visual Studio Code project settings
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Node.h"

#include "Tools/FBuild/FBuildCore/Graph/VSCodeCppConfig.h"

#include "Core/Strings/AString.h"

// VSCodeProjectNode
//------------------------------------------------------------------------------
class VSCodeProjectNode : public Node
{
	REFLECT_NODE_DECLARE( VSCodeProjectNode )
public:
	explicit VSCodeProjectNode();
	virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
	virtual ~VSCodeProjectNode();

	virtual bool IsAFile() const { return false; }

	static inline Node::Type GetTypeS() { return Node::VSCODEPROJECT_NODE; }

	const Array< VSCodeCppConfig > & GetConfigs() const { return m_Configs; }
	const AString & GetPath() const { return m_ProjectPath;  }
	const AString & GetName() const { return m_ProjectName; }
private:
	virtual BuildResult DoBuild( Job * job ) override;
    virtual void PostLoad( NodeGraph & nodeGraph ) override;

	Array< VSCodeCppConfig > m_Configs;
	AString m_ProjectPath;
	AString m_ProjectName;
    Array< AString >    m_PreBuildDependencyNames;
};
