#include <iostream>
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include "Ray.h"

glm::vec3 color(const ray& r) {
	glm::vec3 unit_direction = glm::normalize(r.direction());
	float t = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

void random() {
	/* for testing purposes
	glm::vec3 test(1.0f);
	std::cout << glm::to_string(test) << std::endl;
	std::cin.ignore();
	*/
}

int main() {
	std::ofstream file;
	file.open("test.ppm");
	
	int nx = 200;
	int ny = 100;
	file << "P3\n" << nx << " " << ny << "\n255\n";

	glm::vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
	glm::vec3 vertical(0.0f, 2.0f, 0.0f);
	glm::vec3 origin(0.0f);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			glm::vec3 col = color(r);

			int ir = int(255.99 * col.x);
			int ig = int(255.99 * col.y);
			int ib = int(255.99 * col.z);

			file << ir << " " << ig << " " << ib << "\n";
		}
	}

	file.close();
	std::cout << "done" << std::endl;
	std::cin.ignore();

	return 0;
}