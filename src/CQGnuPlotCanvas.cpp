#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QKeyEvent>

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlotWindow *window) :
 QWidget(0), window_(window), pressed_(false)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

CQGnuPlotCanvas::
~CQGnuPlotCanvas()
{
}

void
CQGnuPlotCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  window_->paint(&p);
}

void
CQGnuPlotCanvas::
mousePressEvent(QMouseEvent *e)
{
  pressed_ = true;

  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  window_->setCurrentGroup(group);

  if (group)
    group->mousePress(e->pos());

  window_->mousePress(e->pos());
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CPoint2D p;

  if (pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()), p)) {
    window_->showPos(p.x, p.y);
  }

  window_->mouseMove(e->pos(), pressed_);
}

void
CQGnuPlotCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  window_->setCurrentGroup(group);

  if (group)
    group->mouseRelease(e->pos());

  window_->mouseRelease(e->pos());

  pressed_ = false;
}

void
CQGnuPlotCanvas::
keyPressEvent(QKeyEvent *e)
{
  QPoint pos = this->mapFromGlobal(QCursor::pos());

  CQGnuPlotGroup *group = window_->getGroupAt(pos);

  window_->setCurrentGroup(group);

  window_->keyPress(e->key(), e->modifiers());
}

bool
CQGnuPlotCanvas::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  QPoint pos(p.x, p.y);

  CQGnuPlotGroup *group = window_->getGroupAt(pos);
  if (! group) return false;

  CGnuPlotRenderer *renderer = group->app()->renderer();

  renderer->setRegion(group->region());

  renderer->setRange(group->getDisplayRange(1, 1));

  renderer->pixelToWindow(p, w);

  group->unmapLogPoint(&w.x, &w.y);

  return true;
}

bool
CQGnuPlotCanvas::
event(QEvent *e)
{
  if (e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);

    CQGnuPlot::TipRect tip;

    if (window_->mouseTip(helpEvent->pos(), tip))
      QToolTip::showText(helpEvent->globalPos(), tip.str, this, tip.rect.toRect());

    return true;
  }

  return QWidget::event(e);
}
