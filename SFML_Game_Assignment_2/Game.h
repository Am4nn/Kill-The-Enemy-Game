#pragma once

#include "EntityManager.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SP; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
struct WindowConfig { int W, H, FL, FS; };
struct FontConfig { std::string F = ""; int S = 0, R = 0, G = 0, B = 0; };

class Game
{
public:

	Game(const std::string& config);
	void run();

private:

	sf::RenderWindow	m_window;
	sf::Font			m_font;
	sf::Text			m_text;
	sf::Text			m_textSW;
	
	EntityManager		m_entities; // vector of entities to maintain.
	
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletconfig;
	WindowConfig		m_windowConfig;
	FontConfig			m_fontConfig;

	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;

	bool				m_paused = false;
	bool				m_running = true;

	//
	int					counter_right_click = 0;
	int					counter_special_weapon = 0;
	bool				flag_special_weapon = false;

	sf::Vertex			line[2];
	
	std::shared_ptr<Entity> minE;
	//

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);
	void setPaused();

	// System
	void sMovement();
	void sUserInput();
	void sLifeSpan();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void specialWeapon1(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void specialWeapon2(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void specialWeapon3(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void renderSpecialWeapon3Line();
	void addScoreDecAnimation();
};