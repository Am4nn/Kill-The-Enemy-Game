#pragma once

#include "Components.h"

class Entity
{
public:

	friend class EntityManager;

	// Component pointers
	std::shared_ptr<CTransform> cTransform; // CTransform *cTransform = NULL;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespane> cLifespane;

	// private member access functions
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();

private:
	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "default";

	// constructor and destructor
	Entity(const size_t id, const std::string& tag);
};