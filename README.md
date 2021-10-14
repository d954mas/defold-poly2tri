

## Poly2tri lua wrapper

This is a Lua wrapper around [poly2tri](https://github.com/jhasse/poly2tri). It provides fast, stable Constrained Delauney Triangulation of convex or concave polygons with or without holes. 
Fork of [lua-poly2tri](https://github.com/TannerRogalsky/lua-poly2tri)

If you like the extension you can support me on patreon.
It will help me make more items for defold.

[![](https://c5.patreon.com/external/logo/become_a_patron_button.png)](https://www.patreon.com/d954mas)


<img src="https://github.com/d954mas/defold-poly2tri/blob/master/docs/example.jpg">

## Installation

__1)__ Add defold-poly2tri in your own project as a Defold library dependency. Open your game.project file and in the dependencies field under project add:
https://github.com/d954mas/defold-poly2tri/archive/refs/tags/1.0.0.zip

# Usage
```Lua
local vertices = {0, 0, 0, 200, 200, 200, 200, 0}
local holes = {
  {10, 10, 50, 10, 10, 50},
  {100, 10, 175, 10, 175, 100, 100, 50, 10, 100},
  {10, 175, 150, 100, 190, 175}
}
local triangles = poly2tri.triangulate(verts, holes)
```

The output will be a table where each entry is a table consisting of 6 numbers (3 vertices).

# Limitations
There are limitations based on the underlying poly2tri library.

- If you have a cyclic function that generates random points make sure you don't add the same coordinate twice.
- Polygons must not self-intersect.
- Interior holes must not touch other holes, nor touch the polyline boundary.

It might be useful to clean up your polygon data using something like [clipper](https://luapower.com/clipper).
