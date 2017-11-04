#include "h_crane.h"
#include <stack>

#define STARTPOS 0
#define ENDPOS 552
#define ITEMPOS 15
#define UP -280
#define DOWN -80

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
    //a상하     b좌우       c집게 
    
    //START지점 초기화
    b.set_position_sp(STARTPOS);
    b.set_speed_sp(500);
    b.set_command("run-to-abs-pos");
    //sleep(2);
    //크레인 높이 초기화
    a.set_position_sp(UP);
    a.set_speed_sp(500);
    a.set_command("run-to-abs-pos");
    a.set_stop_action("hold");
    a.set_command("stop");
    //sleep(2);
    //집게 벌려두기
    c.set_position_sp(70);
    c.set_speed_sp(500);
    c.set_command("run-to-abs-pos");
    sleep(2);
     
    //stack 선언
    std::stack<int> s;
    while(1)
    {
        a.reset();
        b.reset();
        c.reset();
        // move END 지점
        b.set_position_sp(ENDPOS);
        b.set_speed_sp(500);
        b.set_command("run-to-abs-pos");
        //sleep(2);
        
        // 타이어를 탐지하면
        if(detecting() < ITEMPOS)
        {
            b.stop();
            s.push(b.position_sp());
            //sleep(2);
        }
        
        // END 지점이면
        if(b.position_sp() == ENDPOS) // 값 조정할것
        {
            // 타이어 위치로 이동
            b.set_position_sp(s.top()-10);
            s.pop();
            b.set_speed_sp(300);
            b.set_command("run-to-abs-pos");
            sleep(2);
            // 내린다
            a.set_position_sp(DOWN);
            a.set_speed_sp(300);
            a.set_command("run-to-abs-pos");
            sleep(2);
            // 집게를 오므린다
            c.set_position_sp(0);
            c.set_speed_sp(200);
            c.set_command("run-to-abs-pos");
            sleep(2);
            // 올린다
            a.set_position_sp(UP);
            a.set_speed_sp(400);
            a.set_command("run-to-abs-pos");
            a.set_stop_action("hold");
            a.set_command("stop");
            sleep(2);
        }
        if(s.empty())
            break;
     };
     
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