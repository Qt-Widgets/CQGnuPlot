#ifndef CGnuPlotCoordValue_H
#define CGnuPlotCoordValue_H

#include <CGnuPlotTypes.h>

#include <ostream>

class CGnuPlotRenderer;

class CGnuPlotCoordValue {
 public:
  typedef CGnuPlotTypes::CoordSys CoordSys;

 public:
  CGnuPlotCoordValue(double v=0.0, CoordSys s=CGnuPlotTypes::CoordSys::FIRST) :
   value_(v), system_(s) {
  }

  double value() const { return value_; }
  void setValue(double v) { value_ = v; }

  CoordSys system() const { return system_; }
  void setSystem(CoordSys s) { system_ = s; }

  double getXValue(CGnuPlotRenderer *renderer) const;
  double getYValue(CGnuPlotRenderer *renderer) const;

  double getXDistance(CGnuPlotRenderer *renderer) const;
  double getYDistance(CGnuPlotRenderer *renderer) const;

  double pixelXValue(CGnuPlotRenderer *renderer) const;
  double pixelYValue(CGnuPlotRenderer *renderer) const;

  void print(std::ostream &os, const std::string &axis="x") const {
    if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
      os << "(second " << axis << " axis) ";
    else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
      os << "graph ";
    else if (system_ == CGnuPlotTypes::CoordSys::SCREEN)
      os << "screen ";
    else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
      os << "character  ";

    os << value_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotCoordValue &v) {
    v.print(os);

    return os;
  }

 private:
  double getXValue(CGnuPlotRenderer *renderer, double x) const;
  double getYValue(CGnuPlotRenderer *renderer, double y) const;

  double pixelXValue(CGnuPlotRenderer *renderer, double x) const;
  double pixelYValue(CGnuPlotRenderer *renderer, double y) const;

 private:
  double   value_  { 0.0 };
  CoordSys system_ { CoordSys::FIRST };
};

#endif
