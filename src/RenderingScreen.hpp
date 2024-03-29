#pragma once
#ifndef RMT_RENDERING_SCREEN_HPP
#define RMT_RENDERING_SCREEN_HPP

#include <sfz/Math.hpp>
#include <sfz/Screens.hpp>

#include <sfz/gl/FontRenderer.hpp>
#include <sfz/gl/PostProcessQuad.hpp>
#include <sfz/gl/Program.hpp>

namespace rmt {

using sfz::UpdateOp;
using sfz::UpdateState;

using sfz::vec2;
using sfz::vec3;
using sfz::vec4;
using sfz::mat3;
using sfz::mat4;

class RenderingScreen : public sfz::BaseScreen {
public:
	// Constructors & destructors
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	RenderingScreen() noexcept;
	virtual ~RenderingScreen() = default;

	// Overriden screen methods
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	virtual UpdateOp update(UpdateState& state) override final;
	virtual void render(UpdateState& state) override final;

private:
	gl::Program mProgram;
	gl::PostProcessQuad mPostProcessQuad;
	gl::FontRenderer mFont;

	bool mIntensityRendering = false;
	float mTime = 0.0f;

	vec3 mCamPos, mCamDir, mCamUp;
	float mCamVertFOV = 60.0f;
};

} // namespace rmt
#endif