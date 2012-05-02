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
#include <qthread.h>

class popupDisplay : public QThread
{
public:
	popupDisplay(QImage* image);
	~popupDisplay(void);
	//bool event(QEvent * e);
	void run();

private:

	QSplashScreen* frame;
	
};

#endif