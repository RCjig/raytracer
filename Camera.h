#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES

#include <math.h>
#include "Ray.h"

glm::vec3 random_in_unit_disk() {
	glm::vec3 p;
	
	do {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0f, 1.0f);

		p = 2.0f * glm::vec3(dis(gen), dis(gen), 0.0f) - glm::vec3(1.0f, 1.0f, 0.0f);
	} while (glm::dot(p, p) >= 1.0f);

	return p;
}

class camera {
public:
	camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2.0f;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = glm::normalize(lookfrom - lookat);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);
		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}
	ray get_ray(float s, float t) { 
		glm::vec3 rd = lens_radius * random_in_unit_disk();
		glm::vec3 offset = u * rd.x + v * rd.y;
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}

	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 u, v, w;
	float lens_radius;
};

#endif // !CAMERAH
