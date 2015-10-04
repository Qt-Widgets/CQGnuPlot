#include <CGnuPlotStylePoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPointObject.h>

CGnuPlotStylePoints::
CGnuPlotStylePoints() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::POINTS)
{
}

void
CGnuPlotStylePoints::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA c = lineStyle.calcColor(group, CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();

  //------

  uint np = plot->numPoints2D();

  if (! renderer->isPseudo() && lineStyle.isTipPoints())
    plot->updatePointCacheSize(np);

  //------

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  uint pointNum = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    CPoint2D p(0, 0);

    uint valueNum = 0;

    if      (reals.size() == 1) {
      p.x = pointNum;
      p.y = reals[valueNum++];
    }
    else if (reals.size() >= 2) {
      p.x = reals[valueNum++];
      p.y = reals[valueNum++];
    }

    if (renderer->isPseudo() && ! renderer->isInside(p))
      continue;

    p = group->mapLogPoint(plot->xind(), plot->yind(), 1, p);

    double size1 = pointSize;

    if ((! isCalcColor && valueNum < reals.size()) ||
        (  isCalcColor && valueNum < reals.size() - 1))
      size1 = reals[valueNum++];

    CRGBA c1 = c;

    if (isCalcColor && valueNum < reals.size()) {
      double z = reals[valueNum++];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        c1 = lineStyle.calcColor(plot, z);
    }

    if (! renderer->isPseudo() && lineStyle.isTipPoints()) {
      CGnuPlotPointObject *point = plot->pointObjects()[pointNum];

      point->setPoint(p);

      if (! point->testAndSetUsed()) {
        point->setPointType  (pointType);
        point->setColor      (c1);
        point->setLineWidth  (lw);
        point->setPointString(plot->pointTypeStr());

        if (size1 > 0)
          point->setSize(size1);
        else
          point->resetSize();
      }
    }
    else {
      if (pointType == CGnuPlotTypes::SymbolType::STRING)
        renderer->drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                                   plot->pointTypeStr(), c1);
      else
        renderer->drawSymbol(p, pointType, size1, c1, lw, true);
    }

    ++pointNum;
  }

  if (! renderer->isPseudo()) {
    for (const auto &point : plot->pointObjects())
      point->draw(renderer);
  }
}

void
CGnuPlotStylePoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  //bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();

  //---

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  for (const auto &ip : plot->getPoints3D()) {
    for (const auto &point : ip.second) {
      CPoint3D p;
      int      ind;

      (void) plot->mapPoint3D(point, p, ind);

      renderer->drawSymbol(p, pointType, pointSize, c, lw, true);
    }
  }
}

void
CGnuPlotStylePoints::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();

  CPoint2D pm = (p1 + p2)/2;

  renderer->drawSymbol(pm, pointType, pointSize, c, lw, true);
}

CBBox2D
CGnuPlotStylePoints::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
