
#include <cmath>
#include <assert.h>
#include "Player.h"




Player::Player(vec3 position, float radius):radius(radius) {

    this->position = vec3(position.x, position.y - 1.5f, position.z);
	
	}

Player::~Player(){}


/* getters and setters */


vec3 Player::getPosition() const{

	return this->position;
}
vec3 Player::getView() const{

	return this->view;
}


float Player::getRadius() const{
return this->radius;

}

void Player::setPosition(vec3 position){

this->position.x = position.x;
this->position.z = position.z;

}

void Player::setView(vec3 view){
	this->view = view;

}


void Player::setRadius(float radius){
	this->radius=radius;

}





