#include "h_crane.h"
#include <stack>

#define STARTPOS 0
#define ENDPOS 550
#define ITEMPOS 10.0
#define UP -250
#define DOWN 0

class Crain : public CraneCrane
{
private:
    ev3dev::touch_sensor touch_q;
    ev3dev::ultrasonic_sensor ultrasonic_q; // 울트라 센서
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
{
    /*
    a.reset();
    b.reset();
    c.reset();
    
    a.set_position_sp(-300);
    a.set_speed_sp(300);
    a.set_command("run-to-abs-pos");
    
    
    while(1)
    {
        if (detecting() > 10)
        {
            a.set_speed_sp(-0.1);
            a.run_forever();
            break;
        }
    }
    sleep(1);
    
    
    for(int i=0; i<3; i++)
    {
        
        while (true)
        {
            if (detecting() <= 10)
            {
                b.stop();
                b.set_position_sp(20);
                b.set_speed_sp(300);
                b.run_to_rel_pos();
                b.stop();
                break;
            }
        }
        sleep(1);
        
        b.set_position_sp(550);
        b.set_speed_sp(300);
        b.run_to_abs_pos();
        b.stop_action();
        
        a.set_position_sp(0);
        a.set_speed_sp(90);
        a.run_to_abs_pos();
        a.stop_action();
        sleep(1);
        
        c.set_speed_sp(300);
        c.run_forever();
        sleep(1);
        
        a.set_position_sp(-200);
        a.set_speed_sp(300);
        a.run_to_rel_pos();
        a.stop_action();
        
        b.set_speed_sp(300);
        b.set_position_sp(550);
        b.run_to_abs_pos();
        sleep(1);
        
        a.set_position_sp(20);
        a.set_speed_sp(100);
        a.run_to_rel_pos();
        sleep(1);
        
        c.set_speed_sp(300);
        c.set_position_sp(0);
        c.run_to_abs_pos();
        sleep(0.9);
        
        if(i==2) break;
        
        a.set_position_sp(-300);
        a.set_speed_sp(200);
        a.run_to_abs_pos();

        while(true)
        {
            if (detecting() >= 40)
            {
                a.set_speed_sp(-0.73);
                a.run_forever();
                break;
            }
        }
        b.set_position_sp(10);
        b.set_speed_sp(get_speed());
        b.run_to_abs_pos();
    }
*/
    a.reset();
    b.reset();
    c.reset();
    
    
    b.set_position_sp(STARTPOS);
    b.set_speed_sp(500);
    b.set_command("run-to-abs-pos");
    sleep(2);
    
    //크레인 높이 초기화
    a.set_position_sp(UP);
    a.set_speed_sp(700);
    a.set_command("run-to-abs-pos");
    a.set_speed_sp(7);
    a.run_forever();
    //a.set_stop_action("hold");
    //a.set_command("stop");
    //sleep(2);
    
    //집게 벌려두기
    c.set_position_sp(0);
    c.set_speed_sp(500);
    c.set_command("run-to-abs-pos");
    sleep(2);
     
    //stack 선언
    std::stack<int> s;
    while(1)
    {
        // END 지점으로 이동
        b.set_position_sp(5);
        b.set_speed_sp(500);
        b.set_command("run-to-rel-pos");
        //sleep(1);
        //b.set_position_sp(ENDPOS);
        //b.set_speed_sp(300);
        //b.run_forever();
        //b.set_command("run-to-abs-pos");
        //sleep(2);
        
        if (detecting() < ITEMPOS)// 타이어를 탐지하면
        {
            s.push(b.position_sp()); // 위치조정
            std::cout << b.position_sp() << std::endl;
            std::cout << s.top() << std::endl;
        }
        
        // END 지점이면
        if(b.position_sp() == ENDPOS)
        {
            b.stop();
            while(1)
            {
            // 타이어 위치로 이동
            b.set_position_sp(s.top());
            s.pop();
            b.set_speed_sp(500);
            b.set_command("run-to-abs-pos");
            sleep(2);
            // 내린다
            a.set_position_sp(DOWN);
            a.set_speed_sp(500);
            a.set_command("run-to-abs-pos");
            sleep(2);
            // 집게를 오므린다
            c.set_position_sp(-70);
            c.set_speed_sp(500);
            c.set_command("run-to-rel-pos");
            //sleep(2);
            // 올린다
            a.set_position_sp(UP);
            a.set_speed_sp(500);
            a.set_command("run-to-abs-pos");
            a.set_stop_action("brake");
            a.set_command("stop");
            sleep(2);
            }
            if(s.empty())
                break;
        }
        
        
        a.stop();
        b.stop();
        c.stop();
     };
}     
     /*   
        if(b.position_sp() == 0)   // END 지점이면
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
    */


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