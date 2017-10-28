#include "h_crane.h"

CraneCrane::CraneCrane()
: m_down(false), m_up(false), m_right(false),
  m_left(false), m_enter(false), m_escape(false)
{
    tNow = clock(); //start time
}
        
CraneCrane::~CraneCrane()
{
    clock_t nDiff = clock() - this->tNow;
    std::cout<<"Timer :"<<((float)nDiff)/CLOCKS_PER_SEC<<" Sec"<<std::endl; //report progress time (sec)
}

