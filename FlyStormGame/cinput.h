#include "global.h"

class CInput
{
public:
                         CInput(void);
                         ~CInput(void);
    bool                 InitDirectInput(void);
    bool                 InitKeyboard(void);
    bool                 Update(void);
	bool				 MouseEvent(MSG*);
	bool				 KeyboardEvent(MSG*);
	enum MyKeys {UP,DOWN,LEFT,RIGHT,SPACE} keys;
	bool                 KeyPressed(CInput::MyKeys pressedKey);
private:
    LPDIRECTINPUT8       m_pDIObject;
    LPDIRECTINPUTDEVICE8 m_pDIKeyboardDevice;
    char                 m_KeyBuffer[256];
};