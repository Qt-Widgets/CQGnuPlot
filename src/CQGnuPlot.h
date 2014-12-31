#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>

#include <QString>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotPlot;

class CQGnuPlot : public CGnuPlot {
 public:
  struct TipRect {
    QString str;
    QRectF  rect;
  };

 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CGnuPlotWindow *createWindow() override;
  CGnuPlotPlot   *createPlot(CGnuPlotWindow *window) override;

  void timeout() override;

  void load(const std::string &filename);
};

#endif
