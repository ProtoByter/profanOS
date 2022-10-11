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

typedef struct Color_t {
    int r;
    int g;
    int b;
} Color_t;

typedef struct Cube_t {
    float d;
    Vertex3D_t vertices[8];
    Color_t couleurs[36];
    int indices[36];
} Cube_t;

float crossProduct(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t v2);
void fillTriangle(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t v2);
Cube_t Cube_t_init(Vertex3D_t position, float size);
Vertex2D_t Vertex2D_t_project(Vertex3D_t vector);
float max(float a, float b, float c);
float min(float a, float b, float c);
int floor(float value);
int ceil(float num);

int main(int arg) {

    c_fskprint("Lancement du moteur 3d !\n");
    
    Cube_t cube = Cube_t_init((Vertex3D_t) {0, 0, 0}, 100);
    c_fskprint("%s\n", cube);

    c_vga_320_mode();

    // on itère sur tout les triangles du cube
    for (int i = 0; i < 36; i+=3) {

        // on projette les points
        Vertex2D_t v0 = Vertex2D_t_project(cube.vertices[cube.indices[i]]);
        Vertex2D_t v1 = Vertex2D_t_project(cube.vertices[cube.indices[i+1]]);
        Vertex2D_t v2 = Vertex2D_t_project(cube.vertices[cube.indices[i+2]]);

        // on décale tout pour centrer au milieu du canvas
        v0.x+=c_vga_get_width()/2; v0.y+=c_vga_get_height()/2;
        v1.x+=c_vga_get_width()/2; v1.y+=c_vga_get_height()/2;
        v2.x+=c_vga_get_width()/2; v2.y+=c_vga_get_height()/2;

        // et enfin, on dessine notre triangle
        fillTriangle(v0,v1,v2);
    }

    while (1);

    c_vga_text_mode();

    return arg;
}

Vertex2D_t Vertex2D_t_project(Vertex3D_t vector) {
    return (Vertex2D_t) {vector.x, vector.y};
}

Cube_t Cube_t_init(Vertex3D_t position, float size) {
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
            (Color_t) {255,0,0}, (Color_t) {255,0,0}, (Color_t) {255,0,0},
            (Color_t) {255,0,0}, (Color_t) {255,0,0}, (Color_t) {255,0,0},
        
            (Color_t) {255,255,0}, (Color_t) {0,255,255}, (Color_t) {255,0,255},
            (Color_t) {255,255,0}, (Color_t) {255,0,255}, (Color_t) {0,255,0},
        
            (Color_t) {0,0,255}, (Color_t) {0,0,255}, (Color_t) {0,0,255},
            (Color_t) {0,0,255}, (Color_t) {0,0,255}, (Color_t) {0,0,255},
        
            (Color_t) {255,255,0}, (Color_t) {255,255,0}, (Color_t) {255,255,0},
            (Color_t) {255,255,0}, (Color_t) {255,255,0}, (Color_t) {255,255,0},
        
            (Color_t) {255,0,255}, (Color_t) {255,0,255}, (Color_t) {255,0,255},
            (Color_t) {255,0,255}, (Color_t) {255,0,255}, (Color_t) {255,0,255},
        
            (Color_t) {0,255,255}, (Color_t) {0,255,255}, (Color_t) {0,255,255},
            (Color_t) {0,255,255}, (Color_t) {0,255,255}, (Color_t) {0,255,255}
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
    return (float)((v1.x - v0.x) * -(v2.y - v0.y) - -(v1.y - v0.y) * (v2.x - v0.x));
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
    }
*/

void fillTriangle(Vertex2D_t v0, Vertex2D_t v1, Vertex2D_t v2) {
    int minX = floor(min(v0.x, v1.x, v2.x));
    int maxX = ceil(max(v0.x, v1.x, v2.x));
    int minY = floor(min(v0.y, v1.y, v2.y));
    int maxY = ceil(max(v0.y, v1.y, v2.y));

    Vertex2D_t p = (Vertex2D_t) {0, 0};

    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            p.x = x + 0.5; p.y = y + 0.5;
            if (crossProduct(v1, v2, p) < 0 || crossProduct(v2, v0, p) < 0 || crossProduct(v0, v1, p) < 0) {
                continue;
            }
            c_vga_put_pixel(x, y, 63);
        }
    }


}

int floor(float value) {
    return (int)value;
}

int ceil(float num) {
    int inum = (int)num;
    if (num < 0 || num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

float min(float a, float b, float c) {
    float liste[3] = {a, b, c};
    float mini = a;
    for (int i=0; i<3; i++) {
        if (liste[i] < mini) {
            mini = liste[i];
        }
    }
    return mini;
}

float max(float a, float b, float c) {
    float liste[3] = {a, b, c};
    float maxi = a;
    for (int i=0; i<3; i++) {
        if (liste[i] > maxi) {
            maxi = liste[i];
        }
    }
    return maxi;
}