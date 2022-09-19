#ifndef MACROSDEF_H
#define MACROSDEF_H

#define  IsKeyPressed(nVirtKey)     ((GetKeyState(nVirtKey) & (1<<(sizeof(SHORT)*8-1))) != 0);

#endif // MACROSDEF_H
