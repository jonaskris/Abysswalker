#pragma once
#include <GL/glew.h>
#include <vector>

namespace abyssengine {
	class Buffer;
	class VertexArray
	{
	private:
		GLuint arrayID;
		std::vector<Buffer*> buffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};
}