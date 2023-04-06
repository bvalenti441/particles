// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include <random>
#include "agl/window.h"
#include <ctime>

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  bool exploded;
};

class viewer : public Window {
public:

  viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(300);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    // 10 firework "main" particles to draw from in an object pool
    for (int i = 0; i < 10; ++i) {
        Particle particle;
        particle.color = vec4(agl::randomUnitCube(), 0);
        particle.vel = vec3(0, 0.2f, 0);
        particle.size = 0.5f;
        particle.rot = 0;
        particle.exploded = false;
        mParticles.push_back(particle);
    }
    // other particles for trailing 
    for (int i = 10; i < size; ++i)
    {
        Particle particle;
        particle.size = 0.3f;
        particle.exploded = false;
        particle.rot = 0;
        particle.vel = agl::randomUnitCube();
        mParticles.push_back(particle);
    }
  }

  void updateConfetti()
  {   
      int onePerFrame = true;
      for (int i = 0; i < 10; ++i) 
      {
          if ((mParticles[i].color.a <= 0) && (frame % 100 == 0) && onePerFrame) {
              onePerFrame = false;
              mParticles[i].exploded = false;
              mParticles[i].pos = vec3(agl::random() - 2.0f, -3.0, 0);
              mParticles[i].vel = vec3(0.5f, 2.0f, 0);
              vec3 c = agl::randomUnitCube();
              float lightProp = std::max(std::max(1.0f - c.x, 1.0f - c.y), 1.0f - c.z);
              mParticles[i].color = vec4(c.x + (lightProp / 2.0f), c.y + (lightProp / 2.0f), c.z + (lightProp / 2.0), 1.0f);
          }
          else if ((mParticles[i].color.a == 1.0f) && (mParticles[i].exploded == false) && (agl::random() > 0.999f || mParticles[i].vel.y <= 0 || mParticles[i].pos.x >= 2.9f)) {
              mParticles[i].exploded = true;
              int trails = 0;
              for (int j = 10; j < mParticles.size(); ++j) {
                  if (mParticles[j].color.a <= 0 && trails < 10) {
                      trails++;
                      mParticles[j].color = mParticles[i].color;
                      mParticles[j].pos = mParticles[i].pos;
                      mParticles[j].vel = agl::randomUnitCube();
                  }
              }
          }
          else if (mParticles[i].exploded) {
              mParticles[i].color.a -= 0.01f;
          }
          else if (mParticles[i].color.a == 1.0f) {
              mParticles[i].pos += mParticles[i].vel * dt();
              mParticles[i].vel += vec3(0, -0.5f, 0) * dt();
          }
      }

      for (int j = 10; j < mParticles.size(); ++j) {
          if (mParticles[j].color.a > 0 && mParticles[j].pos.y > -3.0f) {
              mParticles[j].color.a -= 0.005;
              mParticles[j].pos += dt() * mParticles[j].vel;
              mParticles[j].vel += dt() * vec3(0, -0.5f, 0);
          }
      }
      frame++;
  }

  void drawConfetti()
  {
      renderer.loadTexture("particle", "../textures/ParticleFlare.png", 0);

      for (int i = 0; i < mParticles.size(); i++)
      {
        Particle particle = mParticles[i];
        renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
      }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    updateConfetti();
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  int frame = 0;
  std::vector<Particle> mParticles;

};

int main(int argc, char** argv)
{
  viewer viewer;
  viewer.run();
  return 0;
}
