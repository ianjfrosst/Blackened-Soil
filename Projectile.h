#include "Vector2D.h"
#include "GameObject.h"


class Projectile : GameObject {
	
public :
	virtual void render(sf::RenderWindow &window) override;
};
