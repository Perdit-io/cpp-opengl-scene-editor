#include "TextureCommand.h"
#include "GameObject.h"

TextureCommand::TextureCommand(GameObject* obj, Texture* newTex)
    : m_Obj(obj), m_NewTexture(newTex) {
    m_OldTexture = obj->texture;
}

void TextureCommand::Execute() {
    m_Obj->texture = m_NewTexture;
}

void TextureCommand::Undo() {
    m_Obj->texture = m_OldTexture;
}
