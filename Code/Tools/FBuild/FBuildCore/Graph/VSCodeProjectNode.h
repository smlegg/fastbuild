// VSCodeProjectNode.h - a node that builds Visual Studio Code project settings
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

#include "Tools/FBuild/FBuildCore/Helpers/VSCodeProjectGenerator.h"

#include "Core/Strings/AString.h"

// VSCodeProjectConfig
//-----------------------------------------------------------------------------
struct VSCodeProjectConfig
{
	REFLECT_STRUCT_DECLARE( VSCodeProjectConfig )

	VSCodeProjectConfig();
	~VSCodeProjectConfig();

	AString m_Config;
	Array< AString > m_Defines;
	Array< AString > m_IncludePath;
	Array< AString > m_ForcedInclude;
	AString m_Compiler;
	Array< AString > m_CompilerArgs;
	AString m_IntellisenseMode;
	bool m_LimitSymbolsToIncludedHeaders;
	AString m_DatabaseFilename;
	AString m_Target;
	Node * m_TargetNode;

    static bool ResolveTargets( NodeGraph & nodeGraph,
                                Array< VSCodeProjectConfig > & configs,
                                const BFFToken * iter = nullptr,
								const Function * function = nullptr );
};


// VSCodeProjectNode
//------------------------------------------------------------------------------
class VSCodeProjectNode : public FileNode
{
	REFLECT_NODE_DECLARE( VSCodeProjectNode )
public:
	explicit VSCodeProjectNode();
	virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
	virtual ~VSCodeProjectNode();

	static inline Node::Type GetTypeS() { return Node::VSCODEPROJECT_NODE; }

	const Array< VSCodeProjectConfig > & GetConfigs() const { return m_Configs; }
	const AString & GetPath() const { return m_ProjectPath;  }
	const AString & GetName() const { return m_ProjectName; }
private:
	virtual BuildResult DoBuild( Job * job ) override;
    virtual void PostLoad( NodeGraph & nodeGraph ) override;

	Array< VSCodeProjectConfig > m_Configs;
	AString m_ProjectPath;
	AString m_ProjectName;
};
