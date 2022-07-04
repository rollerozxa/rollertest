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

#ifndef __ANDROID__

#include "discord.h"
#include <ctime>
#include "discord_rpc.h"
#include <string>
#include <assert.h>
#include <log.h>
#include <cstring>

std::unique_ptr<Discord> g_discord;
const std::string Discord::s_application_id = "857961580586467359";

struct DataRichPresence
{
	std::string state = "";
	std::string details = "";
	uint64_t start_timestamp;
	std::string large_image_key = "default";
	std::string large_image_text = "Minetest is a FOSS voxel-based game engine.";
	std::string small_image_key = "";
	std::string small_image_text = "";
	std::string party_id = "";
	uint32_t party_size = 0;
	uint32_t party_max = 0;
};

Discord::Discord()
{
	m_data = std::unique_ptr<DataRichPresence>(new DataRichPresence);
	m_data->start_timestamp = time(0);
}

Discord::~Discord()
{
	Discord_Shutdown();
}

std::unique_ptr<Discord> Discord::createDiscordSingleton()
{
	assert(!g_discord);
	return std::unique_ptr<Discord>(new Discord());
}

void Discord::init()
{
	DiscordEventHandlers handlers{};
	/*
		Handlers list :
		ready, disconnected, errored, joinGame, spectateGame, joinRequest
	*/
	//handlers.ready = Discord::handleDiscordReady;
	//handlers.errored = Discord::handleDiscordError;
	Discord_Shutdown();
	Discord_Initialize(s_application_id.c_str(), &handlers, 1, NULL);
}

void Discord::setState(const std::string &state)
{
	m_data->state = state.substr(0, 128);
}

void Discord::setDetails(const std::string &details)
{
	m_data->details = details.substr(0, 128);
}

void Discord::updatePresence()
{
	DiscordRichPresence discord_presence;
	memset(&discord_presence, 0, sizeof(discord_presence));

	discord_presence.state = m_data->state.c_str();
	discord_presence.details = m_data->details.c_str();
	discord_presence.startTimestamp = m_data->start_timestamp;
	discord_presence.largeImageKey = m_data->large_image_key.c_str();
	discord_presence.largeImageText = m_data->large_image_text.c_str();
	discord_presence.smallImageKey = m_data->small_image_key.c_str();
	discord_presence.smallImageText = m_data->small_image_text.c_str();
	discord_presence.partyId = m_data->party_id.c_str();
	discord_presence.partySize = m_data->party_size;
	discord_presence.partyMax = m_data->party_max;
	Discord_UpdatePresence(&discord_presence);
}

void Discord::handleDiscordReady()
{
	infostream << "Discord is ready" << std::endl;
}

void Discord::handleDiscordError(int errcode, const char *message)
{
	errorstream << "Error from Discord RPC: " << std::to_string(errcode) << " " << message
		    << std::endl;
}

#endif
