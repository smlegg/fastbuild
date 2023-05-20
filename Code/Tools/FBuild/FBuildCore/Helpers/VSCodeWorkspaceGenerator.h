// VSCodeWorkspaceGenerator - Generate Visual Studio Code workspace file
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "VSCodeCppPropertiesGenerator.h"

// Forward Declarations
//------------------------------------------------------------------------------
class VSCodeProjectNode;
struct VSCodeWorkspaceFolder;

// VSCodeProjectGenerator
//-----------------------------------------------------------------------------
class VSCodeWorkspaceGenerator : public VSCodeCppPropertiesGenerator
{
public:
	VSCodeWorkspaceGenerator();
	~VSCodeWorkspaceGenerator();

	const AString & Generate( const Array< VSCodeProjectNode * > & projects,
							  const Array< VSCodeWorkspaceFolder > & folders );
};
