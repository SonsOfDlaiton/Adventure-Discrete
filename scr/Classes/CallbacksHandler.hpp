#ifndef CALLBACKHANDLER_H
#define CALLBACKHANDLER_H
#include "Util.hpp"

class CallbacksHandler {
public:
    CallbacksHandler();
    CallbacksHandler(const CallbacksHandler& orig);
    virtual ~CallbacksHandler();
	
	static void drawScene(void);
	static void reshape(int width, int height);
	static void framesCheck(int n);
	static void update(int n);
private:
   static unsigned long int previousFramesInGame;
};

#endif /* CALLBACKHANDLER_H */
