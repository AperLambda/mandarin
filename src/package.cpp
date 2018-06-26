/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#include "../include/mandarin/package.h"
#include <fifo_map.hpp>
#include <utility>
#include <fstream>

#define CHECK_COMPONENT(package, key) if (!(package).count(key)) return {};
using namespace std;
using namespace lambdacommon;
using json = nlohmann::basic_json<mandarin_fifo_map>;

namespace mandarin
{
	Package::Package(string name, string description, string version, string license, vector<string> authors,
	                 string homepage) : _name(move(name)),
	                                    _description(move(description)),
	                                    _version(move(version)),
	                                    _authors(move(authors)),
	                                    _license(move(license)),
	                                    _homepage(move(homepage))
	{}

	const std::string &Package::getName() const
	{
		return _name;
	}

	const string &Package::getDescription() const
	{
		return _description;
	}

	const std::string &Package::getVersion() const
	{
		return _version;
	}

	json Package::toJson() const
	{
		json jsonObj;
		jsonObj["name"] = _name;
		jsonObj["description"] = _description;
		jsonObj["version"] = _version;
		jsonObj["authors"] = _authors;
		jsonObj["license"] = _license;
		if (!_homepage.empty())
			jsonObj["homepage"] = _homepage;
		return jsonObj;
	}

	namespace package
	{
		template<class T>
		T getOrDefault(const json &package, const string &key, T defaultValue)
		{
			if (package.count(key))
				return package[key];
			else return defaultValue;
		}

		optional<Package> MANDARIN_API loadPackage(const json &package)
		{
			CHECK_COMPONENT(package, "name")
			CHECK_COMPONENT(package, "description")
			CHECK_COMPONENT(package, "version")
			CHECK_COMPONENT(package, "license")
			auto authors = getOrDefault(package, "authors", vector<string>{});
			auto homepage = getOrDefault<string>(package, "homepage", "");

			return Package{package["name"], package["description"], package["version"], package["license"], authors, homepage};
		}

		optional<Package> MANDARIN_API
		createPackage(const fs::FilePath &path, const string &name, const string &description, const string &version, const string &license,
		              const vector<string> &authors)
		{
			auto packageDirectory = fs::FilePath{path.toString() + '/' + name};
			// Cannot create package without directory.
			if (!packageDirectory.exists() && !packageDirectory.mkdir(true))
				return {};

			Package package{name, description, version, license, authors};
			auto jsonObj = package.toJson();

			ofstream ostream((packageDirectory / "package.json").toString());
			ostream << jsonObj.dump(2) << endl;
			ostream.close();
			return loadPackage(jsonObj);
		}

		optional<Package> MANDARIN_API searchPackage(const string &name)
		{
			return {};
		}
	}

	Database::Database(string name, uri::URI uri) : _name(std::move(name)), _uri(std::move(uri))
	{}

	const string &Database::getName() const
	{
		return _name;
	}

	const uri::URI &Database::getURI() const
	{
		return _uri;
	}

	namespace database
	{
		map<uint32_t, Database> databases;

		bool MANDARIN_API initDatabases()
		{
			auto dbFile = getDataFolder() / "db.json";

			ifstream inputStream{dbFile.toString()};
			json j;
			inputStream >> j;
			inputStream.close();

			if (!j.count("db"))
				return false;

			json db = j.at("db");
			uint32_t i = 0;
			databases.clear();
			for (json::iterator item = db.begin(); item != db.end(); item++)
			{
				databases[i] = Database{item.key(), uri::fromString(item.value())};
				i++;
			}

			return true;
		}

		map<uint32_t, Database> MANDARIN_API getDatabases()
		{
			return databases;
		}
	}
}

#undef CHECK_COMPONENT