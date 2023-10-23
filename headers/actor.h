#ifndef ACTOR_H
#define ACTOR_H

// very basic class, meant for player and enemy to inherit from for common health and damage mechanics
class Actor
{
protected:

    float maxHealth;

    float currentHealth;

    bool dead = false;

public:
    Actor(float health);

    bool isDead();

    void takeDamage(float damage);

    void heal(float healing);
};
#endif