/*uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix1;
uniform vec3 uColor;
uniform vec3 uLColor;

//attribute vec3 aPosition;
//attribute vec3 aNormal;
//attribute vec3 aLightVec;

varying vec3 vColor;
varying vec4 vPosition;

void main() {
  gl_FragColor = vec4(vColor.r, vColor.g, vColor.b, 1.0);
  gl_FragColor = vPosition;
}*/
struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

varying vec3 vColor;

uniform mat4 uModelMatrix1;
uniform mat4 uViewMatrix;
uniform Material uMat;
uniform vec3 uLColor;
uniform float uMode;

varying vec3 normal;
varying mat4 view;
varying vec4 light;
varying vec4 reflection;
varying vec4 vPosition;
varying vec3 camPos;

void main() {
  float diffusedR, diffusedG, diffusedB;
  float specularR, specularG, specularB;
  float ambientR, ambientG, ambientB;
  vec4 L, N, V, R, T;
  float dist;

     //Diffusion
     //dist = 1.0/pow(distance(vPosition, light), 2.0);
     dist = 1.0;
     //L = view * light;
     L = vec4(-1.0 * light.x, -1.0 * light.y, -1.0 * light.z, 1.0);
     //L = vec4(-1.0 * L.x, -1.0 * L.y, L.z, 0.0);
     N = normalize(vec4(normal, 0));
     N = uModelMatrix1 * N;
     diffusedR = dist * uLColor.r * uMat.dColor.r * max(dot(N, L), 0.0);
     diffusedG = dist * uLColor.g * uMat.dColor.g * max(dot(N, L), 0.0);
     diffusedB = dist * uLColor.b * uMat.dColor.b * max(dot(N, L), 0.0);

     //Specular
     V = uViewMatrix*vec4(0,0,1,0);//vec4(normalize(camPos), 0.0);

     R = normalize(reflection);
     float temp = max(dot(V, R), 0.0);
      
     specularR = uLColor.r * uMat.sColor.r * pow(temp, uMat.shine);
     specularG = uLColor.g * uMat.sColor.g * pow(temp, uMat.shine);
     specularB = uLColor.b * uMat.sColor.b * pow(temp, uMat.shine);
     
     //Ambient
     ambientR = uLColor.r * uMat.aColor.r;
     ambientG = uLColor.g * uMat.aColor.g;
     ambientB = uLColor.b * uMat.aColor.b;

     //gl_FragColor = vec4(diffusedR + ambientR, diffusedG + ambientG, diffusedB + ambientB, 1.0);
     //gl_FragColor = vec4(diffusedR + ambientR, diffusedG + ambientG, diffusedB + ambientB, 1.0);
     gl_FragColor = vec4(diffusedR*1.0 + specularR*1.0 + ambientR*1.0, diffusedG*1.0 + specularG*1.0 + ambientG*1.0, diffusedB*1.0 + specularB*1.0 + ambientB*1.0, 1.0);
	//gl_FragColor = vec4(uLColor.r,uLColor.g,uLColor.b,1.0);
	//gl_FragColor = vec4(uMat.sColor.r,uMat.sColor.g,temp,1.0);
	//gl_FragColor = vPosition;
//     gl_FragColor = vec4(vColor.r, vColor.g, vColor.b, 1.0);
}
