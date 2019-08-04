#include "pch.h"
#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
	G_DestroyTexture(texture);
	texture = NULL;
}
Button::~Button()
{
	clickSound = NULL;
}