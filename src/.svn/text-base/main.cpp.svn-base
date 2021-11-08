/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2009 Andrey Stroganov <savthe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <signal.h>

#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>

#include "debug.hpp"
#include "appdefs.hpp"
#include "configurer.hpp"
#include "filesystem.hpp"
#include "logger.hpp"
#include "streamlog.hpp"
#include "application.hpp"

void onShutdown(int n)
{
	Application::inst().shutdown();
}

void initSignalHandler()
{
	struct sigaction sa;

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGHUP);
	sigprocmask(SIG_BLOCK, &sigset, 0);
	sa.sa_handler = onShutdown;

	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
}

int main(int argc, char *argv[])
{
	int verbLevel = 1;

	bool daemon = false;
	bool configure = false;

	for (int i = 1; i < argc; ++i) 
	{
		if(std::strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) 
		{
			std::cout << "List of options:" << std::endl;
			std::cout << "-h, --help           This help list" << std::endl;
			std::cout << "-d, --daemon         Daemon mode (work in background)" << std::endl;
			std::cout << "-c, --configure      Configure Sharelin and exit" << std::endl;
			std::cout << "-v n, --verbose n    Set verbose level to n" << std::endl;
			return 0;
			
		} 
		else if(std::strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--daemon") == 0 ) 
		{
			daemon = true;
			verbLevel = 0;
		}
		else if(std::strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--configure") == 0 ) 
			configure = true;

		if(std::strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0 ) 
		{
			if( i == argc - 1 ) 
			{
				std::cout << "Use -v n, where n -- verbose level";
				return 1;
			}
			verbLevel = std::atoi(argv[++i]);
		}
	}

	FileSystem::PrepareFolder(app::home());

	if(configure) return Configurer::Run();

	if(daemon && fork() > 0) return 0;

	std::srand(time(0));

	Logger::Instance()->Init(app::logFile(), 1024 * 1024, verbLevel);

	initSignalHandler();

	Application::inst().run();

	Logger::Instance()->Shutdown();

	return 0;
}


