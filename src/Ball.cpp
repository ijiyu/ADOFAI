#include "Ball.h"

Ball::Ball(const Vector2<float> pos, const float r, const bool rotating, const color c)
    : pos(pos), r(r), rotating(rotating), c(c) {
}

void Ball::display() const {
    const Circle circle(pos.x, pos.y, r, c);
    circle.display();
}

void Ball::update() {
    if (rotating &&other != nullptr ) {
        const float outerR = (pos-other->pos).magnitude();
        theta += w * deltaTime;
        pos = other->pos + Vector2(outerR*std::cos(theta), outerR*std::sin(theta));
    }
}