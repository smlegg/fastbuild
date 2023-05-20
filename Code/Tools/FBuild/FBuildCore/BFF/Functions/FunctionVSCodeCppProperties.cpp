// FunctionVSCodeCppProperties
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
// FBuild
#include "FunctionVSCodeCppProperties.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFStackFrame.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFVariable.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeCppPropertiesNode.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionVSCodeCppProperties::FunctionVSCodeCppProperties()
: Function("VSCodeCppProperties")
{
}


// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeCppProperties::AcceptsHeader() const
{
	return true;
}


// CreateNode
//------------------------------------------------------------------------------
/*virtual*/ Node * FunctionVSCodeCppProperties::CreateNode() const
{
    return FNEW( VSCodeCppPropertiesNode );
}

//------------------------------------------------------------------------------
