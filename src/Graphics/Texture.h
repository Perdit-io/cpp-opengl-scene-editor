#pragma once
#include <string>

class Texture {
public:
    Texture(const std::string& path, const std::string& type);
    ~Texture();

    void Bind(unsigned int unit = 0) const;

    unsigned int GetID() const { return m_ID; }
    std::string GetType() const { return m_Type; }
    std::string GetPath() const { return m_Path; }

private:
    unsigned int m_ID;
    std::string m_Type;
    std::string m_Path;
};
