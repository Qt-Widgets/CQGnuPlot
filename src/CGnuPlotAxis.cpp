#include <CGnuPlotAxis.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

#include <CMathGen.h>
#include <CStrUtil.h>

#include <cstring>
#include <algorithm>

#define MIN_GOOD_TICKS 4
#define MAX_GOOD_TICKS 12
#define OPTIMUM_TICKS  10

#define MAX_GAP_TESTS 14

struct CGnuPlotAxisIncrementTest {
  double factor;
  int    numTicks;
  double incFactor;
};

static CGnuPlotAxisIncrementTest
axesIncrementTests[MAX_GAP_TESTS] = {
  {  1.0, 5, 0 },
  {  1.2, 3, 0 },
  {  2.0, 4, 0 },
  {  2.5, 5, 0 },
  {  4.0, 4, 0 },
  {  5.0, 5, 0 },
  {  6.0, 3, 0 },
  {  8.0, 4, 0 },
  { 10.0, 5, 0 },
  { 12.0, 3, 0 },
  { 20.0, 4, 0 },
  { 25.0, 5, 0 },
  { 40.0, 4, 0 },
  { 50.0, 5, 0 }
};

CGnuPlotAxis::
CGnuPlotAxis(CGnuPlotGroup *group, const std::string &id, COrientation direction,
             double start, double end) :
 group_    (group),
 id_       (id),
 direction_(direction),
 start_    (start),
 end_      (end),
 start1_   (start),
 end1_     (end)
{
  calc();
}

CGnuPlotAxis::
~CGnuPlotAxis()
{
}

CGnuPlot *
CGnuPlotAxis::
app() const
{
  return group()->app();
}

void
CGnuPlotAxis::
setRange(double start, double end)
{
  start_ = start;
  end_   = end;

  calc();
}

void
CGnuPlotAxis::
setMajorIncrement(double i)
{
  majorIncrement_ = i;

  calc();
}

void
CGnuPlotAxis::
setTickIncrement(int tickIncrement)
{
  tickIncrement_ = tickIncrement;

  calc();
}

void
CGnuPlotAxis::
setTickSpaces(double *tickSpaces, int numTickSpaces)
{
  tickSpaces_.resize(numTickSpaces);

  memcpy(&tickSpaces_[0], tickSpaces, numTickSpaces*sizeof(double));
}

void
CGnuPlotAxis::
setLabel(const std::string &str)
{
  label_ = str;
}

void
CGnuPlotAxis::
setTimeFormat(const std::string &fmt)
{
  timeFmt_ = fmt;
}

bool
CGnuPlotAxis::
calc()
{
  numTicks1_ = 1;
  numTicks2_ = 0;

  reverse_ = (start_ > end_);

  //------

  // Ensure Axis Start and End are in the Correct Order
  double minAxis = std::min(start_, end_);
  double maxAxis = std::max(start_, end_);

  //------

  // Calculate Length

  double length = fabs(maxAxis - minAxis);

  if (length == 0.0)
    return false;

  //------

  // Calculate nearest Power of Ten to Length

  int power = CMathGen::RoundDown(log10(length));

  //------

  if (majorIncrement_ <= 0.0) {
    // Set Default Increment to 0.1 * Power of Ten
    double increment = 0.1;

    if      (power < 0) {
      for (int i = 0; i < -power; ++i)
        increment /= 10.0;
    }
    else if (power > 0) {
      for (int i = 0; i <  power; ++i)
        increment *= 10.0;
    }

    //------

    // Calculate other test Increments

    for (int i = 0; i < MAX_GAP_TESTS; ++i)
      axesIncrementTests[i].incFactor = increment*axesIncrementTests[i].factor;

    //------

    // Set Default Start/End to Force Update

    start1_ = 0.0;
    end1_   = 0.0;

    //------

    // Test each Increment in turn

    int numGaps, numGapTicks;

    for (int i = 0; i < MAX_GAP_TESTS; ++i) {
      if (tickIncrement_ > 0) {
        int incFactor1 = (int) axesIncrementTests[i].incFactor;

        if (((double) incFactor1) != axesIncrementTests[i].incFactor)
          continue;

        if (incFactor1 % tickIncrement_ != 0)
          continue;
      }

      testAxisGaps(minAxis, maxAxis,
                   axesIncrementTests[i].incFactor,
                   axesIncrementTests[i].numTicks,
                   &start1_, &end1_, &increment,
                   &numGaps, &numGapTicks);
    }

    //------

    // Set the Gap Positions

    numTicks1_ = CMathGen::RoundDown((end1_ - start1_)/increment + 0.5);
    numTicks2_ = numGapTicks;
  }
  else {
    start1_    = start_;
    end1_      = end_;
    numTicks1_ = CMathGen::RoundDown((end1_ - start1_)/majorIncrement_ + 0.5);
    numTicks2_ = 5;
  }

  numTicks1_ = std::min(numTicks1_, 256);
  numTicks2_ = std::min(numTicks1_, 10);

  return true;
}

bool
CGnuPlotAxis::
testAxisGaps(double start, double end, double testIncrement, int testNumGapTicks,
             double *start1, double *end1, double *increment, int *numGaps, int *numGapTicks)
{
  // Calculate New Start and End implied by the Test Increment

  double newStart = CMathGen::RoundDown(start/testIncrement)*testIncrement;
  double newEnd   = CMathGen::RoundUp  (end  /testIncrement)*testIncrement;

  while (newStart > start)
    newStart -= testIncrement;

  while (newEnd < end)
    newEnd += testIncrement;

  int testNumGaps = CMathGen::RoundUp((newEnd - newStart)/testIncrement);

  //------

  // If nothing set yet just update values and return

  if (*start1 == 0.0 && *end1 == 0.0) {
    *start1 = newStart;
    *end1   = newEnd;

    *increment   = testIncrement;
    *numGaps     = testNumGaps;
    *numGapTicks = testNumGapTicks;

    return true;
  }

  //------

  // If the current number of gaps is not within the acceptable range
  // and the new number of gaps is within the acceptable range then
  // update current

  if ((   *numGaps <  MIN_GOOD_TICKS ||    *numGaps >  MAX_GOOD_TICKS) &&
      (testNumGaps >= MIN_GOOD_TICKS && testNumGaps <= MAX_GOOD_TICKS)) {
    *start1 = newStart;
    *end1   = newEnd;

    *increment    = testIncrement;
    *numGaps     = testNumGaps;
    *numGapTicks = testNumGapTicks;

    return true;
  }

  //------

  // If the current number of gaps is not within the acceptable range
  // and the new number of gaps is not within the acceptable range then
  // consider it for update of current if better fit

  if ((   *numGaps < MIN_GOOD_TICKS ||    *numGaps > MAX_GOOD_TICKS) &&
      (testNumGaps < MIN_GOOD_TICKS || testNumGaps > MAX_GOOD_TICKS)) {
    // Calculate how close fit is to required range

    double delta1 = fabs(newStart - start) + fabs(newEnd - end);

    //------

    // If better fit than current fit or equally good fit and
    // number of gaps is nearer to optimum (OPTIMUM_TICKS) then
    // update current

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(int(testNumGaps) - OPTIMUM_TICKS) < abs(int(*numGaps) - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = newStart;
      *end1   = newEnd;

      *increment   = testIncrement;
      *numGaps     = testNumGaps;
      *numGapTicks = testNumGapTicks;

      return true;
    }
  }

  //------

  // If the current number of gaps is within the acceptable range
  // and the new number of gaps is within the acceptable range then
  // consider it for update of current if better fit

  if ((   *numGaps >= MIN_GOOD_TICKS &&    *numGaps <= MAX_GOOD_TICKS) &&
      (testNumGaps >= MIN_GOOD_TICKS && testNumGaps <= MAX_GOOD_TICKS)) {
    // Calculate how close fit is to required range

    double delta1 = fabs(newStart - start) + fabs(newEnd - end);

    //------

    // If better fit than current fit or equally good fit and
    // number of gaps is nearer to optimum (OPTIMUM_TICKS) then
    // update current

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(int(testNumGaps) - OPTIMUM_TICKS) < abs(int(*numGaps) - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = newStart;
      *end1   = newEnd;

      *increment   = testIncrement;
      *numGaps     = testNumGaps;
      *numGapTicks = testNumGapTicks;

      return true;
    }
  }

  return false;
}

double
CGnuPlotAxis::
getMajorIncrement() const
{
  if (majorIncrement_ > 0.0)
    return majorIncrement_;
  else {
    if (numTicks1_ > 0)
      return (end1_ - start1_)/numTicks1_;
    else
      return 0.0;
  }
}

double
CGnuPlotAxis::
getMinorIncrement() const
{
  if (numTicks1_ > 0 && numTicks2_ > 0)
    return (end1_ - start1_)/(numTicks1_*numTicks2_);
  else
    return 0.0;
}

bool
CGnuPlotAxis::
hasTicLabels() const
{
  return group_->hasTicLabels(id_);
}

const CGnuPlotAxisData::RTicLabels &
CGnuPlotAxis::
getTicLabels() const
{
  return group_->ticLabels(id_);
}

std::string
CGnuPlotAxis::
getValueStr(int i, double pos) const
{
  return group_->getAxisValueStr(id_, i, pos);
}

void
CGnuPlotAxis::
drawAxis(CGnuPlotRenderer *renderer, double pos, bool first)
{
  renderer_ = renderer;

  //---

  CFontPtr font = renderer_->getFont();

  CRGBA c(0,0,0);

  //---

  std::string str;

  if (! isDisplayed())
    return;

  double maxW = 0;
  double maxH = font->getCharHeight();

  if (direction_ == CORIENTATION_HORIZONTAL) {
    double py = renderer_->pixelHeightToWindowHeight(2);

    bbox_ = CBBox2D(getStart(), pos - py, getEnd(), pos + py);
  }
  else {
    double px = renderer_->pixelWidthToWindowWidth(2);

    bbox_ = CBBox2D(pos - px, getStart(), pos + px, getEnd());
  }

  //---

  // Draw Axis Line

  if (isDrawLine()) {
    if (direction_ == CORIENTATION_HORIZONTAL)
      drawClipLine(CPoint2D(getStart(), pos), CPoint2D(getEnd(), pos), c);
    else
      drawClipLine(CPoint2D(pos, getStart()), CPoint2D(pos, getEnd()), c);
  }

  //---

  // Draw Ticks Marks and Labels

  double increment = getMajorIncrement();

  double pos1 = getStart();
  double pos2 = pos1;

  for (int i = 0; i < getNumMajorTicks(); ++i) {
    // Calculate Next Tick Point

    pos2 = (i + 1)*increment + getStart();

    //------*/

    // Draw Major Tick

    if (pos1 >= getStart() && pos1 <= getEnd()) {
      // Draw Tick Mark

      if (isDrawTickMark(first)) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawAxisTick(CPoint2D(pos1, pos),
           (isTickInside(first) ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
        else
          drawAxisTick(CPoint2D(pos, pos1),
           (isTickInside(first) ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
      }

      //------*/

      // Create Tick Label (Override with application supplied string if required)

      if (! hasTicLabels()) {
        str = getValueStr(i, pos1);

        maxW = std::max(maxW, font->getStringWidth(str));

        //------*/

        // Draw Tick Label

        if (isDrawTickLabel(first)) {
          if (direction_ == CORIENTATION_HORIZONTAL)
            drawTickLabel(CPoint2D(pos1, pos), str, first);
          else
            drawTickLabel(CPoint2D(pos, pos1), str, first);
        }
      }
    }

    //------*/

    // Draw Minor Ticks (use user defined distribution if defined)

    if (getTickSpaces() == 0) {
      double dt = (pos2 - pos1)/getNumMinorTicks();

      if (checkMinorTickSize(dt)) {
        for (int j = 1; j < getNumMinorTicks(); j++) {
          double pos3 = j*dt + pos1;

          if (isDrawTickMark(first) && isDrawMinorTickMark()) {
            if (pos3 >= getStart() && pos3 <= getEnd()) {
              if (direction_ == CORIENTATION_HORIZONTAL)
                drawAxisTick(CPoint2D(pos3, pos),
                  (isTickInside(first) ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), false);
              else
                drawAxisTick(CPoint2D(pos, pos3),
                  (isTickInside(first) ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), false);
            }
          }
        }
      }
    }
    else {
      for (int j = 1; j <= getNumTickSpaces() - 2; j++) {
        double pos3 = getTickSpace(j)*(pos2 - pos1) + pos1;

        if (isDrawTickMark(first) && isDrawMinorTickMark()) {
          if (pos3 >= getStart() && pos3 <= getEnd()) {
            if (direction_ == CORIENTATION_HORIZONTAL)
              drawAxisTick(CPoint2D(pos3, pos),
                (isTickInside(first) ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), false);
            else
              drawAxisTick(CPoint2D(pos, pos3),
                (isTickInside(first) ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), false);
          }
        }
      }
    }

    //------*/

    pos1 = pos2;
  }

  //------*/

  // Draw Last Major Tick

  if (pos1 >= getStart() && pos1 <= getEnd()) {
    // Draw Tick Mark

    if (isDrawTickMark(first)) {
      if (direction_ == CORIENTATION_HORIZONTAL)
        drawAxisTick(CPoint2D(pos1, pos),
          (isTickInside(first) ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
      else
        drawAxisTick(CPoint2D(pos, pos1),
          (isTickInside(first) ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
    }

    //------*/

    // Create Tick Label (Override with application supplied string if required)

    if (! hasTicLabels()) {
      str = getValueStr(getNumMajorTicks(), pos1);

      maxW = std::max(maxW, font->getStringWidth(str));

      //------*/

      // Draw Tick Label

      if (isDrawTickLabel(first)) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawTickLabel(CPoint2D(pos1, pos), str, first);
        else
          drawTickLabel(CPoint2D(pos, pos1), str, first);
      }
    }
  }

  //------*/

  // Draw Custom Tic Labels

  if (hasTicLabels()) {
    const CGnuPlotAxisData::RTicLabels &ticLabels = getTicLabels();

    for (const auto &label : ticLabels) {
      double      r = label.first;
      std::string s = label.second;

      maxW = std::max(maxW, font->getStringWidth(s));

      if (direction_ == CORIENTATION_HORIZONTAL)
        drawTickLabel(CPoint2D(r, pos), s, false);
      else
        drawTickLabel(CPoint2D(pos, r), s, false);
    }
  }

  //------*/

  // Draw Axis Label

  if (isDrawLabel(first)) {
    double x1 = getStart(), y1 = pos;
    double x2 = getEnd  (), y2 = pos;

    group_->mapLogPoint(&x1, &y1);
    group_->mapLogPoint(&x2, &y2);

    double mid = (x1 + x2)/2;

    group_->unmapLogPoint(&mid, &y1);

    const std::string &astr = getLabel();

    if (direction_ == CORIENTATION_HORIZONTAL)
      drawAxisLabel(CPoint2D(mid, pos), astr, maxH, first);
    else
      drawAxisLabel(CPoint2D(pos, mid), astr, maxW, first);
  }
}

void
CGnuPlotAxis::
drawAxisTick(const CPoint2D &p, CDirectionType type, bool large)
{
  CPoint2D p1 = p;

  if (reverse_) {
    if (direction_ == CORIENTATION_HORIZONTAL)
      p1.x = end_ - (p.x - start_);
    else
      p1.y = end_ - (p.y - start_);
  }

  if (clip_ && ! renderer_->clip().inside(p1))
    return;

  double psize = 6;

  if (! large)
    psize = 3;

  CRGBA c(0,0,0);

  if (type == CDIRECTION_TYPE_LEFT || type == CDIRECTION_TYPE_RIGHT) {
    double dx = (type == CDIRECTION_TYPE_LEFT ? -1 : 1);

    double x1 = p1.x + dx*renderer_->pixelWidthToWindowWidth(psize);

    drawLine(p1, CPoint2D(x1, p1.y), c);
  }
  else {
    double dy = (type == CDIRECTION_TYPE_DOWN  ? -1 : 1);

    double y1 = p1.y + dy*renderer_->pixelHeightToWindowHeight(psize);

    drawLine(p1, CPoint2D(p1.x, y1), c);
  }
}

void
CGnuPlotAxis::
drawTickLabel(const CPoint2D &p, const std::string &str, bool first)
{
  CPoint2D p1 = p;

  if (reverse_) {
    if (direction_ == CORIENTATION_HORIZONTAL)
      p1.x = end_ - (p.x - start_);
    else
      p1.y = end_ - (p.y - start_);
  }

  if (clip_ && ! renderer_->clip().inside(p1))
    return;

  // bool rotatedText = (direction_ == CORIENTATION_VERTICAL);
  bool rotatedText = false;

  if (direction_ == CORIENTATION_HORIZONTAL) {
    if (isLabelInside(first))
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -8, str);
    else
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,  8, str);
  }
  else {
    if (rotatedText) {
      if (isLabelInside(first))
        drawVAlignedText(p1, CHALIGN_TYPE_LEFT , 0, CVALIGN_TYPE_CENTER,  8, str);
      else
        drawVAlignedText(p1, CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, -8, str);
    }
    else
      if (isLabelInside(first))
        drawHAlignedText(p1, CHALIGN_TYPE_LEFT ,  8, CVALIGN_TYPE_CENTER, 0, str);
      else
        drawHAlignedText(p1, CHALIGN_TYPE_RIGHT, -8, CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawAxisLabel(const CPoint2D &p, const std::string &str, int maxSize, bool first)
{
  if (direction_ == CORIENTATION_HORIZONTAL) {
    if (isLabelInside(first))
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -(maxSize + 12), str);
    else
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,   maxSize + 12 , str);
  }
  else {
    if (isLabelInside(first))
      drawVAlignedText(p, CHALIGN_TYPE_LEFT ,    maxSize + 12 , CVALIGN_TYPE_CENTER, 0, str);
    else
      drawVAlignedText(p, CHALIGN_TYPE_RIGHT , -(maxSize + 12), CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawGrid(CGnuPlotRenderer *renderer, double start, double end)
{
  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //------*/

  lineDash_ = CLineDash(gridDashes, numGridDashes);

  CRGBA c(0,0,0);

  //------*/

  if (gridMajor_ || gridMinor_) {
    if (direction_ == CORIENTATION_HORIZONTAL) { // x-axis
      double xincrement = getMajorIncrement();

      for (int i = 0; i <= getNumMajorTicks(); ++i) {
        double x = i*xincrement + getStart();

        if (gridMajor_) {
          if (x >= getStart() && x <= getEnd())
            drawClipLine(CPoint2D(x, start), CPoint2D(x, end), c);
        }

        if (gridMinor_) {
          double xincrement1 = getMinorIncrement();

          if (checkMinorTickSize(xincrement1)) {
            for (int j = 0; j < getNumMinorTicks(); ++j) {
              double x1 = x + j*xincrement1;

              if (x1 >= getStart() && x1 <= getEnd())
                drawClipLine(CPoint2D(x1, start), CPoint2D(x1, end), c);
            }
          }
        }
      }
    }
    else {
      double yincrement = getMajorIncrement();

      for (int i = 0; i <= getNumMajorTicks(); ++i) {
        double y = i*yincrement + getStart();

        if (gridMajor_) {
          if (y >= getStart() && y <= getEnd())
            drawClipLine(CPoint2D(start, y), CPoint2D(end, y), c);
        }

        if (gridMinor_) {
          double yincrement1 = getMinorIncrement();

          if (checkMinorTickSize(yincrement1)) {
            for (int j = 0; j <= getNumMinorTicks(); ++j) {
              double y1 = y + j*yincrement1;

              if (y1 >= getStart() && y1 <= getEnd())
                drawClipLine(CPoint2D(start, y1), CPoint2D(end, y1), c);
            }
          }
        }
      }
    }
  }

  lineDash_ = CLineDash();
}

void
CGnuPlotAxis::
drawRadialGrid(CGnuPlotRenderer *renderer)
{
  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //------*/

  lineDash_ = CLineDash(gridDashes, numGridDashes);

  //------*/

  CRGBA c(0,0,0);

  double da = M_PI/6;
  int    na = int(2*M_PI/da + 0.5);

  double r1 = 0.0;
  double r2 = getEnd();

  for (int i = 0; i < na; ++i) {
    double a = i*da;

    double x1 = r1*cos(a);
    double y1 = r1*sin(a);
    double x2 = r2*cos(a);
    double y2 = r2*sin(a);

    drawClipLine(CPoint2D(x1, y1), CPoint2D(x2, y2), c);
  }

  if (gridMajor_) {
    double xincrement = getMajorIncrement();

    for (int i = 0; i <= getNumMajorTicks(); ++i) {
      double x = i*xincrement + getStart();

      if (x >= getStart() && x <= getEnd())
        renderer->drawEllipse(CPoint2D(0, 0), x, x, 0, c, 1, lineDash_);

      if (gridMinor_) {
        double xincrement1 = getMinorIncrement();

        if (checkMinorTickSize(xincrement1)) {
          for (int j = 0; j < getNumMinorTicks(); ++j) {
            double x1 = x + j*xincrement1;

            renderer->drawEllipse(CPoint2D(0, 0), x1, x1, 0, c, 1, lineDash_);
          }
        }
      }
    }
  }

  lineDash_ = CLineDash();
}

bool
CGnuPlotAxis::
checkMinorTickSize(double d) const
{
  double dp = 0;

  if (direction_ == CORIENTATION_HORIZONTAL)
    dp = renderer_->windowWidthToPixelWidth(d);
  else
    dp = renderer_->windowHeightToPixelHeight(d);

  return (dp > 2);
}

void
CGnuPlotAxis::
drawClipLine(const CPoint2D &p1, const CPoint2D &p2, const CRGBA &c)
{
  CPoint2D p11 = p1;
  CPoint2D p21 = p2;

  if (! clip_ || renderer_->clipLine(p11, p21))
    drawLine(p11, p21, c);
}

void
CGnuPlotAxis::
drawLine(const CPoint2D &p1, const CPoint2D &p2, const CRGBA &c)
{
  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  group_->mapLogPoint(&x1, &y1);
  group_->mapLogPoint(&x2, &y2);

  // TODO: custom width and color
  renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), 1, c, lineDash_);
}

void
CGnuPlotAxis::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  group_->mapLogPoint(&x, &y);

  CPoint2D pos1(x, y);

  CRGBA c(0,0,0);

  if (enhanced_) {
    //CFontPtr font = renderer_->getFont();

    CGnuPlotText text(str);

    CBBox2D bbox = text.calcBBox(renderer_);

    double dx = 0.0, dy = 0.0;

    if      (halign == CHALIGN_TYPE_LEFT  ) dx = 0;
    else if (halign == CHALIGN_TYPE_CENTER) dx = -bbox.getWidth()/2;
    else if (halign == CHALIGN_TYPE_RIGHT ) dx = -bbox.getWidth();

    if      (valign == CVALIGN_TYPE_BOTTOM) dy = bbox.getHeight();
    else if (valign == CVALIGN_TYPE_CENTER) dy = bbox.getHeight()/2;
    else if (valign == CVALIGN_TYPE_TOP   ) dy = 0;

    //dy -= renderer_->pixelHeightToWindowHeight(font->getCharAscent());

    double xo = renderer_->pixelWidthToWindowWidth  (xOffset);
    double yo = renderer_->pixelHeightToWindowHeight(yOffset);

    dx += xo;
    dy -= yo;

    CBBox2D bbox1 = bbox.moveBy(pos1 + CPoint2D(dx, dy));

    bbox1.setYMax(pos1.y + dy);

    text.draw(renderer_, bbox1, CHALIGN_TYPE_CENTER, c);
  }
  else {
    renderer_->drawHAlignedText(pos1, halign, xOffset, valign, yOffset, str, c);
  }
}

void
CGnuPlotAxis::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  CRGBA c(0,0,0);

  group_->mapLogPoint(&x, &y);

  renderer_->drawVAlignedText(CPoint2D(x, y), halign, xOffset, valign, yOffset, str, c);
}
