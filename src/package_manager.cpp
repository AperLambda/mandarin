/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/mandarin/package_manager.h"
#include <lambdacommon/system/system.h>
#include <process.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;
using namespace TinyProcessLib;
using namespace lambdacommon;
using json = nlohmann::json;

namespace mandarin
{
	namespace package_manager
	{
		ActionType MANDARIN_API getActionTypeFromString(const std::string &str)
		{
			if (lambdastring::equalsIgnoreCase(str, "arg"))
				return ARG;
			else if (lambdastring::equalsIgnoreCase(str, "cmd"))
				return CMD;
			// TODO: Adds a better error handler.
			return CMD;
		}

		Action::Action(const string &name, const string &value, ActionType type) : _name(name), _value(value),
		                                                                           _type(type)
		{}

		const string &Action::getName() const
		{
			return _name;
		}

		void Action::setName(const string &name)
		{
			Action::_name = name;
		}

		const string &Action::getValue() const
		{
			return _value;
		}

		void Action::setValue(const string &value)
		{
			Action::_value = value;
		}

		ActionType Action::getType() const
		{
			return _type;
		}

		void Action::setType(ActionType type)
		{
			Action::_type = type;
		}

		std::string Action::format() const
		{
			return (_type == ActionType::ARG ? "-" : "") + _value;
		}

		PackageManager::PackageManager(const string &name, const string &baseCmd, const Action &testAction) : _name(
				name), _baseCmd(baseCmd), _testAction(testAction)
		{
		}

		bool PackageManager::exists()
		{
			Process process{(_baseCmd + " " + _testAction.format()), "", [](const char *bytes, size_t n)
				{}};
			int returnCode;
			while (!process.try_get_exit_status(returnCode))
			{}
			return returnCode == 0;
		}

		PackageManager MANDARIN_API getPackageManager(const string &name)
		{
			fs::FilePath path = getDataFolder() / "package_managers" / (name + ".json");

			if (!path.exists())
				throw runtime_error("Cannot load package manager '" + name + "': file doesn't exist!");

			ifstream input{path.toString()};

			json jsonObj;

			input >> jsonObj;

			json jActions = jsonObj["actions"];
			return PackageManager(jsonObj["name"], jsonObj["base_cmd"],
			                      Action{"test", jActions["test"]["value"], getActionTypeFromString(jActions["test"]["type"])});
		}
	}
}