// ////////////////////////////////////////////////////////////
// Created on 3/21/18.
// Copyright (c) 2018. All rights reserved.
//
//  ___________________________$$$$$$$\__________
// | $$\    $$\                $$  __$$\      | ||
// |_$$ |___$$ |_$$\______$$\__$$ |__$$ |_____|_||
// | $$ |   $$ | $$$\    $$$ | $$$$$$$  |  () | ||
// |_\$$\__$$  |_$$$$\__$$$$ |_$$  ____/______|_||
// |  \$$\$$  /  $$\$$\$$ $$ | $$ |        () | ||
// |___\$$$  /___$$ \$$$  $$ |_$$ |___________|_||
// |    \$  /    $$ |\$  /$$ | \__|           | ||
// |_____\_/_____$$ |_\_/_$$ |________________|_||
//               \__|     \__|
// The Visual Music Project
// Created by Logan T. Barnes
// ////////////////////////////////////////////////////////////
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace gl {

template <typename T>
TCamera<T>::TCamera()
{
    lookAt(glm::tvec3<T>(0, 0, 0), glm::tvec3<T>(0, 0, -1));
    perspective(60, 1, 1, 1000);
    ortho(-1, 1, -1, 1);
}

template <typename T>
void TCamera<T>::lookAt(const glm::tvec3<T> &eye, const glm::tvec3<T> &point, const glm::tvec3<T> &up)
{
    eyeVector_ = eye;
    lookVector_ = glm::normalize(point - eyeVector_);
    upVector_ = up;
    rightVector_ = glm::cross(lookVector_, upVector_);

    viewFromWorldMatrix_ = glm::lookAt(eyeVector_, point, upVector_);

    perspectiveScreenFromWorldMatrix_ = perspectiveScreenFromViewMatrix_ * viewFromWorldMatrix_;
    orthographicScreenFromWorldMatrix_ = orthographicScreenFromViewMatrix_ * viewFromWorldMatrix_;
}

template <typename T>
void TCamera<T>::perspective(T fovyDegrees, T aspect, T zNear, T zFar)
{
    fovYDegrees_ = fovyDegrees;
    fovYRadians_ = glm::radians(fovYDegrees_);
    aspectRatio_ = aspect;
    nearPlane_ = zNear;
    farPlane_ = zFar;

    perspectiveScreenFromViewMatrix_ = glm::perspective(fovYRadians_, aspectRatio_, nearPlane_, farPlane_);

    perspectiveScreenFromWorldMatrix_ = perspectiveScreenFromViewMatrix_ * viewFromWorldMatrix_;
}

template <typename T>
void TCamera<T>::ortho(T left, T right, T bottom, T top)
{
    ortho(left, right, bottom, top, orthoNear_, orthoFar_);
}

template <typename T>
void TCamera<T>::ortho(T left, T right, T bottom, T top, T near, T far)
{
    orthoLeft_ = left;
    orthoRight_ = right;
    orthoBottom_ = bottom;
    orthoTop_ = top;
    orthoNear_ = near;
    orthoFar_ = far;

    orthographicScreenFromViewMatrix_
        = glm::ortho(orthoLeft_, orthoRight_, orthoBottom_, orthoTop_, orthoNear_, orthoFar_);

    orthographicScreenFromWorldMatrix_ = orthographicScreenFromViewMatrix_ * viewFromWorldMatrix_;
}

template <typename T>
const glm::tvec3<T> &TCamera<T>::getEyeVector() const
{
    return eyeVector_;
}
template <typename T>
const glm::tvec3<T> &TCamera<T>::getLookVector() const
{
    return lookVector_;
}
template <typename T>
const glm::tvec3<T> &TCamera<T>::getUpVector() const
{
    return upVector_;
}
template <typename T>
const glm::tvec3<T> &TCamera<T>::getRightVector() const
{
    return rightVector_;
}
template <typename T>
const glm::tmat4x4<T> &TCamera<T>::getViewFromWorldMatrix() const
{
    return viewFromWorldMatrix_;
}
template <typename T>
T TCamera<T>::getFovYDegrees() const
{
    return fovYDegrees_;
}
template <typename T>
T TCamera<T>::getFovYRadians() const
{
    return fovYRadians_;
}
template <typename T>
T TCamera<T>::getAspectRatio() const
{
    return aspectRatio_;
}
template <typename T>
T TCamera<T>::getNearPlane() const
{
    return nearPlane_;
}
template <typename T>
T TCamera<T>::getFarPlane() const
{
    return farPlane_;
}
template <typename T>
const glm::tmat4x4<T> &TCamera<T>::getPerspectiveScreenFromViewMatrix() const
{
    return perspectiveScreenFromViewMatrix_;
}
template <typename T>
T TCamera<T>::getOrthoLeft() const
{
    return orthoLeft_;
}
template <typename T>
T TCamera<T>::getOrthoRight() const
{
    return orthoRight_;
}
template <typename T>
T TCamera<T>::getOrthoBottom() const
{
    return orthoBottom_;
}
template <typename T>
T TCamera<T>::getOrthoTop() const
{
    return orthoTop_;
}
template <typename T>
const glm::tmat4x4<T> &TCamera<T>::getOrthographicScreenFromViewMatrix() const
{
    return orthographicScreenFromViewMatrix_;
}
template <typename T>
const glm::tmat4x4<T> &TCamera<T>::getPerspectiveScreenFromWorldMatrix() const
{
    return perspectiveScreenFromWorldMatrix_;
}
template <typename T>
const glm::tmat4x4<T> &TCamera<T>::getOrthoScreenFromWorldMatrix() const
{
    return orthographicScreenFromWorldMatrix_;
}

template <typename T>
void TCamera<T>::setEyeVector(const glm::tvec3<T> &eyeVector)
{
    lookAt(eyeVector, eyeVector + lookVector_, upVector_);
}
template <typename T>
void TCamera<T>::setLookVector(const glm::tvec3<T> &lookVector)
{
    lookAt(eyeVector_, eyeVector_ + lookVector, upVector_);
}
template <typename T>
void TCamera<T>::setUpVector(const glm::tvec3<T> &upVector)
{
    lookAt(eyeVector_, eyeVector_ + lookVector_, upVector);
}
template <typename T>
void TCamera<T>::setFovYDegrees(T fovYDegrees)
{
    perspective(fovYDegrees, aspectRatio_, nearPlane_, farPlane_);
}
template <typename T>
void TCamera<T>::setAspectRatio(T aspectRatio)
{
    perspective(fovYDegrees_, aspectRatio, nearPlane_, farPlane_);
}
template <typename T>
void TCamera<T>::setNearPlane(T nearPlane)
{
    perspective(fovYDegrees_, aspectRatio_, nearPlane, farPlane_);
}
template <typename T>
void TCamera<T>::setFarPlane(T farPlane)
{
    perspective(fovYDegrees_, aspectRatio_, nearPlane_, farPlane);
}
template <typename T>
void TCamera<T>::setOrthoLeft(T orthoLeft)
{
    ortho(orthoLeft, orthoRight_, orthoBottom_, orthoTop_, orthoNear_, orthoFar_);
}
template <typename T>
void TCamera<T>::setOrthoRight(T orthoRight)
{
    ortho(orthoLeft_, orthoRight, orthoBottom_, orthoTop_, orthoNear_, orthoFar_);
}
template <typename T>
void TCamera<T>::setOrthoBottom(T orthoBottom)
{
    ortho(orthoLeft_, orthoRight_, orthoBottom, orthoTop_, orthoNear_, orthoFar_);
}
template <typename T>
void TCamera<T>::setOrthoTop(T orthoTop)
{
    ortho(orthoLeft_, orthoRight_, orthoBottom_, orthoTop, orthoNear_, orthoFar_);
}
template <typename T>
void TCamera<T>::setOrthoNear(T orthoNear)
{
    ortho(orthoLeft_, orthoRight_, orthoBottom_, orthoTop_, orthoNear, orthoFar_);
}
template <typename T>
void TCamera<T>::setOrthoFar(T orthoFar)
{
    ortho(orthoLeft_, orthoRight_, orthoBottom_, orthoTop_, orthoNear_, orthoFar);
}

template class TCamera<float>;

template class TCamera<double>;

} // namespace gl
