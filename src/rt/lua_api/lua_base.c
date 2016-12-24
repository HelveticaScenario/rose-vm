#include "rt/lua_api/lua_base.h"

rose_runtime_base* rose_lua_base_get_runtime_base(lua_State* L) {
    rose_runtime_base* r = NULL;
    int uidx = lua_upvalueindex(1);
    if (lua_islightuserdata(L, uidx) == 1) {
        r = lua_touserdata(L, uidx);
    } else {
        luaL_error(L, "something went horribly wrong: upvalueindex 1 was not a lightuserdata");
    }
    return r;
}