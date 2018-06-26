/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of mandarin.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef MANDARIN_PACKAGE_H
#define MANDARIN_PACKAGE_H

#include "mandarin.h"
#include <lambdacommon/system/uri.h>
#include <nlohmann/json.hpp>
#include <optional>

namespace mandarin
{
	class MANDARIN_API Package
	{
	protected:
		std::string _name;
		std::string _description;
		std::string _version;
		std::vector<std::string> _authors;
		std::string _license;
		std::string _homepage;
	public:
		Package(std::string name, std::string description, std::string version, std::string license,
		        std::vector<std::string> authors = {}, std::string homepage = "");

		/*!
		 * Gets the name of the package.
		 * @return The package name.
		 */
		const std::string &getName() const;

		/*!
		 * Gets the description of the package.
		 * @return The package description.
		 */
		const std::string &getDescription() const;

		/*!
		 * Gets the version of the package.
		 * @return The package version.
		 */
		const std::string &getVersion() const;

		nlohmann::basic_json<mandarin_fifo_map> toJson() const;
	};

	namespace package
	{
		extern std::optional<Package> MANDARIN_API loadPackage(const nlohmann::basic_json<mandarin_fifo_map> &package);

		extern std::optional<Package> MANDARIN_API
		createPackage(const lambdacommon::fs::FilePath &path, const std::string &name, const std::string &description,
		              const std::string &version, const std::string &license,
		              const std::vector<std::string> &authors = {});

		extern std::optional<Package> MANDARIN_API searchPackage(const std::string &name);
	}

	class Database
	{
	private:
		std::string _name;
		lambdacommon::uri::URI _uri;

	public:
		Database() : _name(""), _uri({"", "", "", lambdacommon::Address::EMPTY})
		{}

		Database(std::string name, lambdacommon::uri::URI uri);

		const std::string &getName() const;

		const lambdacommon::uri::URI &getURI() const;
	};

	namespace database
	{
		/*!
		 * Loads the database in the configuration.
		 * @return True if success, else false.
		 */
		extern bool MANDARIN_API initDatabases();

		extern bool MANDARIN_API updateDatabases();

		extern std::map<uint32_t, Database> MANDARIN_API getDatabases();


	}
}

#endif //MANDARIN_PACKAGE_H