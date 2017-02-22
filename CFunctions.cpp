#include <random>
#include <algorithm>

namespace bcrypt {
	#include "bcrypt/bcrypt.h"
}
#include "CFunctions.h"
#include "extra/CLuaArguments.h"

int CFunctions::gensalt(lua_State* luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TNUMBER)
		{
			int rounds = luaL_checkinteger(luaVM, 1);
			if (rounds < 10 || rounds > 31)
			{
				luaL_error(luaVM, "Bad number of rounds @ 'bcrypt_gensalt'");
				lua_pushboolean(luaVM, false);
			}
			else
			{
				char input[16] = { 0 };
				char output[64] = { 0 };

				std::random_device rd;
				std::mt19937 gen(rd());
				std::generate_n(input, sizeof(input), gen);

				int err = bcrypt::bcrypt_gensalt(rounds, input, output);
				if (err) {
					luaL_error(luaVM, "Unable to generate salt @ 'bcrypt_gensalt'");
					lua_pushboolean(luaVM, false);
				}
				else {
					lua_pushstring(luaVM, output);
				}
			}
		}
		else
		{
			luaL_error(luaVM, "Bad argument @ 'bcrypt_gensalt'");
			lua_pushboolean(luaVM, false);
		}
		return 1;
	}
	return 0;
}

int CFunctions::hashpw(lua_State* luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			const char* password = luaL_checkstring(luaVM, 1);
			const char* salt = luaL_checkstring(luaVM, 2);
			char salt2[BCRYPT_HASHSIZE] = { 0 };
			char hash[BCRYPT_HASHSIZE] = { 0 };

			strcpy_s(salt2, salt);
			int err = bcrypt::bcrypt_hashpw(password, salt2, hash);
			if (err)
			{
				luaL_error(luaVM, "Unable to generate hash @ 'bcrypt_hashpw'");
				lua_pushboolean(luaVM, false);
			}
			else
			{
				lua_pushstring(luaVM, hash);
			}
		}
		else
		{
			luaL_error(luaVM, "Bad argument @ 'bcrypt_hashpw'");
			lua_pushboolean(luaVM, false);
		}
		return 1;
	}
	return 0;
}

int CFunctions::checkpw(lua_State* luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			const char* password = luaL_checkstring(luaVM, 1);
			const char* hash = luaL_checkstring(luaVM, 2);
			char hash2[BCRYPT_HASHSIZE] = { 0 };
			strcpy_s(hash2, hash);

			int err = bcrypt::bcrypt_checkpw(password, hash);
			lua_pushboolean(luaVM, err == 0);
		}
		else
		{
			luaL_error(luaVM, "Bad argument @ 'bcrypt_checkpw'");
			lua_pushboolean(luaVM, false);
		}
		return 1;
	}
	return 0;
}
