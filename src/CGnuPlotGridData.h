#ifndef CGnuPlotGridData_H
#define CGnuPlotGridData_H

struct CGnuPlotGridData {
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

  bool      enabled        { false };
  DrawLayer layer          { DrawLayer::DEFAULT };
  double    polarAngle     { 0 };
  int       majorLineStyle { -1 };
  int       minorLineStyle { -1 };
  int       majorLineType  { -1 };
  int       minorLineType  { -1 };
  double    majorLineWidth { 1.0 };
  double    minorLineWidth { 1.0 };
};

#endif
