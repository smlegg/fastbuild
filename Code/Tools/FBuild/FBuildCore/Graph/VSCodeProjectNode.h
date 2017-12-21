// VSCodeProjectNode.h - a node that builds Visual Studio Code project settings
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"

#include "Tools/FBuild/FBuildCore/Helpers/VSCodeProjectGenerator.h"

#include "Core/Strings/AString.h"

// VSCodeProjectNode
//------------------------------------------------------------------------------
class VSCodeProjectNode : public FileNode
{
public:
	explicit VSCodeProjectNode( const AString & projectOutput,
								const AString & projectPath,
								const AString & projectName,
								const Array< VSCodeProjectConfig > & configs );
	virtual ~VSCodeProjectNode();

	static inline Node::Type GetTypeS() { return Node::VSCODEPROJECT_NODE; }

	const Array< VSCodeProjectConfig > & GetConfigs() const { return m_Configs; }
	const AString & GetPath() const { return m_ProjectPath;  }
	const AString & GetName() const { return m_ProjectName; }

	static Node * Load( NodeGraph & nodeGraph, IOStream & stream );
	virtual void Save( IOStream & stream ) const override;
private:
	virtual BuildResult DoBuild( Job * job ) override;

	AString m_ProjectPath;
	AString m_ProjectName;
	Array< VSCodeProjectConfig > m_Configs;
};
