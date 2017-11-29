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
    // Hardware Configuration
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
        return 235; // 230
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
    void go_up(float speed);
    void move_to_detect(int n);
    void move_to_target();
    void pick_and_up();
    void move_to_finish();
    void go_down_and_open();
    void move_to_start();
};
 
void Crain::move_to_start()
{
    //move to start
    b.set_speed_sp(2.5*get_speed());
    b.set_position_sp(-400);
    b.run_to_abs_pos();
    sleep(1);
}

 
void Crain::go_up(float speed)
{
    //up
    a.set_speed_sp(speed*get_speed());
    a.set_position_sp(-300);
    a.run_to_abs_pos();
    sleep(1);
    a.set_speed_sp(-1);
    a.run_forever();
}
 
void Crain::move_to_detect(int n)
{
    //move untill target is found
    b.set_speed_sp(n*get_speed());
    b.run_forever();
    while (true)
    {
        if (distance() <= 11.1) // && distance() >= 7.7
        {
            b.stop();
            break;
        }
    }
}
 
void Crain::move_to_target()
{
    //move to target
    b.set_position_sp(-5); // -5
    b.set_speed_sp(50); //
    b.run_to_rel_pos();
 
    
    a.set_position_sp(400); // 300
    a.set_speed_sp(1.8*get_speed());
    a.run_to_rel_pos();
 
}
 
void Crain::pick_and_up()
{
    //pick and up the target
    c.set_speed_sp(get_speed());
    c.run_forever();
    sleep(0.9);
    Crain::go_up(1.8);
}
 
void Crain::move_to_finish()
{
    //move to finish
    b.set_speed_sp(2.5*get_speed());
    b.set_position_sp(485); // 480
    b.run_to_abs_pos();
    sleep(1);
}
 
void Crain::go_down_and_open()
{
    //go down
    a.set_position_sp(200); // 190
    a.set_speed_sp(1.5*get_speed());
    a.run_to_rel_pos();
    sleep(1);
    //open tongs
    c.set_speed_sp(get_speed());
    c.set_position_sp(0);
    c.run_to_abs_pos();
    sleep(0.9);
}
 
 
void Crain::yechan_code()
{
    //b.reset();
    //Crain::move_to_start();
    a.reset();
    b.reset();
    c.reset();
    
    Crain::go_up(2.0);
    for (int i=0;i<3;i++)
    {
        if (i==0)
            Crain::move_to_detect(1);
        else
            Crain::move_to_detect(-1);
        Crain::move_to_target();
      
        sleep(2); // 1
        
        Crain::pick_and_up();
        Crain::move_to_finish();
        Crain::go_down_and_open();
        if (i == 2)
        break;
        Crain::go_up(2.0);
        sleep(0.8);
        
    }
      
}
 
int main()
{     
    Crain crain;
    bool flag = true;
    while(flag){
        if(crain.get_touch_pressed()==true)
        { 
            Crain* crain = new Crain;
            crain->yechan_code(); 
            delete crain;
            flag = false;
        }
    }
}