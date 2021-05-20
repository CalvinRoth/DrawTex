#include "state.h"
#include <QString>
state::state(QString latex, int type)
{
    latexCode = latex ;
    drawType = type;
}


void state::pb(int arg){
    args.push_back(arg);
}
