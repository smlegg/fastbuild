// VSCodeCppConfig.h - C/C++ configuration properties
//---------------------------------------------------
#pragma once

#include "Node.h"

// VSCodeCppConfig
//-----------------------------------------------------------------------------
struct VSCodeCppConfig
{
	REFLECT_STRUCT_DECLARE( VSCodeCppConfig )

	VSCodeCppConfig();
	~VSCodeCppConfig();

	AString m_Config;
	Array< AString > m_Defines;
	Array< AString > m_IncludePath;
	Array< AString > m_ForcedInclude;
	AString m_Compiler;
	Array< AString > m_CompilerArgs;
	AString m_CppStandard;
	AString m_IntellisenseMode;
	bool m_LimitSymbolsToIncludedHeaders;
	AString m_DatabaseFilename;
	AString m_ConfigurationProvider;
	AString m_Target;
	Node * m_TargetNode;

     bool ResolveTargets( NodeGraph & nodeGraph,
                          const BFFToken * iter = nullptr,
						  const Function * function = nullptr );
};
