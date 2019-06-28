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

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(glm::vec3(0.5f)));
	int i = 1;

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0f, 1.0f);

			float choose_mat = dis(gen);
			glm::vec3 center(a + 0.8f * dis(gen), 0.2f, b + 0.9f * dis(gen));
			if (glm::length((center - glm::vec3(4.0f, 0.2f, 0.0f))) > 0.9f) {
				if (choose_mat < 0.8f) {
					list[i++] = new sphere(center, 0.2f, new lambertian(glm::vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen))));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2f,
						new metal(glm::vec3(0.5f * (1 + dis(gen)), 0.5f * (1 + dis(gen)), 0.5f * (1 + dis(gen))), 0.5f * dis(gen)));
				}
				else {
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(glm::vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitable_list(list, i);
}

int main() {
	std::ofstream file;
	file.open("test.ppm");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0f, 1.0f);
	
	int nx = 1200;
	int ny = 800;
	int ns = 10;
	file << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[5];
	list[0] = new sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(glm::vec3(0.1f, 0.2f, 0.5f)));
	list[1] = new sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(glm::vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f));
	list[3] = new sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dielectric(1.5f));
	list[4] = new sphere(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dielectric(1.5f));
	hitable *world = new hitable_list(list, 5);
	world = random_scene();

	/*
	float R = cos(M_PI / 4);
	hitable *list[2];

	list[0] = new sphere(glm::vec3(-R, 0.0f, -1.0f), R, new lambertian(glm::vec3(0.0f, 0.0f, 1.0f)));
	list[1] = new sphere(glm::vec3(R, 0.0f, -1.0f), R, new lambertian(glm::vec3(1.0f, 0.0f, 0.0f)));
	*/

	glm::vec3 lookfrom(13.0f, 2.0f, 3.0f);
	glm::vec3 lookat(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.1f;

	camera cam(lookfrom, lookat, glm::vec3(0.0f, 1.0f, 0.0f), 20, float(nx) / float(ny), aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			glm::vec3 col(0.0f);
			for (int s = 0; s < ns; s++) {
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