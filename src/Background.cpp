#include "Background.h"

Background::Background(Core* core_) :
	m_core(core_)
{
	m_back = m_core->textureManager->getTexture(TEXTURES::Background1);

	//Calculate real size
	m_sizeX = m_core->targetResolution.x;
	scaling = m_sizeX / m_back->w;
	m_sizeY = m_back->h * scaling;

	//Calculate starting pos
	m_pos[0] = m_core->targetResolution.y - m_sizeY; //Current texture
	m_pos[1] = m_pos[0] - m_sizeY; //Upcoming texture
}

void Background::update(float frameTime_)
{
	//Move both positions down
	m_pos[0] += m_spd * frameTime_;
	m_pos[1] += m_spd * frameTime_;

	//Check if one is outfield
	if (m_pos[1] + m_sizeY >= m_core->targetResolution.y)
	{
		//Update next texture pos
		m_pos[0] = m_pos[1];
		m_pos[1] = m_pos[0] - m_sizeY;
	}
}

void Background::draw()
{
	m_core->renderTexture(m_back->getSprite(), 0, m_pos[0], m_sizeX, m_sizeY);
	m_core->renderTexture(m_back->getSprite(), 0, m_pos[1], m_sizeX, m_sizeY);
}
