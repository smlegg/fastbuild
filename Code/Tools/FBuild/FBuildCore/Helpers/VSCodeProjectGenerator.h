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
struct VSCodeProjectConfig;

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
	void ResolveIncludeFile( const AString & fileName, const Array< AString > & paths, AString & resolved );
	void ResolveIncludeFiles( const Array < AString > &fileNames, const Array< AString > & paths, Array< AString > & resolved );
};
