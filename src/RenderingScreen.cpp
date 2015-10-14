#include "RenderingScreen.hpp"

#include "sfz/gl/OpenGL.hpp"

namespace rmt {

RenderingScreen::RenderingScreen() noexcept
{
	
}

UpdateOp RenderingScreen::update(UpdateState& state)
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
}

void RenderingScreen::render(UpdateState& state)
{

}

} // namespace rmt