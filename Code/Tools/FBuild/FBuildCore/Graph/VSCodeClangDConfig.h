// VSCodeClangDConfig.h - ClangD configuration properties
//---------------------------------------------------
#pragma once

#include "Node.h"

// VSCodeClangDConfig
//-----------------------------------------------------------------------------
struct VSCodeClangDConfig
{
	REFLECT_STRUCT_DECLARE( VSCodeClangDConfig )

	VSCodeClangDConfig();
	~VSCodeClangDConfig();

	AString m_Name;
	AString m_Command;
	AString m_CWD;
};
