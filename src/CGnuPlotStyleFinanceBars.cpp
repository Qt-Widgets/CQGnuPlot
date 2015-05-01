#include <CGnuPlotStyleFinanceBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleFinanceBars::
CGnuPlotStyleFinanceBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FINANCEBARS)
{
}

void
CGnuPlotStyleFinanceBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA  lc = lineStyle.calcColor(CRGBA(0,0,0));
  double lw = lineStyle.width();

  double sl = 0;

  if (! renderer->isPseudo())
    sl = renderer->pixelWidthToWindowWidth(4);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if ((! isCalcColor && reals.size() < 5) || (isCalcColor && reals.size() < 6))
      continue;

    double date  = reals[0];
    double open  = reals[1];
    double low   = reals[2];
    double high  = reals[3];
    double close = reals[4];

    CRGBA lc1 = lc;

    if (isCalcColor) {
      double x = reals[5];

      lc1 = lineStyle.color().calcColor(plot, x);
    }

    renderer->drawClipLine(CPoint2D(date, low), CPoint2D(date, high), lw, lc1);

    renderer->drawClipLine(CPoint2D(date, open ), CPoint2D(date - sl, open ), lw, lc1);
    renderer->drawClipLine(CPoint2D(date, close), CPoint2D(date + sl, close), lw, lc1);
  }
}

CBBox2D
CGnuPlotStyleFinanceBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
