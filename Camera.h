#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class camera {
public:
	camera() {
		lower_left_corner = glm::vec3(-2.0f, -1.0f, -1.0f);
		horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
		vertical = glm::vec3(0.0f, 2.0f, 0.0f);
		origin = glm::vec3(0.0f);
	}
	ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }

	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
};

#endif // !CAMERAH
