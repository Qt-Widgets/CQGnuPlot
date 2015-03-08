#ifndef CGnuPlotSVGRenderer_H
#define CGnuPlotSVGRenderer_H

#include <CGnuPlot.h>
#include <CGnuPlotRenderer.h>

class CGnuPlotSVGDevice;

class CGnuPlotSVGRenderer : public CGnuPlotRenderer {
 public:
  CGnuPlotSVGRenderer(CGnuPlotSVGDevice *device);
 ~CGnuPlotSVGRenderer() override;

  std::ostream &os();

  void clear(const CRGBA &c);

  void drawPoint  (const CPoint2D &p, const CRGBA &c) override;
  void drawSymbol (const CPoint2D &p, SymbolType type, double size, const CRGBA &c) override;
  void drawPath   (const std::vector<CPoint2D> &points, double w, const CRGBA &c,
                   const CLineDash &dash) override;
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c,
                   const CLineDash &dash) override;
  void drawRect   (const CBBox2D &rect, const CRGBA &c, double width) override;
  void patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
                   const CRGBA &fg, const CRGBA &bg) override;
  void fillRect   (const CBBox2D &rect, const CRGBA &c) override;
  void drawBezier (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4,
                   double width, const CRGBA &c) override;

  void drawPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c) override;
  void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c) override;

  void drawEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c,
                   double width) override;
  void fillEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c) override;

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) override;

  void drawPieSlice(const CPoint2D &pc, double r, double angle1, double angle2,
                    const CRGBA &c=CRGBA(1,1,1)) override;

 private:
  CGnuPlotSVGDevice *device_;
};

#endif