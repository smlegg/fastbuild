// VSCodeWorkspaceGenerator - Generate Visual Studio Code workspace file
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "ProjectGeneratorBase.h"

// Forward Declarations
//------------------------------------------------------------------------------
class VSCodeProjectNode;

// VSCodeFolder
//-----------------------------------------------------------------------------
class VSCodeWorkspaceFolder
{
public:
	VSCodeWorkspaceFolder();
	~VSCodeWorkspaceFolder();

	AString m_Path;
	AString m_Name;

	static bool Load( IOStream & stream, Array< VSCodeWorkspaceFolder > & folders );
	static void Save( IOStream & stream, const Array< VSCodeWorkspaceFolder > & folders );
};

// VSCodeProjectGenerator
//-----------------------------------------------------------------------------
class VSCodeWorkspaceGenerator : public ProjectGeneratorBase
{
public:
	VSCodeWorkspaceGenerator();
	~VSCodeWorkspaceGenerator();

	const AString & Generate( const Array< VSCodeProjectNode * > & projects,
							  const Array< VSCodeWorkspaceFolder > & folders );
};