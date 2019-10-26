#include <string>
#include <iostream>
#include <limits>
#include <tuple>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::numeric_limits;
using std::make_tuple;

#include "io.h"
#include "matrix.h"
#include "omp.h"

const float FLT_MAX = 1E+37;
const int width = 1000;                         // Width  of the image
const int height = 1000;                        // Height of the image
const float M_PI = 3.14159265358979323846;      // Yeah, pi number
const int MAX_REFLECTION_DEPTH = 6;             // Max depth for soft shadows
bool hard_shadows = false;                      // Flag for soft/hard shadows

float max(float a, float b) { return (a>=b) ? a : b;}
float min(float a, float b) { return (a<=b) ? a : b;}
float abs(float a) { return (a>=0) ? a : -a;}

// Turns degrees into radians
float radians(float degrees) { return (degrees) * M_PI / 180.0;}

// Turns float number into integer in range of [0, 255]
int right_value(float x)
{
    int res;
    if (x<0) {res=0;}
    else if (x>255) {res=255;}
    else {res = static_cast<int>(x);}

    return res;
}

// Checks the amount of parametres for the program
void check_argc(int argc, int from, int to=numeric_limits<int>::max())
{
    if (argc < from)
        throw string("too few arguments for operation");
    if (argc > to)
        throw string("too many arguments for operation");
}


class float3 {
public:
    float x,y,z;
    float3() : x(0), y(0), z(0) {}
    float3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
    float3 operator+  (float3 a) {return float3(x+a.x, y+a.y, z+a.z);}
    float3 operator-  (float3 a) {return float3(x-a.x, y-a.y, z-a.z);}
    float3 operator+= (float3 a) {return float3(x+a.x, y+a.y, z+a.z);}
    float3 operator*  (float  a) {return float3(x*a, y*a, z*a);}
    float3 operator*  (float3 b) {return float3(x*b.x, y*b.y, z*b.z);}
    float3 operator/  (float  a) {return float3(x/a, y/a, z/a);}
    float3 normalize() {
        float vec_len = sqrt(x*x + y*y + z*z);
        // Just check so that we don't divide by zero
        return (vec_len <= 0) ? float3(0,0,0) : float3(x, y, z)/vec_len;
    }
};

class Ray {
public:
    float3 origin = float3(0,0,0);
    float3 direction = float3(0,0,0);

    Ray():origin(float3(0,0,0)), direction(float3(0,0,0)) {}
    Ray(float3 a, float3 b):origin(a), direction(b) {}
};

// Finds cross of two vectors
float3 cross(float3 x, float3 y) {
    return float3(x.y*y.z - y.y*x.z,
                  x.z*y.x - y.z*x.x,
                  x.x*y.y - y.x*x.y);
}

// Dot product of two vectors
float dot(float3 v, float3 b) {return v.x*b.x + v.y*b.y + v.z*b.z;}

class Lights {
public:
    float3 position = float3(0,0,0);
    float3 size = float3(0,0,0);

    Lights() {
        position = float3(0,0,0);
        size = float3(0,0,0);
    }

    Lights(float3 _position, float3 _size) {
        position = _position;
        size = _size;
    }

    ~Lights() {}
};

class Object {
public:
    // All objects have common features: position, color, normal
    float3 position = float3(0,0,0);
    float3 color = float3(0,0,0);
    float3 normal = float3(0,0,0);

    Object() {
        position = float3(0,0,0);
        color = float3(0,0,0);
        normal = float3(0,0,0);
    }

    Object(float3 _position, float3 _color, float3 _normal) {
        position = _position;
        color = _color;
        normal = _normal;
    }

    virtual ~Object() {}

    virtual bool intersection(Ray ray, float* t) {
        return false;
    }

    virtual float3 get_normal(float3 _p0, int *shininess, float3* diffuse_color, float3* specular_color) {
        return normal;
    }

};

class Plane : public Object {
public:
    float3 plane_normal = float3(0,0,0);

    Plane() {
        plane_normal = float3(0,0,0);
    }

    Plane(float3 _position, float3 _normal, float3 _color) {
        position = _position;
        plane_normal = _normal;
        color = _color;
    }

    bool intersection(Ray ray, float* t) {
        float denom = dot(ray.direction, plane_normal);

        // Comparison of float numbers is not accurate - we must set accuracy
        if (abs(denom) < 0.000001f) {
            return false;
        } else {
            float result = dot((position - ray.origin), plane_normal) / denom;
            *t = result;
            return *t >= 0;
        }
    }

    float3 get_normal(float3 _p0, int *shininess, float3* diffuse_color, float3* specular_color) {
        *shininess = 0;
        *diffuse_color = float3(0.8, 0.8, 0.8);
        *specular_color = color;
        // We set the normal when declare the plane
        return plane_normal;
    }
};

class Triangle : public Object {
public:
    float3 a = float3(0,0,0);
    float3 b = float3(0,0,0);
    float3 c = float3(0,0,0);

    Triangle() {
        a = float3(0,0,0);
        b = float3(0,0,0);
        c = float3(0,0,0);
    }

    Triangle(float3 _a, float3 _b, float3 _c, float3 _color) {
        a = _a;
        b = _b;
        c = _c;
        color = _color;
    }

    // Moller-Trumbore fase ray-triangle intersection algorithm
    bool intersection(Ray ray, float* t) {
        float3 e1 = b - a;
        float3 e2 = c - a;

        float u = dot((ray.origin - a), cross(ray.direction, e2)) / dot(e1, (cross(ray.direction, e2)));
        float v = dot(ray.direction, (cross((ray.origin - a), e1)) / dot(e1, cross(ray.direction, e2)));

        if ((u < 0) || (u > 1)) {
            return false;
        }
        else if ((v < 0) || (u + v > 1)) {
            return false;
        } else {
            float t0 = dot(e2, cross((ray.origin - a), e1)) / dot(e1, cross(ray.direction, e2));
            *t = t0;
            return true;
        }
    }

    float3 get_normal(float3 _p0, int *shininess, float3* diffuse_color, float3* specular_color) {
        *shininess = 128;
        *diffuse_color = color;
        *specular_color = float3(0.7, 0.7, 0.7);

        float3 v1 = a - b;
        float3 v2 = b - c;
        return float3(v1.y * v2.z - v1.z * v2.y,
                      v1.z * v2.x - v1.x * v2.z,
                      v1.x * v2.y - v1.y * v2.x).normalize() * (-1);
    }

};

class Sphere : public Object {
public:
    float radius = 0;

    Sphere() {
        radius = 0;
    }

    Sphere(float3 _position, float _radius, float3 _color) {
        position = _position;
        radius = _radius;
        color = _color;
    }

    bool intersection(Ray ray, float *t) {
        float3 L = position - ray.origin;
        float tca = dot(L, ray.direction);
        if (tca < 0) {
            return false;
        }
        else {
            float s2 = (dot(L,L)) - (tca * tca);
            float s = sqrt(s2);
            if (s > radius) {
                return false;
            }
            else {
                float thc = sqrt((radius * radius) - s2);
                float t0 = tca - thc;
                *t = t0;
                return true;
            }
        }
    }

    float3 get_normal(float3 _p0, int *shininess, float3* diffuse_color, float3* specular_color) {
        *shininess = 128;
        *diffuse_color = color;
        *specular_color = float3(0.7, 0.7, 0.7);
        return (_p0 - position);
    }

};

Object* Scene[9];
Lights* area_light = new Lights(float3(-2.5f, 20, -2.5f), float3(9, 0.1f, 9)); 

// Determine scene objects
void SetScene() {
    Scene[0] = new Plane(float3(0,-4,0), float3(0,1,0), float3(0.2, 0.2, 0.2)); // Dark grey - floor
    Scene[1] = new Sphere(float3(0,0,-20), 4, float3(0, 0, 0)); // Black shiny boi
    Scene[2] = new Sphere(float3(5,-1,-15), 2, float3(0.55, 0, 0)); // Yellow
    Scene[3] = new Sphere(float3(5,0,-25), 3, float3(0.65, 0.77, 0.97)); // Light blue
    Scene[4] = new Sphere(float3(-5.5,0,-15), 3, float3(0, 1, 0)); // Green

    // Rhombus
    Scene[5] = new Triangle(float3(0,-3,-10), float3(1,-3,-10), float3(1,-3.99,-10), float3(1, 0, 0));
    Scene[6] = new Triangle(float3(0,-3,-10), float3(1,-3,-10), float3(1,-2,-10), float3(0, 0, 0));
    Scene[7] = new Triangle(float3(2,-3,-10), float3(1,-3,-10), float3(1,-2,-10), float3(0, 1, 0));
    Scene[8] = new Triangle(float3(2,-3,-10), float3(1,-3,-10), float3(1,-3.99,-10), float3(0, 0, 0));
}

// Returns color recieved from scene intersection (depth for soft shadows)
float3 RayTrace(Ray ray, int depth) {
    float3 pix_color = float3(0,0,0);

    float cur_distance = 0.0f;   // Current distance to an object
    float min_dist = FLT_MAX;    // Minimum distance for this ray
    int   obj_index = -1;        // Index of the closest object

    for (uint k = 0; k < sizeof(Scene) / sizeof(Scene[0]); k++) {
        bool hit = Scene[k]->intersection(ray, &cur_distance);

        // If there is a hit with the object and the current distance is less
        // than min_dist, then set the new minimum and set the index to the new
        // index
        if (hit && cur_distance < min_dist) {
            min_dist = cur_distance;
            obj_index = k;
        }
    }

    // If we hit at least one object
    if (obj_index != -1) { 

        // Hit position
        float3 hit_point = ray.origin + (ray.direction * min_dist);

        // Area light source
        // If we use hard shadows - we will need only area_light_center
        // If we use soft shadows - all the variables will be used
        float3 area_light_pos    = float3(area_light->position.x, area_light->position.y, area_light->position.z);
        float3 area_light_size   = float3(area_light->size.x, area_light->size.y, area_light->size.z);
        float3 area_light_center = float3(area_light_pos.x + (area_light_size.x / 2), 
                                        area_light_pos.y + (area_light_size.y / 2), 
                                        area_light_pos.z + (area_light_size.z / 2));
        float3 lightIntensity(1.0, 1.0, 1.0);

        // Declaring - this 3 variables will soon be changed by Object methods
        float3 diffuse_color(0, 0, 0);
        float3 specular_color(0, 0, 0);
        int shininess = 0;

        // Ambient (multiplicate by 0.07 for shades)
        float3 ambient = Scene[obj_index]->color * float3(0.07, 0.07, 0.07);

        // Diffuse
        float3 light_direction = (area_light_center - hit_point).normalize();
        float3 normal = (Scene[obj_index]->get_normal(hit_point, &shininess, &diffuse_color, &specular_color)).normalize();
        float3 diffuse = diffuse_color * lightIntensity * max(0.0f, dot(light_direction, normal));

        // Specular
        float3 reflect_specular = (normal * 2 * (dot(light_direction, normal)) - light_direction).normalize();
        float maxCalc = max(0.0f, dot(reflect_specular, (ray.origin - hit_point).normalize()));
        float3 specular = specular_color * lightIntensity * pow(maxCalc, shininess);

        // Reflection
        if ((depth < MAX_REFLECTION_DEPTH) && (shininess > 0)) {
            float3 refl_ray_direction = ray.direction - normal * 2 * dot(ray.direction, normal);
            refl_ray_direction = refl_ray_direction.normalize();
            float3 refl_ray_origin = hit_point;
            float3 reflectColor = RayTrace(Ray(refl_ray_origin, refl_ray_direction), depth+1) * 0.15f;
            return pix_color = diffuse + specular + reflectColor;
        }

        // Check if lightning hits objects
        int light_obj_index = -1;

        // Hard shadows
        if (hard_shadows) {
                for (uint l=0; l < sizeof(Scene) / sizeof(Scene[0]); ++l) {
                    bool hit_by_light = Scene[l]->intersection(Ray(hit_point + (normal*1e-4f), light_direction), &cur_distance);

                    if (hit_by_light && cur_distance < min_dist) {
                        min_dist = cur_distance;
                        light_obj_index = l;
                    }
                }

                if (light_obj_index != -1) {
                    pix_color = ambient;
                }
                else {
                    pix_color = diffuse + specular;
                }
            }
        // Soft shadows
        else {
            float area_light_length = 15.0f;
            float increment = area_light_size.x / area_light_length;
            float rays_hit = 1.0f;
            float total_rays = area_light_length * area_light_length;

            for (float m = 0; m < area_light_size.x; m += increment) {
                for (float n = 0; n < area_light_size.z; n += increment) {
                    cur_distance = 0.0f;
                    min_dist = FLT_MAX;
                    light_obj_index = -1;

                    area_light_pos = float3(m, area_light_pos.y, n);
                    light_direction = (area_light_pos - hit_point).normalize();

                    // Obvious
                    for (int l = 0; l < static_cast<int>(sizeof(Scene) / sizeof(Scene[0])); ++l) {
                        if (obj_index != l) {
                            bool hit_by_light = Scene[l]->intersection(Ray(hit_point, light_direction), &cur_distance);
                            if (hit_by_light && cur_distance < min_dist) {
                                min_dist = cur_distance;
                                light_obj_index = l;
                            }
                        }
                    }

                    if (light_obj_index != -1) {
                        rays_hit = rays_hit - (1 / total_rays);
                    }
                }
            }

            pix_color = float3((diffuse + specular) * rays_hit);
        }
    }
    // If we didn't hit any object, then set the color to the background color
    else {
        // Sky blue color
        pix_color = float3(0.46, 0.73, 0.99);
    }

    return pix_color;

}

float3 ToWorldCoordinates(int i, int j) {

    // Normalize the pixel position to the range [0,1]
    // Adding 0.5 so that ray passes through the center of the pixel
    float pixNormX = (i + 0.5) / width;
    float pixNormY = (j + 0.5) / height;

    // If the image isn't square we should get an aspect ratio
    // so that all the cells are square
    float aspect_ratio = width / height;

    // Transform coordinates from [0,1] to [-1,1]
    float pixRemapX = (2 * pixNormX - 1) * aspect_ratio;
    float pixRemapY = 1 - 2 * pixNormY;

    // Creating Field of View - FoV (in degrees)
    float FoV = 60.0;
    float pixel_x = pixRemapX * tan(radians(FoV) / 2);
    float pixel_y = pixRemapY * tan(radians(FoV) / 2);

    // The image plane is 1 point away from camera position
    float3 pixel_world_position = float3(pixel_x, pixel_y, -1);

    // Camera is positioned in the center - (0,0,0)
    float3 direction = pixel_world_position - float3(0,0,0);
    direction = direction.normalize();

    // We return the direction of the ray from camera to the pixel[i,j]
    return direction;
}

int main(int argc, char **argv)
{
    // Check input
    try { 
        check_argc(argc,1,2);
    } catch (const string &s) {
        cerr << "Error: " << s << endl;

        // If we finish on this - we must clear globaly declared dynamic memory
        cout << "Clearing dynamic memory\n";
        delete area_light;
        for (uint i = 0; i < sizeof(Scene) / sizeof(Scene[0]); ++i) {
            delete Scene[i];
        }
        return 1;
    }

    // Argv parser
    hard_shadows = false;
    string action;
    if (argc == 1) cout << "Soft shadows (by default)\n";
    else {
        action = argv[1];
        if (action == "--hard") {
            cout << "Hard shadows" << endl;
            hard_shadows = true;
        }
        else if (action == "--soft") {
            cout << "Soft shadows" << endl;
            hard_shadows = false;
        }
        else cout << "Can't understand the " << action << " flag.\nSoft shadows (by default)\n";
    }

    // Destination image
    Image res(height, width);      

    // Creating image plain 
    float3 **image = new float3*[width];
    for (int i = 0; i < width; i++) image[i] = new float3[height];

    // Position all the objects on the scene
    SetScene();

    cout << "\nLaunching rays through pixels\n";
    // Using parallel loop for better perfomance
    #pragma omp parallel for 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Transform image coordinates into world coordinates
            Ray ray;
            ray.origin = float3(0,0,0);
            ray.direction = ToWorldCoordinates(x, y);

            // Get the pixel color from RayTrace
            image[x][y] = RayTrace(ray, 0);

            int r = right_value(image[x][y].x * 255);
            int g = right_value(image[x][y].y * 255);
            int b = right_value(image[x][y].z * 255);

            res(y,x) = make_tuple(r,g,b);
        }
    }

    // Saving the Result Image with the name in argv[1]
    // This is from EasyBMP.h
    if (action == "--hard") {
        save_image(res, "../../image/result_hard.bmp");
        cout << "image/result_hard.bmp\n";
    }
    else {
        save_image(res, "../../image/result_soft.bmp");
        cout << "image/result_soft.bmp\n";
    }

    // Clear dynamic memory
    cout << "Clearing dynamic memory\n";
    for (int i = 0; i < width; ++i) {
        delete [] image[i];
    }
    delete [] image;
    for (uint i = 0; i < sizeof(Scene) / sizeof(Scene[0]); ++i) {
        delete Scene[i];
    }
    delete area_light;

    return 0;
}