#include "Projectile.h"
#include <cmath>

Projectile::Projectile(int lane, const glm::vec2& pos, float speed, bool friendly,
                       AttackHeight height, int damage)
    : lane(lane),
      position(pos),
      speed(speed),
      friendly(friendly),
      attackHeight(height),
      damage(damage),
      active(true),
      animTimer(0.0f)
{
}

void Projectile::Update(float dt) {
    if (!active) return;

    animTimer += dt;
    position.x += speed * dt;

    // Out of screen bounds check (window is 800 wide)
    if (position.x < -50.0f || position.x > 850.0f) {
        active = false;
    }
}

BoundingBox Projectile::GetBounds() const {
    if (friendly) {
        return BoundingBox(position.x - 10.0f, position.y - 4.0f, 20.0f, 8.0f);
    } else if (attackHeight == AttackHeight::HIGH) {
        return BoundingBox(position.x - 8.0f, position.y - 8.0f, 16.0f, 16.0f);
    } else {
        return BoundingBox(position.x - 8.0f, position.y - 8.0f, 16.0f, 16.0f);
    }
}

void Projectile::Render(Renderer& renderer) {
    if (!active) return;

    if (friendly) {
        // Royal Blue Player Arrow / Magic Shard (Composite: wooden shaft + steel head + fletching)
        // Shaft
        renderer.DrawQuad(position - glm::vec2(8.0f, 1.5f), glm::vec2(16.0f, 3.0f),
                          glm::vec4(0.85f, 0.70f, 0.40f, 1.0f));
        // Steel / Magic Arrowhead
        glm::vec2 tip = position + glm::vec2(12.0f, 0.0f);
        glm::vec2 top = position + glm::vec2(6.0f, -4.0f);
        glm::vec2 bot = position + glm::vec2(6.0f, 4.0f);
        renderer.DrawTriangle(tip, top, bot, glm::vec4(0.3f, 0.65f, 1.0f, 1.0f)); // Royal Blue glow
        // Fletching (Blue feathers)
        renderer.DrawTriangle(position - glm::vec2(10.0f, 0.0f),
                              position - glm::vec2(6.0f, -3.0f),
                              position - glm::vec2(6.0f, 3.0f),
                              glm::vec4(0.2f, 0.45f, 0.9f, 1.0f));
    } else {
        if (attackHeight == AttackHeight::HIGH) {
            // Upper Ranged: Fiery Arbalest / Catapult Bomb (must duck!)
            // Core fireball + flaming outer rim + spark trail
            float pulse = 0.5f + 0.5f * sinf(animTimer * 12.0f);
            renderer.DrawCircle(position, 8.0f + pulse * 2.0f, glm::vec4(0.95f, 0.35f, 0.1f, 1.0f), 12);
            renderer.DrawCircle(position, 4.5f, glm::vec4(1.0f, 0.9f, 0.2f, 1.0f), 10);
            // Fiery tail particles
            renderer.DrawCircle(position + glm::vec2(7.0f, -2.0f), 3.0f, glm::vec4(0.85f, 0.15f, 0.1f, 0.7f), 8);
        } else {
            // Lower Ranged: Rolling Spiked Bomb / Low Spear (must jump!)
            // Iron outer ring + spikes + center bolt
            renderer.DrawCircle(position, 7.5f, glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 12);
            renderer.DrawCircle(position, 3.5f, glm::vec4(0.75f, 0.20f, 0.15f, 1.0f), 8);
            // Ground dust / sparks
            renderer.DrawQuad(position + glm::vec2(3.0f, 4.0f), glm::vec2(6.0f, 3.0f),
                              glm::vec4(0.5f, 0.4f, 0.3f, 0.6f));
        }
    }
}

