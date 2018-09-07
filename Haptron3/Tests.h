#pragma once

#include "Graphics.h"
#include "HaptronLevel.h"

namespace Haptron
{
	class LSDTile : public Graphics::SceneComponent {
		static GLUtils::Shader* load_shader();
		static Graphics::GraphicsResource<GLUtils::Shader, LSDTile::load_shader> shader;
		GLuint vao, vbo, ibo, tbo;
		void create_vao();
	public:
		LSDTile();
		~LSDTile();
		void render();
		void init();
		void dispose();
	};
}