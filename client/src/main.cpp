#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdint>
#include <memory>
#include <WIZ/logging/MultiLogger.h>
#include <WIZ/logging/StdOutLogger.h>
#include <WIZ/logging/DateTimeLoggerWrapper.h>
#include <WIZ/logging/TagLoggerWrapper.h>
#include <WIZ/asset/AssetLoader.h>
#include <WIZ/asset/MusicAsset.h>

#include "HelloSFMLScreen.h"
#include "ExampleGame.h"

#include <WIZ/logging/DailyFileLogger.h>

#ifdef OS_SWITCH
	#include <switch.h>
#endif

int main(int argc, char* argv[])
{
#ifdef OS_SWITCH
	// Setup NXLink
	socketInitializeDefault();
	nxlinkStdio();

	Result rc = romfsInit();
#endif

    std::shared_ptr<wiz::Logger> logger = std::unique_ptr<wiz::Logger>(new wiz::TagLoggerWrapper(
			new wiz::DateTimeLoggerWrapper(
                new wiz::MultiLogger({
                    new wiz::DailyFileLogger("logs/", wiz::LogLevel::DEBUG),
                    new wiz::StdOutLogger(wiz::LogLevel::DEBUG)
                }), "[%H:%M:%S]")));

	std::shared_ptr<wiz::AssetLoader> loader = std::make_shared<wiz::AssetLoader>(*logger);

    /*

    The following code works but isn't fully implemented nor tested on switch

    MusicAsset greenlife(asset("greenlife.ogg"));
    MusicAsset jump(asset("jump.ogg"));

    loader->load(greenlife);
    loader->load(jump);

    while(loader->getProgress() < 1.0f)
        loader->update(std::chrono::milliseconds(1000));

    logger->info("Done loading");
     */

#ifdef OS_SWITCH
    logger->info("OS is Switch");
#endif

#ifdef OS_WINDOWS
    logger->info("OS is Windows");
#endif

#ifdef OS_MAC
    logger->info("OS is Mac");
#endif

#ifdef OS_UNIX
    logger->info("OS is Unix");
#endif


	sf::VideoMode mode;

#ifdef OS_SWITCH
	mode = sf::VideoMode::getDesktopMode();
#else
	mode = sf::VideoMode(640, 480, 32);
#endif

	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(mode, "SFML Game Template Project");

	ExampleGame game(window, logger, loader);

	game.setScreen(new HelloSFMLScreen(game));

	while (window->isOpen()) {
		game.update();
	}

#ifdef OS_SWITCH
	socketExit();
#endif
	return EXIT_SUCCESS;
}
