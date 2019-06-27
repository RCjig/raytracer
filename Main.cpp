#include <iostream>
#include <fstream>

#include <limits>
#include <glm/gtx/string_cast.hpp>
#include <random>

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"

glm::vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		glm::vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return glm::vec3(0.0f);
		}
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

	hitable *list[4];
	list[0] = new sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(glm::vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(glm::vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new metal(glm::vec3(0.8f), 1.0f));

	hitable *world = new hitable_list(list, 4);
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
				col += color(r, world, 0);

			}
			
			col /= float(ns);
			col = glm::vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
			int ir = int(255.99 * col.x);
			int ig = int(255.99 * col.y);
			int ib = int(255.99 * col.z);

			file << ir << " " << ig << " " << ib << "\n";
		}
		std::cout << j << std::endl;
	}

	file.close();
	std::cout << "done" << std::endl;
	std::cin.ignore();

	return 0;
}