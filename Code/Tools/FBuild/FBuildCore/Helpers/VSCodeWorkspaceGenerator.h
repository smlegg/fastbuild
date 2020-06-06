// VSCodeWorkspaceGenerator - Generate Visual Studio Code workspace file
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "ProjectGeneratorBase.h"

// Forward Declarations
//------------------------------------------------------------------------------
class VSCodeProjectNode;
struct VSCodeWorkspaceFolder;

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