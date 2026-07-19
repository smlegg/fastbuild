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
struct VSCodeClangDConfig;

// VSCodeProjectGenerator
//-----------------------------------------------------------------------------
class VSCodeWorkspaceGenerator : public VSCodeCppPropertiesGenerator
{
public:
	VSCodeWorkspaceGenerator();
	~VSCodeWorkspaceGenerator();

	const AString & Generate( const Array< VSCodeProjectNode * > & projects,
							  const Array< VSCodeWorkspaceFolder > & folders,
							  const AString & clangDPath,
							  const AString & compileCommandsPath,
							  const Array< VSCodeClangDConfig > & clangDConfigs );
};
