#pragma once
#ifndef RMT_RENDERING_SCREEN_HPP
#define RMT_RENDERING_SCREEN_HPP

#include <sfz/Math.hpp>
#include <sfz/Screens.hpp>

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
};

} // namespace rmt
#endif