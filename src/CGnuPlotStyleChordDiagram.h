#ifndef CGnuPlotStyleChordDiagram_H
#define CGnuPlotStyleChordDiagram_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleChordDiagram : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleChordDiagram();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif