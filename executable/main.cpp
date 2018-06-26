#include <mandarin/mandarin.h>
#include <args.hxx>
#include <lambdacommon/system/system.h>
#include <process.hpp>

using namespace lambdacommon;
using namespace std;
using namespace mandarin;

void printLogo()
{
	cout << "  " << terminal::B_YELLOW << "    " << terminal::B_GREEN << " " << terminal::B_YELLOW << "   "
	     << terminal::RESET
	     << endl;
	cout << " " << terminal::B_YELLOW << "    " << terminal::B_GREEN << "   " << terminal::B_YELLOW << "   "
	     << terminal::RESET
	     << endl;
	cout << terminal::B_YELLOW << "      " << terminal::B_GREEN << " " << terminal::B_YELLOW << "     "
	     << terminal::RESET
	     << endl;
	cout << terminal::B_YELLOW << "         " << terminal::B_LIGHT_YELLOW << " " << terminal::B_YELLOW << "  "
	     << terminal::RESET
	     << endl;
	cout << terminal::B_YELLOW << "         " << terminal::B_LIGHT_YELLOW << " " << terminal::B_YELLOW << "  "
	     << terminal::RESET
	     << endl;
	cout << terminal::B_YELLOW << "            " << terminal::RESET << endl;
	cout << " " << terminal::B_YELLOW << "          " << terminal::RESET << endl;
	cout << "  " << terminal::B_YELLOW << "        " << terminal::RESET << endl;
}

void printVersion()
{
	printLogo();
	cout << endl;
	cout << terminal::TermFormatting::YELLOW << "Mandarin" << terminal::RESET << " v" << mandarin::getVersion() << endl;
}

void printHelp()
{
	cout << "> " << terminal::YELLOW << "mandarin" << terminal::RESET << terminal::LIGHT_RED
	     << " <command> [arguments] [options]" << terminal::RESET << endl;
	cout << endl;
	cout << "  " << terminal::YELLOW << "mandarin" << terminal::RESET << ", the universal package manager. (v"
	     << mandarin::getVersion() << ')' << endl;
	cout << endl;
	cout << "  COMMANDS" << endl;
	cout << endl;
	cout << "  - " << terminal::LIGHT_GREEN << "help" << terminal::RESET << " Displays this help menu." << endl;
	cout << "  - " << terminal::LIGHT_GREEN << "create" << terminal::RESET << " Creates a new package." << endl;
	cout << endl;
	cout << "  OPTIONS" << endl;
	cout << endl;
	cout << "  - " << terminal::GREEN << "-v, --version" << terminal::RESET << " Prints the version of mandarin."
	     << endl;
	cout << endl;
}

int executeCreate()
{
	cout << terminal::LIGHT_GREEN << "Welcome to " << terminal::YELLOW << "Mandarin" << terminal::LIGHT_GREEN
	     << "'s packet creation wizard!" << terminal::RESET << endl;
	goto get_name;
	get_name:
	cout << terminal::YELLOW << "Name: " << terminal::LIGHT_YELLOW;
	string packageName;
	getline(cin, packageName);
	if (packageName.empty())
	{
		cout << terminal::LIGHT_RED << "Name cannot be empty!" << terminal::RESET << endl;
		goto get_name;
	}
	goto get_description;
	get_description:
	cout << terminal::YELLOW << "Description: " << terminal::LIGHT_YELLOW;
	string description;
	getline(cin, description);
	if (description.empty())
		description = "1.0.0";
	goto get_version;
	get_version:
	cout << terminal::YELLOW << "Version (default 1.0.0): " << terminal::LIGHT_YELLOW;
	string version;
	getline(cin, version);
	if (version.empty())
		version = "";
	goto get_authors;
	get_authors:
	string tmpAuthors;
	cout << terminal::YELLOW << "Authors (separated by commas ','): " << terminal::LIGHT_YELLOW;
	getline(cin, tmpAuthors);
	auto authors = lambdastring::split(tmpAuthors, ',');
	if (tmpAuthors.empty())
		cout << terminal::YELLOW << "Warning: " << terminal::LIGHT_RED << "authors field is empty!";
	goto get_license;
	get_license:
	cout << terminal::YELLOW << "License: " << terminal::LIGHT_YELLOW;
	string license;
	getline(cin, license);
	cout << terminal::RESET;
	if (license.empty())
	{
		cout << terminal::LIGHT_RED << "License cannot be empty!" << terminal::RESET << endl;
		goto get_license;
	}
	goto create;

	create:
	fs::FilePath path = {packageName};
	if (path.exists())
	{
		cout << terminal::RED << "Error: " << terminal::LIGHT_RED << path.toString() << " already exists!"
		     << terminal::RESET << endl;
		return MandarinResult::ALREADY_EXISTS;
	}

	auto package = package::createPackage(fs::getCurrentWorkingDirectory(), packageName, description, version, license,
	                                      authors);
	if (!package)
	{
		cout << terminal::RED << "Error: " << terminal::LIGHT_RED << "An unexpected error happened!" << terminal::RESET
		     << endl;
		return MandarinResult::RUNTIME_ERROR;
	}
	else
	{
		cout << terminal::LIGHT_GREEN << "Package '" << terminal::GREEN << package->getName() << terminal::LIGHT_GREEN
		     << "' created!" << terminal::RESET << endl;
		return MandarinResult::SUCCESS;
	}
}

int main(int argc, char **argv)
{
	terminal::setup();

	mandarin::setDataFolder("./data/");

	auto mandarinInit = mandarin::init();
	if (mandarinInit != MandarinResult::SUCCESS)
		return mandarinInit;

	string command;
	if (argc <= 1)
	{
		printLogo();
		cout << endl;
		printHelp();
		return MandarinResult::BAD_USAGE;
	}
	else
	{
		string str{argv[1]};
		if (!lambdastring::startsWith(str, "-"))
			command = str;
	}

	if (command == "help")
	{
		printHelp();
		return MandarinResult::SUCCESS;
	}
	else if (command == "create")
	{
		return executeCreate();
	}

	auto argArray = argv;
	auto argArrayLength = argc;

	if (!command.empty())
	{
		argArray = new char *[argc - 1];
		int newIndex = 0;
		for (int i = 0; i < argc; i++)
		{
			if (i == 1)
				continue;
			argArray[newIndex] = argv[i];
			newIndex++;
		}
		argArrayLength = argArrayLength - 1;
	}

	args::ArgumentParser argParser("mandarin, the universal package manager.", "");
	args::HelpFlag help(argParser, "help", "Displays this help menu.", {'h', "help"});
	args::CompletionFlag completion(argParser, {"complete"});

	args::Group baseGroup(argParser, "Base information:", args::Group::Validators::DontCare);

	args::Flag versionFlag(baseGroup, "version", "Prints the version of mandarin.", {'v', "version"});

	try
	{
		argParser.ParseCLI(argArrayLength, argArray);
	}
	catch (args::Help &)
	{
		printHelp();
		return 0;
	}
	catch (args::ParseError &e)
	{
		cerr << e.what() << endl;
		cerr << argParser;
		return 1;
	}
	catch (args::ValidationError &e)
	{
		cerr << e.what() << endl;
		cerr << argParser;
		return 1;
	}

	if (versionFlag)
		printVersion();

	TinyProcessLib::Process process("yaourt -V", "", [](const char *bytes, size_t n)
		{
			cout << "YAOURT: " << string(bytes, n);
		});

	int stat;
	while (!process.try_get_exit_status(stat))
	{
	}
	cout << "YAOURT EXITED WITH CODE " << stat << endl;

	package_manager::PackageManager pkgManager{"yaourt", "yaourt",
	                                           package_manager::Action{"test", "V", package_manager::ARG}};
	package_manager::PackageManager test = package_manager::getPackageManager("yaourt");

	cout << "YAOURT " << (test.exists() ? "EXISTS" : "DOESN'T EXIST") << endl;

	std::cout << "Hello, World!" << std::endl;
	return MandarinResult::SUCCESS;
}