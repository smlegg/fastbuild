#include "VSCodeClangDConfig.h"

// Reflection
//------------------------------------------------------------------------------
REFLECT_STRUCT_BEGIN_BASE( VSCodeClangDConfig )
	REFLECT( m_Name, "Name", MetaNone() )
	REFLECT( m_Command, "Command", MetaNone() )
	REFLECT( m_CWD, "CWD", MetaOptional() + MetaPath() )
REFLECT_END( VSCodeClangDConfig )

// CONSTRUCTOR (VSCodeClangDConfig)
//------------------------------------------------------------------------------
VSCodeClangDConfig::VSCodeClangDConfig()
{
}

// DESTRUCTOR (VSCodeClangDConfig)
//------------------------------------------------------------------------------
VSCodeClangDConfig::~VSCodeClangDConfig() = default;

