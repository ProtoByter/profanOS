#include "syscall.h"

typedef struct Vertex2D_t {
    float x;
    float y;
} Vertex2D_t;

typedef struct Vertex3D_t {
    float x;
    float y;
    float z;
} Vertex3D_t;

typedef struct Cube_t {
    float d;
    Vertex3D_t vertices[8];
    int indices[36];
} Cube_t;

Cube_t Cube_t_init(Vertex3D_t position, float size);
Vertex2D_t Vertex2D_t_project(Vertex3D_t vector);
float crossProduct(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t v2);

int main(int arg) {

    c_fskprint("Lancement du moteur 3d !\n");
    
    Cube_t cube = Cube_t_init((Vertex3D_t) {0, 0, 0}, 100);
    c_fskprint("%s\n", cube);

    return arg;
}

Vertex2D_t Vertex2D_t_project(Vertex3D_t vector) {
    return  (Vertex2D_t {vector.x, vector.y});


Cube_t Cube_t_init(Vertex3D_t position, int size) {
    float d = size/2;
    return (Cube_t) {
        d, 
        {
            (Vertex3D_t) {position.x - d, position.y - d, position.z + d},
            (Vertex3D_t) {position.x - d, position.y - d, position.z - d},
            (Vertex3D_t) {position.x + d, position.y - d, position.z - d},
            (Vertex3D_t) {position.x + d, position.y - d, position.z + d},
            (Vertex3D_t) {position.x + d, position.y + d, position.z + d},
            (Vertex3D_t) {position.x + d, position.y + d, position.z - d},
            (Vertex3D_t) {position.x - d, position.y + d, position.z - d},
            (Vertex3D_t) {position.x - d, position.y + d, position.z + d}
        },
        {
            0,1,2,
            0,2,3,
            4,5,6,
            4,6,7,
            0,7,6,
            0,6,1,
            3,5,4,
            3,2,5,
            0,4,7,
            0,3,4,
            1,6,5,
            1,5,2
        }
    };
}

/*
crossProduct(v0,v1,v2) {
  return (v1.x - v0.x) * -(v2.y - v0.y) - -(v1.y - v0.y) * (v2.x - v0.x);
}*/

float crossProduct(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t v2) {
    return (float) ((v1.x - v0.x) * -(v2.y - v0.y) - -(v1.y - v0.y) * (v2.x - v0.x));
}

/*
fillTriangle(imageData, v0, v1, v2) {
  var minX = Math.floor(Math.min(v0.x, v1.x, v2.x));
  var maxX = Math.ceil(Math.max(v0.x, v1.x, v2.x));
  var minY = Math.floor(Math.min(v0.y, v1.y, v2.y));
  var maxY = Math.ceil(Math.max(v0.y, v1.y, v2.y));
 
  var data = imageData.data;
  var width = imageData.width;
 
  // le fameux point P
  var p = {};
 
  for (var y = minY; y < maxY; y++) {
    for (var x = minX; x < maxX; x++) {
 
      // on fait notre teste à partir du centre du pixel
      p.x = x + 0.5; p.y = y + 0.5;
 
      // si le point n'est pas à l'intérieur de notre triangle, on colorise pas le pixel
      if (this.crossProduct(v1, v2, p) < 0 || this.crossProduct(v2, v0, p) < 0 || this.crossProduct(v0, v1, p) < 0) {
        continue;
      }
 
      // on récupère le pixel à partir de x et y
      var index = (y * width + x) * 4;
 
      // et on le colorise, pour l'instant on le colore en rouge (on viendra modifier ça plus tard)
      data[index] = 255;
      data[index + 1] = 0;
      data[index + 2] = 0;
      data[index + 3] = 255;
    }
  }
}fillTriangle(imageData, v0, v1, v2) {
  var minX = Math.floor(Math.min(v0.x, v1.x, v2.x));
  var maxX = Math.ceil(Math.max(v0.x, v1.x, v2.x));
  var minY = Math.floor(Math.min(v0.y, v1.y, v2.y));
  var maxY = Math.ceil(Math.max(v0.y, v1.y, v2.y));
 
  var data = imageData.data;
  var width = imageData.width;
 
  // le fameux point P
  var p = {};
 
  for (var y = minY; y < maxY; y++) {
    for (var x = minX; x < maxX; x++) {
 
      // on fait notre teste à partir du centre du pixel
      p.x = x + 0.5; p.y = y + 0.5;
 
      // si le point n'est pas à l'intérieur de notre triangle, on colorise pas le pixel
      if (this.crossProduct(v1, v2, p) < 0 || this.crossProduct(v2, v0, p) < 0 || this.crossProduct(v0, v1, p) < 0) {
        continue;
      }
 
      // on récupère le pixel à partir de x et y
      var index = (y * width + x) * 4;
 
      // et on le colorise, pour l'instant on le colore en rouge (on viendra modifier ça plus tard)
      data[index] = 255;
      data[index + 1] = 0;
      data[index + 2] = 0;
      data[index + 3] = 255;
    }
  }
}
*/

void fillTriangle(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t, v2) {
    
}

int floor(float value) {
    re
  
}