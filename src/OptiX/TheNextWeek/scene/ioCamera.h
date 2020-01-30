#ifndef IO_CAMERA_H
#define IO_CAMERA_H

#include <optix.h>
#include <optixu/optixpp.h>

class ioCamera
{
public:
  ioCamera() {}

  ioCamera(float fromX, float fromY, float fromZ,
           float toX, float toY, float toZ,
           float upX, float upY, float upZ,
           float t0 = 0.f, float t1 = 0.f ) {
    m_origin = optix::make_float3(fromX, fromY, fromZ);
    m_w = optix::normalize(
      optix::make_float3(fromX-toX, fromY-toY, fromZ-toZ)
      );
    m_u = optix::normalize(
      optix::cross(make_float3(upX, upY, upZ), m_w)
      );
    m_v = optix::cross(m_w, m_u);
    m_time0 = t0;
    m_time1 = t1;
  }

  virtual ~ioCamera() {}

  virtual void destroy() = 0;

  virtual void init(optix::Context& context) {
    context["cameraOrigin"]->set3fv((float*)&m_origin);
    context["cameraU"]->set3fv((float*)&m_u);
    context["cameraV"]->set3fv((float*)&m_v);
    context["cameraW"]->set3fv((float*)&m_w);
    context["cameraTime0"]->setFloat(m_time0);
    context["cameraTime1"]->setFloat(m_time1);
  }

protected:
  optix::float3 m_origin;
  optix::float3 m_u;
  optix::float3 m_v;
  optix::float3 m_w;
  float m_time0;
  float m_time1;
};


class ioPerspectiveCamera : public ioCamera
{
public:
  ioPerspectiveCamera(
    float fromX, float fromY, float fromZ,
    float toX, float toY, float toZ,
    float upX, float upY, float upZ,
    float vFov, float aspect,
    float t0 = 0.f, float t1 = 0.f)
    : ioCamera(fromX, fromY, fromZ,
               toX, toY, toZ,
               upX, upY, upZ,
               t0, t1)
    {
      float theta = vFov * 3.14159f/180.0f;
      m_halfHeight = tanf(theta/2.0f);
      m_halfWidth = aspect * m_halfHeight;
    }

  virtual void init(optix::Context& context)
    {
      ioCamera::init(context);
      context["cameraHalfHeight"]->setFloat(m_halfHeight);
      context["cameraHalfWidth"]->setFloat(m_halfWidth);
      context["cameraType"]->setInt(0);
    }

  virtual void destroy() { }

private:
  float m_halfHeight;
  float m_halfWidth;
};

class ioEnvironmentCamera : public ioCamera
{
public:
  ioEnvironmentCamera(
    float fromX, float fromY, float fromZ,
    float toX, float toY, float toZ,
    float upX, float upY, float upZ,
    float t0 = 0.f, float t1 = 0.f)
     : ioCamera(fromX, fromY, fromZ,
                 toX, toY, toZ,
                 upX, upY, upZ) { }

  virtual void init(optix::Context& context)
    {
      ioCamera::init(context);
      context["cameraType"]->setInt(1);
    }

  virtual void destroy() { }

};

class ioOrthographicCamera : public ioCamera
{
public:
  ioOrthographicCamera(
    float fromX, float fromY, float fromZ,
    float toX, float toY, float toZ,
    float upX, float upY, float upZ,
    float height, float width,
    float t0 = 0.f, float t1 = 0.f)
     : ioCamera(fromX, fromY, fromZ,
                 toX, toY, toZ,
                 upX, upY, upZ)
    {
      m_halfHeight = height/2.0f;
      m_halfWidth = width/2.0f;
    }

  virtual void init(optix::Context& context)
    {
      ioCamera::init(context);
      context["cameraHalfHeight"]->setFloat(m_halfHeight);
      context["cameraHalfWidth"]->setFloat(m_halfWidth);
      context["cameraType"]->setInt(2);
    }

  virtual void destroy() { }

private:
  float m_halfHeight;
  float m_halfWidth;
};
#endif //!IO_CAMERA_H