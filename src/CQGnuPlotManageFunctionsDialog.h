#ifndef CQGnuPlotManageFunctionsDialog_H
#define CQGnuPlotManageFunctionsDialog_H

#include <CQDialog.h>

class CQGnuPlotMainWindow;
class QTreeWidget;
class QLineEdit;

class CQGnuPlotManageFunctionsDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotManageFunctionsDialog(CQGnuPlotMainWindow *window);

  void createWidgets(QWidget *frame) override;

  QSize sizeHint() const;

 private slots:
  void addSlot();

  void updateFunctions();

 private:
  CQGnuPlotMainWindow *window_;
  QTreeWidget         *tree_;
  QLineEdit           *nameEdit_;
  QLineEdit           *exprEdit_;
  QSize                size_;
};

#endif
