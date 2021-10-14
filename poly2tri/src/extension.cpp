#define EXTENSION_NAME poly2tri
#define LIB_NAME "poly2tri"
#define MODULE_NAME "poly2tri"

#include <dmsdk/sdk.h>
#include "poly2tri.h"

// include the Defold SDK



#include <dmsdk/sdk.h>

#define luaL_check(c,L, ...)    if (!(c)) luaL_error(L, __VA_ARGS__)

static int api_triangulate(lua_State *L) {
  luaL_check(lua_gettop(L) == 2, L, "Pass two tables: vertices and holes.");
  luaL_check(lua_istable(L, 1) == 1, L, "First arg must be a table of vertices.");
  luaL_check(lua_istable(L, 2) == 1, L, "Second arg must be a table of holes.");
  size_t vertexcount = lua_objlen(L, 1);
  luaL_check(vertexcount >= 6, L, "Only got %zu vertices. Pass at least 6.", vertexcount);
  luaL_check(vertexcount % 2 == 0, L, "Pass an even number of vertices. Got %zu.", vertexcount);
  std::vector<p2t::Point*> verts;
  verts.reserve(vertexcount / 2);
  lua_pushnil(L);
  while (lua_next(L, 1) != 0) {
    double x = (double) lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_next(L, 1);
    double y = (double) lua_tonumber(L, -1);
    lua_pop(L, 1);

    verts.push_back(new p2t::Point(x, y));
  }

  p2t::CDT* cdt = new p2t::CDT(verts);

  lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    lua_pushnil(L);

    luaL_check(lua_istable(L, -2) == 1, L, "Each hole must be a table of vertices.");
    size_t holevertexcount = lua_objlen(L, -2);
    luaL_check(holevertexcount >= 6, L, "Only got %zu vertices for a hole. Pass at least 6.", holevertexcount);
    luaL_check(holevertexcount % 2 == 0, L, "Pass an even number of vertices for a hole. Got %zu.", holevertexcount);
    std::vector<p2t::Point*> hole;
    hole.reserve(holevertexcount);

    while (lua_next(L, -2) != 0) {
      double x = (double) lua_tonumber(L, -1);
      lua_pop(L, 1);
      lua_next(L, -2);
      double y = (double) lua_tonumber(L, -1);
      lua_pop(L, 1);

      hole.push_back(new p2t::Point(x, y));
    }
    cdt->AddHole(hole);

    lua_pop(L, 1);
  }

  cdt->Triangulate();
  std::vector<p2t::Triangle*> tris = cdt->GetTriangles();
  lua_createtable(L, tris.size(), 0);
  for (size_t i = 0; i < tris.size(); ++i)
  {
    lua_createtable(L, 6, 0);

    lua_pushnumber(L, tris[i]->GetPoint(0)->x);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, tris[i]->GetPoint(0)->y);
    lua_rawseti(L, -2, 2);
    lua_pushnumber(L, tris[i]->GetPoint(1)->x);
    lua_rawseti(L, -2, 3);
    lua_pushnumber(L, tris[i]->GetPoint(1)->y);
    lua_rawseti(L, -2, 4);
    lua_pushnumber(L, tris[i]->GetPoint(2)->x);
    lua_rawseti(L, -2, 5);
    lua_pushnumber(L, tris[i]->GetPoint(2)->y);
    lua_rawseti(L, -2, 6);

    lua_rawseti(L, -2, i + 1);
  }
  for (size_t i = 0; i < verts.size(); ++i)
    delete verts[i];
  delete cdt;
  return 1;
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"triangulate", api_triangulate},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, 0, 0, FinalizeMyExtension)