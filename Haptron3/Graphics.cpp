
#include "Graphics.h"
#include <iostream>

using namespace std;

namespace Haptron {
	namespace Graphics {
		GLRenderUnit render_unit;
	}
}

/*
GraphicsResource definitions
*/
namespace Haptron {
	namespace Graphics {

		void GraphicsResourceBase::load()
		{
			render_unit.add_graphics_resource(this);
		}

		void GraphicsResourceBase::unload()
		{
			render_unit.remove_graphics_resource(this);
		}

	}
}

/*
Scene definitions
*/

namespace Haptron {
	namespace Graphics {
		Scene::Scene() {
			init = false;
		}

		Scene::~Scene() {

		}

		void Scene::add(SceneComponent * comp, const RenderStage & r)
		{
			if (init) comp->init();
			this->components.insert(comp);
		}

		void Scene::remove(SceneComponent * comp)
		{
			this->components.erase(comp);
		}

		void Scene::render() {
			if (!init) {
				for (auto& a : components)
				{
					a->init();
					a->render();
				}
				init = true;
			}
			else for (auto& a : components)
					a->render();
		}

		void Scene::clear()
		{
			if(init)
				for (auto& a : components)
					a->dispose();
			components.clear();
			init = false;
		}
	}
}

/*
GLRenderUnit definitions
*/
namespace Haptron {
	namespace Graphics {

		void GLRenderUnit::add_graphics_resource(GraphicsResourceBase * gr)
		{
			if (!gl_context_exists)
				throw std::exception("No OpenGL context exists!");

			this->graphics_resources.insert(gr);
			gr->load_resource();
		}

		void GLRenderUnit::remove_graphics_resource(GraphicsResourceBase * gr)
		{
			this->graphics_resources.erase(gr);
			gr->delete_resource();
		}

		GLRenderUnit::GLRenderUnit(): gl_context_exists(false)
		{
			
		}

		GLRenderUnit::~GLRenderUnit()
		{
			if (gl_context_exists)
				destroy();
		}

		void GLRenderUnit::init()
		{
			gl_context_exists = true;
			for (auto& g : graphics_resources)
			{
				g->load_resource();
			}
			int maj, min;
			glGetIntegerv(GL_MAJOR_VERSION, &maj);
			glGetIntegerv(GL_MINOR_VERSION, &min);
		}

		void GLRenderUnit::destroy()
		{
			for (auto& r : graphics_resources)
				r->delete_resource();
			graphics_resources.clear();
			gl_context_exists = false;
		}

		void GLRenderUnit::render() {
			double width = render_unit.render_surface.width;
			double height = render_unit.render_surface.height;
			double aspect = width / height;
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glClearColor(0, 0, 0, 1);
			glEnable(GL_DEPTH_TEST);
			glLoadIdentity();
			float* pm = projection_matrix;
			if (aspect > 1) {
				memset(pm, 0, 16 * sizeof(float));
				pm[0] = 2/aspect;
				pm[5] = 2;
				pm[10] = 1;
				pm[15] = 1;
			}
			else {
				memset(pm, 0, 16 * sizeof(float));
				pm[0] = 2;
				pm[5] = 2*aspect;
				pm[10] = 1;
				pm[15] = 1;
			}
			glMatrixMode(GL_MODELVIEW);
			scene.render();
		}
	}
}