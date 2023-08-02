--Minetest
--Copyright (C) 2023 ROllerozxa
--
--This program is free software; you can redistribute it and/or modify
--it under the terms of the GNU Lesser General Public License as published by
--the Free Software Foundation; either version 2.1 of the License, or
--(at your option) any later version.
--
--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU Lesser General Public License for more details.
--
--You should have received a copy of the GNU Lesser General Public License along
--with this program; if not, write to the Free Software Foundation, Inc.,
--51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

server_password = ""

local function get_formspec()
	local fs = {
		"formspec_version[6]",
		"size[6,7.5,true]",
		"position[0.5,0.55]",

		"style[lbl_title;border=false]",
		"button[0,0;6,0.9;lbl_title;Configure Server]",

		"checkbox[0.5,1.25;server_announce;", fgettext("Announce Server"), ";",
			dump(core.settings:get_bool("server_announce")), "]",

		"field[0.5,2.25;5,0.75;playername;", fgettext("Name"), ";",
			core.formspec_escape(core.settings:get("name")), "]",

		"pwdfield[0.5,3.75;5,0.75;password;", fgettext("Password"), "]",

		"field[0.5,5.25;5,0.75;serverport;", fgettext("Server Port"), ";",
			core.formspec_escape(core.settings:get("port")), "]",

		"button[0.5,6.4;5,0.8;btn_save;", fgettext("Save"), "]"
	}

	return table.concat(fs)
end

local function buttonhandler(this, fields)
	if fields.server_announce then
		core.settings:set("server_announce", fields.server_announce)
		return true
	end

	if fields.btn_save or fields["key_enter"] then
		if fields["key_enter"] then
			-- HACK: See dlg_create_world.lua:348
			this.parent.dlg_create_world_closed_at = core.get_us_time()
		end

		core.settings:set("name", fields.playername)
		core.settings:set("port", fields.serverport)

		server_password = fields.password

		this:delete()

		return true
	end

	return false
end

function create_config_server_dlg()
	return dialog_create("config_server", get_formspec, buttonhandler, nil)
end
