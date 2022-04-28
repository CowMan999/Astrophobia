#define DEF_HP 100

class bTurret : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	sf::Clock atk_clock3;

public:
	bTurret()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(turret_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(turret_t.getSize().x/2, turret_t.getSize().y/2);
		m_spr.setPosition(0,-430);
		m_spr.setColor(inv_game_color);
		m_spr.setRotation(90);
	}

	void update()
	{
		// rotation time - V3
		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();
		m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  2.5*deltatime));


		// attacks
		if(hp < DEF_HP && hp > 30)
		{
			if(atk_clock.getElapsedTime() > sf::seconds(2))
			{
				enemies.push_back(new Missile(sf::Vector2f(0,-430), m_spr.getRotation()));
				enemy_types.push_back(et_Missile);
				atk_clock.restart();
			}

			if(atk_clock2.getElapsedTime() > sf::seconds(6))
			{
				enemies.push_back(new Bball(sf::Vector2f(0,-430)));
				enemy_types.push_back(et_Bball);
				atk_clock2.restart();
			}

		}
		else if(hp > DEF_HP) // buff for later rounds
		{
			if(atk_clock.getElapsedTime() > sf::seconds(1.25))
			{
				enemies.push_back(new Bball(sf::Vector2f(0,-430)));
				enemy_types.push_back(et_Bball);
				atk_clock.restart();
			}
		}
		else // last ditch attempt to kill the player
		{
			if(atk_clock3.getElapsedTime() > sf::seconds(2.5))
			{
				enemies.push_back(new Missile(sf::Vector2f(400,-600), m_spr.getRotation()));
				enemies.push_back(new Missile(sf::Vector2f(-400,-600), m_spr.getRotation()));
				enemy_types.push_back(et_Missile);
				enemy_types.push_back(et_Missile);
				atk_clock3.restart();
			}
		}

		if(atk_clock3.getElapsedTime() > sf::seconds(6))
		{
			enemies.push_back(new Missile(sf::Vector2f(400,-600), m_spr.getRotation()));
			enemies.push_back(new Missile(sf::Vector2f(-400,-600), m_spr.getRotation()));
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			atk_clock3.restart();
		}

	}
};

class bSpaceship : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	int speed_m = 200;

public:
	bSpaceship()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bspaceship_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(bspaceship_t.getSize().x/2, bspaceship_t.getSize().y/2);
		m_spr.setPosition(0,-430);
		m_spr.setColor(inv_game_color);
		m_spr.setRotation(90);
	}

	void update()
	{
		// rotation time - V4 - ULTIMATE EDITION
		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();
		
		if(hp < 31)
			m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  10*deltatime));
		else
			m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  1.75*deltatime));

		int angle = m_spr.getRotation();
		
		if(hp < 31)
			m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)speed_m * 0.35F * deltatime);
		else
			m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)speed_m * deltatime);

		// attacks

		if(atk_clock.getElapsedTime() > sf::seconds(0.15) && hp > 30 && hp < DEF_HP)
		{
			enemies.push_back(new Pellet(m_spr.getPosition(), m_spr.getRotation()));
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}

		if(atk_clock.getElapsedTime() > sf::seconds(3.05) && hp > DEF_HP)
		{
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+90));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+180));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+270));
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			atk_clock.restart();
		}

		if(atk_clock2.getElapsedTime() > sf::seconds(3.5 + static_cast<float>(hp)/50) && hp > 30)
		{
			enemies.push_back(new Bball(m_spr.getPosition()));
			enemy_types.push_back(et_Bball);
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()));
			enemy_types.push_back(et_Missile);
			atk_clock2.restart();
		}

		// final phase 
		if(atk_clock.getElapsedTime() > sf::seconds(0.125) && hp < 31)
		{
			sf::Vector2f gpos = player.getPosition();
			sf::Vector2f mpos = m_spr.getPosition();
			enemies.push_back(new Pellet(m_spr.getPosition(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y) + (rand()%11) - 5));
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}

	}
};

class bSpintower : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	sf::Clock atk_clock3;
	int spinspeed = 360;

public:
	bSpintower()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bspintower_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(bspintower_t.getSize().x/2, bspintower_t.getSize().y/2);
		m_spr.setPosition(0,0);
		m_spr.setColor(inv_game_color);
	}

	void update()
	{
		// rotation time - easy mode
		m_spr.rotate(spinspeed*deltatime);


		// attacks
		if(hp < DEF_HP && hp > 30)
		{
			if(atk_clock.getElapsedTime() > sf::seconds(0.105 + static_cast<double>(rand()%10) / 80 ))
			{
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()));
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()+180));
				enemy_types.push_back(et_Pellet);
				enemy_types.push_back(et_Pellet);
				atk_clock.restart();
			}

		}
		else if(hp > DEF_HP) // buff for later rounds
		{
			if(atk_clock.getElapsedTime() > sf::seconds(3))
			{
				enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()));
				enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+90));
				enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+180));
				enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+270));
				enemy_types.push_back(et_Missile);
				enemy_types.push_back(et_Missile);
				enemy_types.push_back(et_Missile);
				enemy_types.push_back(et_Missile);
				atk_clock.restart();
			}
		}
		else // last ditch attempt to kill the player
		{
			sf::Vector2f gpos = player.getPosition();
			sf::Vector2f mpos = m_spr.getPosition();
			if(atk_clock.getElapsedTime() > sf::seconds(0.08 + static_cast<double>(rand()%10) / 80 ))
			{
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()));
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()+90));
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()+180));
				enemies.push_back(new Pellet(sf::Vector2f(0,0), m_spr.getRotation()+270));
				enemy_types.push_back(et_Pellet);
				enemy_types.push_back(et_Pellet);
				enemy_types.push_back(et_Pellet);
				enemy_types.push_back(et_Pellet);
				atk_clock.restart();
			}
		}

		if(atk_clock2.getElapsedTime() > sf::seconds(6.5) && hp < DEF_HP)
		{
			enemies.push_back(new Bball(sf::Vector2f(0,0)));
			enemy_types.push_back(et_Bball);
			atk_clock2.restart();
		}

		if(atk_clock3.getElapsedTime() > sf::seconds(9) && hp < DEF_HP)
		{
			enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()));
			enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+90));
			enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+180));
			enemies.push_back(new Missile(sf::Vector2f(0,0), m_spr.getRotation()+270));
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			atk_clock3.restart();
		}

	}
};

class bVship : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	int speed_m = 250;

public:
	bVship()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bvship_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(bvship_t.getSize().x/2, bvship_t.getSize().y/2);
		m_spr.setPosition(0,-430);
		m_spr.setColor(inv_game_color);
		m_spr.setRotation(90);
	}

	void update()
	{
		// rotation time - V4 - ULTIMATE EDITION
		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();
		
		if(hp < 31)
			m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  1.45*deltatime));
		else
			m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  1.75*deltatime));

		int angle = m_spr.getRotation();
		
		if(hp < 31)
			m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)speed_m * 3.5F * deltatime);
		else
			m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)speed_m * deltatime);

		// attacks

		if(atk_clock.getElapsedTime() > sf::seconds(0.1) && hp < DEF_HP && hp > 31)
		{
			enemies.push_back(new Pellet(m_spr.getPosition(), m_spr.getRotation() + (rand()%(hp)/2) - hp/4));
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}
		else if(atk_clock.getElapsedTime() > sf::seconds(0.75) && hp < DEF_HP)
		{
			enemies.push_back(new Pellet(m_spr.getPosition(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y)));
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}

		if(atk_clock.getElapsedTime() > sf::seconds(3))
		{
			enemies.push_back(new Ufo(m_spr.getPosition()));
			enemy_types.push_back(et_Ufo);
			atk_clock.restart();
		}

		if(atk_clock2.getElapsedTime() > sf::seconds(4.5 + static_cast<float>(hp)/50) && hp > 31)
		{
			enemies.push_back(new Ufo(m_spr.getPosition()));
			enemy_types.push_back(et_Ufo);
			atk_clock2.restart();
		}

	}
};

class bMeteor : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	sf::Clock atk_clock3;

public:
	bMeteor()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bmeteor_t);
		m_spr.setScale(2,2);
		m_spr.setRotation(90);
		m_spr.setOrigin(bmeteor_t.getSize().x/2, bmeteor_t.getSize().y/2);
		m_spr.setPosition(0,-430);
		m_spr.setColor(inv_game_color);
	}

	void update()
	{
		// rotation time - V3
		sf::Vector2f gpos = player.getPosition() / 6.0f;
		sf::Vector2f mpos = m_spr.getPosition();
		m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  2.5*deltatime));


		// attacks
		if(atk_clock.getElapsedTime() > sf::seconds(6.5))
		{
			enemies.push_back(new Ufo(sf::Vector2f(0,-430)));
			enemy_types.push_back(et_Ufo);
			atk_clock.restart();
		}

		if (atk_clock2.getElapsedTime() > sf::seconds(0.125))
		{
			enemies.push_back(new Pellet(sf::Vector2f((rand()%1920) - 960,-530), m_spr.getRotation()));
			static_cast<Pellet*>(enemies[enemies.size()-1])->getSprite().setTexture(meteor_t);
			enemy_types.push_back(et_Pellet);
			atk_clock2.restart();
		}

		if(atk_clock2.getElapsedTime() > sf::seconds(0.04) && hp < 50)
		{
			enemies.push_back(new Pellet(sf::Vector2f((rand()%1920) - 960,-530), m_spr.getRotation()));
			static_cast<Pellet*>(enemies[enemies.size()-1])->getSprite().setTexture(meteor_t);
			enemy_types.push_back(et_Pellet);
			atk_clock2.restart();
		}
		
		if(atk_clock3.getElapsedTime() > sf::seconds(8.5) && hp < DEF_HP)
		{
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+90));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+180));
			enemies.push_back(new Missile(m_spr.getPosition(), m_spr.getRotation()+270));
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			atk_clock3.restart();
		}

	}
};


class bPyramid : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	sf::Clock atk_clock3;

public:
	bPyramid()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bpyramid_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(bpyramid_t.getSize().x/2, bpyramid_t.getSize().y);
		m_spr.setPosition(0,540);
		m_spr.setColor(inv_game_color);
	}

	void update()
	{

		// attacks
		if(atk_clock.getElapsedTime() > sf::seconds(6.5))
		{
			enemies.push_back(new Missile(m_spr.getPosition(), 0));
			enemies.push_back(new Missile(m_spr.getPosition(), 180));
			enemy_types.push_back(et_Missile);
			enemy_types.push_back(et_Missile);
			atk_clock.restart();
		}

		if (atk_clock2.getElapsedTime() > sf::seconds(1.25))
		{
			enemies.push_back(new Laser(player.getPosition(), 3840, rand()%2, 0.7, 1));
			enemy_types.push_back(et_Laser);
			atk_clock2.restart();
		}

		if(atk_clock3.getElapsedTime() > sf::seconds(0.2) && hp < DEF_HP)
		{
			enemies.push_back(new Pellet(sf::Vector2f((rand()%1920) - 960,530), 270));
			enemy_types.push_back(et_Pellet);
			atk_clock3.restart();
		}
		

	}
};

class bFleshwall : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	sf::Clock atk_clock3;

public:
	bFleshwall()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bfleshwall_t);
		m_spr.setScale(4,4);
		m_spr.setOrigin(bfleshwall_t.getSize().x/2, bfleshwall_t.getSize().y/2 - 35);
		m_spr.setPosition(0,-540);
		m_spr.setColor(inv_game_color);
	}

	void update()
	{

		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();

		// attacks
		if(atk_clock.getElapsedTime() > sf::seconds(7.5))
		{
			if(rand()%2)
			{
				enemies.push_back(new Ufo(sf::Vector2f(600,-450)));
				enemies.push_back(new Bball(sf::Vector2f(-600,-450)));
				enemy_types.push_back(et_Ufo);
				enemy_types.push_back(et_Bball);
			}
			else
			{

				enemies.push_back(new Bball(sf::Vector2f(600,-450)));
				enemies.push_back(new Ufo(sf::Vector2f(-600,-450)));
				enemy_types.push_back(et_Bball);
				enemy_types.push_back(et_Ufo);

			}

			atk_clock.restart();
		}

		if (atk_clock2.getElapsedTime() > sf::seconds(2.75))
		{
			enemies.push_back(new Laser(player.getPosition(), 2160, 0, 0.7F, 1.35F));
			enemy_types.push_back(et_Laser);
			atk_clock2.restart();
		}

		if(atk_clock3.getElapsedTime() > sf::seconds(0.65) && hp < DEF_HP)
		{
			enemies.push_back(new Pellet(sf::Vector2f(250,-450), coords_to_dir(250, -450, gpos.x, gpos.y)));
			static_cast<Pellet*>(enemies[enemies.size()-1])->getSprite().setTexture(meteor_t);
			enemies.push_back(new Pellet(sf::Vector2f(-250,-450), coords_to_dir(-250, -450, gpos.x, gpos.y)));
			static_cast<Pellet*>(enemies[enemies.size()-1])->getSprite().setTexture(meteor_t);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			atk_clock3.restart();
		}
		

	}
};

class bDship : public Enemy
{
private:
	sf::Clock atk_clock;
	sf::Clock atk_clock2;
	int m_speed = 450;
	int m_speedd = 1250;
	bool dashing = 0;

public:
	bDship()
	{
		hp = DEF_HP;
		isBoss = 1;
		m_spr.setTexture(bdship_t);
		m_spr.setScale(2,2);
		m_spr.setOrigin(bdship_t.getSize().x/2, bdship_t.getSize().y/2);
		m_spr.setPosition(0,-430);
		m_spr.setColor(inv_game_color);
		m_spr.setRotation(90);
	}

	void update()
	{
		// rotation time - V4 - ULTIMATE EDITION
		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();

		m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  1.75*deltatime));

		int angle = m_spr.getRotation();
		//m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)m_speed * deltatime);


		// attacks
		if(atk_clock.getElapsedTime() > sf::seconds(0.5F) && !dashing)
		{
			enemies.push_back(new Pellet(mpos, angle));
			enemies.push_back(new Pellet(mpos, angle+45));
			enemies.push_back(new Pellet(mpos, angle+90));
			enemies.push_back(new Pellet(mpos, angle+135));
			enemies.push_back(new Pellet(mpos, angle+180));
			enemies.push_back(new Pellet(mpos, angle+225));
			enemies.push_back(new Pellet(mpos, angle+270));
			enemies.push_back(new Pellet(mpos, angle+315));
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}

		else if(atk_clock.getElapsedTime() > sf::seconds(0.05F) && dashing)
		{
			enemies.push_back(new Pellet(mpos, angle));
			static_cast<Pellet*>(enemies[enemies.size()-1])->m_speed = 1500;
			enemy_types.push_back(et_Pellet);
			atk_clock.restart();
		}

		// dash
		if(atk_clock2.getElapsedTime() > sf::seconds(4.5))
		{
			if(dashing == 0)
			{
				dashing = 1;
			}


			m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)m_speedd * deltatime);
			if(atk_clock2.getElapsedTime() > sf::seconds(5.25F))
			{
				enemies.push_back(new Laser(mpos, 7680, 1, 1, 1.25));
				enemies.push_back(new Laser(mpos, 4320, 0, 1, 1.25));
				enemy_types.push_back(et_Laser);
				enemy_types.push_back(et_Laser);
				enemies.push_back(new Bball(mpos));
				enemy_types.push_back(et_Bball);

				atk_clock2.restart();
				dashing = 0;
			}
		}

	}
};