TEMPLATE = app

QT += widgets

TARGET = CQGnuPlot

DEPENDPATH += .

INCLUDEPATH += . ../include

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR

CONFIG += debug

# Input
SOURCES += \
CQGnuPlot.cpp \
CQGnuPlotAnnotation.cpp \
CQGnuPlotArrow.cpp \
CQGnuPlotAxis.cpp \
CQGnuPlotBar.cpp \
CQGnuPlotBubble.cpp \
CQGnuPlotCanvas.cpp \
CQGnuPlotColorBox.cpp \
CQGnuPlotDevice.cpp \
CQGnuPlotEllipse.cpp \
CQGnuPlotGroup.cpp \
CQGnuPlotKey.cpp \
CQGnuPlotLabel.cpp \
CQGnuPlotLineStyle.cpp \
CQGnuPlotPalette.cpp \
CQGnuPlotPie.cpp \
CQGnuPlotPlot.cpp \
CQGnuPlotPolygon.cpp \
CQGnuPlotRectangle.cpp \
CQGnuPlotRenderer.cpp \
CQGnuPlotTitle.cpp \
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
\
CGnuPlot.cpp \
CGnuPlotArrow.cpp \
CGnuPlotArrowStyle.cpp \
CGnuPlotAxis.cpp \
CGnuPlotBBoxRenderer.cpp \
CGnuPlotBar.cpp \
CGnuPlotBubble.cpp \
CGnuPlotColorBox.cpp \
CGnuPlotColorSpec.cpp \
CGnuPlotContour.cpp \
CGnuPlotCoordValue.cpp \
CGnuPlotDevice.cpp \
CGnuPlotEllipse.cpp \
CGnuPlotFile.cpp \
CGnuPlotGroup.cpp \
CGnuPlotKey.cpp \
CGnuPlotKeyData.cpp \
CGnuPlotLabel.cpp \
CGnuPlotPalette.cpp \
CGnuPlotPie.cpp \
CGnuPlotPlot.cpp \
CGnuPlotPoint.cpp \
CGnuPlotPolygon.cpp \
CGnuPlotPosition.cpp \
CGnuPlotRectangle.cpp \
CGnuPlotRenderer.cpp \
CGnuPlotSVGDevice.cpp \
CGnuPlotSVGRenderer.cpp \
CGnuPlotSize.cpp \
CGnuPlotText.cpp \
CGnuPlotTitle.cpp \
CGnuPlotWindow.cpp \
\
CQPropertyDelegate.cpp \
CQPropertyEditor.cpp \
CQPropertyItem.cpp \
CQPropertyTree.cpp \
CQPaletteChooser.cpp \
CQPaletteDialog.cpp \
\
CMathGeom2D.cpp \
CArcToBezier.cpp \
CSymbol2D.cpp \
\
Expr/CExpr.cpp \
Expr/CExprBValue.cpp \
Expr/CExprCValue.cpp \
Expr/CExprCompile.cpp \
Expr/CExprExecute.cpp \
Expr/CExprFunction.cpp \
Expr/CExprInterp.cpp \
Expr/CExprIValue.cpp \
Expr/CExprOperator.cpp \
Expr/CExprParse.cpp \
Expr/CExprRValue.cpp \
Expr/CExprStrgen.cpp \
Expr/CExprSValue.cpp \
Expr/CExprValue.cpp \
Expr/CExprVariable.cpp \
\
MouseMode/CQMouseMode.cpp \
MouseMode/CQPanMouseMode.cpp \
MouseMode/CQZoomMouseMode.cpp \
\
CQAngleSpinBox.cpp \
CQLineDash.cpp \
CQPoint2DEdit.cpp \
CQIconCombo.cpp \

HEADERS += \
CQGnuPlot.h \
CQGnuPlotAnnotation.h \
CQGnuPlotArrow.h \
CQGnuPlotAxis.h \
CQGnuPlotBar.h \
CQGnuPlotBubble.h \
CQGnuPlotCanvas.h \
CQGnuPlotColorBox.h \
CQGnuPlotDevice.h \
CQGnuPlotEllipse.h \
CQGnuPlotGroup.h \
CQGnuPlotKey.h \
CQGnuPlotLabel.h \
CQGnuPlotLineStyle.h \
CQGnuPlotObject.h \
CQGnuPlotPalette.h \
CQGnuPlotPie.h \
CQGnuPlotPlot.h \
CQGnuPlotPolygon.h \
CQGnuPlotRectangle.h \
CQGnuPlotRenderer.h \
CQGnuPlotTitle.h \
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
\
CGnuPlot.h \
CGnuPlotArrow.h \
CGnuPlotArrowStyle.h \
CGnuPlotAxis.h \
CGnuPlotAxisData.h \
CGnuPlotBBoxRenderer.h \
CGnuPlotBar.h \
CGnuPlotBubble.h \
CGnuPlotCache.h \
CGnuPlotCamera.h \
CGnuPlotColorBox.h \
CGnuPlotColorSpec.h \
CGnuPlotContour.h \
CGnuPlotCoordValue.h \
CGnuPlotDevice.h \
CGnuPlotEllipse.h \
CGnuPlotFile.h \
CGnuPlotFillStyle.h \
CGnuPlotGroup.h \
CGnuPlotKey.h \
CGnuPlotKeyData.h \
CGnuPlotLabel.h \
CGnuPlotLineStyle.h \
CGnuPlotNameValues.h \
CGnuPlotObject.h \
CGnuPlotPalette.h \
CGnuPlotPie.h \
CGnuPlotPlot.h \
CGnuPlotPoint.h \
CGnuPlotPointStyle.h \
CGnuPlotPolygon.h \
CGnuPlotPosition.h \
CGnuPlotPrefValue.h \
CGnuPlotRectangle.h \
CGnuPlotRenderer.h \
CGnuPlotSVGDevice.h \
CGnuPlotSVGRenderer.h \
CGnuPlotSize.h \
CGnuPlotStyle.h \
CGnuPlotText.h \
CGnuPlotTitle.h \
CGnuPlotTypes.h \
CGnuPlotUtil.h \
CGnuPlotWindow.h \
\
CQPropertyDelegate.h \
CQPropertyEditor.h \
CQPropertyItem.h \
CQPropertyTree.h \
CQPaletteChooser.h \
CQPaletteDialog.h \
\
CMathGeom2D.h \
CArcToBezier.h \
CSymbol2D.h \
CStrUniqueMatch.h \
\
Expr/CExpr.h \
Expr/CExprBValue.h \
Expr/CExprCValue.h \
Expr/CExprCompile.h \
Expr/CExprError.h \
Expr/CExprExecute.h \
Expr/CExprFunction.h \
Expr/CExprI.h \
Expr/CExprInterp.h \
Expr/CExprIValue.h \
Expr/CExprOperator.h \
Expr/CExprParse.h \
Expr/CExprRValue.h \
Expr/CExprSValue.h \
Expr/CExprValue.h \
Expr/CExprVariable.h \
\
MouseMode/CQMouseMode.h \
MouseMode/CQPanMouseMode.h \
MouseMode/CQPanZoomIFace.h \
MouseMode/CQZoomMouseMode.h \
\
CQAngleSpinBox.h \
CQLineDash.h \
CQPoint2DEdit.h \
CQIconCombo.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. MouseMode Expr ../include \
../../CQApp/include \
../../CQUtil/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRGBName/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCQUtil -lCReadLine -lCUtil -lCFont -lCImageLib -lCConfig \
-lCRGBName -lCFile -lCOS -lCStrUtil -lCRegExp \
-lpng -ljpeg -ltre -lreadline
