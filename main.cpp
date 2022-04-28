#define SFML_STATIC
#define FPS 120
#define DEL_BULLET(i) delete bullets[i]; bullets.erase(bullets.begin()+i); delete bullet_speeds[i]; bullet_speeds.erase(bullet_speeds.begin()+i);
#define UPDATE_BOSS_HP_C(i) boss_hp_counter.setString(i); boss_hp_counter.setOrigin(boss_hp_counter.getGlobalBounds().width/2, boss_hp_counter.getGlobalBounds().height/2);

#define NAME "ASTROPHOBIA"

#ifndef VERSION
#define VERSION 1
#endif

#ifdef DEMO_BUILD
#define VERSION_DATA "Astrophobia Demo, Build "
#endif

#ifndef DEMO_BUILD
#define VERSION_DATA "Prerelease 7, Build "
#endif

#include <SFML/Graphics.hpp>
#include <Chronometer.hpp>
#include <dir_to_coords.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <windows.h>
#include <wtypes.h>

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   const HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);
   horizontal = desktop.right;
   vertical = desktop.bottom;
}


void draw();
void load();
void start();
void events();
void update();
void restart();
void kill_all();
void load_colors();

// global vars
sf::RenderWindow window;
sf::Event event;
sf::Color bgcolor(0,0,30,0xff), game_color, inv_game_color;
sf::Texture player_t, bullet_t, missile_t, bball_t, turret_t, pellet_t, bspaceship_t, bspintower_t, ufo_t, bvship_t, bmeteor_t, meteor_t, laser_t, bpyramid_t, bfleshwall_t, bdship_t;
sf::Sprite player, bullet_temp;
sf::Chronometer d_clock, dialog_clock, b_clock, r_clock;
sf::Chronometer* boss_clock;
sf::Font font1;
sf::Text game_over, game_over2, boss_hp_counter, menu_text_title, menu_text_subtitle, paused_text, paused_text2, dialog_text, version_info;
sf::FloatRect screen;
sf::Sound boss_summon_sound, boss_kill_sound;
sf::SoundBuffer boss_summon_sound_b, boss_kill_sound_b;
sf::Music battle_song;
sf::Music menu_song;
sf::RectangleShape rect;

enum keys {
	key_W,
	key_A,
	key_S,
	key_D,
	key_MOUSE,
};

enum Enemy_type {
	et_Missile,
	et_Bball,
	et_Pellet,
	et_Ufo,
	et_Laser,

	// boss enemies
	ebt_Turret,
	ebt_Spaceship,
	ebt_Spintower,
	ebt_Vship,
	ebt_Meteor,
	ebt_Pyramid,
	ebt_Fleshwall,
	ebt_Dship,
};

bool key_pressed[5]{};
float deltatime{};
int speed = 1750;
int b_speed = 650;
float c_speed = speed*0.75;
float drag = 5;
float dialog_length = 0;
float dialog_delay = 0;
bool dead = 0;
float bullet_cooldown = 0.075f;
sf::Vector2f player_vel{};
int old_boss{};
int old_boss2{};
bool old_dead{};
bool menu_active = 1;
bool paused = 0;
Enemy_type old_boss_type = ebt_Turret;

// game is centered around this value
unsigned char boss;


// lists of things
std::vector<sf::Sprite*> bullets;
std::vector<sf::Vector2f*> bullet_speeds;
std::vector<void*> enemies;
std::vector<Enemy_type> enemy_types;


// this file is already massive, bosses and enemies go to other files
#include "enemy.cpp"
#include "boss.cpp"

// dialog framework
void dialog(std::string text, float x, float y = 0)
{

	dialog_text.setString(text);
	dialog_text.setOrigin(dialog_text.getGlobalBounds().width/2, dialog_text.getGlobalBounds().height/2);
	dialog_text.setPosition(rect.getPosition());

	dialog_length = x;
	dialog_delay = y;

	dialog_clock.restart();
	
}

int main(int argc, char** argv)
{
	// initial setup
	sf::Image icon;
	int winx{}, winy{};
	srand(time(nullptr));
	SetProcessDPIAware();
	GetDesktopResolution(winx, winy);
	window.create(sf::VideoMode(winx, winy), NAME, sf::Style::Fullscreen);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	icon.loadFromFile("./assets/eb_turret.png");
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	std::filesystem::current_path(std::filesystem::absolute(argv[0]).parent_path());
	screen = sf::FloatRect(window.getView().getCenter()-window.getView().getSize()/2.0F, window.getView().getSize());

	// load textures
	load();

	// setup area and audio
	restart();
	menu_song.play();
	battle_song.setLoop(1);
	menu_song.setLoop(1);

	// slower devices
	d_clock.restart();

	// mainloop
	while(window.isOpen())
	{
		deltatime = d_clock.restart().asSeconds();

		// do events
		events();

		// update things
		update();

		// draw
		draw();
	}

	return 0;
}

void load()
{	
	load_colors();
	player_t.loadFromFile("./assets/spaceship.png");
	player.setTexture(player_t);
	player.setOrigin(player_t.getSize().x/2, player_t.getSize().y/2);
	player.setScale(0.2,0.2);

	bullet_t.loadFromFile("./assets/bullet.png");
	bullet_temp.setTexture(bullet_t);
	bullet_temp.setOrigin(bullet_t.getSize().x/2, bullet_t.getSize().y/2);
	bullet_temp.setScale(0.5,0.5);

	missile_t.loadFromFile("./assets/e_missile.png");
	bball_t.loadFromFile("./assets/e_bball.png");
	turret_t.loadFromFile("./assets/eb_turret.png");
	pellet_t.loadFromFile("./assets/e_pellet.png");
	bspaceship_t.loadFromFile("./assets/eb_spaceship.png");
	bspintower_t.loadFromFile("./assets/eb_spintower.png");
	ufo_t.loadFromFile("./assets/e_ufo.png");
	bvship_t.loadFromFile("./assets/eb_vship.png");
	bmeteor_t.loadFromFile("./assets/eb_meteor.png");
	meteor_t.loadFromFile("./assets/e_meteor.png");
	laser_t.loadFromFile("./assets/e_laser.png");
	bpyramid_t.loadFromFile("./assets/eb_pyramid.png");
	bfleshwall_t.loadFromFile("./assets/eb_fleshwall.png");
	bdship_t.loadFromFile("./assets/eb_dship.png");

	laser_t.setRepeated(1);

	font1.loadFromFile("./assets/font1.otf");
	game_over.setLetterSpacing(5);
	game_over.setFont(font1);
	game_over.setString("Game Over");
	game_over.setCharacterSize(72);
	game_over.setOrigin(game_over.getGlobalBounds().width/2, game_over.getGlobalBounds().height/2);
	game_over.setPosition(0,0);
	game_over.setFillColor(sf::Color::Black);
	game_over.setOutlineThickness(3);
	game_over.setOutlineColor(sf::Color::White);

	game_over2 = game_over;
	game_over2.setCharacterSize(48);
	game_over2.setString("(R)estart");
	game_over2.setOrigin(game_over2.getGlobalBounds().width/2, game_over2.getGlobalBounds().height/2);
	game_over2.setPosition(0,75);
	game_over2.setFillColor(sf::Color::Black);
	game_over2.setOutlineThickness(3);
	game_over2.setOutlineColor(sf::Color::White);

	boss_hp_counter = game_over2;
	UPDATE_BOSS_HP_C(std::to_string(DEF_HP)+" HP");
	boss_hp_counter.setPosition(0,450);

	boss_summon_sound_b.loadFromFile("./assets/boss_summon.ogg");
	boss_summon_sound.setBuffer(boss_summon_sound_b);
	boss_kill_sound_b.loadFromFile("./assets/boss_kill.ogg");
	boss_kill_sound.setBuffer(boss_kill_sound_b);

	battle_song.openFromFile("./assets/battle.ogg");
	menu_song.openFromFile("./assets/menu.ogg");

	menu_text_title = game_over;
	menu_text_title.setCharacterSize(100);
	menu_text_title.setString(NAME);
	menu_text_title.setStyle(sf::Text::Italic);
	menu_text_title.setOrigin(menu_text_title.getGlobalBounds().width/2, menu_text_title.getGlobalBounds().height/2);
	menu_text_title.setPosition(0,-300);
	menu_text_title.setFillColor(sf::Color::Black);
	menu_text_title.setOutlineThickness(3);
	menu_text_title.setOutlineColor(sf::Color::White);

	menu_text_subtitle = menu_text_title;
	menu_text_subtitle.setStyle(sf::Text::Regular);
	menu_text_subtitle.setCharacterSize(48);
	menu_text_subtitle.setString("  (P)lay\n  (H)elp\n  (Q)uit\n(C)redits");
	menu_text_subtitle.setOrigin(menu_text_subtitle.getGlobalBounds().width/2, menu_text_subtitle.getGlobalBounds().height/2);
	menu_text_subtitle.setPosition(0,0);
	menu_text_subtitle.setFillColor(sf::Color::Black);
	menu_text_subtitle.setOutlineThickness(3);
	menu_text_subtitle.setOutlineColor(sf::Color::White);

	paused_text = menu_text_title;
	paused_text.setStyle(sf::Text::Regular);
	paused_text.setString("PAUSED");
	paused_text.setStyle(sf::Text::Italic);
	paused_text.setOrigin(paused_text.getGlobalBounds().width/2, paused_text.getGlobalBounds().height/2);
	
	paused_text2 = menu_text_subtitle;
	paused_text2.setString("(ESC) Continue\n(B)ack to Menu");
	paused_text2.setOrigin(paused_text2.getGlobalBounds().width/2, paused_text2.getGlobalBounds().height/2);

	rect.setFillColor(sf::Color::Black);
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(3);
	rect.setSize(sf::Vector2f(1280, 180));
	rect.setOrigin(1280/2, 180/2);
	rect.setPosition(0, 420);

	dialog_text = menu_text_subtitle;
	dialog_text.setCharacterSize(38);

	version_info = menu_text_subtitle;
	version_info.setOutlineThickness(1);
	version_info.setCharacterSize(24);
	version_info.setPosition(-950, 530);
	version_info.setString(VERSION_DATA+std::to_string(VERSION));
	version_info.setOrigin(0,version_info.getGlobalBounds().height);
}

void draw()
{
	window.clear(bgcolor);

	// draw bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		window.draw(*bullets[i]);
	}

	// draw enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		window.draw(*static_cast<Enemy*>(enemies[i]));
	}

	// player time
	if(!dead)
		window.draw(player);

	// death message
	if(dead)
	{
		window.draw(game_over);
		window.draw(game_over2);
	}

	window.draw(boss_hp_counter);

	if(menu_active)
	{
		window.draw(menu_text_title);
		window.draw(menu_text_subtitle);
		window.draw(version_info);
	}

	if(paused)
	{
		window.draw(paused_text);
		window.draw(paused_text2);
	}

	if(dialog_clock.getElapsedTime() > sf::seconds(dialog_delay) && dialog_clock.getElapsedTime() < sf::seconds(dialog_length) )
	{
		window.draw(rect);
		window.draw(dialog_text);
	}

	// render
	window.display();
}

void events()
{
	// event loop
	while (window.pollEvent(event))
	{
		switch (event.type)
		{

		// all this switch statement copy and paste is pretty self explanitory, no comments needed.
		case sf::Event::Closed:
			window.close();
			break;
		
		case sf::Event::KeyPressed:
			
			switch (event.key.code)
			{
			case sf::Keyboard::W: key_pressed[key_W] = 1; break;
			case sf::Keyboard::A: key_pressed[key_A] = 1; break;
			case sf::Keyboard::S: key_pressed[key_S] = 1; break;
			case sf::Keyboard::D: key_pressed[key_D] = 1; break;
			case sf::Keyboard::Space: load_colors(); break;
			
			// this is legitematly painful
			case sf::Keyboard::Escape: if(!menu_active && !dead) {if(!paused) {paused = 1; battle_song.pause(); menu_song.play();} else {paused = 0; battle_song.play(); menu_song.pause();}} break;
			case sf::Keyboard::R: if(dead) restart(); break;
			case sf::Keyboard::B: if(paused) restart(); break;
			case sf::Keyboard::P: if(menu_active) start(); break;
			case sf::Keyboard::C: if(menu_active) { dialog("Main Programmer / Artist : Edward Mayen\nCredit to @gooseninja for making the music\nand to @mr_sec1 for testing the game.", 6.0f, 0.0f); } break;
			case sf::Keyboard::H: if(menu_active) { system("assets\\tutorial.mp4"); } break;
			case sf::Keyboard::Q: if(menu_active) window.close(); break;

			// I need to stop with these trashy one-liners
			case sf::Keyboard::M: if(battle_song.getVolume() == 100) {battle_song.setVolume(0); menu_song.setVolume(0); boss_kill_sound.setVolume(0); boss_summon_sound.setVolume(0);}
				else {battle_song.setVolume(100); menu_song.setVolume(100); boss_kill_sound.setVolume(100); boss_summon_sound.setVolume(100);} break;
			default: break;
			}

			break;

		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::W: key_pressed[key_W] = 0; break;
			case sf::Keyboard::A: key_pressed[key_A] = 0; break;
			case sf::Keyboard::S: key_pressed[key_S] = 0; break;
			case sf::Keyboard::D: key_pressed[key_D] = 0; break;
			default: break;
			}
			
			break;

		case sf::Event::Resized:
			d_clock.restart();
			break;

		// mouse inputs
		case sf::Event::MouseButtonPressed: if(event.mouseButton.button == sf::Mouse::Button::Left) key_pressed[key_MOUSE] = 1; break;
		case sf::Event::MouseButtonReleased: if(event.mouseButton.button == sf::Mouse::Button::Left) key_pressed[key_MOUSE] = 0; break;

		default:
			break;
		}
		
	}
}

void update()
{
	// define variables
	sf::Vector2f player_pos = player.getPosition();
	sf::Vector2f player_old_pos = player_pos;
	float player_rot{};

	int tmp_speed = speed;
	int num_k_p{};

	num_k_p += key_pressed[key_W];
	num_k_p += key_pressed[key_A];
	num_k_p += key_pressed[key_S];
	num_k_p += key_pressed[key_D];

	if(num_k_p > 0 && !paused && !dead)
	{
		// semi-mitigating the funky aspects of diagonal movement
		if(num_k_p > 1)
		{
			tmp_speed = c_speed;
		}

		// update player position and velocity
		if(key_pressed[key_W]) {
			player_vel.y -= tmp_speed * deltatime;
		}
		if(key_pressed[key_A]) {
			player_vel.x -= tmp_speed * deltatime;
		}
		if(key_pressed[key_S]) {
			player_vel.y += tmp_speed * deltatime;
		}
		if(key_pressed[key_D]) {
			player_vel.x += tmp_speed * deltatime;
		}
	}
	
	// I need to take a physics course, this is nowhere near accurate
	if(!dead && !paused)
	{
		player_pos += player_vel * deltatime;
		player_vel -= player_vel * drag * deltatime;
	}


	// keep player on screen
	if(!screen.contains(player_pos))
	{
		player_pos = player_old_pos;
		player_vel = sf::Vector2f(0,0);
	}

	if(!paused && !dead)
	{
		player.setPosition(player_pos);

		// rotation time - code golf edition
		sf::Vector2f mpos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		player.setRotation(lerp_angle(player.getRotation(), coords_to_dir(player_pos.x, player_pos.y, mpos.x, mpos.y),  10*deltatime));
	}

	// player updated, now it's bullet time :)
	if(key_pressed[key_MOUSE] && !(dead || paused))
	{
		// if mouse pressed...
		if(b_clock.getElapsedTime() > sf::seconds(bullet_cooldown))
		{
			// and timer finished...
			b_clock.restart();

			// bullet template should inherit player properties...
			bullet_temp.setPosition(player.getPosition());
			bullet_temp.setRotation(player.getRotation() + (rand()%11)-5);

			// NOW FIRE!
			bullets.push_back(new sf::Sprite(bullet_temp));
			bullet_speeds.push_back(new sf::Vector2f(dir_to_coords_x(bullet_temp.getRotation()), dir_to_coords_y(bullet_temp.getRotation())));
		}
	}

	if(!paused)
	{

		// update bullets
		for (int i = 0; i < bullets.size() && bullets.size() != 0; i++)
		{
			bullets[i]->move(*bullet_speeds[i]*(float)b_speed*deltatime);

			// bullet exited screen
			if(r_clock.getElapsedTime() > sf::seconds(1) && !screen.contains(bullets[i]->getPosition()))
			{
				if(i == bullets.size()-1)
					r_clock.restart();
				DEL_BULLET(i)
				i--;
				continue;
			}

			for(int j = 0; j < enemies.size(); j++)
			{
				// chunky collision line
				if(static_cast<Enemy*>(enemies[j])->getSprite().getGlobalBounds().contains(bullets[i]->getPosition()))
				{
					DEL_BULLET(i)
					i--;

					if(static_cast<Enemy*>(enemies[j])->isBoss)
					{
						UPDATE_BOSS_HP_C(std::to_string(static_cast<Enemy*>(enemies[j])->hp - 1) + " HP");
					}

					// damage enemy and check if it's killed
					static_cast<Enemy*>(enemies[j])->damage(1);

					break;
				}

			}

		}

	}

	if(!paused)
	{
		// update enemies
		for (int i = 0; i < enemies.size(); i++)
		{
			switch (enemy_types[i])
			{
			case et_Missile:
				static_cast<Missile*>(enemies[i])->update();
				break;
			
			case et_Bball:
				static_cast<Bball*>(enemies[i])->update();
				break;

			case et_Pellet:
				static_cast<Pellet*>(enemies[i])->update();
				break;

			case et_Ufo:
				static_cast<Ufo*>(enemies[i])->update();
				break;

			case et_Laser:
				static_cast<Laser*>(enemies[i])->update();
				break;

			case ebt_Turret:
				static_cast<bTurret*>(enemies[i])->update();
				break;

			case ebt_Spaceship:
				static_cast<bSpaceship*>(enemies[i])->update();
				break;

			case ebt_Spintower:
				static_cast<bSpintower*>(enemies[i])->update();
				break;

			case ebt_Vship:
				static_cast<bVship*>(enemies[i])->update();
				break;

			case ebt_Meteor:
				static_cast<bMeteor*>(enemies[i])->update();
				break;

			case ebt_Pyramid:
				static_cast<bPyramid*>(enemies[i])->update();
				break;

			case ebt_Fleshwall:
				static_cast<bFleshwall*>(enemies[i])->update();
				break;

			case ebt_Dship:
				static_cast<bDship*>(enemies[i])->update();
				break;

			default:
				break;
			}

			// enemy died
			if(static_cast<Enemy*>(enemies[i])->hp < 1)
			{
				if(static_cast<Enemy*>(enemies[i])->isBoss)
					boss++;
				delete static_cast<Enemy*>(enemies[i]);
				enemies.erase(enemies.begin()+i);
				enemy_types.erase(enemy_types.begin()+i);
			}
		}
	}

	// spawn new boss if old died
	if(old_boss != boss)
	{
		if(old_boss2 != boss)
		{

			// dialog
			switch (boss)
			{
			case 3:
				dialog("If you can hear my radio brodcast,\nhead to the Andromeda Galaxy.\nYou are not alone.", 3.85, .15);
				break;
			
			case 5:
				dialog("There is an asteroid belt near our\nprerimiter, approach with caution", 3.85, .15);
				break;

			#ifdef DEMO_BUILD
			case 6:
				dialog("That's the end of the demo,\nthanks for playing!", 3.85, .15);
				break;
			#endif

			case 7:
				dialog("I'm picking up a large clump of organic matter\non my radar, be warned for massive alien\nlifeforms.", 3.85, .15);
				break;

			default:
				break;
			}
				
			old_boss2 = boss;
			// lets see if this will be a rage game or not
			// kill_all();
			boss_clock = new sf::Chronometer();

			// death screech
			if(boss != 1) boss_kill_sound.play();
		}

		// pause and play bossclock accordingly
		if(paused)
		{
			if(boss_clock->isRunning())
			{
				boss_clock->pause();
			}
		}
		else
		{
			if(!boss_clock->isRunning())
			{
				boss_clock->resume();
			}
		}

		if((boss_clock->getElapsedTime() > sf::seconds(4) && boss != 1) || boss == 1)
		{
			old_boss++;
			
			// chose boss
			while(1)
			{

				// custom boss selection
				switch (boss)
				{
				case 5:
					enemies.push_back(new bMeteor());
					enemy_types.push_back(ebt_Meteor);
					break;

				#ifdef DEMO_BUILD
				case 6:
					restart();
					break;
				#endif

				case 8:
					enemies.push_back(new bFleshwall());
					enemy_types.push_back(ebt_Fleshwall);
					break;


				default:
					break;
				}

				if(enemies.size() && static_cast<Enemy*>(enemies[enemies.size()-1])->isBoss)
				{
					break;
				}

				int selection = rand()%8;

				if(selection == 0 && old_boss_type != ebt_Turret && boss < 5)
				{
					enemies.push_back(new bTurret());
					enemy_types.push_back(ebt_Turret);
					break;
				}

				else if(selection == 1 && old_boss_type != ebt_Spaceship)
				{
					enemies.push_back(new bSpaceship());
					enemy_types.push_back(ebt_Spaceship);
					break;
				}

				else if(selection == 2 && old_boss_type != ebt_Spintower)
				{
					enemies.push_back(new bSpintower());
					enemy_types.push_back(ebt_Spintower);
					break;
				}

				else if(selection == 3 && old_boss_type != ebt_Vship)
				{
					enemies.push_back(new bVship());
					enemy_types.push_back(ebt_Vship);
					break;
				}

				else if(selection == 4 && old_boss_type != ebt_Meteor && boss > 4)
				{
					enemies.push_back(new bMeteor());
					enemy_types.push_back(ebt_Meteor);
					break;
				}

				else if(selection == 5 && old_boss_type != ebt_Pyramid && boss > 4)
				{
					enemies.push_back(new bPyramid());
					enemy_types.push_back(ebt_Pyramid);
					break;
				}

				else if(selection == 6 && old_boss_type != ebt_Fleshwall && boss > 8)
				{
					enemies.push_back(new bFleshwall());
					enemy_types.push_back(ebt_Fleshwall);
					break;
				}

				else if(selection == 7 && old_boss_type != ebt_Dship && boss > 4)
				{
					enemies.push_back(new bDship());
					enemy_types.push_back(ebt_Dship);
					break;
				}

			}

			// update prev boss type
			old_boss_type = enemy_types[enemy_types.size()-1];

			// increase health based on boss #
			static_cast<Enemy*>(enemies[enemies.size()-1])->hp += (boss-1)*10;
			UPDATE_BOSS_HP_C(std::to_string(static_cast<Enemy*>(enemies[enemies.size()-1])->hp) + " HP");
			delete boss_clock;

			// play sound
			boss_summon_sound.play();
		}
	}

	// play menu audio if player died
	if(old_dead != dead)
	{
		old_dead = dead;
		menu_song.play();
		battle_song.pause();
	}
	
}

void load_colors()
{
	// chose random game theme color, and update EVERYTHING
	game_color = sf::Color(rand()%256, rand()%256, rand()%256, 0xff);
	inv_game_color = sf::Color(game_color.r - 128, game_color.g - 128, game_color.b - 128, 0xff);
	player.setColor(game_color);
	bullet_temp.setColor(game_color);

	// stuff in vectors
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->setColor(game_color);
	for (int i = 0; i < enemies.size(); i++)
		static_cast<Enemy*>(enemies[i])->getSprite().setColor(inv_game_color);
}

void restart()
{
	kill_all();
	player.setPosition(0,350);
	player.setRotation(270);
	dead = 0;
	old_dead = 0;
	player_vel = {};

	// respawn boss and reset boss couunter
	boss = 0;
	old_boss = 0;
	old_boss2 = 0;
	old_boss_type = ebt_Spintower;
	UPDATE_BOSS_HP_C("");

	paused = 0;
	menu_active = 1;

	menu_song.play();
	battle_song.pause();

}

void start()
{
	kill_all();
	player.setPosition(0,350);
	player.setRotation(270);
	dead = 0;
	old_dead = 0;
	player_vel = {};

	// spawn boss
	boss = 1;


	UPDATE_BOSS_HP_C(std::to_string(DEF_HP)+" HP");

	// play audio
	boss_summon_sound.play();
	menu_song.pause();
	battle_song.play();

	paused = 0;
	menu_active = 0;
}

void kill_all()
{
	// mass extinction
	while(enemies.size() != 0)
	{
		delete static_cast<Enemy*>(enemies[0]);
		enemies.erase(enemies.begin()+0);
		enemy_types.erase(enemy_types.begin()+0);
	}
	while(bullets.size() != 0)
	{
		DEL_BULLET(0);
	}

}
