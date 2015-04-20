#include <CGnuPlotStyleBoxes.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBarObject.h>

CGnuPlotStyleBoxes::
CGnuPlotStyleBoxes() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXES)
{
}

void
CGnuPlotStyleBoxes::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CBBox2D bbox = plot->getBBox();

  double ymin = bbox.getYMin();

  double bw = plot->boxWidth().getSpacing(plot->getXSpacing());

  double y2 = std::max(0.0, ymin);

  CRGBA ftc = (plot->fillType() == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  const CRGBA &lc = lineStyle.calcColor(ftc);
  const CRGBA &fc = lineStyle.calcColor(CRGBA(0,0,0));

  //---

  int n = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double y = reals[1];

    if (IsNaN(y))
      continue;

    ++n;
  }

  if (! renderer->isPseudo())
    plot->updateRectCacheSize(n);

  //---

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    if (IsNaN(y))
      continue;

    uint ind = 2;

    double bw1 = bw;

    if (plot->boxWidth().isCalc()) {
      if ((isCalcColor && reals.size() == 4) || (! isCalcColor && reals.size() == 3))
        bw1 = reals[ind++];
    }

    CRGBA lc1 = lc;

    if (isCalcColor && ind < reals.size()) {
      double x = reals[ind];

      lc1 = lineStyle.color().calcColor(plot, x);
    }

    CBBox2D bbox(x - bw1/2, y2, x + bw1/2, y);

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = plot->barObjects()[i];

      bar->setBBox(bbox);

      if (! bar->isInitialized()) {
        bar->setValue(y);

        bar->setWidth      (bw1);
        bar->setFillType   (plot->fillType());
        bar->setFillPattern(plot->fillPattern());
        bar->setFillColor  (fc);

        bar->setBorder   (true);
        bar->setLineColor(lc1);

        bar->setInitialized(true);
      }
    }
    else
      renderer->drawRect(bbox, lc1, 1);

    ++i;
  }

  if (! renderer->isPseudo())
    plot->drawBars(renderer);
}

CBBox2D
CGnuPlotStyleBoxes::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}