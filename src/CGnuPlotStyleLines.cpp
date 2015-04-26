#include <CGnuPlotStyleLines.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleLines::
CGnuPlotStyleLines() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::LINES)
{
}

void
CGnuPlotStyleLines::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  //------

  uint np = plot->numPoints2D();

  CPoint2D p1;

  uint i = 0;

  typedef std::vector<CPoint2D> Points;

  while (i < np) {
    for ( ; i < np; ++i) {
      const CGnuPlotPoint &point1 = plot->getPoint2D(i);

      if (point1.getPoint(p1))
        break;
    }

    ++i;

    Points points;

    points.push_back(p1);

    for ( ; i < np; ++i) {
      CPoint2D p2;

      const CGnuPlotPoint &point2 = plot->getPoint2D(i);

      if (! point2.getPoint(p2) || point2.isDiscontinuity())
        break;

      points.push_back(p2);

      p1 = p2;
    }

    // TODO: clip
    renderer->drawPath(points, lineStyle.width(), c, lineStyle.dash());
  }

#if 0
  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (! point1.isDiscontinuity() && point1.getPoint(p1) && point2.getPoint(p2))
      renderer->drawClipLine(p1, p2, lineStyle.width(), c),
                             lineStyle.dash());
  }
#endif
}

void
CGnuPlotStyleLines::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  //---

  for (const auto &ip : plot->getPoints3D()) {
    uint np = ip.second.size();

    CPoint3D p1;

    uint i = 0;

    typedef std::vector<CPoint3D> Points;

    while (i < np) {
      // find first point
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = ip.second[i];

        if (plot->mapPoint3D(point1, p1))
          break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      // get next continuous points
      for ( ; i < np; ++i) {
        CPoint3D p2;

        const CGnuPlotPoint &point2 = ip.second[i];

        if (! plot->mapPoint3D(point2, p2) || point2.isDiscontinuity())
          break;

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
      renderer->drawPath(points, lineStyle.width(), c, lineStyle.dash());
    }
  }
}

void
CGnuPlotStyleLines::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle= plot->lineStyle();

  renderer->drawLine(p1, p2, lineStyle.width(), lineStyle.calcColor(CRGBA(1,0,0)));
}

CBBox2D
CGnuPlotStyleLines::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
