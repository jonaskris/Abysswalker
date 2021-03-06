#pragma once
#include <iostream>
#include "../../mathUtils.h"
#include "../vectors/vec3f.h"
#include "../vectors/vec4f.h"

namespace abyssengine {
	namespace math {
		struct mat4f
		{
			union {
				float elements[4 * 4];
				vec4f columns[4];
			};

			mat4f() : columns{ vec4f{}, vec4f{}, vec4f{}, vec4f{} } {}
			mat4f(const vec4f& column0, const vec4f& column1, const vec4f& column2, const vec4f& column3) : columns{ column0, column1, column2, column3 } {}
			mat4f(const mat4f& other) : mat4f(other.columns[0], other.columns[1], other.columns[2], other.columns[3]) {}

			mat4f& multiply(const mat4f& other)
			{
				mat4f temp;

				for (size_t row = 0; row < 4; row++)
				{
					for (size_t column = 0; column < 4; column++)
					{
						float sum = 0.0f;
						for (size_t e = 0; e < 4; e++)
						{
							sum += elements[row + e * 4] * other.elements[e + column * 4];
						}
						temp.elements[row + column * 4] = sum;
					}
				}

				memcpy(elements, temp.elements, 4 * 4 * sizeof(float));
				return *this;
			}

			vec4f multiply(const vec4f& vec) const
			{
				vec4f returnVector;
			
				returnVector.x = vec.x * elements[0] + vec.y * elements[4] + vec.z * elements[8] + vec.w * elements[12];
				returnVector.y = vec.x * elements[1] + vec.y * elements[5] + vec.z * elements[9] + vec.w * elements[13];
				returnVector.z = vec.x * elements[2] + vec.y * elements[6] + vec.z * elements[10] + vec.w * elements[14];
				returnVector.w = vec.x * elements[3] + vec.y * elements[7] + vec.z * elements[11] + vec.w * elements[15];

				return returnVector;
			}

			mat4f decomposeToTranslation()
			{
				mat4f returnMatrix = identity();				

				returnMatrix.columns[3].x = columns[3].x;		
				returnMatrix.columns[3].y = columns[3].y;
				returnMatrix.columns[3].z = columns[3].z;

				return returnMatrix;
			}

			mat4f decomposeToScale()
			{
				mat4f returnMatrix = identity();

				returnMatrix.columns[0].x = columns[0].magnitude();
				returnMatrix.columns[1].y = columns[1].magnitude();
				returnMatrix.columns[2].z = columns[2].magnitude();

				return returnMatrix;
			}

			mat4f decomposeToRotation()
			{
				mat4f returnMatrix = identity();
				mat4f scaleMatrix = decomposeToScale();

				returnMatrix.columns[0] = columns[0] / scaleMatrix.columns[0].x;
				returnMatrix.columns[1] = columns[1] / scaleMatrix.columns[1].y;
				returnMatrix.columns[2] = columns[2] / scaleMatrix.columns[2].z;

				return returnMatrix;
			}

			vec4f operator*(const vec4f& vec) const { return multiply(vec); }

			mat4f operator*(const mat4f& other) const { return mat4f(*this).multiply(other); }
			mat4f& operator*=(const mat4f& other) { return multiply(other); }

			static mat4f identity(const float& scalar = 1.0f)
			{
				mat4f returnMatrix;

				for (size_t i = 0; i < 4; i++)
					returnMatrix.columns[i].elements[i] = scalar;

				return returnMatrix;
			}

			static mat4f translate(const vec3f& translation)
			{
				mat4f returnMatrix = identity();
			
				returnMatrix.columns[3].x = translation.x;
				returnMatrix.columns[3].y = translation.y;
				returnMatrix.columns[3].z = translation.z;
			
				return returnMatrix;
			}

			static mat4f rotate(const float& angle, const vec3f& axis)
			{
				mat4f returnMatrix = identity();
			
				float r = math::toRadians(angle);
				float c = cosf(r);
				float s = sinf(r);
				float omc = 1.0f - c;
				
				float x = axis.x;
				float y = axis.y;
				float z = axis.z;
			
				returnMatrix.columns[0].x = x * x * omc + c;
				returnMatrix.columns[0].y = x * y * omc + z * s;
				returnMatrix.columns[0].z = x * z * omc - y * s;
				returnMatrix.columns[0].w = 0.0f;
			
				returnMatrix.columns[1].x = x * y * omc - z * s;
				returnMatrix.columns[1].y = y * y * omc + c;
				returnMatrix.columns[1].z = y * z * omc + x * s;
				returnMatrix.columns[1].w = 0.0f;
			
				returnMatrix.columns[2].x = x * z * omc + y * s;
				returnMatrix.columns[2].y = y * z * omc - x * s;
				returnMatrix.columns[2].z = z * z * omc + c;
				returnMatrix.columns[2].w = 0.0f;
			
				returnMatrix.columns[3].x = 0.0f;
				returnMatrix.columns[3].y = 0.0f;
				returnMatrix.columns[3].z = 0.0f;
				returnMatrix.columns[3].w = 1.0f;
			
				return returnMatrix;
			}

			static mat4f scale(const vec3f& factor)
			{
				mat4f returnMatrix = identity();
			
				returnMatrix.columns[0].x = factor.x;
				returnMatrix.columns[1].y = factor.y;
				returnMatrix.columns[2].z = factor.z;
			
				return returnMatrix;
			}

			static mat4f perspective(float fov, float aspectRatio, float near, float far)
			{
				mat4f returnMatrix = mat4f();
			
				fov = math::toRadians(fov);
			
				returnMatrix.columns[0].x = 1 / (aspectRatio * tanf(fov / 2));
				returnMatrix.columns[0].y = 0;
				returnMatrix.columns[0].z = 0;
				returnMatrix.columns[0].w = 0;
			
				returnMatrix.columns[1].x = 0;
				returnMatrix.columns[1].y = 1 / (tanf(fov / 2));
				returnMatrix.columns[1].z = 0;
				returnMatrix.columns[1].w = 0;
			
				returnMatrix.columns[2].x = 0;
				returnMatrix.columns[2].y = 0;
				returnMatrix.columns[2].z = -(far + near) / (far - near);
				returnMatrix.columns[2].w = -1;
			
				returnMatrix.columns[3].x = 0;
				returnMatrix.columns[3].y = 0;
				returnMatrix.columns[3].z = -(2 * far * near) / (far - near);
				returnMatrix.columns[3].w = 0;
			
				return returnMatrix;
			}

			static mat4f orthographic(float left, float right, float bottom, float top, float near, float far)
			{
				mat4f returnMatrix = identity();

				returnMatrix.elements[0 + 0 * 4] = 2.0f / (right - left);
				returnMatrix.elements[1 + 1 * 4] = 2.0f / (top - bottom);
				returnMatrix.elements[2 + 2 * 4] = 2.0f / (near - far);

				returnMatrix.elements[0 + 3 * 4] = (left + right) / (left - right);
				returnMatrix.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
				returnMatrix.elements[2 + 3 * 4] = (far + near) / (far - near);

				return returnMatrix;
			}

			static mat4f viewMatrix(const vec3f& cameraPos, const vec3f& lookAtPos, const vec3f& up)
			{
				//mat4f returnMatrix = identity();

				vec3f zaxis = (cameraPos - lookAtPos).normalize();
				vec3f xaxis = (up.cross(zaxis)).normalize();
				vec3f yaxis = zaxis.cross(xaxis);


				mat4f orientation = math::mat4f::identity();
				orientation.columns[0] = math::vec4f(xaxis.x, yaxis.x, zaxis.x, 0.0f);
				orientation.columns[1] = math::vec4f(xaxis.y, yaxis.y, zaxis.y, 0.0f);
				orientation.columns[2] = math::vec4f(xaxis.z, yaxis.z, zaxis.z, 0.0f);
				orientation.columns[3] = math::vec4f(0.0f, 0.0f, 0.0f, 1.0f);


				mat4f translation = math::mat4f::identity();
				translation.columns[0] = math::vec4f(1.0f, 0.0, 0.0f, 0.0f);
				translation.columns[1] = math::vec4f(0.0f, 1.0f, 0.0f, 0.0f);
				translation.columns[2] = math::vec4f(0.0f, 0.0f, 1.0f, 0.0f);
				translation.columns[3] = math::vec4f(-cameraPos.x, -cameraPos.y, -cameraPos.z, 1.0f);
				//vec3f f = (lookAtPos - cameraPos).normalize();
				//vec3f u = up.normalize();
				//vec3f s = f.cross(u).normalize();
				//u = s.cross(f);
				//
				//returnMatrix.elements[0 + 0 * 4] = s.x;
				//returnMatrix.elements[0 + 1 * 4] = s.y;
				//returnMatrix.elements[0 + 2 * 4] = s.z;
				//returnMatrix.elements[1 + 0 * 4] = u.x;
				//returnMatrix.elements[1 + 1 * 4] = u.y;
				//returnMatrix.elements[1 + 2 * 4] = u.z;
				//returnMatrix.elements[2 + 0 * 4] = -f.x;
				//returnMatrix.elements[2 + 1 * 4] = -f.y;
				//returnMatrix.elements[2 + 2 * 4] = -f.z;
				//returnMatrix.elements[0 + 3 * 4] = -s.dot(cameraPos);
				//returnMatrix.elements[1 + 3 * 4] = -u.dot(cameraPos);
				//returnMatrix.elements[2 + 3 * 4] = f.dot(cameraPos);
				


				return orientation * translation;
			}
		};
	}
}