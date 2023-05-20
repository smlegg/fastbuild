// Function VSCodeCppProperties
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Function.h"

// Forward Declarations
//------------------------------------------------------------------------------
class BFFIterator;
class VSCodeCppPropertiesNode;

// FunctionVSCodeCppProperties
//------------------------------------------------------------------------------
class FunctionVSCodeCppProperties : public Function
{
public:
	explicit		FunctionVSCodeCppProperties();
	inline virtual ~FunctionVSCodeCppProperties() override = default;

protected:
	virtual bool AcceptsHeader() const override;
    virtual Node * CreateNode() const override;
};

//------------------------------------------------------------------------------
