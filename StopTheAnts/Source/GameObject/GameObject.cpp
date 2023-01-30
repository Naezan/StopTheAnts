#include "../pch.h"
#include "GameObject.h"

Object::Object(ID2D1SolidColorBrush* brushType, std::pair<int, int> pos) : m_BrushType(brushType), position(pos)
{
}

Object::~Object()
{
}
