#ifndef CGnuPlotRectangle_H
#define CGnuPlotRectangle_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotRectangle : public CGnuPlotGroupAnnotation {
 public:
  typedef COptValT<CGnuPlotPosition> Position;
  typedef COptValT<CGnuPlotSize>     Size;

 public:
  static const char *getName() { return "rectangle"; }

  CGnuPlotRectangle(CGnuPlotGroup *group);

  virtual ~CGnuPlotRectangle() { }

  CGnuPlotRectangle *setData(const CGnuPlotRectangle *rect);

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::RECTANGLE; }

  const Position &getFrom() const { return from_; }
  void setFrom(const CGnuPlotPosition &p) { from_ = p; }

  const Position &getTo() const { return to_; }
  void setTo(const CGnuPlotPosition &p) { to_ = p; }

  const Position &getRTo() const { return rto_; }
  void setRTo(const CGnuPlotPosition &p) { rto_ = p; }

  const Position &getCenter() const { return center_; }
  void setCenter(const CGnuPlotPosition &p) { center_ = p; }

  const Size &getSize() const { return size_; }
  void setSize(const CGnuPlotSize &p) { size_ = p; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void setLineType(int lt) { lt_ = lt; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  CBBox2D calcBBox() const;

  void initClip() override;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void print(std::ostream &os) const override;

 protected:
  Position          from_;
  Position          to_;
  Position          rto_;
  Position          center_;
  Size              size_;
  CGnuPlotFillStyle fs_;
  COptInt           lt_;
  COptReal          lw_;
  mutable CBBox2D   bbox_;
  mutable CRGBA     lc_;
};

typedef std::shared_ptr<CGnuPlotRectangle> CGnuPlotRectangleP;

#endif
