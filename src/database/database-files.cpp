/*
Minetest
Copyright (C) 2017 nerzhul, Loic Blot <loic.blot@unix-experience.fr>

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

#include <cassert>
#include "convert_json.h"
#include "database-files.h"
#include "remoteplayer.h"
#include "settings.h"
#include "porting.h"
#include "filesys.h"
#include "server/player_sao.h"
#include "util/string.h"

ModMetadataDatabaseFiles::ModMetadataDatabaseFiles(const std::string &savedir):
	m_storage_dir(savedir + DIR_DELIM + "mod_storage")
{
}

bool ModMetadataDatabaseFiles::getModEntries(const std::string &modname, StringMap *storage)
{
	Json::Value *meta = getOrCreateJson(modname);
	if (!meta)
		return false;

	const Json::Value::Members attr_list = meta->getMemberNames();
	for (const auto &it : attr_list) {
		Json::Value attr_value = (*meta)[it];
		(*storage)[it] = attr_value.asString();
	}

	return true;
}

bool ModMetadataDatabaseFiles::setModEntry(const std::string &modname,
	const std::string &key, const std::string &value)
{
	Json::Value *meta = getOrCreateJson(modname);
	if (!meta)
		return false;

	(*meta)[key] = Json::Value(value);
	m_modified.insert(modname);

	return true;
}

bool ModMetadataDatabaseFiles::removeModEntry(const std::string &modname,
		const std::string &key)
{
	Json::Value *meta = getOrCreateJson(modname);
	if (!meta)
		return false;

	Json::Value removed;
	if (meta->removeMember(key, &removed)) {
		m_modified.insert(modname);
		return true;
	}
	return false;
}

void ModMetadataDatabaseFiles::beginSave()
{
}

void ModMetadataDatabaseFiles::endSave()
{
	if (m_modified.empty())
		return;

	if (!fs::CreateAllDirs(m_storage_dir)) {
		errorstream << "ModMetadataDatabaseFiles: Unable to save. '"
				<< m_storage_dir << "' cannot be created." << std::endl;
		return;
	}
	if (!fs::IsDir(m_storage_dir)) {
		errorstream << "ModMetadataDatabaseFiles: Unable to save. '"
				<< m_storage_dir << "' is not a directory." << std::endl;
		return;
	}

	for (auto it = m_modified.begin(); it != m_modified.end();) {
		const std::string &modname = *it;

		const Json::Value &json = m_mod_meta[modname];

		if (!fs::safeWriteToFile(m_storage_dir + DIR_DELIM + modname, fastWriteJson(json))) {
			errorstream << "ModMetadataDatabaseFiles[" << modname
					<< "]: failed to write file." << std::endl;
			++it;
			continue;
		}

		it = m_modified.erase(it);
	}
}

void ModMetadataDatabaseFiles::listMods(std::vector<std::string> *res)
{
	// List in-memory metadata first.
	for (const auto &pair : m_mod_meta) {
		res->push_back(pair.first);
	}

	// List other metadata present in the filesystem.
	for (const auto &entry : fs::GetDirListing(m_storage_dir)) {
		if (!entry.dir && m_mod_meta.count(entry.name) == 0)
			res->push_back(entry.name);
	}
}

Json::Value *ModMetadataDatabaseFiles::getOrCreateJson(const std::string &modname)
{
	auto found = m_mod_meta.find(modname);
	if (found != m_mod_meta.end())
		return &found->second;

	Json::Value meta(Json::objectValue);

	std::string path = m_storage_dir + DIR_DELIM + modname;
	if (fs::PathExists(path)) {
		std::ifstream is(path.c_str(), std::ios_base::binary);

		Json::CharReaderBuilder builder;
		builder.settings_["collectComments"] = false;
		std::string errs;

		if (!Json::parseFromStream(builder, is, &meta, &errs)) {
			errorstream << "ModMetadataDatabaseFiles[" << modname
					<< "]: failed to decode data: " << errs << std::endl;
			return nullptr;
		}
	}

	return &(m_mod_meta[modname] = std::move(meta));
}
