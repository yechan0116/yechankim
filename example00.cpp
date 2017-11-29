#include "h_crane.h"
 
 
class Crain : public CraneCrane
{
private:
    ev3dev::touch_sensor touch_q;
    ev3dev::ultrasonic_sensor ultra_p;
    ev3dev::motor a;
    ev3dev::motor b;
    ev3dev::motor c;
    
    
public:
    Crain():m_speed(0),  touch_q(ev3dev::INPUT_2), ultra_p(ev3dev::INPUT_1), a(ev3dev::OUTPUT_B), b(ev3dev::OUTPUT_C), c(ev3dev::OUTPUT_A)
    {
        
    }
    
    int m_speed;
    
    bool get_touch_pressed()
    {
        return touch_q.is_pressed();
    }
    
    virtual float distance()
    {
        return ultra_p.distance_centimeters();
    }
    
    virtual bool get_down()
    {
        return m_down;
    }
 
    virtual bool get_up()
    {
        return m_up;
    }
 
    virtual bool get_right()
    {
        return m_right;
    }
 
    virtual bool get_left()
    {
        return m_left;
    }
 
    virtual bool get_enter()
    {
        return m_enter;
    }
 
    virtual bool get_escape()
    {
        return m_escape;
    }
 
    virtual int  get_speed()
    {
        return 300;
    }
    
    virtual void set_down(bool val)
    {
        m_down = val;
    }
    
    virtual void set_up(bool val)
    {
        m_up = val;    
    }
    
    virtual void set_right(bool val)
    {
        m_right = val;   
    }
    virtual void set_left(bool val)
    {
        m_left = val; 
    } 
    
    virtual void set_enter(bool val)
    {
        m_enter = val;    
    }
    
    virtual void set_escape(bool val)
    {
        m_escape = val;    
    }
 
    virtual void set_speed(int val)
    {
        m_speed = val;    
    }
public:
    void yechan_code();
    void up();
    void move_and_detect(int n);
    void move_to_item();
    void pick_and_up();
    void move_to_finish();
    void down();
};
 
void Crain::up()
{
    a.set_speed_sp(get_speed());
    a.set_position_sp(-270);
    a.run_to_abs_pos();
    sleep(1);
    a.set_speed_sp(-1);
    a.run_forever();
}
 
void Crain::move_and_detect(int n)
{
    b.set_speed_sp(n*get_speed());
    b.run_forever();
    while (1)
    {
        if (distance() <= 12.0)
        {
            b.stop();
            break;
        }
    }
}
 
void Crain::move_to_item()
{
    b.set_position_sp(-3);
    b.set_speed_sp(30);
    b.run_to_rel_pos();
 
    a.set_position_sp(200);
    a.set_speed_sp(2*get_speed());
    a.run_to_rel_pos();
}
 
void Crain::pick_and_up()
{
    c.set_speed_sp(get_speed());
    c.run_forever();
    sleep(0.9);
    Crain::up();
}
 
void Crain::move_to_finish()
{
    b.set_speed_sp(2.5*get_speed());
    b.set_position_sp(490);
    b.run_to_abs_pos();
    sleep(1);
}
 
void Crain::down()
{
    a.set_position_sp(180);
    a.set_speed_sp(1.5*get_speed());
    a.run_to_rel_pos();
    sleep(1);
    
    c.set_speed_sp(get_speed());
    c.set_position_sp(0);
    c.run_to_abs_pos();
    sleep(0.9);
}
 
void Crain::yechan_code()
{ 
    a.reset();
    b.reset();
    c.reset();
    
    Crain::up();
    
    for (int i = 0; i < 3; i++)
    {
        if (i==0) Crain::move_and_detect(1);
        else Crain::move_and_detect(-1);
        Crain::move_to_item();
        sleep(0.5);
        Crain::pick_and_up();
        Crain::move_to_finish();
        Crain::down();
        sleep(1);
        if (i == 2) break;
        Crain::up();
        sleep(2);
    }
}
 
int main()
{     
    Crain crain;
    while(1){
        if(crain.get_touch_pressed())
        { 
            Crain* crain = new Crain;
            crain->yechan_code(); 
            delete crain;
        }
    }
}