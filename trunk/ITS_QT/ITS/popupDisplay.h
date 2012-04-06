 #ifndef POPUPDISPLAY_H
 #define POPUPDISPLAY_H

#include <QFrame>
#include <QLabel>
#include <QImage>
#include <QEvent>
#include <QWheelEvent>
#include <QSplashScreen>
#include <QPainter>
#include <QBitmap>

class popupDisplay : public QFrame
{
public:
	popupDisplay(/*QFrame* frm,*/QImage* image);
	~popupDisplay(void);
	bool event(QEvent * e);
protected:
    void closeEvent(QCloseEvent *event);
private:

//	QSplashScreen* frame;
	QLabel* lbl;
	QPixmap* pix;
	double ratio;
};

#endif