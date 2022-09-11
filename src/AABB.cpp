//
// Created by Michael Stengel on 9/7/2022.
//

#include "AABB.h"
bool AABB::intersects(const AABB& first) const {
  const auto second = this;

  const bool collision_x = first.m_position.x + first.m_size.x >= second->m_position.x &&
                          second->m_position.x + second->m_size.x >= first.m_position.x;

  const bool collision_y = first.m_position.y + first.m_size.y >= second->m_position.y &&
                          second->m_position.y + second->m_size.y >= first.m_position.y;

  return collision_x && collision_y;
}
