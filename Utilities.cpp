#include "Utilities.hpp"

void Utilities::CreateCylinder(float xRadius, float zRadius, int iStep)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 360; i += iStep) {
        float u = (float)i * (M_PI / 180.0);
        glNormal3f(std::cos(u), 0.0, std::sin(u));
        glVertex3f(std::cos(u) * xRadius, -0.5, std::sin(u) * zRadius);
        glNormal3f(std::cos(u), 0.0, std::sin(u));
        glVertex3f(std::cos(u) * xRadius, 0.5, std::sin(u) * zRadius);
    }
    glEnd();
}

void Utilities::CreateSphere(float xRadius, float yRadius, float zRadius, int iStep, int jStep, int startingAngle, int finishingAngle)
{
    
    for (int i = 0; i < 360; i += iStep) {
        glBegin(GL_TRIANGLE_STRIP);
        float u = (float)i * (M_PI / 180.0);
        float w = (float)(i + iStep) * (M_PI / 180.0);
        for (int j = startingAngle; j <= finishingAngle; j += jStep) {
            float v = (float)j * (M_PI / 180.0);
            glNormal3f(cos(v) * cos(u),
                       cos(v) * sin(u),
                       sin(v));
            glVertex3f(xRadius * cos(v) * cos(u),
                       yRadius * cos(v) * sin(u),
                       zRadius * sin(v));
            glNormal3f(cos(v) * cos(w),
                       cos(v) * sin(w),
                       sin(v));
            glVertex3f(xRadius * cos(v) * cos(w),
                       yRadius * cos(v) * sin(w),
                       zRadius * sin(v));
        }
        glEnd();
    }
}

void Utilities::CreateSquare(float width, float height, int stacks, int slices, bool hasTextureCoordinates, float numberOfTimesTextureRepeats)
{
    float xStep = width / stacks;
    float yStep = height / slices;
    
    float xStepTexture = numberOfTimesTextureRepeats / stacks;
    float yStepTexture = numberOfTimesTextureRepeats / slices;
    
    for (int i = 0; i <= stacks; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            glNormal3f(0.0, 0.0, -1.0);
            if (hasTextureCoordinates == true) {
                glTexCoord2f(i * xStepTexture, j * yStepTexture);
            }
            glVertex3f(i * xStep, j * yStep, 0.0);
            glNormal3f(0.0, 0.0, -1.0);
            if (hasTextureCoordinates == true) {
                glTexCoord2f((i + 1) * xStepTexture, j * yStepTexture);
            }
            glVertex3f((i + 1) * xStep, j * yStep, 0.0);
        }
        glEnd();
    }
}

void Utilities::CreateDisk(float xRadius, float yRadius, int iStep, int finishingAngle)
{
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, 0.0);
    for (int i = 0; i <= finishingAngle; i += iStep) {
        float u = (float)i * (M_PI / 180.0);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(std::cos(u) * xRadius, std::sin(u) * yRadius, 0.0);
    }
    glEnd();
}

void Utilities::CreateTorus(float xRadius, float yRadius, float zRadius, float innerRadius, int iStep, int jStep, int startingAngle, int finishingAngle)
{
    
    for (int i = startingAngle; i < finishingAngle; i += iStep) {
        glBegin(GL_TRIANGLE_STRIP);
        float u = (float)i * (M_PI / 180.0);
        float w = (float)(i + iStep) * (M_PI / 180.0);
        for (int j = -180; j <= 180; j += jStep) {
            float v = (float)j * (M_PI / 180.0);
            glNormal3f(cos(v) * cos(u),
                       cos(v) * sin(u),
                       sin(v));
            glVertex3f(xRadius * cos(v) * cos(u) + innerRadius * cos(u),
                       yRadius * cos(v) * sin(u) + innerRadius * sin(u),
                       zRadius * sin(v));
            glNormal3f(cos(v) * cos(w),
                       cos(v) * sin(w),
                       sin(v));
            glVertex3f(xRadius * cos(v) * cos(w) + innerRadius * cos(w),
                       yRadius * cos(v) * sin(w) + innerRadius * sin(w),
                       zRadius * sin(v));
        }
        glEnd();
    }
}

// Funcao que dado uma posicao diz se e uma intersaccao (todos os seus componentes nao teem parte decimal)
bool Utilities::IsIntersection(const Vector position)
{
    double integerPart;
    return ((modf(position.x(), &integerPart) == 0.0) && (modf(position.y(), &integerPart) == 0.0) && (modf(position.z(), &integerPart) == 0.0));
}

// Funcao que arredonda um numero
float Utilities::RoundNumber(float number)
{
    double integerPart;
    double decimalPart = modf(number, &integerPart);
    return ((number >= 0.0) ? ((decimalPart < 0.5) ? floor(number) : ceil(number)) : ((decimalPart >= 0.5) ? floor(number) : ceil(number)));
}

// Funcao que retorna a interseccao mais proxima de uma determinada posicao
const Vector Utilities::NearestIntersection(const Vector position)
{
    return Vector(Utilities::RoundNumber(position.x()), Utilities::RoundNumber(position.y()), Utilities::RoundNumber(position.z()));
}
