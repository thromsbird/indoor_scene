#ifndef ROOM_H
#define ROOM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "shader_s.h"
using namespace std;

#ifndef PI
#define PI 3.1415926//这个不用解释了
#endif
#ifndef PI2
#define PI2 6.2831853//2PI
#endif
const int Spherem = 50;
const int Spheren = 50;

class Sphere {
private:

	int vertNum;
	unsigned int VBO,EBO;
	void setupMesh()
	{
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);		
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		

		glBindVertexArray(0);
	}
public:
	//GLuint* vboId;
	//GLuint vert, texcoord;
	vector <float> vertices;//保存顶点与法向量的指针
	vector <unsigned int> indices;
	//GLfloat* texcoords;//保存纹理坐标的指针
	unsigned int VAO;

	Sphere(int m, int n)//m是纵向细分程度,n是横向细分程度
	{
		
		float radius = 1.0f;
		// 经度方向切割100段
		const int VERTICAL_SLICE = 20;
		float vertical_step = (float)(glm::two_pi<float>() / VERTICAL_SLICE);
		// 纬度方向切割50段
		const int HORIZONTAL_SLICE = 10;
		float horizontal_step = (float)(glm::pi<float>() / HORIZONTAL_SLICE);

		unsigned int start_index = 0;
		unsigned int current_index = 0;
		// 纬度方向上将球体分割成50段，即切割成50个不同半径的同心圆
		for (size_t i = 0; i <= HORIZONTAL_SLICE; ++i)
		{
			start_index = current_index;
			float vertical_angle = horizontal_step * i;
			float z_coord = radius * std::cos(vertical_angle);
			float sub_radius = radius * std::sin(vertical_angle);
			// 经度方向将球体切割成100段
			for (size_t j = 0; j <= VERTICAL_SLICE; j++)
			{
				float horizontal_angle = vertical_step * j;
				float x_coord = sub_radius * std::cos(horizontal_angle);
				float y_coord = sub_radius * std::sin(horizontal_angle);

				// 一圈结束了，起点和重点重合
				if (j == VERTICAL_SLICE)
				{
					vertices.push_back(vertices[start_index]);
					vertices.push_back(vertices[start_index + 1]);
					vertices.push_back(vertices[start_index + 2]);
				}
				else
				{
					// 上面在计算x, y, z坐标时将Y轴和Z轴调换了
					// 因此在这里要调整坐标系为右手坐标系
					vertices.push_back(x_coord);
					vertices.push_back(z_coord);
					vertices.push_back(y_coord);
				}
				// 保存每一圈的起始点索引，方便计算每一圈的重合点
				current_index += 3;

				if (i > 0 && j > 0)
				{
					// 相邻上一圈中的顶点索引
					unsigned int bottom_ring_a = (VERTICAL_SLICE + 1)*i + j;
					unsigned int bottom_ring_b = (VERTICAL_SLICE + 1)*i + j - 1;
					// 相邻下一圈中的顶点索引
					unsigned int top_ring_a = (VERTICAL_SLICE + 1)*(i - 1) + j;
					unsigned int top_ring_b = (VERTICAL_SLICE + 1)*(i - 1) + j - 1;

					// j == 1时，相邻上一圈收缩成一个点
					if (j == 1)
					{
						indices.push_back(bottom_ring_a);
						indices.push_back(top_ring_a);
						indices.push_back(top_ring_b);
					}
					// j == HORIZONTAL_SLICE时，相邻下一圈收缩成一个点
					else if (j == HORIZONTAL_SLICE)
					{
						indices.push_back(bottom_ring_a);
						indices.push_back(top_ring_b);
						indices.push_back(bottom_ring_b);
					}
					else
					{
						// 逆时钟方向连接顶点，每次连接成一个四边形
						// 逆时钟方向避免三角形被背面剔除处理掉
						indices.push_back(bottom_ring_a);
						indices.push_back(top_ring_a);
						indices.push_back(top_ring_b);
						indices.push_back(bottom_ring_a);
						indices.push_back(top_ring_b);
						indices.push_back(bottom_ring_b);
					}
				}
			}
		}
		setupMesh();
	}
	void draw(Shader shader) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
		glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
};

#endif
