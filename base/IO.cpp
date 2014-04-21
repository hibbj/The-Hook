#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"
#include "MStackHelp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <list>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>

#include "Helper.h"
#include "IO.h"

float startX, startY;
float endX, endY;

glm::vec3 gaze;
glm::vec3 w, u;

void glfwGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

   if(xpos > g_width || xpos < 0 || ypos < 0 || ypos > g_height) {
      return;
   }

   endX = xpos;
   endY = g_height-ypos-1;

   float startWX = p2w_x(startX);
   float startWY = p2w_y(startY);
   float endWX = p2w_x(endX);
   float endWY = p2w_y(endY);
   float diff;

   //Calculate change in X
   if(startX < endX) {
      diff = endX - startX;
      beta += (diff * M_PI)/g_width;
   }
   else if(startX > endX){
      diff = startX - endX;
      beta -= (diff * M_PI)/g_width;
   }

   //Calculate change in Y
   if(startY < endY && alpha <= 0.98) {
      diff = endY - startY;
      alpha += (diff * M_PI)/g_width;
   }
   else if(startY > endY && alpha >= -0.98) {
      diff = startY - endY;
      alpha -= (diff * M_PI)/g_width;
   }
   //Update lookAt
   //printf("alpha: %f, beta: %f\n", alpha, beta);
   lookAtx = 1.0 * cos(alpha) * cos(beta);
   lookAty = 1.0 * sin(alpha);
   lookAtz = 1.0 * cos(alpha) * cos(M_PI/2.0 - beta);

   lookAtx += eye.x;
   lookAty += eye.y;
   lookAtz += eye.z;

   //printf("x: %f, y: %f, z: %f\n", lookAtx, lookAty, lookAtz);

   lookAtPoint = glm::vec3(lookAtx, lookAty, lookAtz);

  startX = g_width/2.0;// = endX;
  startY = g_height/2.0-1;// endY;


}



/*void glfw(GLFWwindow *window, int button, int action, int mods) {
   
}*/

//the keyboard callback
void glfwKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
  gaze = lookAtPoint - eye;

  w = gaze/magnitude(gaze);
  w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
  u = glm::cross(up, w)/magnitude(glm::cross(up, w));
  switch( key ) {
    case GLFW_KEY_S:
       eye = glm::vec3(eye.x + 0.1 * w.x, eye.y, eye.z + 0.1 * w.z);
       lookAtPoint = glm::vec3(0.1 * w.x + lookAtPoint.x, lookAtPoint.y, 0.1 * w.z + lookAtPoint.z);
      break;
    case GLFW_KEY_W:
       eye = glm::vec3(eye.x - 0.1 * w.x, eye.y, eye.z - 0.1 * w.z);
       lookAtPoint = glm::vec3(lookAtPoint.x - 0.1 * w.x, lookAtPoint.y, lookAtPoint.z - 0.1 * w.z);
      break;
    case GLFW_KEY_D:
       eye = glm::vec3(eye.x + 0.1 * u.x, eye.y, eye.z + 0.1 * u.z);
       lookAtPoint = glm::vec3(lookAtPoint.x + 0.1 * u.x, lookAtPoint.y, lookAtPoint.z + 0.1 * u.z);
      break;
    case GLFW_KEY_A:
       eye = glm::vec3(eye.x - 0.1 * u.x, eye.y, eye.z - 0.1 * u.z);
       lookAtPoint = glm::vec3(lookAtPoint.x - 0.1 * u.x, lookAtPoint.y, lookAtPoint.z - 0.1 * u.z);
      break;
    case GLFW_KEY_Q:
      exit( EXIT_SUCCESS );
      break;
  }

}
