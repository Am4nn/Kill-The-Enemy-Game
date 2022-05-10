#include "Game.h"

Game::Game(const std::string& config)
{
	std::cout << " \n\nGame by - Aman Arya\n\n";

	Game::init(config);
}

void Game::run()
{

	while (m_running)
	{
		if (m_paused)
		{
			sRender();
			sUserInput();
			continue;
		}

		m_entities.update();
		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sLifeSpan();
		sRender();

		// increase the current frame
		m_currentFrame++;
	}
}


void Game::init(const std::string& config)
{
	// read config files

	std::ifstream fin(config);
	std::string ig;
	
	fin >> ig >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
	fin >> ig >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
	fin >> ig >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
	fin >> ig >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SP;
	fin >> ig >> m_bulletconfig.SR >> m_bulletconfig.CR >> m_bulletconfig.S >> m_bulletconfig.FR >> m_bulletconfig.FG >> m_bulletconfig.FB >> m_bulletconfig.OR >> m_bulletconfig.OG >> m_bulletconfig.OB >> m_bulletconfig.OT >> m_bulletconfig.V >> m_bulletconfig.L;
	
	// set window
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Kill The Enemies : SFML-Game-1");
	m_window.setFramerateLimit(m_windowConfig.FL);

	// set font
	if (!m_font.loadFromFile("fonts/mvboli.ttf"))
	{
		std::cout << "Failed to load font !!!\n";
		return;
	}

	// set Score Text
	m_text.setFont(m_font);
	m_text.setCharacterSize(m_fontConfig.S); // in pixels, not points!
	m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
	m_text.setStyle(sf::Text::Bold);
	m_text.setPosition(0.0f, 0.0f);

	// set Speacial Weapon Text
	m_textSW.setFont(m_font);
	m_textSW.setCharacterSize(25); // in pixels, not points!
	m_textSW.setFillColor(sf::Color::White);
	m_textSW.setStyle(sf::Text::Italic);
	m_textSW.setPosition(400.0f, 0.0f);

	// spawn player
	spawnPlayer();
}

void Game::setPaused()
{
	m_paused = !m_paused;
}


// System

void Game::sMovement()
{
	Vec2 playerVel(0, 0);
	if (m_player->cInput->left)
	{
		playerVel.x -= 1;
	}
	if (m_player->cInput->right)
	{
		playerVel.x += 1;
	}
	if (m_player->cInput->up)
	{
		playerVel.y -= 1;
	}
	if (m_player->cInput->down)
	{
		playerVel.y += 1;
	}
	playerVel.normalize();
	playerVel *= m_playerConfig.S;
	m_player->cTransform->velocity = playerVel;

	for (auto& p : m_entities.getEntities("player"))
	{
		p->cTransform->velocity = playerVel;
	}

	for (auto& e : m_entities.getEntities())
	{
		if (e->cTransform)
		{
			e->cTransform->pos += e->cTransform->velocity;
		}
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event is triggered when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// set player's input component "up" to true
				m_player->cInput->up = true;
				break;

			case sf::Keyboard::S:
				// set player's input component "down" to true
				m_player->cInput->down = true;
				break;

			case sf::Keyboard::A:
				// set player's input component "left" to true
				m_player->cInput->left = true;
				break;

			case sf::Keyboard::D:
				// set player's input component "right" to true
				m_player->cInput->right = true;
				break;

			case sf::Keyboard::P:
				setPaused();
				break;

			case sf::Keyboard::Escape:
				m_running = false;
				break;

			default:
				break;
			}
		}

		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// set player's input component "up" to false
				m_player->cInput->up = false;
				break;

			case sf::Keyboard::S:
				// set player's input component "down" to false
				m_player->cInput->down = false;
				break;

			case sf::Keyboard::A:
				// set player's input component "left" to false
				m_player->cInput->left = false;
				break;

			case sf::Keyboard::D:
				// set player's input component "right" to false
				m_player->cInput->right = false;
				break;

			case sf::Keyboard::Numpad1:
			case sf::Keyboard::Num1:
				counter_special_weapon = 1;
				break;

			case sf::Keyboard::Numpad2:
			case sf::Keyboard::Num2:
				counter_special_weapon = 2;
				break;

			case sf::Keyboard::Numpad3:
			case sf::Keyboard::Num3:
				counter_special_weapon = 3;
				break;

			default:
				counter_special_weapon = 0;
				break;
			}
		}

		// this event is triggered when a mouse button is pressed
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// call spawn bullet here
				for (auto& p : m_entities.getEntities("player"))
				{
					spawnBullet(p, Vec2((float)event.mouseButton.x, (float)event.mouseButton.y));
				}
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				// spawn special weapon bullet here
				spawnSpecialWeapon(m_player, Vec2((float)event.mouseButton.x, (float)event.mouseButton.y));
			}
		}

	}
}

void Game::sLifeSpan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (!e->cLifespane) { continue; }

		if (e->cLifespane->remainingLife > 0) { e->cLifespane->remainingLife -= 1; }

		auto& color = e->cShape->circle.getFillColor();
		auto& outlineColor = e->cShape->circle.getOutlineColor();
		int newAlpha = 255 * e->cLifespane->remainingLife/e->cLifespane->totalLife;
		sf::Color newColor(color.r, color.g, color.b, newAlpha);
		sf::Color newOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, newAlpha);
		e->cShape->circle.setFillColor(newColor);
		e->cShape->circle.setOutlineColor(newOutlineColor);

		if (e->cLifespane->remainingLife <= 0) { 
			e->destroy();
		}
	}
}

void Game::sRender()
{
	
	m_window.clear();

	if (counter_right_click % 2 && flag_special_weapon)
	{
		renderSpecialWeapon3Line();
	}

	for (auto& e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		e->cShape->circle.rotate(5.5f);

		m_window.draw(e->cShape->circle);
	}

	m_text.setString("SCORE : " + std::to_string (m_score));
	m_window.draw(m_text);

	switch (counter_special_weapon)
	{
	case 1:
		m_textSW.setString("Speacial Weapon 1 : Activated !");
		m_window.draw(m_textSW);
		break;

	case 2:
		m_textSW.setString("Speacial Weapon 2 : Activated !");
		m_window.draw(m_textSW);
		break;

	case 3:
		m_textSW.setString("Speacial Weapon 3 : Activated !");
		m_window.draw(m_textSW);
		break;

	default:
		m_textSW.setString("No Speacial Weapon Activated !");
		m_window.draw(m_textSW);
		break;
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	// use (m_currentFrame - m_lastEnemySpawnTime) to determine
	// how long it has been since last enemy spawned
	// using config file determine after how much time enemies 
	// should spawn.

	if (m_lastEnemySpawnTime + m_enemyConfig.SP <= m_currentFrame) {
		spawnEnemy();
	}
}

void Game::sCollision()
{
	// use collision radius not shape radius

	// TODO : for enemy and enemy
	//
	// for player and enemy
	// for enemy and bullet
	// for player and wall
	// for enemy and wall

	for (auto& e : m_entities.getEntities())
	{
		if (e->tag() == "enemy")
		{
			for (auto& f : m_entities.getEntities("player"))
			{
				if (e->cTransform->pos.distSq(f->cTransform->pos) <= (e->cCollision->radius + f->cCollision->radius) * (e->cCollision->radius + f->cCollision->radius))
				{
					spawnSmallEnemies(e);
					e->destroy();
					if (f == m_player)
					{
						f->cTransform->pos = Vec2(m_windowConfig.W / 2.0f, m_windowConfig.H / 2.0f);
						m_score -= 1000;
						addScoreDecAnimation();
						if (m_score < 0) { m_score = 0; }

						// checking for special weapon 2
						for (auto& ep : m_entities.getEntities("player"))
						{
							if (ep == m_player) { continue; }
							ep->destroy();
						}
					}
					else
					{
						f->destroy();
						spawnSmallEnemies(f);
					}
				}
			}

			for (auto& f : m_entities.getEntities("bullet"))
			{
				if (e->cTransform->pos.distSq(f->cTransform->pos) <= (e->cCollision->radius + f->cCollision->radius) * (e->cCollision->radius + f->cCollision->radius))
				{
					spawnSmallEnemies(e);
					e->destroy();
					f->destroy();
					if (e->cScore) {
						m_score += e->cScore->score;
					}
				}
			}

			for (auto& f : m_entities.getEntities("enemy"))
			{
				Vec2 C1C2 = f->cTransform->pos - e->cTransform->pos;
				float collisionLength = e->cCollision->radius + f->cCollision->radius - C1C2.dist(Vec2());
				
				if (e != f && collisionLength >= 0)
				{
					C1C2.normalize();
					Vec2 vRel = e->cTransform->velocity - f->cTransform->velocity;
					float speed = Vec2::dotProduct(C1C2, vRel);

					// no concept of mass here, so taking a default mass for calculation
					float mass1 = 25.0f, mass2 = 25.0f;

					if (speed >= 0)
					{
						float impulse = 2.0f * speed / (mass1 + mass2);

						e->cTransform->velocity.x -= (impulse * mass2 * C1C2.x);
						e->cTransform->velocity.y -= (impulse * mass2 * C1C2.y);
						f->cTransform->velocity.x += (impulse * mass1 * C1C2.x);
						f->cTransform->velocity.y += (impulse * mass1 * C1C2.y);
					}

					// no two entity go inside each other
					C1C2 *= collisionLength / 2.0f;
					f->cTransform->pos += C1C2;
					e->cTransform->pos -= C1C2;
					


					// cap the max velocity of balls
					// if (e->cTransform->velocity.dist(Vec2()) > m_enemyConfig.VMAX) e->cTransform->velocity.changeMagnitude(m_enemyConfig.VMAX);
					// if (f->cTransform->velocity.dist(Vec2()) > m_enemyConfig.VMAX) f->cTransform->velocity.changeMagnitude(m_enemyConfig.VMAX);

					// check if no entity after collision goes out of bounds
					if (e->cTransform->pos.x >= m_window.getSize().x - e->cCollision->radius) e->cTransform->pos.x = m_window.getSize().x - e->cCollision->radius - 1;
					if (e->cTransform->pos.y >= m_window.getSize().y - e->cCollision->radius) e->cTransform->pos.y = m_window.getSize().y - e->cCollision->radius - 1;

					if (f->cTransform->pos.x >= m_window.getSize().x - f->cCollision->radius) f->cTransform->pos.x = m_window.getSize().x - f->cCollision->radius - 1;
					if (f->cTransform->pos.y >= m_window.getSize().y - f->cCollision->radius) f->cTransform->pos.y = m_window.getSize().y - f->cCollision->radius - 1;


					if (e->cTransform->pos.x <= 0 + e->cCollision->radius) e->cTransform->pos.x = 1 + e->cCollision->radius;
					if (e->cTransform->pos.y <= 0 + e->cCollision->radius) e->cTransform->pos.y = 1 + e->cCollision->radius;

					if (f->cTransform->pos.x <= 0 + f->cCollision->radius) f->cTransform->pos.x = 1 + f->cCollision->radius;
					if (f->cTransform->pos.x <= 0 + f->cCollision->radius) f->cTransform->pos.x = 1 + f->cCollision->radius;

					// spawnSmallEnemies(e);
				}
			}

			if (e->cTransform->pos.x >= m_windowConfig.W - e->cCollision->radius || e->cTransform->pos.x <= e->cCollision->radius)
			{
				e->cTransform->velocity.x *= -1;
			}
			if (e->cTransform->pos.y >= m_windowConfig.H - e->cCollision->radius || e->cTransform->pos.y <= e->cCollision->radius)
			{
				e->cTransform->velocity.y *= -1;
			}
		}
		if (e->tag() == "player")
		{
			if (e->cTransform->pos.x >= m_windowConfig.W - e->cCollision->radius)
			{
				e->cTransform->pos.x = m_windowConfig.W - e->cCollision->radius - 1;
			}
			if (e->cTransform->pos.x <= e->cCollision->radius)
			{
				e->cTransform->pos.x = e->cCollision->radius + 1;
			}
			if (e->cTransform->pos.y >= m_windowConfig.H - e->cCollision->radius)
			{
				e->cTransform->pos.y = m_windowConfig.H - e->cCollision->radius - 1;
			}
			if (e->cTransform->pos.y <= e->cCollision->radius)
			{
				e->cTransform->pos.y = e->cCollision->radius + 1;
			}
		}
		
	}
}


void Game::spawnPlayer()
{
	auto e = m_entities.addEntity("player");

	e->cTransform = std::make_shared<CTransform>(Vec2(m_windowConfig.W / 2.0f, m_windowConfig.H / 2.0f), Vec2(), 0.0f);
	e->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	e->cInput = std::make_shared<CInput>();
	e->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

	m_player = e;
}

void Game::spawnEnemy()
{
	// spwan enemy

	srand((unsigned int)time(0));
	
	float speed = m_enemyConfig.SMIN + (rand() % (int)(1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN));
	int vertices = m_enemyConfig.VMIN + (rand() % (int)(1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));
	sf::Color color(rand() % 256, rand() % 256, rand() % 256);
	int enemy_x = m_enemyConfig.CR + (rand() & (int)(1 + (m_windowConfig.W - m_enemyConfig.CR-2) - m_enemyConfig.CR));
	int enemy_y = m_enemyConfig.CR + (rand() & (int)(1 + (m_windowConfig.H-m_enemyConfig.CR-2) - m_enemyConfig.CR));
	int enemy_Vx = m_enemyConfig.CR + (rand() & (int)(1 + (m_windowConfig.W - m_enemyConfig.CR-2) - m_enemyConfig.CR));
	int enemy_Vy = m_enemyConfig.CR + (rand() & (int)(1 + (m_windowConfig.H - m_enemyConfig.CR-2) - m_enemyConfig.CR));
	Vec2 enemy_pos((float)enemy_x, (float)enemy_y);
	Vec2 enemy_vel = Vec2((float)enemy_Vx, (float)enemy_Vy).normalize() * speed;
	
	auto e = m_entities.addEntity("enemy");

	e->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	e->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, color, sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	e->cTransform = std::make_shared<CTransform>(enemy_pos, enemy_vel, 0.0f);
	e->cScore = std::make_shared<CScore>(e->cShape->circle.getPointCount()*10);

	// record when the last enemy spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// number of small enemies = vertices of original enemies
	// small enemy is half the size of original
	// small enemies are worth double points of the original enemy

	int vertices = e->cShape->circle.getPointCount();
	float speed = e->cTransform->velocity.dist(Vec2());
	int seAngleDiff = 360/vertices;
	int seAngle = 0;

	for (int i = 0; i < vertices; i++)
	{
		float seAngleInRadians = 3.14159f * (float)seAngle / 180;
		Vec2 vel = Vec2(speed*(float)cos(seAngleInRadians), speed*(float)sin(seAngleInRadians));

		auto se = m_entities.addEntity("smallEnemy");
		
		se->cLifespane = std::make_shared<CLifespane>(m_enemyConfig.L);
		se->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius()/2, e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor() , e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		se->cTransform = std::make_shared<CTransform>(e->cTransform->pos, vel, seAngle);
		
		seAngle += seAngleDiff;
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> e, const Vec2& mousePos)
{
	// bullet from entity to target

	auto b = m_entities.addEntity("bullet");

	b->cCollision = std::make_shared<CCollision>(m_bulletconfig.CR);
	b->cLifespane = std::make_shared<CLifespane>(m_bulletconfig.L);
	b->cShape = std::make_shared<CShape>(m_bulletconfig.SR, m_bulletconfig.V, sf::Color(m_bulletconfig.FR, m_bulletconfig.FG, m_bulletconfig.FB), sf::Color(m_bulletconfig.OR, m_bulletconfig.OG, m_bulletconfig.OB), m_bulletconfig.OT);
	
	Vec2 vel = mousePos - e->cTransform->pos;
	vel *= (m_bulletconfig.S / vel.dist(Vec2(0, 0)));
	
	b->cTransform = std::make_shared<CTransform>(Vec2(e->cTransform->pos.x, e->cTransform->pos.y), vel, 0.0f);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> e, const Vec2& mousePos)
{
	// TODO
	// color (252, 194, 1)

	switch (counter_special_weapon)
	{
	case 1:
		specialWeapon1(e, mousePos);
		break;

	case 2:
		specialWeapon2(e, mousePos);
		break;

	case 3:
		specialWeapon3(e, mousePos);
		break;

	default:
		break;
	}
}

void Game::specialWeapon1(std::shared_ptr<Entity> e1, const Vec2& mousePos)
{
	e1->cTransform->pos = Vec2(mousePos);

	int vertices = 8;
	int bulletCount = 20;
	float speed = 8;
	int seAngleDiff = 360 / bulletCount;
	int seAngle = 0;

	for (int i = 0; i < bulletCount; i++)
	{
		float seAngleInRadians = 3.14159f * (float)seAngle / 180;
		Vec2 vel = Vec2(speed * (float)cos(seAngleInRadians), speed * (float)sin(seAngleInRadians));

		auto se = m_entities.addEntity("bullet");

		se->cCollision = std::make_shared<CCollision>(m_bulletconfig.CR);
		se->cLifespane = std::make_shared<CLifespane>(m_enemyConfig.L);
		se->cShape = std::make_shared<CShape>(m_bulletconfig.SR, vertices, e1->cShape->circle.getFillColor(), e1->cShape->circle.getOutlineColor(), e1->cShape->circle.getOutlineThickness());
		se->cTransform = std::make_shared<CTransform>(e1->cTransform->pos, vel, seAngle);

		seAngle += seAngleDiff;
	}
}

void Game::specialWeapon2(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	// TODO : spawn more players

	auto p = m_entities.addEntity("player");
	p->cTransform = std::make_shared<CTransform>(mousePos, Vec2(), 0.0f);
	p->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	p->cInput = std::make_shared<CInput>();
	p->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
}

void Game::specialWeapon3(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	counter_right_click++;
	if (counter_right_click % 2)
	{
		m_window.setFramerateLimit(15);
		
		float minDistSq = m_windowConfig.W * m_windowConfig.W * 4.0f;
		for (auto& e : m_entities.getEntities("enemy"))
		{
			if (mousePos.distSq(e->cTransform->pos) < minDistSq)
			{
				minDistSq = mousePos.distSq(e->cTransform->pos);
				minE = e;
				flag_special_weapon = true;
			}
		}
	}
	else
	{
		m_window.setFramerateLimit(60);

		if (flag_special_weapon)
		{
			Vec2 temp = minE->cTransform->pos;
			minE->cTransform->pos = m_player->cTransform->pos;
			m_player->cTransform->pos = temp;
		}
	}
}

void Game::renderSpecialWeapon3Line()
{
	line[0].position = sf::Vector2f(minE->cTransform->pos.x, minE->cTransform->pos.y);
	line[0].color = sf::Color::Magenta;
	line[1].position = sf::Vector2f(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	line[1].color = sf::Color::Black;

	m_window.draw(line, 2, sf::PrimitiveType::Lines, sf::RenderStates::Default);
}

void Game::addScoreDecAnimation()
{
	auto sc = m_entities.addEntity("scoreDec");
	sc->cTransform = std::make_shared<CTransform>(Vec2(155.0f ,20.0f), Vec2(), 0.0f);
	sc->cLifespane = std::make_shared<CLifespane>(60);
	sc->cShape = std::make_shared<CShape>(42.0f, 20, sf::Color::Transparent, sf::Color::White, 4.0f);
}