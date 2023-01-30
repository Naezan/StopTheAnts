#pragma once

class Object
{
public:
	Object(ID2D1SolidColorBrush* brushType, std::pair<int, int> pos);
	virtual ~Object();

public:
	ID2D1SolidColorBrush* m_BrushType;
	std::pair<int, int> position;
};
