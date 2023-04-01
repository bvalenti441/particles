// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
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
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(100);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    // "main" particle
    Particle particle;
    particle.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    particle.vel = vec3(0, 0, 0);
    particle.size = 0.25f;
    particle.rot = 0;
    mParticles.push_back(particle);

    // other particles
    for (int i = 1; i < size; i++)
    {
      particle.size = 0;
      particle.color = vec4(agl::randomUnitCube(), 1.0f);
      mParticles.push_back(particle);
    }
  }

  void updateConfetti()
  {   
      // update "main" star
      float theta = elapsedTime();
      float px = cos(theta);
      float py = sin(theta);
      mParticles[0].vel = normalize(vec3(px, py, 0) - mParticles[0].pos);
      mParticles[0].pos = vec3(px, py, 0);

      //update other stars
      bool singleNew = true;
      for (int i = 1; i < mParticles.size(); ++i) 
      {
          mParticles[i].pos += dt() * mParticles[i].vel;
          mParticles[i].color.a -= 0.01;
          mParticles[i].size = mParticles[i].size * 1.01f;
          if ((mParticles[i].size == 0 && singleNew) || mParticles[i].color.a <= 0) {
              singleNew = false;
              mParticles[i].color = vec4(agl::randomUnitCube(), 1.0f);
              mParticles[i].size = 0.25f;
              mParticles[i].pos = mParticles[0].pos;
              mParticles[i].vel = -mParticles[0].vel + agl::randomUnitCube();
          }
      }
  }

  void drawConfetti()
  {
      renderer.loadTexture("particle", "../textures/star4.png", 0);

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

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
