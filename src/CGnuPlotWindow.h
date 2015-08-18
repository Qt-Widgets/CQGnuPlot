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

  //---

  void addGroup(CGnuPlotGroup *group);

  const Groups &groups() const { return groups_; }
  uint numGroups() const { return groups_.size(); }

  void fitGroups();

  //---

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  //---

  void reset3D();

  void draw();

 private:
  CGnuPlot* plot_     { 0 };
  CISize2D  size_     { 100, 100 };
  bool      is3D_     { false };
  Groups    groups_;
  CRGBA     backgroundColor_ { 1, 1, 1};
};

#endif
