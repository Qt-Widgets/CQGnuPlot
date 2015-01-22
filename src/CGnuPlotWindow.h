#ifndef CGnuPlotWindow_H
#define CGnuPlotWindow_H

#include <vector>

#include <CGnuPlot.h>

class CGnuPlotWindow {
 public:
  typedef std::vector<CGnuPlotGroup *> Groups;

 public:
  CGnuPlotWindow(CGnuPlot *plot);

  virtual ~CGnuPlotWindow();

  CGnuPlot *app() const { return plot_; }

  void clear();

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  const CISize2D &size() const { return size_; }
  void setSize(const CISize2D &s) { size_ = s; }

  void addGroup(CGnuPlotGroup *group);

  const Groups &groups() const { return groups_; }
  uint numGroups() const { return groups_.size(); }

  bool hidden3D() const { return hidden3D_; }
  void setHidden3D(bool b) { hidden3D_ = b; }

  bool surface3D() const { return surface3D_; }
  void setSurface3D(bool b) { surface3D_ = b; }

  bool contour3D() const { return contour3D_; }
  void setContour3D(bool b) { contour3D_ = b; }

  bool pm3D() const { return pm3D_; }
  void setPm3D(bool b) { pm3D_ = b; }

  const CGnuPlot::Camera &camera() const { return camera_; }
  void setCamera(const CGnuPlot::Camera &c) { camera_ = c; }

  void setCameraEnabled(bool b);

  void setCameraRotateX(double a);
  void setCameraRotateY(double a);
  void setCameraRotateZ(double a);

  void setCameraXMin(double x);
  void setCameraXMax(double x);
  void setCameraYMin(double y);
  void setCameraYMax(double y);
  void setCameraNear(double z);
  void setCameraFar (double z);

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  void reset3D();

  void draw();

 private:
  CGnuPlot*        plot_      { 0 };
  CISize2D         size_      { 100, 100 };
  bool             is3D_      { false };
  Groups           groups_;
  CGnuPlot::Camera camera_;
  bool             hidden3D_  { false };
  bool             surface3D_ { true  };
  bool             contour3D_ { false };
  bool             pm3D_      { false };
  CRGBA            backgroundColor_ { 1, 1, 1};
};

#endif
