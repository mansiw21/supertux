//  $Id$
//
//  Dart - Your average poison dart
//  Copyright (C) 2006 Christoph Sommer <supertux@2006.expires.deltadevelopment.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <config.h>

#include "dart.hpp"

namespace {
  const float SPEED = 200;
}

Dart::Dart(const lisp::Lisp& reader) : set_direction(false), parent(0), soundSource(0)
{
  reader.get("x", start_position.x);
  reader.get("y", start_position.y);
  sprite = sprite_manager->create("images/creatures/dart/dart.sprite");
  bbox.set_size(sprite->get_current_hitbox_width(), sprite->get_current_hitbox_height());
  physic.enable_gravity(false);
  countMe = false;
}

Dart::Dart(float pos_x, float pos_y, Direction d, const BadGuy* parent = 0) : set_direction(true), initial_direction(d), parent(parent), soundSource(0)
{
  start_position.x = pos_x;
  start_position.y = pos_y;
  sprite = sprite_manager->create("images/creatures/dart/dart.sprite");
  bbox.set_size(sprite->get_current_hitbox_width(), sprite->get_current_hitbox_height());
  physic.enable_gravity(false);
  countMe = false;
}

Dart::Dart(const Dart& other)
	: BadGuy(other), set_direction(other.set_direction), initial_direction(other.initial_direction), parent(other.parent)
{
  soundSource = sound_manager->create_sound_source("sounds/flame.wav");
}

Dart::~Dart()
{
  delete soundSource;
}

bool 
Dart::updatePointers(const GameObject* from_object, GameObject* to_object)
{
  if (from_object == parent) {
    parent = dynamic_cast<Dart*>(to_object);
    return true;
  }
  return false;
}

void
Dart::write(lisp::Writer& writer)
{
  writer.start_list("dart");
  writer.write_float("x", start_position.x);
  writer.write_float("y", start_position.y);
  writer.end_list("dart");
}

void
Dart::activate()
{  
  if (set_direction) dir = initial_direction;
  physic.set_velocity_x(dir == LEFT ? -::SPEED : ::SPEED);
  sprite->set_action(dir == LEFT ? "flying-left" : "flying-right");

  delete soundSource;
  soundSource = sound_manager->create_sound_source("sounds/flame.wav");
  if(soundSource) {
    soundSource->set_position(get_pos());
    soundSource->set_looping(true);
    soundSource->set_gain(1.0);
    soundSource->set_reference_distance(32);
    soundSource->play();
  } else {
    log_warning << "Couldn't start Dart ambient sound" << std::endl;
  }
}

void
Dart::deactivate()
{  
  delete soundSource;
  soundSource = 0;
  remove_me();
}

void 
Dart::active_update(float elapsed_time)
{
  BadGuy::active_update(elapsed_time);
  if (soundSource) soundSource->set_position(get_pos());
}


HitResponse 
Dart::collision_solid(GameObject& , const CollisionHit& )
{
  sound_manager->play("sounds/stomp.wav", get_pos());
  remove_me();
  return ABORT_MOVE;
}

HitResponse 
Dart::collision_badguy(BadGuy& badguy, const CollisionHit& )
{
  // ignore collisions with parent
  if (&badguy == parent) {
    return FORCE_MOVE;
  }
  sound_manager->play("sounds/stomp.wav", get_pos());
  remove_me();
  badguy.kill_fall();
  return ABORT_MOVE;
}

HitResponse 
Dart::collision_player(Player& player, const CollisionHit& hit)
{
  sound_manager->play("sounds/stomp.wav", get_pos());
  remove_me();
  return BadGuy::collision_player(player, hit);
}

IMPLEMENT_FACTORY(Dart, "dart")

