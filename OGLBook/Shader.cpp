#include "Shader.h"

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    //Open files
    vShaderFile.open(vertexShader);
    fShaderFile.open(fragmentShader);
    std::stringstream vShaderStream, fShaderStream;

    //std::stringstream: A stream class that operates on strings. It allows you to read from and write to a string as if it were a file or console stream.

    vShaderStream << vShaderFile.rdbuf(); //This is a common and efficient way to read the entire content of a file into a string stream.                               
    fShaderStream << fShaderFile.rdbuf(); //vShaderFile.rdbuf() : Returns a pointer to the stream buffer object associated with vShaderFile.The stream buffer is the underlying object that handles the actual character I / O.

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str(); 
    fragmentCode = fShaderStream.str();

    /*
    vShaderStream.str(): Calls the str() member function of the vShaderStream object.
    This function returns a std::string object containing a copy of all the characters currently stored in the string stream.
    */
  }
  catch (std::ifstream::failure e) {
    std::cout << "ERROR IN OPENING/READING FILE" << std::endl;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  int success;
  char infoLog[512];
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
  }

  //fragment shader

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
  }

  //link shaders
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR SHADER PROGRAM LINKIN FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use()
{
  glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
  GLint location = glGetUniformLocation(ID, name.c_str());

  if (location == -1) {
    std::cout << "WARNING: Uniform '" << name << "' not found or active in shader program " << ID << " (location was -1)!" << std::endl;
  }

  glUniform1i(location, static_cast<GLint>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
  GLint location = glGetUniformLocation(ID, name.c_str());

  if (location == -1) {
    std::cout << "WARNING: Uniform '" << name << "' not found or active in shader program " << ID << " (location was -1)!" << std::endl;
  }

  glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  GLint location = glGetUniformLocation(ID, name.c_str());

  if (location == -1) {
    std::cout << "WARNING: Uniform '" << name << "' not found or active in shader program " << ID << " (location was -1)!" << std::endl;
  }

  glUniform1f(location, value);
}

Shader::~Shader()
{
  glDeleteProgram(ID);
}
