#ifndef CGnuPlotBarObject_H
#define CGnuPlotBarObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBarObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  CGnuPlotBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBarObject() { }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  bool isInitialized() const { return initialized_; }
  void setInitialized(bool b) { initialized_ = b; }

  FillType fillType() const { return fillType_; }
  void setFillType(FillType t) { fillType_ = t; }

  FillPattern fillPattern() const { return fillPattern_; }
  void setFillPattern(FillPattern t) { fillPattern_ = t; }

  const COptRGBA &fillColor() const { return fillColor_; }
  void setFillColor(const COptRGBA &c) { fillColor_ = c; }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  bool hasBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  const COptRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const COptRGBA &c) { lineColor_ = c; }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  double width() const { return width_; }
  void setWidth(double r) { width_ = r; }

  bool inside(const CPoint2D &p) const override;

  std::string tip() const override;

  CBBox2D tipRect() const { return bbox_; }

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBBox2D         bbox_        { 0, 0, 1, 1 };
  double          value_       { 0.0 }; // associated value
  bool            initialized_ { false };
  FillType        fillType_    { FillType::EMPTY };
  FillPattern     fillPattern_ { FillPattern::NONE };
  COptRGBA        fillColor_;
  bool            border_      { true }; // border displayed
  COptRGBA        lineColor_;
  double          width_       { 1.0 }; // border value
};

#endif
