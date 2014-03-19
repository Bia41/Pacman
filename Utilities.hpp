#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "OpenGL.hpp"
#include <cmath>
#include "Vector.hpp"

// Angulos que definem o numero de stacks ou slices das formas que sao criadas com as funcoes de desenho
#define ANGLE_I_STEP 10
#define ANGLE_J_STEP 10

namespace Utilities
{
	// Funcoes de desenho variadas
    void CreateCylinder(float xRadius, float zRadius, int iStep);
    void CreateSphere(float xRadius, float yRadius, float zRadius, int iStep, int jStep, int startingAngle, int finishingAngle);
    void CreateSquare(float width, float height, int stacks, int slices, bool hasTextureCoordinates, float numberOfTimesTextureRepeats);
    void CreateDisk(float xRadius, float yRadius, int iStep, int finishingAngle);
    void CreateTorus(float xRadius, float yRadius, float zRadius, float innerRadius, int iStep, int jStep, int startingAngle, int finishingAngle);

    float RoundNumber(float number);
    bool IsIntersection(const Vector position);
    const Vector NearestIntersection(const Vector position);
}

#endif
