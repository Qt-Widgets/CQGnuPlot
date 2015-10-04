#ifndef CQGnuPlotLabel_H
#define CQGnuPlotLabel_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotLabel.h>

#include <QPointF>
#include <QFont>
#include <QColor>

class CQGnuPlotLabel : public CQGnuPlotAnnotation, public CGnuPlotLabel {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(bool    displayed      READ isDisplayed       WRITE setDisplayed     )
  Q_PROPERTY(QString text           READ getText           WRITE setText          )
  Q_PROPERTY(QPointF pos            READ getPos            WRITE setPos           )
  Q_PROPERTY(QColor  color          READ color             WRITE setColor         )
  Q_PROPERTY(double  angle          READ getAngle          WRITE setAngle         )
  Q_PROPERTY(bool    enhanced       READ isEnhanced        WRITE setEnhanced      )
  Q_PROPERTY(QFont   font           READ getFont           WRITE setFont          )
  Q_PROPERTY(QPointF offset         READ getOffset         WRITE setOffset        )
  Q_PROPERTY(int     lineType       READ lineType          WRITE setLineType      )
  Q_PROPERTY(bool    boxFill        READ isBoxFill         WRITE setBoxFill       )
  Q_PROPERTY(QColor  boxFillColor   READ getBoxFillColor   WRITE setBoxFillColor  )
  Q_PROPERTY(bool    box            READ hasBox            WRITE setBox           )
  Q_PROPERTY(QColor  boxStrokeColor READ getBoxStrokeColor WRITE setBoxStrokeColor)
  Q_PROPERTY(double  boxStrokeWidth READ getBoxStrokeWidth WRITE setBoxStrokeWidth)
  Q_PROPERTY(bool    showPoint      READ showPoint         WRITE setShowPoint     )
  Q_PROPERTY(int     pointType      READ pointType         WRITE setPointType     )
  Q_PROPERTY(double  pointSize      READ pointSize         WRITE setPointSize     )
  Q_PROPERTY(double  pointWidth     READ pointWidth        WRITE setPointWidth    )
  Q_PROPERTY(bool    hypertext      READ hasHypertext      WRITE setHypertext     )

  Q_PROPERTY(CQGnuPlotEnum::HAlignType align READ getAlign WRITE setAlign)

 public:
  CQGnuPlotLabel(CQGnuPlotGroup *group);

  QString getText() const;
  void setText(const QString &s);

  CQGnuPlotEnum::HAlignType getAlign() const;
  void setAlign(const CQGnuPlotEnum::HAlignType &a);

  QPointF getPos() const;
  void setPos(const QPointF &p);

  QFont getFont() const;
  void setFont(const QFont &f);

  QPointF getOffset() const;
  void setOffset(const QPointF &p);

  double getAngle() const { return CGnuPlotLabel::getAngle().getValue(0); }

  QColor color() const;
  void setColor(const QColor &c);

  QColor getBoxFillColor() const;
  void setBoxFillColor(const QColor &c);

  QColor getBoxStrokeColor() const;
  void setBoxStrokeColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
