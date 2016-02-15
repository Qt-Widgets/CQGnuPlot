#include <CQGnuPlotPositionEdit.h>
#include <CQRealSpin.h>
#include <CQPixmapCache.h>
#include <QStackedWidget>
#include <QHBoxLayout>

#include <svg/coord_char_svg.h>
#include <svg/coord_first_svg.h>
#include <svg/coord_graph_svg.h>
#include <svg/coord_screen_svg.h>
#include <svg/coord_second_svg.h>
#include <svg/coord_txt_svg.h>
#include <svg/coord_single_svg.h>
#include <svg/coord_multi_svg.h>

CQGnuPlotPositionEdit::
CQGnuPlotPositionEdit(QWidget *w) :
 QFrame(w)
{
  setObjectName("positionEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  stack_ = new QStackedWidget;
  stack_->setObjectName("stack");

  //---

  QFrame *singleFrame = new QFrame;
  singleFrame->setObjectName("single");

  QHBoxLayout *singleFrameLayout = new QHBoxLayout(singleFrame);
  singleFrameLayout->setMargin(2); singleFrameLayout->setSpacing(2);

  systemXYZ_ = new CQGnuPlotCoordSys("xyz");

  connect(systemXYZ_, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePosition()));

  xSingleEdit_ = new CQRealSpin;
  ySingleEdit_ = new CQRealSpin;
  zSingleEdit_ = new CQRealSpin;

  connect(xSingleEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));
  connect(ySingleEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));
  connect(zSingleEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));

  singleFrameLayout->addWidget(systemXYZ_);
  singleFrameLayout->addWidget(xSingleEdit_);
  singleFrameLayout->addWidget(ySingleEdit_);
  singleFrameLayout->addWidget(zSingleEdit_);
  singleFrameLayout->addStretch(1);

  //---

  QFrame *multiFrame = new QFrame;
  multiFrame->setObjectName("multi");

  QHBoxLayout *multiFrameLayout = new QHBoxLayout(multiFrame);
  multiFrameLayout->setMargin(2); multiFrameLayout->setSpacing(2);

  systemX_ = new CQGnuPlotCoordSys("x");
  systemY_ = new CQGnuPlotCoordSys("y");
  systemZ_ = new CQGnuPlotCoordSys("z");

  connect(systemX_, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePosition()));
  connect(systemY_, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePosition()));
  connect(systemZ_, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePosition()));

  xMultiEdit_ = new CQRealSpin;
  yMultiEdit_ = new CQRealSpin;
  zMultiEdit_ = new CQRealSpin;

  connect(xMultiEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));
  connect(yMultiEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));
  connect(zMultiEdit_, SIGNAL(valueChanged(double)), this, SLOT(updatePosition()));

  multiFrameLayout->addWidget(systemX_);
  multiFrameLayout->addWidget(xMultiEdit_);
  multiFrameLayout->addWidget(systemY_);
  multiFrameLayout->addWidget(yMultiEdit_);
  multiFrameLayout->addWidget(systemZ_);
  multiFrameLayout->addWidget(zMultiEdit_);
  multiFrameLayout->addStretch(1);

  //---

  stack_->addWidget(singleFrame);
  stack_->addWidget(multiFrame);

  //---

  type_ = new CQGnuPlotPositionTypeCombo;

  connect(type_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStack()));

  //---

  layout->addWidget(stack_);
  layout->addWidget(type_);

  //---

  updateState();
}

void
CQGnuPlotPositionEdit::
setPosition(const CGnuPlotPosition &pos)
{
  position_ = pos;

  updateState();
}

void
CQGnuPlotPositionEdit::
set2D(bool b)
{
  is2D_ = b;

  updateState();
}

void
CQGnuPlotPositionEdit::
updateState()
{
  systemXYZ_->setCoordSys(position_.systemX());

  xSingleEdit_->setValue(position_.point().x);
  ySingleEdit_->setValue(position_.point().y);
  zSingleEdit_->setValue(position_.point().z);

  //---

  systemX_->setCoordSys(position_.systemX());
  systemY_->setCoordSys(position_.systemY());
  systemZ_->setCoordSys(position_.systemZ());

  xMultiEdit_->setValue(position_.point().x);
  yMultiEdit_->setValue(position_.point().y);
  zMultiEdit_->setValue(position_.point().z);

  //---

  systemZ_    ->setVisible(! is2D_);
  zSingleEdit_->setVisible(! is2D_);
  zMultiEdit_ ->setVisible(! is2D_);
}

void
CQGnuPlotPositionEdit::
updatePosition()
{
  if (type_->currentIndex() == 0) {
    CPoint3D p(xSingleEdit_->value(), ySingleEdit_->value(), zSingleEdit_->value());

    position_.setPoint(p);

    position_.setSystem(systemXYZ_->coordSys());
  }
  else {
    CPoint3D p(xMultiEdit_->value(), yMultiEdit_->value(), zMultiEdit_->value());

    position_.setPoint(p);

    position_.setSystemX(systemX_->coordSys());
    position_.setSystemY(systemY_->coordSys());
    position_.setSystemZ(systemZ_->coordSys());
  }
}

void
CQGnuPlotPositionEdit::
updateStack()
{
  stack_->setCurrentIndex(type_->currentIndex());
}

//------

CQGnuPlotCoordSys::
CQGnuPlotCoordSys(const char *name) :
 CQIconCombo(0)
{
  setObjectName(QString("%1coordSys").arg(name));

  addItem(CQPixmapCacheInst->getIcon("COORD_FIRST" ), "First");
  addItem(CQPixmapCacheInst->getIcon("COORD_SECOND"), "Second");
  addItem(CQPixmapCacheInst->getIcon("COORD_GRAPH" ), "Graph");
  addItem(CQPixmapCacheInst->getIcon("COORD_SCREEN"), "Screen");
  addItem(CQPixmapCacheInst->getIcon("COORD_CHAR"  ), "Character");
}

void
CQGnuPlotCoordSys::
setCoordSys(CGnuPlotPosition::CoordSys cs)
{
  switch (cs) {
    case CGnuPlotPosition::CoordSys::FIRST    : setCurrentIndex(0); break;
    case CGnuPlotPosition::CoordSys::SECOND   : setCurrentIndex(1); break;
    case CGnuPlotPosition::CoordSys::GRAPH    : setCurrentIndex(2); break;
    case CGnuPlotPosition::CoordSys::SCREEN   : setCurrentIndex(3); break;
    case CGnuPlotPosition::CoordSys::CHARACTER: setCurrentIndex(4); break;
    default                                   : setCurrentIndex(0); break;
  }
}

CGnuPlotPosition::CoordSys
CQGnuPlotCoordSys::
coordSys() const
{
  switch (currentIndex()) {
    case 0 : return CGnuPlotPosition::CoordSys::FIRST;
    case 1 : return CGnuPlotPosition::CoordSys::SECOND;
    case 2 : return CGnuPlotPosition::CoordSys::GRAPH;
    case 3 : return CGnuPlotPosition::CoordSys::SCREEN;
    case 4 : return CGnuPlotPosition::CoordSys::CHARACTER;
    default: return CGnuPlotPosition::CoordSys::FIRST;
  }
}

//------

CQGnuPlotPositionTypeCombo::
CQGnuPlotPositionTypeCombo(QWidget *w) :
 CQIconCombo(w)
{
  setObjectName("typeCombo");

  addItem(CQPixmapCacheInst->getIcon("COORD_SINGLE"), "Single Type");
  addItem(CQPixmapCacheInst->getIcon("COORD_MULTI" ), "Multiple Types");
}