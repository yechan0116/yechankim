#include "h_crane.h"

class Crain : public CraneCrane
{
private:
    ev3dev::touch_sensor touch_q;
    ev3dev::ultrasonic_sensor ultrasonic_q; // 적외선 센서
    ev3dev::motor a; // 상하
    ev3dev::motor b; // 좌우
    ev3dev::motor c; // 집게
    
public:
    // Hardware Configuration
    Crain():m_speed(0), touch_q(ev3dev::INPUT_2), ultrasonic_q(ev3dev::INPUT_1), a(ev3dev::OUTPUT_B), b(ev3dev::OUTPUT_C), c(ev3dev::OUTPUT_A)
    {
        
    }
    
    int m_speed;
    
    bool get_touch_pressed()
    {
        return touch_q.is_pressed();
    }
    
    int detecting()
    {
        return ultrasonic_q.distance_centimeters();
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
        return 1000;
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
};

void Crain::my_code()
{ //This function is for example, you should develop your own logics
    //while(get_escape() == false)
    //{
        /*
        set_down(ev3dev::button::down.pressed());
        set_up(ev3dev::button::up.pressed());
        set_right(ev3dev::button::right.pressed());
        set_left(ev3dev::button::left.pressed());
        set_escape(ev3dev::button::back.pressed());
        set_enter(ev3dev::button::enter.pressed());
        
        
        if(get_up())
        {   
                a.set_speed_sp(-1*get_speed());
                a.run_forever();
        }   
        if(get_down())
        {
                a.set_speed_sp(get_speed());
                a.run_forever();
        }
        if(get_left())
        {
               b.set_speed_sp(get_speed());
               b.run_forever();
        }
        if(get_right())
        {
               b.set_speed_sp(-1* get_speed());
               b.run_forever();
        }
       
       
        if(!(get_up() | get_down() | get_right() | get_left() | get_enter()))
        {
            a.set_speed_sp(0);
            a.run_forever();
            b.set_speed_sp(0);
            b.run_forever();
        }
    }

    a.stop();
    b.stop();
    */
    
    //a상하     b좌우       c집게 
    
    //START지점 초기화, 크레인 높이 초기화, 집게 벌려두기
    
    b.set_position_sp(0);
    b.set_speed_sp(400);
    b.set_command("run-to-abs-pos");
    sleep(2);
    a.set_position_sp(110);
    a.set_speed_sp(400);
    a.set_command("run-to-abs-pos");
    sleep(2);
    c.set_position_sp(70);
    c.set_speed_sp(400);
    c.set_command("run-to-rel-pos");
    sleep(2);
    
    while (1)
    {
        if(b.position_sp() == 560)   // END 지점이면
        {
            //내린다
            a.set_position_sp(270);
            a.set_speed_sp(400);
            a.set_command("run-to-abs-pos");
            sleep(2);
            //집게를 벌린다
            c.set_position_sp(70);
            c.set_speed_sp(400);
            c.set_command("run-to-rel-pos");
            sleep(2);
            //올린다
            a.set_position_sp(110);
            a.set_speed_sp(400);
            a.set_command("run-to-abs-pos");
            sleep(2);
            // START 지점으로 간다
            b.set_position_sp(0);
            b.set_speed_sp(400);
            b.set_command("run-to-abs-pos");
            sleep(2);
        }
        if(detecting() < 15)    // item 감지하면
        {
            // 좌우모터 멈춘다
            //b.stop();
            b.set_stop_action("hold"); // hold 함수
            // 내린다
            a.set_position_sp(270);
            a.set_speed_sp(300);
            a.set_command("run-to-abs-pos");
            sleep(2);
            // 집게를 오므린다
            c.set_position_sp(-70);
            c.set_speed_sp(200);
            c.set_command("run-to-rel-pos");
            sleep(2);
            // 올린다
            a.set_position_sp(110);
            a.set_speed_sp(400);
            a.set_command("run-to-abs-pos");
            sleep(2);
        }
        // 좌우모터 10씩 이동
        b.set_position_sp(10);
        b.set_speed_sp(400);
        b.set_command("run-to-rel-pos");
        sleep(1);
    };
    /*
    b.set_position_sp(90);
    b.set_speed_sp(200);
    b.set_command("run-to-abs-pos");
    sleep(3);
    
    
    //내리고
    a.set_position_sp(160);
    a.set_speed_sp(200);
    a.set_command("run-to-rel-pos");
    sleep(3);
    
    //벌리고
    c.set_position_sp(70);
    c.set_speed_sp(200);
    c.set_command("run-to-rel-pos");
    sleep(1);
    
    //집고
    c.set_position_sp(-70);
    c.set_speed_sp(200);
    c.set_command("run-to-rel-pos");
    sleep(1);
    
    //올리고
    a.set_position_sp(-200);
    a.set_speed_sp(200);
    a.set_command("run-to-rel-pos");
    sleep(3);
    }
    */    
}

int main()
{     
    Crain crain;
    while(true){
        if(crain.get_touch_pressed()==true){ 
            
        
        crain.my_code(); //This line is for example, you should erase this ex_code in your 'real code' 
  
        }
    }
}