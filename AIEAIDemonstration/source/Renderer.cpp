#include "Renderer.h"
#include "Application2D.h"
#include "Transform.h"

//destructor
Renderer::~Renderer()
{
	//properly deallocate stored textures
	if (texture != nullptr)
	{
		delete texture;
	}
}


//loads a texture
void Renderer::load(char fileName[FILENAME_MAX])
{
	//properly deallocate stored textures before overwriting it with a new texture
	if (texture != nullptr)
	{
		delete texture;
	}

	texture = new aie::Texture(fileName);
}


//called once per frame during the draw call
void Renderer::update()
{
	//renderer requires a texture, region to render to and a transform
	if (texture == nullptr || box == nullptr || box->transform == nullptr)
	{
		return;
	}

	Vector2 mid = box->transform->translation;

	//create a rotation matrix
	Matrix2 rM = Matrix2();
	rM.identity();

	//set the rotation of the matrix
	rM.setRotate(box->transform->rotation);

	//get the bottom left corner of the box (after all transformations)
	Vector2 g_bl = box->min * rM + mid;

	//render the texture at the region specified by the box and it's associated transform
	appPtr->m_renderer2D->drawSprite(texture, g_bl.x, g_bl.y, (box->max.x - box->min.x) * box->transform->scale.x, (box->max.y - box->min.y) * box->transform->scale.y, box->transform->rotation, 1.0f, 0.0f, 0.0f);
}
