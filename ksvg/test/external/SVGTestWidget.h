#ifndef _SVGTESTWIDGET_H
#define _SVGTESTWIDGET_H

#include <tqwidget.h>
#include <tqobject.h>
#include "SVGDocument.h"
#include "KSVGCanvas.h"

class SVGTestWidget : public TQWidget
{
Q_OBJECT
  
public:
	SVGTestWidget(const KURL &url);
	~SVGTestWidget();

private slots:
	void slotRenderingFinished();

protected:
	virtual void paintEvent(TQPaintEvent *event);
	virtual void resizeEvent(TQResizeEvent *event);
/*	virtual void mousePressEvent(TQMouseEvent *event);
	virtual void mouseReleaseEvent(TQMouseEvent *event);
	virtual void keyPressEvent(TQKeyEvent *event);
	virtual void keyReleaseEvent(TQKeyEvent *event);
	virtual void mouseMoveEvent(TQMouseEvent *event);
*/
private:
	TQPoint m_panningPos;
	TQPoint m_oldPanningPos;
	
	KSVG::SVGDocument *m_doc;
	KSVG::KSVGCanvas *m_canvas;
};

#endif

