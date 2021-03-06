#include <CGnuPlotStyleImpulses.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleImpulses::
CGnuPlotStyleImpulses() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::IMPULSES)
{
}

void
CGnuPlotStyleImpulses::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  CGnuPlotAxis *raxis = group->getPlotAxis(CGnuPlotTypes::AxisType::R, 1, true);

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA  lc = stroke.color();
  double lw = stroke.width();

  //---

  double ymin = 0.0;

  if (plot->isPolar())
    ymin = group->raxis().min().getValue(0);
  else
    ymin = plot->bbox2D().getYMin();

  double y2 = std::max(0.0, ymin);

  if (plot->isPolar())
    y2 = std::max(ymin, 1.0);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    CPoint2D p1(reals[0], y2);
    CPoint2D p2(reals[0], reals[1]);

    if (renderer->isPseudo() && ! renderer->isInside(p2))
      continue;

    if (plot->isPolar()) {
      bool inside1, inside2;

      p1.y = raxis->mapLogValue(p1.y);
      p2.y = raxis->mapLogValue(p2.y);

      p1 = plot->convertPolarAxisPoint(p1, inside1);
      p2 = plot->convertPolarAxisPoint(p2, inside2);

      if (! inside1 && ! inside2)
        continue;
    }

    CRGBA lc1 = lc;

    if (reals.size() >= 3 && isCalcColor) {
      double z = reals[2];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        lc1 = lineStyle.calcColor(plot, z);
    }

    renderer->drawClipLine(p1, p2, lc1, lw);
  }
}

void
CGnuPlotStyleImpulses::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  CRGBA  c  = stroke.color();
  double lw = stroke.width();

  renderer->drawLine(p1, p2, c, lw);
}
