#ifndef CGnuPlotBubbleObject_H
#define CGnuPlotBubbleObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBubbleObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotBubbleObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBubbleObject() { }

  const CPoint2D &center() const { return c_; }
  void setCenter(const CPoint2D &c) { c_ = c; }

  double xRadius() const { return xr_; }
  void setXRadius(double r) { xr_ = r; }

  double yRadius() const { return yr_; }
  void setYRadius(double r) { yr_ = r; }

  bool hasColor() const { return color_.isValid(); }

  const CRGBA &color() const { return color_.getValue(); }
  void setColor(const CRGBA &c) { color_ = c; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  CPoint2D    c_     { 0, 0 };
  double      xr_    { 1 };
  double      yr_    { 1 };
  COptRGBA    color_;
  std::string name_  { "" };
  double      value_ { 0 };
};

#endif
