--Minetest
--Copyright (C) 2013 sapier
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

--------------------------------------------------------------------------------

local core_developers = {
	"Perttu Ahola (celeron55) <celeron55@gmail.com>",
	"sfan5 <sfan5@live.de>",
	"Nathanaëlle Courant (Nore/Ekdohibs) <nore@mesecons.net>",
	"Loic Blot (nerzhul/nrz) <loic.blot@unix-experience.fr>",
	"paramat",
	"Andrew Ward (rubenwardy) <rw@rubenwardy.com>",
	"Krock/SmallJoker <mk939@ymail.com>",
	"Lars Hofhansl <larsh@apache.org>",
	"Pierre-Yves Rollo <dev@pyrollo.com>",
	"v-rob <robinsonvincent89@gmail.com>",
	"hecks",
	"Hugues Ross <hugues.ross@gmail.com>",
	"Dmitry Kostenko (x2048) <codeforsmile@gmail.com>",
}

-- For updating active/previous contributors, see the script in ./util/gather_git_credits.py

local active_contributors = {
	"Wuzzy [I18n for builtin, liquid features, fixes]",
	"Zughy [Various features and fixes]",
	"numzero [Graphics and rendering]",
	"Desour [Internal fixes, Clipboard on X11]",
	"Lars Müller [Various internal fixes]",
	"JosiahWI [CMake, cleanups and fixes]",
	"HybridDog [builtin, documentation]",
	"Jude Melton-Houghton [Database implementation]",
	"savilli [Fixes]",
	"Liso [Shadow Mapping]",
	"MoNTE48 [Build fix]",
	"Jean-Patrick Guerrero (kilbith) [Fixes]",
	"ROllerozxa [Code cleanups]",
	"Lejo [bitop library integration]",
	"LoneWolfHT [Build fixes]",
	"NeroBurner [Joystick]",
	"Elias Fleckenstein [Internal fixes]",
	"David CARLIER [Unix & Haiku build fixes]",
	"pecksin [Clickable web links]",
	"srfqi [Android & rendering fixes]",
	"EvidenceB [Formspec]",
}

local previous_core_developers = {
	"BlockMen",
	"Maciej Kasatkin (RealBadAngel) [RIP]",
	"Lisa Milne (darkrose) <lisa@ltmnet.com>",
	"proller",
	"Ilya Zhuravlev (xyz) <xyz@minetest.net>",
	"PilzAdam <pilzadam@minetest.net>",
	"est31 <MTest31@outlook.com>",
	"kahrl <kahrl@gmx.net>",
	"Ryan Kwolek (kwolekr) <kwolekr@minetest.net>",
	"sapier",
	"Zeno",
	"ShadowNinja <shadowninja@minetest.net>",
	"Auke Kok (sofar) <sofar@foo-projects.org>",
	"Aaron Suen <warr1024@gmail.com>",
}

local previous_contributors = {
	"Nils Dagsson Moskopp (erlehmann) <nils@dieweltistgarnichtso.net> [Minetest Logo]",
	"red-001 <red-001@outlook.ie>",
	"Giuseppe Bilotta",
	"Dániel Juhász (juhdanad) <juhdanad@gmail.com>",
	"MirceaKitsune <mirceakitsune@gmail.com>",
	"Constantin Wenger (SpeedProg)",
	"Ciaran Gultnieks (CiaranG)",
	"Paul Ouellette (pauloue)",
	"stujones11",
	"Rogier <rogier777@gmail.com>",
	"Gregory Currie (gregorycu)",
	"JacobF",
	"Jeija <jeija@mesecons.net> [HTTP, particles]",
}

local rollertest_info = {
	"This is a custom build of Minetest called ROllerTest.",
	"Do not report bugs specific to ROllerTest to",
	"the Minetest core developers!"
}

local function buildInfoList(source)
	local ret = {}
	for i = 1, #source do
		ret[i] = core.formspec_escape(source[i])
	end
	return table.concat(ret, ",#FF8888,")
end

local function buildCreditList(source)
	local ret = {}
	for i = 1, #source do
		ret[i] = core.formspec_escape(source[i])
	end
	return table.concat(ret, ",,")
end

return {
	name = "about",
	caption = fgettext("About"),
	cbf_formspec = function(tabview, name, tabdata)
		local logofile = core.formspec_escape(defaulttexturedir.."logo.png")
		local version = core.get_version()
		local openuserdatafolder

		if PLATFORM ~= "Android" then
			openuserdatafolder = [[
				tooltip[userdata;Opens the directory that contains user-provided worlds, games, mods,
and texture packs in a file manager / explorer.]
				button[0,4;3.5,1;userdata;Open User Data Folder]
			]]
		end

		return formspec_wrapper([[
			image[0.75,0.2;2.2,2.2;${logofile}]
			style[label_button;border=false]
			button[0,1.7;3.5,2;label_button;${version_string}]
			tablecolumns[color;text]
			tableoptions[background=#00000000;highlight=#00000000;border=false]
			table[3.5,-0.25;8.45,5.95;list_credits;#FF8888,${rollertest_info},
			,,#FFFF00,${lbl_core_developers},
			,${core_developers},
			,,#FFFF00,${lbl_active_contributors},
			,${active_contributors},
			,,#FFFF00,${lbl_previous_core_developers},
			,${previous_core_developers},
			,,#FFFF00,${lbl_previous_contributors},
			,${previous_contributors},;1]
			${openuserdatafolder}
		]], {
			logofile = logofile,
			version_string = version.project.." "..version.string,
			rollertest_info = buildInfoList(rollertest_info),
			lbl_core_developers = fgettext("Core Developers"),
			core_developers = buildCreditList(core_developers),
			lbl_active_contributors = fgettext("Active Contributors"),
			active_contributors = buildCreditList(active_contributors),
			lbl_previous_core_developers = fgettext("Previous Core Developers"),
			previous_core_developers = buildCreditList(previous_core_developers),
			lbl_previous_contributors = fgettext("Previous Contributors"),
			previous_contributors = buildCreditList(previous_contributors),
			openuserdatafolder = openuserdatafolder
		})
	end,
	cbf_button_handler = function(this, fields, name, tabdata)
		if fields.homepage then
			core.open_url("https://www.minetest.net")
		end

		if fields.userdata then
			core.open_dir(core.get_user_path())
		end
	end,
}
