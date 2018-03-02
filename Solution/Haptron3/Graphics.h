#pragma once

#include <gl\glew.h>

#include "HaptronGame.h"
#include "Shader.h"
#include "Texture.h"
#include <map>
#include <vector>
#include <set>

namespace Haptron {
	namespace Graphics {

		struct GraphicsResourceBase {
			virtual void delete_resource() = 0;
			virtual void load_resource() = 0;
		protected:
			inline GraphicsResourceBase():resource(nullptr) { }
			inline virtual ~GraphicsResourceBase() { if (resource != nullptr) unload(); }
			void load();
			void unload();
			void * resource;
		};

		template<typename T, T*(*f)()>
		struct GraphicsResource : protected GraphicsResourceBase {
			inline virtual ~GraphicsResource() { }
			inline T& operator *() {return *get_resource();}
			inline T* operator ->() {return get_resource(); }
			inline T* read_resource() const { return (T*) resource; }

			inline void load() { this->GraphicsResourceBase::load(); }
			inline void unload() { this->GraphicsResourceBase::unload(); }

		protected:
			inline T* get_resource() {
				if (resource == nullptr) this->load();
				return (T*)resource;
			}
			inline virtual void load_resource() { resource = f(); }
			inline virtual void delete_resource() { delete (T*)(resource); resource = nullptr; }
		};

		class SceneComponent {
		protected:
			inline virtual ~SceneComponent() {};
		public:
			virtual void render() = 0;
		};

		class Scene {
			std::set<SceneComponent*> components;
		public:
			enum RenderStage {
				Primary
			};
			Scene();
			~Scene();
			void add(SceneComponent* comp, const RenderStage& r = RenderStage::Primary);
			void remove(SceneComponent* comp);
			void render();
			void clear();
		};

		struct RenderSurfaceInfo {
			double width;
			double height;
		};

		class GLRenderUnit {
			Scene scene;
			std::set<GraphicsResourceBase*> graphics_resources;
			bool gl_context_exists;
		public:
			float projection_matrix[16];
			RenderSurfaceInfo render_surface;
			GLRenderUnit();
			~GLRenderUnit();
			void init();
			void render();
			void destroy();
			inline Scene& get_scene() { return scene; }

			void add_graphics_resource(GraphicsResourceBase* gr);
			void remove_graphics_resource(GraphicsResourceBase* gr);
		};
		extern GLRenderUnit render_unit;
	}
}