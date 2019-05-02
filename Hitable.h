#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

struct hit_record {
	float t;
	glm::vec3 p;
	glm::vec3 normal;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif // !HITABLEH

