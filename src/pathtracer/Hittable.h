#pragma once
#include <memory>
#include "Ray.h"
#include "Material.h"

class Material;

struct HitRecord {
	glm::vec3 Point;
	glm::vec3 Normal;
	float T;
	std::shared_ptr<Material> Material;
	bool frontFace = false;
	inline void setFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
		frontFace = glm::dot(r.Direction, outwardNormal) < 0;
		Normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual const bool Hit(const Ray& ray, float tmin, float tmax, HitRecord& record) const = 0;

};

