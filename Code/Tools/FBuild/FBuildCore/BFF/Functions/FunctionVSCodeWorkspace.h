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
    virtual Node * CreateNode() const override;
};

//------------------------------------------------------------------------------
