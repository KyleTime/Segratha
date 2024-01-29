#include "HitPoints.h"
#include "../GameObject.h"

using namespace Segratha;

HitPoints::HitPoints(float max, bool destroyOnDeath)
{
    maxHitPoints = max;
    hitPoints = max;
    this->destroyOnDeath = destroyOnDeath;
}

void HitPoints::Awake()
{

}

void HitPoints::Update()
{

}

void HitPoints::Draw(sf::RenderWindow* target)
{
    
}

std::string HitPoints::GetComponentID()
{
    return "HitPoints";
}

float HitPoints::GetCurrentHitPoints()
{
    return hitPoints;
}

void HitPoints::Damage(float hp)
{
    hitPoints -= hp;

    if(hitPoints < 0)
    {
        hitPoints = 0;
        Die();
    }
}

void HitPoints::Heal(float hp)
{
    hitPoints += hp;

    if(hitPoints > maxHitPoints)
    {
        hitPoints = maxHitPoints;
    }
}

void HitPoints::Set(float hp)
{
    //LMAOOOOOOOOO
    //this is supposed to be a clamp
    hitPoints = hp <= maxHitPoints && hp >= 0 ? hp : hp > maxHitPoints ? maxHitPoints : 0;
}

void HitPoints::Die()
{
    if(destroyOnDeath)
        gameObject->Die();
}