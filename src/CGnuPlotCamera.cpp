#include <CGnuPlotCamera.h>
#include <CGnuPlotGroup.h>

CGnuPlotCamera::
CGnuPlotCamera(CGnuPlotGroup *group) :
 group_(group)
{
  init();
}

CGnuPlotCamera::
~CGnuPlotCamera()
{
}

void
CGnuPlotCamera::
init()
{
  coordFrame_.init();

  setDirection(CVector3D(0,0,1));

//projMatrix_.buildPerspective(fov, 1.0, near, far);
  projMatrix_.buildOrtho(xmin_, xmax_, ymin_, ymax_, near_, far_);
//projMatrix_.buildFrustrum(-2, 2, -2, 2, near, far);

  rotateDX(M_PI*rotateX_/180.0);
  rotateDY(M_PI*rotateY_/180.0);
  rotateDZ(M_PI*rotateZ_/180.0);

  if (group_)
    group_->cameraChanged();
}

void
CGnuPlotCamera::
reset()
{
  enabled_    = false;
  xmin_       = -1;
  ymin_       = -1;
  xmax_       = 1;
  ymax_       = 1;
  near_       = 0.1;
  far_        = 100;
  rotateX_    = 60.0;
  rotateY_    = 0.0;
  rotateZ_    = 30.0;
  scaleX_     = 1.0;
  scaleY_     = 1.0;
  scaleZ_     = 1.0;
  axesScale_  = AxesScale::NONE;
  coordFrame_ = CCoordFrame3D();
  direction_  = CVector3D(0, 0, 1);
  fov_        = 90;
  projMatrix_ = CMatrix3DH();
}

void
CGnuPlotCamera::
setMap()
{
  enabled_ = false;

  rotateX_ = 0.0;
  rotateY_ = 0.0;
  rotateZ_ = 0.0;

  init();
}

void
CGnuPlotCamera::
setDirection(const CVector3D &dir)
{
  CVector3D right, up, dir1;

  coordFrame_.getBasis(right, up, dir1);

  dir1 = dir.unit();

  right = up  .crossProduct(dir1);
  up    = dir1.crossProduct(right);

  if (COrthonormalBasis3D::validate(right, up, dir1)) {
    coordFrame_.setBasis(right, up, dir1);

    direction_ = dir;
  }
}

CPoint3D
CGnuPlotCamera::
transform(const CPoint3D &p) const
{
  if (! enabled_) return p;

  // map to unit radius cube centered at 0,0
  CGnuPlotAxisData &xaxis = group_->xaxis(1);
  CGnuPlotAxisData &yaxis = group_->yaxis(1);
  CGnuPlotAxisData &zaxis = group_->zaxis(1);

  double xmin = xaxis.min().getValue(0.0);
  double xmax = xaxis.max().getValue(1.0);
  double ymin = yaxis.min().getValue(0.0);
  double ymax = yaxis.max().getValue(1.0);

  double zmin, zmax;

  planeZRange(zmin, zmax);

  double x1 = CGnuPlotUtil::map(p.x, xmin, xmax, -scaleX_, scaleX_);
  double y1 = CGnuPlotUtil::map(p.y, ymin, ymax, -scaleY_, scaleY_);
  double z1 = CGnuPlotUtil::map(p.z, zmin, zmax, -scaleZ_, scaleZ_);

  // transform to 2D
  CPoint3D p1(x1, y1, z1);

  CPoint3D p2 = coordFrame_.transformTo(p1);

  CPoint3D p3;

  projMatrix_.multiplyPoint(p2, p3);

  // remap back to x/y axis
  double x2 = CGnuPlotUtil::map(p3.x, -1, 1, xmin, xmax);
  double y2 = CGnuPlotUtil::map(p3.y, -1, 1, ymin, ymax);

  double z2 = 0.0;

  if (zaxis.min().isValid() && zaxis.max().isValid())
    z2 = CGnuPlotUtil::map(p3.z, -1, 1, zmin, zmax);

  return CPoint3D(x2, y2, z2);
}

void
CGnuPlotCamera::
planeZRange(double &zmin, double &zmax) const
{
  CGnuPlotAxisData &zaxis = group_->zaxis(1);

  zmin = zaxis.min().getValue(0.0);
  zmax = zaxis.max().getValue(1.0);

  if (xyPlane_.isRelative()) {
    zmin -= xyPlane_.z()*(zmax - zmin);
  }
}

void
CGnuPlotCamera::
showView(std::ostream &os) const
{
  os << "view is " << rotateX_ << ", " << rotateZ_ << " rot_z";
  os << ", " << scaleX_ << ", " << scaleZ_ << " scale_z" << std::endl;

  if      (axesScale_ == AxesScale::NONE)
    os << " axes are independently scaled" << std::endl;
  else if (axesScale_ == AxesScale::XY)
    os << " x/y axes are on the same scale" << std::endl;
  else if (axesScale_ == AxesScale::XYZ)
    os << " x/y/z axes are on the same scale" << std::endl;
}

void
CGnuPlotCamera::
unsetView()
{
  enabled_ = true;

  rotateX_   = 60.0;
  rotateZ_   = 30.0;
  scaleX_    = 1.0;
  scaleZ_    = 1.0;
  axesScale_ = AxesScale::NONE;

  init();
}
