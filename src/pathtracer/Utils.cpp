#include "Utils.h"
#include "fastPRNG.h"

constexpr float PI = 3.141592653589793238462643383279f;

static fastPRNG::fastRand32 fastRandom;

float lengthSquared(const glm::vec3& vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

glm::vec3 randomInUnitDisk() {
    while (true) {
        glm::vec3 p(randomFloatV(), randomFloatV(), 0.0f);
        if (lengthSquared(p) >= 1.0f) continue;
        return p;
    }
}

float randomFloatU() {
	return fastRandom.KISS_UNI<float>();
}

float randomFloatV() {
    return fastRandom.KISS_VNI<float>();
}

bool nearZero(const glm::vec3& vec) {
    float s = 1e-8f;
    return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

glm::vec3 randomInHemisphere(const glm::vec3& normal) {
    glm::vec3 inUnitSphere = randomInUnitSphere();
    if (glm::dot(inUnitSphere, normal) > 0.0f) {
        return inUnitSphere;
    }
    else {
        return -inUnitSphere;
    }
}

glm::vec3 randomUnitVector() {
    float yaw = randomFloatU() * 2.0f * PI;
    float pitch= randomFloatU() * 2.0f * PI;
    glm::vec3 dir;
    dir.x = glm::cos(yaw) * glm::cos(pitch);
    dir.y = glm::sin(pitch);
    dir.z = glm::sin(yaw) * glm::cos(pitch);
    return dir;
}

glm::vec3 randomInUnitSphere() {
    float u = randomFloatU();
    float v = randomFloatU();
    float theta = u * 2.0f * PI;
    float phi = glm::acos(2.0f * v - 1.0f);
    float r = glm::pow(randomFloatU(), 1/3.0f);
    float sinTheta = glm::sin(theta);
    float cosTheta = glm::cos(theta);
    float sinPhi = glm::sin(phi);
    float cosPhi = glm::cos(phi);
    float x = r * sinPhi * cosTheta;
    float y = r * sinPhi * sinTheta;
    float z = r * cosPhi;
    return glm::vec3(x, y, z);
}

glm::vec3 randomColor() {
    return glm::vec3(randomFloatU(), randomFloatU(), randomFloatU());
}
