//
// Created by Michael Stengel on 9/7/2022.
//

#include "AABB.h"
bool AABB::intersects(AABB& first) {
  auto second = *this;

  bool collisionX = first.m_Position.x + first.m_Size.x >= second.m_Position.x &&
                    second.m_Position.x + second.m_Size.x >= first.m_Position.x;

  bool collisionY = first.m_Position.y + first.m_Size.y >= second.m_Position.y &&
                    second.m_Position.y + second.m_Size.y >= first.m_Position.y;

  return collisionX && collisionY;
}
