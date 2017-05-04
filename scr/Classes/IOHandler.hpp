#ifndef IOHANDLER_H
#define IOHANDLER_H
#include "Util.hpp"
#include "GL.hpp"

class IOHandler {
public:
    IOHandler();
    IOHandler(const IOHandler& orig);
    virtual ~IOHandler();

	static void specialKeyboard(int key, int x, int y);
	static void keyboard(unsigned char key, int x, int y);
	static void specialKeyboardUp(int key,int x,int y);
	static void keyboardUp(unsigned char key,int x,int y);
	static void mousePress(int button,int state,int x,int y);
	static void mousePassiveMotion(int x,int y);
	static void mouseActiveMotion(int x,int y);
	static void mouseWheel(int button, int dir, int x, int y);

    static int SpecReleaseKey;
    static bool ReleaseMouseOffSet;
    static bool ReleaseMouseROffSet;
    static bool ReleaseZOffSet;
private:

};

#endif /* IOHANDLER_H */
