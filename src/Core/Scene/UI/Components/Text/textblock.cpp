#include "textblock.h"

TextBlock::TextBlock()
{
	Font font(ResourceManager::GetPath("/Fonts/VeraMono.ttf").c_str(), 400);
	textDrawable = new Text(font);
}

void TextBlock::update()
{
}

void TextBlock::setAction(int action)
{
}

Drawable * TextBlock::getDrawable()
{
	return static_cast<Drawable*>(textDrawable);
}
