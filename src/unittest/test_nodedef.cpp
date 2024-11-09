/*
Minetest
Copyright (C) 2013 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "gamedef.h"
#include "nodedef.h"
#include "network/networkprotocol.h"

#include <catch.h>

#include <ios>
#include <sstream>


TEST_CASE("Given a node definition, "
		"when we serialize and then deserialize it, "
		"then the deserialized one should be equal to the original.",
		"[nodedef]")
{
	ContentFeatures f;
	f.name = "default:stone";
	for (TileDef &tiledef : f.tiledef)
		tiledef.name = "default_stone.png";
	f.is_ground_content = true;

	std::ostringstream os(std::ios::binary);
	f.serialize(os, LATEST_PROTOCOL_VERSION);
	std::istringstream is(os.str(), std::ios::binary);
	ContentFeatures f2;
	f2.deSerialize(is, LATEST_PROTOCOL_VERSION);

	CHECK(f.walkable == f2.walkable);
	CHECK(f.node_box.type == f2.node_box.type);
}
