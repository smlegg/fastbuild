// FunctionVSCodeWorkspace
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
// FBuild
#include "FunctionVSCodeWorkspace.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFStackFrame.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFVariable.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeWorkspaceNode.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionVSCodeWorkspace::FunctionVSCodeWorkspace()
: Function("VSCodeWorkspace")
{
}


// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeWorkspace::AcceptsHeader() const
{
	return true;
}


// CreateNode
//------------------------------------------------------------------------------
/*virtual*/ Node * FunctionVSCodeWorkspace::CreateNode() const
{
    return FNEW( VSCodeWorkspaceNode );
}

//------------------------------------------------------------------------------
