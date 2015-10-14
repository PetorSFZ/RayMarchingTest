#include "RenderingScreen.hpp"

#include <sfz/gl/OpenGL.hpp>
#include <sfz/util/IO.hpp>

#include "GlobalConfig.hpp"

namespace rmt {

RenderingScreen::RenderingScreen() noexcept
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


}

} // namespace rmt