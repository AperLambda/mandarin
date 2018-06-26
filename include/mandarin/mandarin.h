/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef MANDARIN_MANDARIN_H
#define MANDARIN_MANDARIN_H

#include <mandarin_exports.h>
#include <lambdacommon/system/system.h>
#include <fifo_map.hpp>

template<class K, class V, class dummy_compare, class A>
using mandarin_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

#define MANDARIN_VERSION_MAJOR 1
#define MANDARIN_VERSION_MINOR 0
#define MANDARIN_VERSION_PATCH 0

#define MANDARIN_NAME "mandarin"

#include "package.h"
#include "package_manager.h"

namespace mandarin
{
	enum MandarinResult
	{
		SUCCESS = 0b0000,
		RUNTIME_ERROR = 0b0001,
		BAD_USAGE = 0b0010,
		ALREADY_EXISTS = 0b0100,
		MISSING_PACKAGE = 0b1000,
		MISSING_PACKAGE_MANAGER = 0b1001,
		MISSING_DATA_FOLDER = 0b1010,
		MISSING_VAR_FOLDER = 0b1011
	};

	extern std::string MANDARIN_API getVersion();

	extern uint8_t MANDARIN_API init();

	extern void MANDARIN_API setDataFolder(lambdacommon::fs::FilePath path);

	extern lambdacommon::fs::FilePath MANDARIN_API getDataFolder();

	extern lambdacommon::fs::FilePath MANDARIN_API getVarFolder();
}

#endif //MANDARIN_MANDARIN_H