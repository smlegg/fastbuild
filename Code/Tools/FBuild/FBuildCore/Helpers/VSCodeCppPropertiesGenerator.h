// VSCodeCppPropertiesGenerator - generates VSCode C/C++ properties file
// (or can be used as a base class for a generator that contains C/C++ properties)
//---------------------------------------------------------------------------------------------
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
struct VSCodeCppConfig;

// VSCodeCppPropertiesGenerator
//-----------------------------------------------------------------------------
class VSCodeCppPropertiesGenerator : public ProjectGeneratorBase
{
public:
	VSCodeCppPropertiesGenerator();
	~VSCodeCppPropertiesGenerator();

	void GenerateCppConfigs( const Array< VSCodeCppConfig > & configs, const char *indent );
	const AString & Generate( const Array< VSCodeCppConfig > & configs );

private:
	void WritePathList( const Array< AString > & paths, const char * prefix );
	void WriteStringList( const Array< AString > & strings, const char * prefix );
	void ResolveIncludeFile( const AString & fileName, const Array< AString > & paths, AString & resolved );
	void ResolveIncludeFiles( const Array < AString > &fileNames, const Array< AString > & paths, Array< AString > & resolved );
};
