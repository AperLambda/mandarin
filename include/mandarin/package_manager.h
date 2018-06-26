/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef MANDARIN_PACKAGE_MANAGER_H
#define MANDARIN_PACKAGE_MANAGER_H

#include "mandarin.h"
#include <nlohmann/json.hpp>

namespace mandarin
{
	namespace package_manager
	{
		enum ActionType
		{
			ARG,
			CMD
		};

		extern ActionType MANDARIN_API getActionTypeFromString(const std::string &str);

		class MANDARIN_API Action
		{
		private:
			std::string _name;
			std::string _value;
			ActionType _type;

		public:
			Action(const std::string &name, const std::string &value, ActionType type);

			const std::string &getName() const;

			void setName(const std::string &name);

			const std::string &getValue() const;

			void setValue(const std::string &value);

			ActionType getType() const;

			void setType(ActionType type);

			std::string format() const;
		};

		class MANDARIN_API PackageManager
		{
		private:
			std::string _name;
			std::string _baseCmd;
			Action _testAction;

		public:
			PackageManager(const std::string &name, const std::string &baseCmd, const Action &testAction);

			bool exists();
		};

		extern PackageManager MANDARIN_API getPackageManager(const std::string &name);
	}
}

#endif //MANDARIN_PACKAGE_MANAGER_H