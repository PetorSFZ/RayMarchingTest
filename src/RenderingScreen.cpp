#include "RenderingScreen.hpp"

#include <cstdio>

#include <sfz/gl/OpenGL.hpp>
#include <sfz/util/IO.hpp>

#include "GlobalConfig.hpp"

namespace rmt {

RenderingScreen::RenderingScreen() noexcept
:
	mFont{sfz::basePath() + "assets/fonts/SaniTrixieSans.ttf", 4096, 4096, 128, 1024}
{
	mProgram = gl::Program::postProcessFromFile((sfz::basePath() + "assets/shaders/shader.frag").c_str());
}

UpdateOp RenderingScreen::update(UpdateState& state)
{
	GlobalConfig& cfg = GlobalConfig::INSTANCE();

	if (cfg.continuousShaderReload) mProgram.reload();

	return sfz::SCREEN_NO_OP;
}

void RenderingScreen::render(UpdateState& state)
{
	// Enable depth testing
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable culling
	glEnable(GL_CULL_FACE);

	// Clearing screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render post process shader
	glUseProgram(mProgram.handle());
	glViewport(0, 0, state.window.drawableWidth(), state.window.drawableHeight());
	mPostProcessQuad.render();


	mFont.verticalAlign(gl::VerticalAlign::BOTTOM);

	char deltaBuffer[100];
	std::snprintf(deltaBuffer, 100, "Delta: %.1fms", (state.delta*1000.0f));
	char fpsBuffer[100];
	std::snprintf(fpsBuffer, 100, "FPS: %.0f", (1.0f/state.delta));

	float fontSize = state.window.drawableHeight()/24.0f;
	float offset = fontSize*0.04f;

	mFont.begin(state.window.drawableDimensions()/2.0f, state.window.drawableDimensions());
	mFont.write(vec2{offset, fontSize*1.05f - offset}, fontSize, deltaBuffer);
	mFont.write(vec2{offset, -offset}, fontSize, fpsBuffer);
	mFont.end(0, state.window.drawableDimensions(), sfz::vec4{0.0f, 0.0f, 0.0f, 1.0f});

	mFont.begin(state.window.drawableDimensions()/2.0f, state.window.drawableDimensions());
	mFont.write(vec2{0.0f, fontSize*1.05f}, fontSize, deltaBuffer);
	mFont.write(vec2{0.0f, 0.0f}, fontSize, fpsBuffer);
	mFont.end(0, state.window.drawableDimensions(), sfz::vec4{1.0f, 1.0f, 1.0f, 1.0f});
}

} // namespace rmt