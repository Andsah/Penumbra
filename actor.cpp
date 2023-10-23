#include "headers/actor.h"

Actor::Actor(float health):
maxHealth(health){}

bool Actor::isDead()
{
    return dead;
}

void Actor::takeDamage(float damage)
{
    if(!dead) {
        currentHealth -= damage;
        if(currentHealth <= 0) {
            currentHealth = 0;
            dead = true;
        }
    }
}

void Actor::heal(float healing)
{
    if(!dead) {
        currentHealth += healing;
    }
}
