// enemy class, for all things that KILL
class Enemy : public sf::Drawable
{
protected:
	sf::Sprite m_spr;

public:
	short hp{};
	bool isBoss = false;
	bool damage(int amount)
	{
		hp--;
		if(hp < 1) return true;
		return false;
	}

	sf::Sprite& getSprite()
	{
		return m_spr;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_spr, states);
	}

	void update() {}

	void collision() // call this every frame
	{
		if(m_spr.getGlobalBounds().contains(player.getPosition()))
		{
			// set dead flag
			dead = 1;
		}
	}

};


// missile class, derived from enemy
class Missile : public Enemy
{
private:
	int missile_speed{425};

public:
	Missile(sf::Vector2f pos, int angle = 0)
	{
		hp = 3;
		m_spr.setTexture(missile_t);
		m_spr.setOrigin(missile_t.getSize().x/2, missile_t.getSize().y/2);
		m_spr.setColor(inv_game_color);
		m_spr.setScale(0.5, 0.5);
		m_spr.setPosition(pos);
		m_spr.setRotation(angle);
	}

	void update()
	{
		// rotation time - missile edition
		sf::Vector2f gpos = player.getPosition();
		sf::Vector2f mpos = m_spr.getPosition();
		m_spr.setRotation(lerp_angle(m_spr.getRotation(), coords_to_dir(mpos.x, mpos.y, gpos.x, gpos.y),  2.5*deltatime));
		int angle = m_spr.getRotation();
		m_spr.move(sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle)) * (float)missile_speed * deltatime);

		// collision
		collision();
	}
};

// Bball class, derived from enemy
class Bball : public Enemy
{
private:
	int rotspeed{};
	int m_speed{600};
	sf::Vector2f point_dir{};

public:
	Bball(sf::Vector2f pos)
	{
		hp = 9;

		// rotation direction
		if(rand()%2) rotspeed = 720;
		else rotspeed = -720;

		m_spr.setTexture(bball_t);
		m_spr.setOrigin(bball_t.getSize().x/2, bball_t.getSize().y/2);
		m_spr.setColor(inv_game_color);
		m_spr.setScale(0.75, 0.75);
		m_spr.setPosition(pos);

		int angle = coords_to_dir(m_spr.getPosition().x, m_spr.getPosition().y, player.getPosition().x, player.getPosition().y);
		point_dir = sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle));
	}

	void update()
	{
		// spiiiiiin
		m_spr.rotate(rotspeed * deltatime);

		// bounce on collision with wall
		if(!screen.contains(m_spr.getPosition()))
		{
			int angle = coords_to_dir(m_spr.getPosition().x, m_spr.getPosition().y, player.getPosition().x, player.getPosition().y);
			point_dir = sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle));
		}

		m_spr.move(point_dir * (float)m_speed * deltatime);

		// collision
		collision();
	}
};

// Pellet class, derived from enemy
class Pellet : public Enemy
{
private:
	int rotspeed{};
	sf::Vector2f point_dir{};

public:
	int m_speed{700};
	Pellet(sf::Vector2f pos, int angle)
	{
		hp = 1;

		// rotation direction
		if(rand()%2) rotspeed = 720;
		else rotspeed = -720;

		m_spr.setTexture(pellet_t);
		m_spr.setOrigin(pellet_t.getSize().x/2, pellet_t.getSize().y/2);
		m_spr.setColor(inv_game_color);
		m_spr.setPosition(pos);
		m_spr.setRotation(angle);

		point_dir = sf::Vector2f(dir_to_coords_x(angle), dir_to_coords_y(angle));
	}

	void update()
	{
		// spiiiiiin
		m_spr.rotate(rotspeed * deltatime);

		// die on collision with wall
		if(!screen.contains(m_spr.getPosition()))
		{
			hp = 0;
		}

		m_spr.move(point_dir * (float)m_speed * deltatime);

		// collision
		collision();
	}
};

// Ufo class, derived from enemy
class Ufo : public Enemy
{
private:
	int rotspeed{};
	sf::Clock atk_clock;

public:
	Ufo(sf::Vector2f pos)
	{
		hp = 12;

		// setup
		m_spr.setTexture(ufo_t);
		m_spr.setScale(0.75, 0.75);
		m_spr.setOrigin(ufo_t.getSize().x/2, ufo_t.getSize().y/2);
		m_spr.setColor(inv_game_color);
		m_spr.setPosition(pos);
	}

	void update()
	{

		// lerp to position near player
		sf::Vector2f gpos = player.getPosition();
		gpos.y -= 300;
		sf::Vector2f mpos = m_spr.getPosition();

		m_spr.setPosition(lerp(mpos.x, gpos.x, 1.25 * deltatime), lerp(mpos.y, gpos.y, 1.25 * deltatime));

		// attack
		if(atk_clock.getElapsedTime().asSeconds() > 0.5)
		{
			atk_clock.restart();
			enemies.push_back(new Pellet(mpos, 90));
			enemy_types.push_back(et_Pellet);
		}

		// collision
		collision();
	}
};

// Laser class, derived from enemy
class Laser : public Enemy
{
private:
	sf::Clock timer;
	float m_delay{};
	float m_duration{};
	sf::Vector2f point_dir{};

public:
	Laser(sf::Vector2f pos, int length, bool horizontal, float delay = 0, float duration = 1)
	{
		hp = 9999;

		m_spr.setTexture(laser_t);
		m_spr.setScale(0.5,0.5);
		m_spr.setColor(sf::Color(inv_game_color.r, inv_game_color.g, inv_game_color.b, 80));
		
		m_delay = delay;
		m_duration = duration;

		m_spr.setTextureRect(sf::IntRect(0,0,96,length*2));
		m_spr.setOrigin(laser_t.getSize().x/2, length);
		m_spr.setRotation(horizontal * 90);
		m_spr.setPosition(pos);

	}

	void update()
	{
		float time_passed = timer.getElapsedTime().asSeconds();

		// collide now
		if(time_passed > m_delay && time_passed < m_duration)
		{
			m_spr.setColor(inv_game_color);
			collision();
		}

		else if(time_passed > m_duration)
		{
			hp = 0;
		}
	}
};
