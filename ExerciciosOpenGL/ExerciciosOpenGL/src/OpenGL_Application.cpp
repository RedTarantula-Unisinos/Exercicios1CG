#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
CG Class
Universidade do Rio dos Sinos - Porto Alegre, RS. Brazil
Professor Rossana Queiroz
---
João Pedro Rothmann
---
Following TheChernoProject's Youtube series as support
https://www.youtube.com/watch?v=W3gAzLwfIP0&index=1&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/

std::string shader_file = "res/vs_fs.shader";


static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] " << "{{ " << error << " }}" << std::endl;
	}
}

struct ShaderProgram
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgram ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	int numLines = 0;
	while (getline(stream, line))
	{
		numLines++;
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	std::cout << "Lines: " << numLines << std::endl;
	return{ ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */

	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error! GLEW is not okay." << std::endl;

	/* Print the version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	float vPosit[] = {
		-0.5f, -0.5f, 0.0f,  // left 
		0.5f, -0.5f, 0.0f,  // right
		0.0f, 0.5f, 0.0f,  // top 
	};
	float vCol[] = {
		1.0f, 0.0f, 0.0f,  // left 
		0.0f, 1.0f, 0.0f,  // right
		0.0f, 0.0f, 1.0f,  // top 
	};

	unsigned int VAO;
	unsigned int VBOp;
	unsigned int VBOc;


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOp);
	glBindBuffer(GL_ARRAY_BUFFER, VBOp);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vPosit, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBOc);
	glBindBuffer(GL_ARRAY_BUFFER, VBOc);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vCol, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	ShaderProgram source = ParseShader(shader_file);
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shader);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//GLCheckError();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOp);
	glDeleteBuffers(1, &VBOc);
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}