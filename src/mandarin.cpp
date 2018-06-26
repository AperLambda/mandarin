/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/mandarin/mandarin.h"

using namespace std;
using namespace lambdacommon;

namespace mandarin
{
	fs::FilePath *dataFolder;
	fs::FilePath *varFolder;

	string MANDARIN_API getVersion()
	{
		return to_string(MANDARIN_VERSION_MAJOR) + '.' + to_string(MANDARIN_VERSION_MINOR) + '.' +
		       to_string(MANDARIN_VERSION_PATCH);
	}

	uint8_t MANDARIN_API init()
	{
		if (dataFolder == nullptr)
#ifdef LAMBDA_WINDOWS
			setDataFolder(system::getUserDirectory() / ("AppData/Roaming/" + MANDARIN_NAME + "/data"));
#else
			setDataFolder({string("/etc/") + MANDARIN_NAME});
#endif

		if (varFolder == nullptr)
#ifdef LAMBDA_WINDOWS
			varFolder = new fs::FilePath(system::getUserDirectory() / ("AppData/Roaming/" + MANDARIN_NAME + "/var"));
#else
			varFolder = new fs::FilePath{string("/var/lib/") + MANDARIN_NAME};
#endif

		if (!getDataFolder().exists())
			return MISSING_DATA_FOLDER;

		if (!getVarFolder().exists())
			if (!getVarFolder().mkdir(true))
				return MISSING_VAR_FOLDER;

		if (!database::initDatabases())
			return RUNTIME_ERROR;

		return SUCCESS;
	}

	void MANDARIN_API setDataFolder(fs::FilePath path)
	{
		if (dataFolder == nullptr)
			dataFolder = new fs::FilePath(path);
		*dataFolder = move(path);
	}

	fs::FilePath MANDARIN_API getDataFolder()
	{
		return *dataFolder;
	}

	lambdacommon::fs::FilePath MANDARIN_API getVarFolder()
	{
		return *varFolder;
	}
}