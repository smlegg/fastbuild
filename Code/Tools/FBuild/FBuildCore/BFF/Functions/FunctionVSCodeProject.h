// Function VSCodeProject
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Function.h"

// Forward Declarations
//------------------------------------------------------------------------------
class BFFIterator;

// FunctionVSCodeProject
//------------------------------------------------------------------------------
class FunctionVSCodeProject : public Function
{
public:
	explicit		FunctionVSCodeProject();
	inline virtual ~FunctionVSCodeProject() = default;

protected:
	virtual bool AcceptsHeader() const override;

	virtual bool Commit(NodeGraph & nodeGraph, const BFFIterator & funcStartIter) const override;

	bool GetStringFromStruct( const BFFVariable * s, const char * name, AString & result ) const;
};

//------------------------------------------------------------------------------
