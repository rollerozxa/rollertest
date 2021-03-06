/*
Minetest
Copyright (C) 2018 Dumbeldor, Vincent Glize <vincent.glize@live.fr>
Copyright (C) 2022 ROllerozxa <rollerozxa@voxelmanip.se>

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

#pragma once

#ifndef __ANDROID__

#include <iostream>
#include <memory>

struct DataRichPresence;

class Discord
{
public:
	Discord();
	~Discord();

	static std::unique_ptr<Discord> createDiscordSingleton();
	void init();

	void setState(const std::string &state);
	void setDetails(const std::string &details);
	void updatePresence();

	static void handleDiscordError(int errcode, const char *message);
	static void handleDiscordReady();

private:
	static const std::string s_application_id;
	std::unique_ptr<DataRichPresence> m_data;
};

extern std::unique_ptr<Discord> g_discord;

#endif
