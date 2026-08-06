#include "Camera.h"
#include "Utils.h"

Camera::Camera(
	const glm::vec3& lookFrom, const glm::vec3& lookAt,
	int width, int height, float fov, float aperture,
	float focusDistance
)
	: Width(width), Height(height), Fov(fov), Ratio(width / (float)height),
	Direction(lookAt), FocusDistance(focusDistance), Aperture(aperture), Position(lookFrom)
{
	Update();
}

void Camera::Update()
{
	float theta = glm::radians(Fov);
	float h = glm::tan(theta / 2.0f);
	float viewportHeight = 2.0f * h;
	float viewportWidth = Ratio * viewportHeight;

	W = glm::normalize(Position - Direction);
	U = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), W));
	V = glm::cross(W, U);

	Horizontal = FocusDistance * viewportWidth * U;
	Vertical = FocusDistance * viewportHeight * V;
	LowerLeftCorner = Position - Horizontal / 2.0f - Vertical / 2.0f - FocusDistance * W;

	LensRadius = Aperture / 2.0f;
}
