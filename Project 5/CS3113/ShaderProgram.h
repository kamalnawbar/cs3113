#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "cs3113.h"

class ShaderProgram
{
private:
    Shader mShader;
    bool   mIsLoaded;

public:
    static constexpr int NOT_LOADED = -1;

    ShaderProgram();
    ~ShaderProgram();

    bool load(const std::string &vertexPath, const std::string &fragmentPath);
    void unload();

    void begin();
    void end();

    void setVector2(const std::string &name, const Vector2 &value);

    bool isLoaded() const { return mIsLoaded; }
};

#endif