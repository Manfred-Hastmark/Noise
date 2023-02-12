#include "Particle.h"

#include <cmath>

Particle::Particle(float width, float height, float maxVel) : m_ax(0), m_ay(0), m_vx(0), m_vy(0)
{
	m_maxVel = maxVel;
	m_width = width;
	m_height = height;

	//Randomize the startposition
	x = ((float)std::rand()) / ((float)(RAND_MAX / width));
	y = ((float)std::rand()) / ((float)(RAND_MAX / height));
}

void Particle::addForce(float force, float angle)
{
	m_ax = force * cos(angle);
	m_ay = force * sin(angle);
}

void Particle::update()
{
	//Calculate preliminary speed
	float vx = m_vx + m_ax;
	float vy = m_vy + m_ay;

	//Get magnitude of velocity
	float v = sqrt(vx * vx + vy * vy);

	//Make sure that the velocity isn't greater than maxVel
	m_vx = (v < m_maxVel) ? vx : vx * (m_maxVel / v);
	m_vy = (v < m_maxVel) ? vy : vy * (m_maxVel / v);

	prevX = x;
	prevY = y;

	//Update position
	if (m_vx + x < 0)
	{
		x = m_width;
		prevX = x;
	}
	else if (m_vx + x > m_width)
	{
		x = 0;
		prevX = x;
	}
	else
		x += m_vx;


	if (m_vy + y < 0)
	{
		y = m_height;
		prevY = y;
	}
	else if (m_vy + y > m_height)
	{
		y =0;
		prevY = y;
	}
	else
		y += m_vy;
}