// Function VSCodeWorkspace
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Function.h"

// Forward Declarations
//------------------------------------------------------------------------------
class BFFIterator;
class VSCodeProjectNode;

// FunctionVSCodeWorkspace
//------------------------------------------------------------------------------
class FunctionVSCodeWorkspace : public Function
{
public:
	explicit		FunctionVSCodeWorkspace();
	inline virtual ~FunctionVSCodeWorkspace() = default;

protected:
	virtual bool AcceptsHeader() const override;

	virtual bool Commit(NodeGraph & nodeGraph, const BFFIterator & funcStartIter) const override;

	VSCodeProjectNode * ResolveVSCodeProject( NodeGraph & nodeGraph, const BFFIterator & iter, const AString & projectName ) const;
	bool GetStringFromStruct( const BFFVariable * s, const char * name, AString & result ) const;
};

//------------------------------------------------------------------------------
