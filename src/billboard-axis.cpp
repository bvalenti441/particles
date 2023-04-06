// Bryn Mawr College, alinen, 2020
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
        "../shaders/simple-texture.vs",
        "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard
    scalar = vec2(img.width(), img.height());

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }

  void repositionCamera(GLfloat r, GLfloat az, GLfloat el) {
      eyePos.x = r * cos(az) * cos(el);
      eyePos.y = r * sin(el);
      eyePos.z = r * sin(az) * cos(el);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
      if (leftMouseDown) {
          Azimuth += dx * M_PI / 180;
          if (Elevation + dy * M_PI / 180 >= -M_PI / 2 && Elevation + dy * M_PI / 180 <= M_PI / 2) {
              Elevation += dy * M_PI / 180;
          }
          repositionCamera(Radius, Azimuth, Elevation);
      }
  }

  void mouseDown(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
          leftMouseDown = true;
      }
  }

  void mouseUp(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
          leftMouseDown = false;
      }
  }

  void scroll(float dx, float dy) {
      if (dy > 0 && Radius == 1) {
          return;
      }
      else if (dy < 0 && Radius == 10) {
          return;
      }
      else if (dy < 0) {
          Radius++;
      }
      else if (dy > 0) {
          Radius--;
      }
      repositionCamera(Radius, Azimuth, Elevation);
  }

  void draw() {
      renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    vec3 nHat = normalize(eyePos - lookPos);
    float thetaY = atan2(nHat.x, nHat.z);
    vec3 x = vec3(cos(thetaY), 0, -sin(thetaY));
    vec3 y = vec3(0, 1, 0);
    vec3 z = vec3(sin(thetaY), 0, cos(thetaY));
    renderer.rotate(mat3(x, y, z));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec2 scalar;
  GLfloat Radius = 2;
  GLfloat Azimuth = 90;
  GLfloat Elevation = 0;
  bool leftMouseDown = false;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
