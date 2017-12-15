// VSCodeProjectGenerator - Generate Visual Studio Code project settings
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "ProjectGeneratorBase.h"
#include "Core/Strings/AString.h"
#include "Core/Containers/Array.h"

// Forward Declarations
//------------------------------------------------------------------------------
class IOStream;
class Node;
class NodeGraph;

// VSCodeProjectConfig
//-----------------------------------------------------------------------------
class VSCodeProjectConfig
{
public:
	VSCodeProjectConfig();
	~VSCodeProjectConfig();

	AString m_Config;
	Array< AString > m_Defines;
	Array< AString > m_IncludePath;
	AString m_IntellisenseMode;
	bool m_LimitSymbolsToIncludedHeaders;
	AString m_DatabaseFilename;
	Node * m_Target;

	static bool Load( NodeGraph & nodeGraph, IOStream & stream, Array< VSCodeProjectConfig > & configs );
	static void Save( IOStream & stream, const Array< VSCodeProjectConfig > & configs );
};

// VSCodeProjectGenerator
//-----------------------------------------------------------------------------
class VSCodeProjectGenerator : public ProjectGeneratorBase
{
public:
	VSCodeProjectGenerator();
	~VSCodeProjectGenerator();

	const AString & Generate( const Array< VSCodeProjectConfig > & configs );

private:
	void WritePathList( const Array< AString > & paths, const char * prefix );
	void WriteStringList( const Array< AString > & strings, const char * prefix );
};
