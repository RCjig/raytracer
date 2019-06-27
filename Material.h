#ifndef MATERIALH
#define MATERIALH

#include "Hitable.h"

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
	return v - 2.0f * glm::dot(v, n) * n;
}

glm::vec3 random_in_unit_sphere() {
	glm::vec3 p;

	do {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0f, 1.0f);

		p = 2.0f * glm::vec3(dis(gen), dis(gen), dis(gen)) - glm::vec3(1.0f);
	} while (glm::length(p) * glm::length(p) >= 1.0f);

	return p;
}

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const glm::vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const {
		glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	glm::vec3 albedo;
};

class metal : public material {
public:
	metal(const glm::vec3 a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const {
		glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (glm::dot(scattered.direction(), rec.normal) > 0);
	}

	glm::vec3 albedo;
	float fuzz;
};

#endif // !MATERIALH
