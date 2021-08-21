#define _VECTOR_CHASE

#ifdef _VECTOR_CHASE
#include"libOne.h"
class CIRCLE {
    COLOR Color;
    float Radius;
    VECTOR2 Pos, Vel, Acc;
    float Restitution;
    float MaxSpeed=0;
    float MaxForce=0;
    float NearDistance=0;
public:
    CIRCLE(COLOR color, float radius, float px, float py, float restitution=1) {
        Color = color;
        Radius = radius;
        Pos.x = px; Pos.y = py;
        Restitution = restitution;
    }
    const VECTOR2& pos() {
        return Pos;
    }
    void setMaxSpeed(float maxSpeed) {
        MaxSpeed = maxSpeed;
    }
    void setMaxForce(float maxForce) {
        MaxForce = maxForce;
    }
    void setNearDistance(float nearDistance) {
        NearDistance = nearDistance;
    }
    void chase(const VECTOR2& target) {
        VECTOR2 acc = target - Pos;
        float speed = MaxSpeed;
        float distance = acc.mag();
        if (distance < NearDistance) {
            speed = map(distance, 0, NearDistance, 0, MaxSpeed);
        }
        acc.setMag(speed);
        acc -= Vel;
        acc.limmit(MaxForce);
        applyForce(acc);
    }
    void runAway(const VECTOR2& target) {
        VECTOR2 acc = Pos - target;
        if (acc.mag() < NearDistance) {
            acc.setMag(MaxSpeed);
            acc -= Vel;
            acc.limmit(MaxForce);
            applyForce(acc);
        }
    }
    void applyForce(const VECTOR2& force) {
        Acc += force;
    }
    void move() {
        Vel += Acc;
        Pos += Vel;
        Acc *= 0;
    }
    void edge() {
        if (Pos.x > width + Radius || Pos.x < -Radius) {
            Pos.x = random(Radius, width - Radius);
            Pos.y = -random(50, 100);
            Vel *= 0;
        }
        if (Pos.y > height - Radius) {
            Pos.y = height - Radius;
            Vel.y *= -Restitution;
        }
    }
    void edgeStay() {
        if (Pos.x < Radius)Pos.x = Radius;
        if (Pos.x > width - Radius)Pos.x = width - Radius;
        if (Pos.y < Radius)Pos.y = Radius;
        if (Pos.y > height - Radius)Pos.y = height - Radius;
    }
    void collision(CIRCLE& other) {
        if (this != &other) {
            VECTOR2 v = Pos - other.Pos;
            float dist = Radius + other.Radius;
            if (v.sqMag() < dist * dist) {
                Pos = other.Pos + v.normalize() * dist;
                Vel = v.dot(-Vel) * 2 * v + Vel;
                Vel *= Restitution;
            }
        }
    }
    void draw() {
        colorMode(HSV);
        noStroke();
        fill(Color);
        circle(Pos.x, Pos.y, Radius * 2);
    }
    void printAcc() {
        print(Acc.mag());
    }
};
#include<vector>
void gmain() {
    window(1920, 1080, full);
    //hideCursor();

    std::vector<CIRCLE> balls;
    COLOR color(0, 128, 255);
    float radius = 120;
    int num = 12;
    for (int i = 0; i < num; i++) {
        color.r = 6.28f / num * i;//hue
        float r = radius - radius / num * i;//半径を小さくしていく
        balls.emplace_back(color, r, width/2, height/2);
    }
    
    while (notQuit) {
        fill(0,0,0);
        rect(0, 0, width, height);
        VECTOR2 target(mouseX,mouseY);
        for (CIRCLE& ball : balls) {
            ball.setMaxSpeed(10);
            ball.setMaxForce(2);
            ball.setNearDistance(100);
            ball.chase(target);
            ball.move();
            ball.draw();
            target = ball.pos();
        }
    }
}
#endif

#ifdef _VECTOR_FORCE
#include"libOne.h"
class CIRCLE {
    COLOR Color;
    float Radius;
    VECTOR2 Pos, Vel, Acc;
    float Restitution;
public:
    CIRCLE(COLOR color, float radius, float px, float py, float restitution) {
        Color = color; 
        Radius = radius;
        Pos.x = px; Pos.y = py; 
        Restitution = restitution;
    }
    void applyForce(const VECTOR2& force) {
        Acc += force;
    }
    void move() {
        Vel += Acc;
        Pos += Vel;
        Acc *= 0;
    }
    void edge() {
        if (Pos.x > width + Radius || Pos.x < -Radius) {
            Pos.x = random(Radius,width-Radius);
            Pos.y = -random(50, 100);
            Vel *= 0;
        }
        if (Pos.y > height - Radius) {
            Pos.y = height - Radius;
            Vel.y *= -Restitution;
        }
    }
    void collision(CIRCLE& other) {
        if (this != &other) {
            VECTOR2 v = Pos - other.Pos;
            float dist = Radius + other.Radius;
            if (v.sqMag() < dist * dist) {
                Pos = other.Pos + v.normalize() * dist;
                Vel = v.dot(-Vel) * 2 * v + Vel;
                Vel *= Restitution;
            }
        }
    }
    void draw() {
        noStroke();
        fill(Color);
        circle(Pos.x, Pos.y, Radius * 2);
    }
};
#include<vector>
void gmain() {
    window(1100, 1080, full);
    hideCursor();
    colorMode(HSV);
    std::vector<CIRCLE> balls;
    for (int i = 0; i < 30; i++) {
        COLOR color(random()%7, 128, 255);
        float radius = 20;
        float px = random(radius, width - radius);
        float py = -random(50, 100);
        float restitution = 0.5f;
        balls.emplace_back(color, radius, px, py, restitution);
    }
    std::vector<CIRCLE> nails;
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 10; i++) {
            COLOR color(2, 128, 180);
            float radius = 5;
            float px = 100 + 100 * i;
            float py = 200 + 100 * j;
            float restitution = 0;
            nails.emplace_back(color, radius, px, py, restitution);
        }
    }

    while (notQuit) {
        clear();
        fill(80);
        rect(0, 0, width, height);

        VECTOR2 gravity(0, 0.2f);
        VECTOR2 wind;
        if (isPress(KEY_D))wind.x = 0.5f;
        if (isPress(KEY_A))wind.x = -0.5f;
        for (CIRCLE& ball : balls) {
            ball.applyForce(gravity);
            ball.applyForce(wind);
            ball.move();
            for (CIRCLE& other : balls) {
                ball.collision(other);
            }
            for (CIRCLE& nail : nails) {
                ball.collision(nail);
            }
            ball.edge();
            ball.draw();
        }
        for (CIRCLE& nail : nails) {
            nail.draw();
        }
    }
}
#endif

#ifdef _VECTOR_COLLISION
#include"libOne.h"
class SEGMENT {
    VECTOR2 Sp, Ep;
    VECTOR2 N;//垂直ベクトル
public:
    SEGMENT(float spx, float spy, float epx, float epy) {
        Sp.x = spx; Sp.y = spy; Ep.x = epx; Ep.y = epy;
        VECTOR2 v = Ep - Sp;
        N.x = -v.y;
        N.y = v.x;
        N.normalize();
    }
    void draw() {
        strokeWeight(5);
        stroke(0);
        mathLine(Sp.x, Sp.y, Ep.x, Ep.y);
    }
    const VECTOR2& n(){ return N; }
    const VECTOR2& sp(){ return Sp; }
    const VECTOR2& ep(){ return Ep; }
};
class BALL {
    VECTOR2 Pos, Vel;
    float Speed = 0.02f;
    float Radius = 0.1f;
    float Hue = 0;
public:
    BALL(float px, float py, float vx, float vy, float sp, float r, float hue) {
        Pos.x = px; Pos.y = py; Vel.x = vx; Vel.y = vy; 
        Speed = sp;
        Radius = r;
        Hue = hue;
    }
    void setPos(const VECTOR2& pos) { 
        Pos = pos; 
    }
    void setVel(const VECTOR2& vel) {
        Vel = vel;
        Vel.setMag(Speed);
    }
    void manual() {
        Vel.x = Vel.y = 0;
        if (isPress(KEY_D))Vel.x = 1;
        if (isPress(KEY_A))Vel.x = -1;
        if (isPress(KEY_W))Vel.y = 1;
        if (isPress(KEY_S))Vel.y = -1;
        Vel.setMag(Speed);
    }
    void move() {
        Pos += Vel;
    }
    void collision(SEGMENT& seg) {
        const VECTOR2& n = seg.n();
        VECTOR2 v1 = Pos - seg.sp();
        float dist = n.dot(v1);
        if (absolute(dist) < Radius) {
            VECTOR2 v2 = Pos - seg.ep();
            if (n.cross(v1) < 0 && n.cross(v2) > 0) {
                float excess = Radius - absolute(dist);
                if(dist>0)
                    Pos += n * excess;
                else
                    Pos += -n * excess;
                calcReflectVel(n);
            }
            else if (v1.sqMag() < Radius * Radius) {
                Pos = seg.sp() + v1.normalize() * Radius;
                calcReflectVel(v1);
            }
            else if (v2.sqMag() < Radius * Radius) {
                Pos = seg.ep() + v2.normalize() * Radius;
                calcReflectVel(v2);
            }
        }
    }
    void calcReflectVel(const VECTOR2& n) {
        Vel = n.dot(-Vel) * 2 * n + Vel;
    }
    void draw() {
        noStroke();
        colorMode(HSV);
        fill(Hue, 128, 255, 220);
        mathCircle(Pos.x, Pos.y, Radius * 2);
    }
};
#include<vector>
void gmain() {
    window(1000, 1000);
    
    float p[] = {
        -1.8f, -0.2f,
         1.0f, -1.5f,
         2.0f,  1.0f,
        -4.0f,  2.0f,
        -1.0f, -4.0f,
         3.0f, -1.0f,
    };
    std::vector<SEGMENT> segments;
    int numLine = sizeof(p) / 8 - 1;
    for (int i = 0; i < numLine; i++) {
        segments.emplace_back(
            p[i * 2 + 0],
            p[i * 2 + 1],
            p[i * 2 + 2],
            p[i * 2 + 3]);
    }
    
    std::vector<BALL> balls;
    for (int i = 0; i < 50; i++) {
        balls.emplace_back(
            random(-1, 1),
            random(-1, 1),
            random(-0.04f, 0.04f),
            random(-0.04f, 0.04f),
            0,
            random(0.1f, 0.3f),
            random(6.28f)
        );
    }
    while (notQuit) {
        clear(60);
        mathAxis(5,60);

        for (BALL& ball : balls) {
            ball.move();
            for (SEGMENT& segment : segments) {
                ball.collision(segment);
            }
            ball.draw();
        }
        for (SEGMENT& segment : segments) {
            segment.draw();
        }
    }
}
#endif

#ifdef _VECTOR_COLLISION_1
#include"libOne.h"
class SEGMENT {
    VECTOR2 Sp, Ep;
    VECTOR2 N;//垂直ベクトル
public:
    SEGMENT(float spx, float spy, float epx, float epy) {
        Sp.x = spx; Sp.y = spy; Ep.x = epx; Ep.y = epy;
        VECTOR2 v = Ep - Sp;
        N.x = -v.y;
        N.y = v.x;
        N.normalize();
    }
    void draw() {
        strokeWeight(5);
        stroke(0);
        mathLine(Sp.x, Sp.y, Ep.x, Ep.y);
    }
    const VECTOR2& n() { return N; }
    const VECTOR2& sp() { return Sp; }
    const VECTOR2& ep() { return Ep; }
};
class BALL {
    VECTOR2 Pos, Vel;
    float Speed = 0.02f;
    float Radius = 0.1f;
    float Hue = 0;
public:
    BALL(float px, float py, float vx, float vy, float sp, float r, float hue) {
        Pos.x = px; Pos.y = py; Vel.x = vx; Vel.y = vy;
        Speed = sp;
        Radius = r;
        Hue = hue;
    }
    void setPos(const VECTOR2& pos) { Pos = pos; }
    void setVel(const VECTOR2& vel) {
        Vel = vel;
        Vel.setMag(Speed);
    }
    void manual() {
        Vel.x = Vel.y = 0;
        if (isPress(KEY_D))Vel.x = 1;
        if (isPress(KEY_A))Vel.x = -1;
        if (isPress(KEY_W))Vel.y = 1;
        if (isPress(KEY_S))Vel.y = -1;
        Vel.setMag(Speed);
    }
    void move() {
        Pos += Vel;
    }
    void collision(SEGMENT& seg) {
        const VECTOR2& n = seg.n();
        VECTOR2 v1 = Pos - seg.sp();
        float dist = n.dot(v1);
        if (absolute(dist) < Radius) {
            VECTOR2 v2 = Pos - seg.ep();
            if (n.cross(v1) < 0 && n.cross(v2) > 0) {
                float excess = Radius - absolute(dist);
                if (dist > 0)
                    Pos += n * excess;
                else
                    Pos += -n * excess;
                calcReflectVel(n);
            }
            else if (v1.sqMag() < Radius * Radius) {
                Pos = seg.sp() + v1.normalize() * Radius;
                calcReflectVel(v1);
            }
            else if (v2.sqMag() < Radius * Radius) {
                Pos = seg.ep() + v2.normalize() * Radius;
                calcReflectVel(v2);
            }
        }
    }
    void calcReflectVel(const VECTOR2& n) {
        Vel = n.dot(-Vel) * 2 * n + Vel;
    }
    void draw() {
        noStroke();
        colorMode(HSV);
        fill(Hue, 128, 255, 220);
        mathCircle(Pos.x, Pos.y, Radius * 2);
    }
};
#include<vector>
void gmain() {
    window(1000, 1000);
    BALL ball(0, 1, 0, 0, 0.02f, 0.3f, 0);
    SEGMENT segment(-1, 0.5f, 1, -0.5f);
    while (notQuit) {
        clear(200);
        mathAxis(3,128);
        ball.manual();
        ball.move();
        ball.collision(segment);
        ball.draw();
        segment.draw();
    }
}
#endif

#ifdef _VECTOR_REFLECT
#include"libOne.h"
class SEGMENT {
    VECTOR2 Sp, Ep;
    VECTOR2 N;//垂直ベクトル
public:
    SEGMENT(float spx, float spy, float epx, float epy) {
        Sp.x = spx; Sp.y = spy; Ep.x = epx; Ep.y = epy;
        VECTOR2 v = Ep - Sp;
        N.x = -v.y;
        N.y = v.x;
        N.normalize();
    }
    void draw() {
        stroke(0);
        mathLine(Sp.x, Sp.y, Ep.x, Ep.y);
        stroke(255, 0, 0);
        mathArrow(0, 0, N.x, N.y);
    }
    const VECTOR2& n() { return N; }
    const VECTOR2& sp() { return Sp; }
};
class BALL {
    VECTOR2 Pos, Vel;
    float Speed = 0.02f;
    float Radius = 0.1f;
public:
    void setPos(const VECTOR2& pos) { Pos = pos; }
    void setVel(const VECTOR2& vel) { 
        Vel = vel; 
        Vel.setMag(Speed);
    }
    void move() {
        Pos += Vel;
    }
    void collision(const VECTOR2& pos, const VECTOR2& n) {
        VECTOR2 v = Pos - pos;
        float dist = n.dot(v);
        if (absolute(dist) < 0.01f) {
            Vel = n.dot(-Vel) * 2 * n + Vel;
        }
    }
    void draw() {
        fill(255, 255, 0);
        mathCircle(Pos.x, Pos.y, Radius * 2);
    }
};
void gmain() {
    window(1000, 1000);
    SEGMENT segment(-2, 5, 2, -5);
    VECTOR2 n = segment.n();
    BALL ball;
    while (notQuit) {
        clear(180);
        mathAxis(5);
        strokeWeight(5);
        VECTOR2 mouse(mathMouseX, mathMouseY);
        VECTOR2 vel = -mouse;
        VECTOR2 ref = n.dot(-vel) * 2 * n + vel;
        
        //マウスから原点へのベクトル
        stroke(0, 180, 0);
        mathArrow(mouse.x, mouse.y, 0, 0);
        //速度ベクトル
        stroke(128);
        mathArrow(0, 0, vel.x, vel.y);
        //反射ベクトル
        stroke(0, 0, 255);
        mathArrow(0, 0, ref.x, ref.y);

        ////垂線ベクトルと速度の逆ベクトルの内積　* 垂線ベクトル
        stroke(60);
        VECTOR2 dn = n.dot(-vel) * n;
        mathArrow(0, 0, dn.x, dn.y);
        mathLine(-vel.x, -vel.y, dn.x, dn.y);
        //2倍する
        VECTOR2 dn2 = dn * 2;
        mathArrow(0, 0, dn2.x, dn2.y);
        //速度ベクトルを加算する
        mathArrow(0+dn2.x, 0+dn2.y, vel.x+dn2.x, vel.y+dn2.y);

        segment.draw();

        if (isTrigger(MOUSE_LBUTTON)) {
            ball.setPos(mouse);
            ball.setVel(vel);
        }
        ball.move();
        ball.collision(segment.sp(), segment.n());
        ball.draw();
    }
}
#endif

#ifdef _VECTOR_SUB
//視覚化して理解するベクトルの引き算
#include"libOne.h"
void gmain() {
    window(1000, 1000, full);
    while (notQuit) {
        clear(60);
        fill(180);
        stroke(0);
        rect(0, 0, width, height);
        mathAxis(3.1f);
        VECTOR2 a(-1, 1);
        VECTOR2 b(mathMouseX, mathMouseY);
        VECTOR2 c = a - b;
        strokeWeight(5);
        //va
        stroke(255, 0, 0);
        mathArrow(0, 0, a.x, a.y);
        //vb
        stroke(0, 200, 0);
        mathArrow(0, 0, b.x, b.y);
        stroke(0, 200, 0, 80);
        mathArrow(0 + a.x, 0 + a.y, -b.x + a.x, -b.y + a.y);
        //vc
        stroke(0, 0, 255);
        mathArrow(0, 0, c.x, c.y);
        stroke(0, 0, 255, 80);
        mathArrow(b.x, b.y, a.x, a.y);

        fill(0);
        textSize(50);
        text("c = a - b", 10, 60);
        mathText("a", a.x, a.y);
        mathText("b", b.x, b.y);
        mathText("c", c.x, c.y);
    }
}
#endif

#ifdef _VECTOR_ADD
//視覚化して理解するベクトルの足し算
#include"libOne.h"
void gmain() {
    window(1000, 1000, full);
    while (notQuit) {
        clear(60);
        fill(180);
        stroke(0);
        rect(0, 0, width, height);
        mathAxis(3.1f);
        VECTOR2 a(-1, 1);
        VECTOR2 b( mathMouseX, mathMouseY);
        VECTOR2 c = a + b;
        strokeWeight(5);
        //va
        stroke(255, 0, 0);
        mathArrow(0, 0, a.x, a.y);
        //vb
        stroke(0, 200, 0);
        mathArrow(0, 0, b.x, b.y);
        stroke(0, 200, 0, 50);
        mathArrow(0+a.x, 0+a.y, b.x+a.x, b.y+a.y);
        //vc
        stroke(0, 0, 255);
        mathArrow(0, 0, c.x, c.y);

        fill(0);
        textSize(50);
        text("c = a + b", 10, 60);
        mathText("a", a.x, a.y);
        mathText("b", b.x, b.y);
        mathText("c", c.x, c.y);
    }
}
#endif
