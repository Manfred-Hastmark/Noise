#pragma once
class Particle
{
		
public:
	//Position
	int x, y;
	int prevX, prevY;

	Particle() : x(0), y(0), prevX(0), prevY(0), m_width(0), m_height(0), m_vx(0), m_vy(0), m_ax(0), m_ay(0), m_maxVel(0) {}

	Particle(float width, float height, float maxVel);

	//Add a force to the particle, angle measured in radians
	void addForce(float force, float angle);

	//Moves the particle
	void update();

private:

	//Width and height of draw-window
	int m_width, m_height;

	//Velocity
	float m_vx, m_vy, m_maxVel;

	//Acceleration
	float m_ax, m_ay;	
};

