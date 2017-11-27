    //a가 위아래, b양옆, c 집게

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
        return 350;
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
    void my_code();
    void move_to_start();
    void go_up();
    void move_to_detect();
    void move_to_target();
    void pick_and_up();
    void move_to_finish();
    void go_down_and_open();
};

void Crain::move_to_start()
{

    b.set_position_sp(10);
    b.set_speed_sp(get_speed());
    b.run_to_abs_pos();
}

void Crain::go_up()
{
    //up
    a.set_position_sp(-200);
    a.set_speed_sp(700);
    a.set_command("run-to-rel-pos");
    while(true)
    {
        if (distance() > 11.0)
        {
            a.set_speed_sp(-2);
            a.run_forever();
            break;
        }
    }
    sleep(1);
}

void Crain::move_to_detect()
{
    //move untill target is found
    b.set_speed_sp(0.5*get_speed());
    b.run_forever();
    while (1)
    {
        if (distance() <= 11.0)
        {
            b.stop();
            
            break;
        }
    }
}

void Crain::move_to_target()
{
    //move to target
    b.set_position_sp(58);
    b.set_speed_sp(get_speed());
    b.run_to_rel_pos();
    b.stop_action();
    
    a.set_position_sp(15);
    a.set_speed_sp(get_speed()/4);
    a.run_to_rel_pos();
    a.stop_action();

}

void Crain::pick_and_up()
{
    //pick and up the target
    c.set_speed_sp(get_speed());
    c.run_forever();
    sleep(1);
    Crain::go_up();
}

void Crain::move_to_finish()
{
    //move to finish
    b.set_speed_sp(get_speed());
    b.set_position_sp(615);
    b.run_to_abs_pos();
    sleep(1);
}

void Crain::go_down_and_open()
{
    //go down
    a.set_position_sp(20);
    a.set_speed_sp(get_speed());
    a.run_to_rel_pos();
    sleep(1);
    //open tongs
    c.set_speed_sp(get_speed());
    c.set_position_sp(0);
    c.run_to_abs_pos();
    sleep(0.9);
}

void Crain::my_code()
{
    //a.reset();
    b.reset();
    c.reset();
    
    Crain::go_up();
    /*
    for (int i=0;i<3;i++)
    {
        Crain::move_to_detect();
        sleep(0.9);
        
        Crain::move_to_target();
        sleep(1);
        
        Crain::pick_and_up();
        Crain::move_to_finish();
        Crain::go_down_and_open();
        if (i == 2)
            break;
        Crain::go_up();
        Crain::move_to_start();
        sleep(2);
    }
    */
        
}

int main()
{     
    Crain crain;
    while(true){
        if(crain.get_touch_pressed()==true){ 
            
        Crain* crain = new Crain;
        crain->my_code(); //This line is for example, you should erase this ex_code in your 'real code' 
        delete crain;
        }
    }
}