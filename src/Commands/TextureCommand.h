#pragma once
#include "Command.h"

class GameObject;
class Texture;

class TextureCommand : public Command {
public:
    TextureCommand(GameObject* obj, Texture* newTex);

    void Execute() override;
    void Undo() override;

private:
    GameObject* m_Obj;
    Texture* m_NewTexture;
    Texture* m_OldTexture;
};
