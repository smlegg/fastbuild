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
								const Array< VSCodeProjectConfig > & configs );
	virtual ~VSCodeProjectNode();

	const Array< VSCodeProjectConfig > & GetConfigs() const { return m_Configs; }

	static Node * Load( NodeGraph & nodeGraph, IOStream & stream );
	virtual void Save( IOStream & stream ) const override;
private:
	virtual BuildResult DoBuild( Job * job ) override;

	Array< VSCodeProjectConfig > m_Configs;
};
