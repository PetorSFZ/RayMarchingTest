#include <iostream>
#include <memory>
#include <string>

#include <sfz/Assert.hpp>
#include <sfz/GL.hpp>
#include <sfz/gl/OpenGL.hpp>
#include <sfz/Math.hpp>
#include <sfz/Screens.hpp>
#include <sfz/SDL.hpp>

#include "GlobalConfig.hpp"
#include "RenderingScreen.hpp"

#undef main // Remove SDL hack until we can get it to compile properly

int main()
{
	using namespace sdl;
	using std::string;

	rmt::GlobalConfig& cfg = rmt::GlobalConfig::INSTANCE();
	cfg.save();

	Session sdlSession{{InitFlags::EVENTS, InitFlags::VIDEO, InitFlags::GAMECONTROLLER}};

	// Make sure selected display index is valid
	const int numDisplays = SDL_GetNumVideoDisplays();
	if (numDisplays < 0) std::cerr << "SDL_GetNumVideoDisplays() failed: " << SDL_GetError() << std::endl;
	if (cfg.displayIndex >= numDisplays) {
		std::cerr << "Display index " << cfg.displayIndex << " is invalid, number of displays is "
		          << numDisplays << ". Resetting to 0." << std::endl;
		cfg.displayIndex = 0;
		cfg.save();
	}

	// Must be set before creating window apparently
	if (cfg.msaa > 0) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, cfg.msaa);
	}

	Window window{"Ray Marching Test", cfg.windowWidth, cfg.windowHeight,
	     {WindowFlags::OPENGL, WindowFlags::RESIZABLE, WindowFlags::ALLOW_HIGHDPI}};

	gl::Context glContext{window.mPtr, 3, 3, gl::GLContextProfile::CORE};

	// Sets correct displaymode
	SDL_DisplayMode cfgDataMode;
	cfgDataMode.w = cfg.resolutionX;
	cfgDataMode.h = cfg.resolutionY;
	cfgDataMode.format = 0;
	cfgDataMode.refresh_rate = cfg.refreshRate;
	cfgDataMode.driverdata = 0;
	SDL_DisplayMode closest;
	if (SDL_GetClosestDisplayMode(cfg.displayIndex, &cfgDataMode, &closest) == NULL) {
		std::cerr << "SDL_GetClosestDisplayMode() failed: " << SDL_GetError() << std::endl;
	}
	if (SDL_SetWindowDisplayMode(window.mPtr, &closest) < 0) {
		std::cerr << "SDL_SetWindowDisplayMode() failed: " << SDL_GetError() << std::endl;
	}

	// Initializes GLEW, must happen after GL context is created.
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		sfz_error(string{"GLEW init failure: "} + ((const char*)glewGetErrorString(glewError)) + "\n");
	}
	if (gl::checkAllGLErrors()) std::cerr << "^^^ Above errors caused by glewInit()." << std::endl;

	// VSync
	int vsyncInterval = 1;
	if (cfg.vsync == 0) vsyncInterval = 0;
	else if (cfg.vsync == 1) vsyncInterval = 1;
	else if (cfg.vsync == 2) vsyncInterval = -1;
	if (SDL_GL_SetSwapInterval(vsyncInterval) < 0) {
		std::cerr << "SDL_GL_SetSwapInterval() failed: " << SDL_GetError() << std::endl;
	}

	// Fullscreen
	int fullscreenFlags = 0;
	if (cfg.fullscreenMode == 0) fullscreenFlags = 0;
	else if (cfg.fullscreenMode == 1) fullscreenFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	else if (cfg.fullscreenMode == 2) fullscreenFlags = SDL_WINDOW_FULLSCREEN;
	if (SDL_SetWindowFullscreen(window.mPtr, fullscreenFlags) < 0) {
		std::cerr << "SDL_SetWindowFullscreen() failed: " << SDL_GetError() << std::endl;
	}

	sfz::runGameLoop(window, std::shared_ptr<sfz::BaseScreen>{new rmt::RenderingScreen{}});

	return 0;
}