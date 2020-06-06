// FunctionVSCodeProject
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
// FBuild
#include "FunctionVSCodeProject.h"
#include "Tools/FBuild/FBuildCore/FBuild.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFStackFrame.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFVariable.h"
#include "Tools/FBuild/FBuildCore/Graph/NodeGraph.h"
#include "Tools/FBuild/FBuildCore/Graph/VSCodeProjectNode.h"

// Core
#include "Core/Strings/AStackString.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionVSCodeProject::FunctionVSCodeProject()
: Function( "VSCodeProject" )
{
}

// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionVSCodeProject::AcceptsHeader() const
{
	return true;
}

// CreateNode
//------------------------------------------------------------------------------
/*virtual*/ Node * FunctionVSCodeProject::CreateNode() const
{
    return FNEW( VSCodeProjectNode );
}

//------------------------------------------------------------------------------
