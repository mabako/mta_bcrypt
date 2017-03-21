#include "module.h"
#include "luaimports.h"

ILuaModuleManager10 *pModuleManager = NULL;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
	pModuleManager = pManager;

	// Set the module info - If you can not read this in the console, the module is broken
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

	if (!ImportLua())
	{
		pModuleManager = nullptr;
		return false;
	}

	return true;
}


MTAEXPORT void RegisterFunctions(lua_State * luaVM)
{
	if (pModuleManager && luaVM)
	{
		pModuleManager->RegisterFunction(luaVM, "bcrypt_gensalt", CFunctions::gensalt);
		pModuleManager->RegisterFunction(luaVM, "bcrypt_hashpw", CFunctions::hashpw);
		pModuleManager->RegisterFunction(luaVM, "bcrypt_checkpw", CFunctions::checkpw);
	}
}


MTAEXPORT bool DoPulse(void)
{
	return true;
}

MTAEXPORT bool ShutdownModule(void)
{
	return true;
}
