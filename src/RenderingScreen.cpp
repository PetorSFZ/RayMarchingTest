#include "RenderingScreen.hpp"

#include <cstdio>

#include <sfz/Math.hpp>
#include <sfz/gl/OpenGL.hpp>
#include <sfz/util/IO.hpp>

#include "GlobalConfig.hpp"

namespace rmt {

RenderingScreen::RenderingScreen() noexcept
:
	mFont{sfz::basePath() + "assets/fonts/SaniTrixieSans.ttf", 4096, 4096, 128, 1024}
{
	mProgram = gl::Program::postProcessFromFile((sfz::basePath() + "assets/shaders/shader.frag").c_str());

	mCamPos = vec3(0.0f, 1.0f, -2.0f);
	mCamDir = vec3(0.0f, 0.0f, 1.0f);
	mCamUp = vec3(0.0f, 1.0f, 0.0f);
}

UpdateOp RenderingScreen::update(UpdateState& state)
{
	GlobalConfig& cfg = GlobalConfig::INSTANCE();

	if (cfg.continuousShaderReload) mProgram.reload();

	//static size_t lastSize = 0;
	//if (cfg.continuousShaderReload && sfz::sizeofFile("assets/shaders/shader.frag") != lastSize) {
	//	mProgram.reload();
	//	lastSize = sfz::sizeofFile("assets/shaders/shader.frag");
	//}

	for (auto& event : state.events) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: return sfz::SCREEN_QUIT;
			case 'w':
			case 'W':
				mCamPos += (mCamDir * 25.0f * state.delta);
				break;
			case 's':
			case 'S':
				mCamPos -= (mCamDir * 25.0f * state.delta);
				break;
			case 'a':
			case 'A':
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			mCamPos += (-right * 25.0f * state.delta); }
			break;
			case 'd':
			case 'D':
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			mCamPos += (right * 25.0f * state.delta); }
			break;

			case 'q':
			case 'Q':
				mCamPos += (sfz::vec3{0.0f, -1.0f, 0.0} *25.0f * state.delta);
				break;
			case 'e':
			case 'E':
				mCamPos += (sfz::vec3{0.0f, 1.0f, 0.0} *25.0f * state.delta);
				break;

			case SDLK_UP:
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			sfz::mat3 xTurn = sfz::rotationMatrix3(sfz::vec3{0.0f, -1.0f, 0.0f}, 0.0f*sfz::PI()*state.delta);
			sfz::mat3 yTurn = sfz::rotationMatrix3(right, 1.0f*sfz::PI()*state.delta);
			mCamDir = (yTurn * xTurn * mCamDir);
			mCamUp = (yTurn * xTurn * mCamUp); }
			break;
			case SDLK_DOWN:
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			sfz::mat3 xTurn = sfz::rotationMatrix3(sfz::vec3{0.0f, -1.0f, 0.0f}, 0.0f*sfz::PI()*state.delta);
			sfz::mat3 yTurn = sfz::rotationMatrix3(right, -1.0f*sfz::PI()*state.delta);
			mCamDir = (yTurn * xTurn * mCamDir);
			mCamUp = (yTurn * xTurn * mCamUp); }
			break;
			case SDLK_LEFT:
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			sfz::mat3 xTurn = sfz::rotationMatrix3(sfz::vec3{0.0f, -1.0f, 0.0f}, -1.0f*sfz::PI()*state.delta);
			sfz::mat3 yTurn = sfz::rotationMatrix3(right, 0.0f*sfz::PI()*state.delta);
			mCamDir = (yTurn * xTurn * mCamDir);
			mCamUp = (yTurn * xTurn * mCamUp); }
			break;
			case SDLK_RIGHT:
			{sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			sfz::mat3 xTurn = sfz::rotationMatrix3(sfz::vec3{0.0f, -1.0f, 0.0f}, 1.0f*sfz::PI()*state.delta);
			sfz::mat3 yTurn = sfz::rotationMatrix3(right, 0.0f*sfz::PI()*state.delta);
			mCamDir = (yTurn * xTurn * mCamDir);
			mCamUp = (yTurn * xTurn * mCamUp); }
			break;
			}
			break;
		}
	}

	if (state.controllers.find(0) != state.controllers.end()) {
		const sdl::GameController& ctrl = state.controllers.at(0);

		float currentSpeed = 3.0f;
		float turningSpeed = sfz::PI();

		// Triggers
		if (ctrl.leftTrigger > ctrl.triggerDeadzone) {
			currentSpeed += (ctrl.leftTrigger * 12.0f);
		}
		else if (ctrl.rightTrigger > ctrl.triggerDeadzone) {
			currentSpeed += (ctrl.rightTrigger * 12.0f);
		}

		// Analogue Sticks
		if (sfz::length(ctrl.rightStick) > ctrl.stickDeadzone) {
			sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			sfz::mat3 xTurn = sfz::rotationMatrix3(sfz::vec3{0.0f, -1.0f, 0.0f}, ctrl.rightStick[0]*turningSpeed*state.delta);
			sfz::mat3 yTurn = sfz::rotationMatrix3(right, ctrl.rightStick[1]*turningSpeed*state.delta);
			mCamDir = (yTurn * xTurn * mCamDir);
			mCamUp = (yTurn * xTurn * mCamUp);
		}
		if (sfz::length(ctrl.leftStick) > ctrl.stickDeadzone) {
			sfz::vec3 right = sfz::normalize(sfz::cross(mCamDir, mCamUp));
			mCamPos += ((mCamDir * ctrl.leftStick[1] + right * ctrl.leftStick[0]) * currentSpeed * state.delta);
		}

		// Shoulder buttons
		if (ctrl.leftShoulder == sdl::ButtonState::DOWN || ctrl.leftShoulder == sdl::ButtonState::HELD) {
			mCamPos -= (sfz::vec3{0, 1, 0} *currentSpeed * state.delta);
		} else if (ctrl.rightShoulder == sdl::ButtonState::DOWN || ctrl.rightShoulder == sdl::ButtonState::HELD) {
			mCamPos += (sfz::vec3{0, 1, 0} *currentSpeed * state.delta);
		}

		// Menu buttons
		if (ctrl.back == sdl::ButtonState::UP) {
			return sfz::SCREEN_QUIT;
		}
	}

	sfz_assert_debug(sfz::approxEqual(sfz::dot(mCamDir, mCamUp), 0.0f, 0.1f));

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
	
	gl::setUniform(mProgram, "uCamPos", mCamPos);
	gl::setUniform(mProgram, "uCamDir", mCamDir);
	gl::setUniform(mProgram, "uCamUp", mCamUp);
	gl::setUniform(mProgram, "uCamRight", sfz::normalize(sfz::cross(mCamDir, mCamUp)));

	const float yFOV = mCamVertFOV * sfz::DEG_TO_RAD();
	vec2 camProjRect;
	camProjRect.y = 2.0f * std::tan(yFOV/2.0f);
	camProjRect.x = camProjRect.y * ((float)state.window.drawableWidth()/(float)state.window.drawableHeight());
	gl::setUniform(mProgram, "uCamProjRect", camProjRect);
	
	mPostProcessQuad.render();

	char deltaBuffer[50];
	std::snprintf(deltaBuffer, 50, "Delta: %.1fms", (state.delta*1000.0f));
	char fpsBuffer[50];
	std::snprintf(fpsBuffer, 50, "FPS: %.0f", (1.0f/state.delta));
	std::string camPosStr = "Pos: " + sfz::to_string(mCamPos);
	std::string camDirStr = "Dir: " + sfz::to_string(mCamDir);

	float fontSize = state.window.drawableHeight()/32.0f;
	float offset = fontSize*0.04f;

	mFont.verticalAlign(gl::VerticalAlign::BOTTOM);
	
	mFont.begin(state.window.drawableDimensions()/2.0f, state.window.drawableDimensions());
	mFont.write(vec2{offset, fontSize*3.15f - offset}, fontSize, camPosStr);
	mFont.write(vec2{offset, fontSize*2.1f - offset}, fontSize, camDirStr);
	mFont.write(vec2{offset, fontSize*1.05f - offset}, fontSize, deltaBuffer);
	mFont.write(vec2{offset, -offset}, fontSize, fpsBuffer);
	mFont.end(0, state.window.drawableDimensions(), sfz::vec4{0.0f, 0.0f, 0.0f, 1.0f});

	mFont.begin(state.window.drawableDimensions()/2.0f, state.window.drawableDimensions());
	mFont.write(vec2{0.0f, fontSize*3.15f}, fontSize, camPosStr);
	mFont.write(vec2{0.0f, fontSize*2.1f}, fontSize, camDirStr);
	mFont.write(vec2{0.0f, fontSize*1.05f}, fontSize, deltaBuffer);
	mFont.write(vec2{0.0f, 0.0f}, fontSize, fpsBuffer);
	mFont.end(0, state.window.drawableDimensions(), sfz::vec4{1.0f, 1.0f, 1.0f, 1.0f});
}

} // namespace rmt