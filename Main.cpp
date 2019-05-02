#include <iostream>
#include <fstream>
#include <limits>
#include <glm/gtx/string_cast.hpp>
#include <random>

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

void random() {
	/* for testing purposes
	glm::vec3 test(1.0f);
	std::cout << glm::to_string(test) << std::endl;
	std::cin.ignore();
	*/
}

glm::vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), rec)) {
		return 0.5f * glm::vec3(rec.normal.x + 1.0f, rec.normal.y + 1.0f, rec.normal.z + 1.0f);
	}
	else {
		glm::vec3 unit_direction = glm::normalize(r.direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}

int main() {
	std::ofstream file;
	file.open("test.ppm");
	
	int nx = 200;
	int ny = 100;
	int ns = 100;
	file << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[2];
	list[0] = new sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);

	hitable *world = new hitable_list(list, 2);
	camera cam;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			glm::vec3 col(0.0f);

			for (int s = 0; s < ns; s++) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> dis(0.0f, 1.0f);

				float u = float(i + dis(gen)) / float(nx);
				float v = float(j + dis(gen)) / float(ny);

				ray r = cam.get_ray(u, v);
				glm::vec3 p = r.point_at_parameter(2.0f);
				col += color(r, world);

			}
			
			col /= float(ns);
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