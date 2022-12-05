#pragma once

#include <SFML/Graphics.hpp>

namespace nbi
{
    enum key_action
    {
        key_press,
        key_release
    };
    
    struct key_t
    {
        bool ctrl = false;
        bool shift = false;
        bool alt = false;
        char principal = '@';
        key_action action = key_press;
        key_t(){}
        key_t(bool ctrl_in, bool shift_in, bool alt_in)
        {
            ctrl  = ctrl_in;
            shift = shift_in;
            alt   = alt_in;
            principal = '@';
        }
        key_t(char principal_in)
        {
            ctrl  = false;
            shift = false;
            alt   = false;
            principal = principal_in;
        }
        key_t operator+(const key_t& rhs)
        {
            key_t output;
            output.ctrl  = rhs.ctrl  || ctrl;
            output.shift = rhs.shift || shift;
            output.alt   = rhs.alt   || alt;
            output.principal = rhs.principal;
            output.action = key_press;
            return output;
        }
        
        bool operator==(const key_t& rhs)
        {
            bool output = true;
            output = output && (ctrl      == rhs.ctrl);
            output = output && (shift     == rhs.shift);
            output = output && (alt       == rhs.alt);
            output = output && (principal == rhs.principal);
            output = output && (action    == rhs.action);
            return output;
        }
        
        int get_val() const
        {
            int output = 0;
            output += principal;
            output += 1000*ctrl;
            output += 10000*shift;
            output += 100000*alt;
            output += 1000000*(int)action;
            return output;
        }
        
        std::string get_string() const
        {
            std::string output = "";
            if (action == key_release) output += "!";
            if (ctrl)                  output += "ctrl";
            if (ctrl && principal!=0)  output += "+";
            if (shift)                 output += "shift";
            if (shift && principal!=0) output += "+";
            if (alt)                   output += "alt";
            if (alt && principal!=0)   output += "+";
            if (principal!=0)          output += principal;
            return output;
        }
        
        bool operator < (const key_t& rhs) const
        {
            return get_val() < rhs.get_val();
        }
        
    };

    namespace key
    {
        static key_t ctrl (true,  false, false);
        static key_t shift(false, true,  false);
        static key_t alt  (false, false, true);
        //
        static key_t a('a');
        static key_t b('b');
        static key_t c('c');
        static key_t d('d');
        static key_t e('e');
        static key_t f('f');
        static key_t g('g');
        static key_t h('h');
        static key_t i('i');
        static key_t j('j');
        static key_t k('k');
        static key_t l('l');
        static key_t m('m');
        static key_t n('n');
        static key_t o('o');
        static key_t p('p');
        static key_t q('q');
        static key_t r('r');
        static key_t s('s');
        static key_t t('t');
        static key_t u('u');
        static key_t v('v');
        static key_t w('w');
        static key_t x('x');
        static key_t y('y');
        static key_t z('z');
        //
        static key_t n0('0');
        static key_t n1('1');
        static key_t n2('2');
        static key_t n3('3');
        static key_t n4('4');
        static key_t n5('5');
        static key_t n6('6');
        static key_t n7('7');
        static key_t n8('8');
        static key_t n9('9');
        //
        static key_t esc('E');
        static key_t tab('T');
        static key_t del('D');
        static key_t nil('@');
        
        key_t press(const key_t& key_in)
        {
            key_t output  = key_in;
            output.action = key_press;
            return output;
        }
        
        key_t release(const key_t& key_in)
        {
            key_t output  = key_in;
            output.action = key_release;
            return output;
        }
    }
    
    static bool is_key_event(const sf::Event& event, key_t& key_out)
    {
        //this is a mess... oh well
        using namespace key;
        if (event.type != sf::Event::KeyPressed && event.type != sf::Event::KeyReleased)
        {
            key_out = nil;
            return false;
        }
        key_t output;
        switch (event.key.code)
        {
            case sf::Keyboard::A:      { output = a;   break;}
            case sf::Keyboard::B:      { output = b;   break;}
            case sf::Keyboard::C:      { output = c;   break;}
            case sf::Keyboard::D:      { output = d;   break;}
            case sf::Keyboard::E:      { output = e;   break;}
            case sf::Keyboard::F:      { output = f;   break;}
            case sf::Keyboard::G:      { output = g;   break;}
            case sf::Keyboard::H:      { output = h;   break;}
            case sf::Keyboard::I:      { output = i;   break;}
            case sf::Keyboard::J:      { output = j;   break;}
            case sf::Keyboard::K:      { output = k;   break;}
            case sf::Keyboard::L:      { output = l;   break;}
            case sf::Keyboard::M:      { output = m;   break;}
            case sf::Keyboard::N:      { output = n;   break;}
            case sf::Keyboard::O:      { output = o;   break;}
            case sf::Keyboard::P:      { output = p;   break;}
            case sf::Keyboard::Q:      { output = q;   break;}
            case sf::Keyboard::R:      { output = r;   break;}
            case sf::Keyboard::S:      { output = s;   break;}
            case sf::Keyboard::T:      { output = t;   break;}
            case sf::Keyboard::U:      { output = u;   break;}
            case sf::Keyboard::V:      { output = v;   break;}
            case sf::Keyboard::W:      { output = w;   break;}
            case sf::Keyboard::X:      { output = x;   break;}
            case sf::Keyboard::Y:      { output = y;   break;}
            case sf::Keyboard::Z:      { output = z;   break;}
            case sf::Keyboard::Num0:   { output = n0;  break;}
            case sf::Keyboard::Num1:   { output = n1;  break;}
            case sf::Keyboard::Num2:   { output = n2;  break;}
            case sf::Keyboard::Num3:   { output = n3;  break;}
            case sf::Keyboard::Num4:   { output = n4;  break;}
            case sf::Keyboard::Num5:   { output = n5;  break;}
            case sf::Keyboard::Num6:   { output = n6;  break;}
            case sf::Keyboard::Num7:   { output = n7;  break;}
            case sf::Keyboard::Num8:   { output = n8;  break;}
            case sf::Keyboard::Num9:   { output = n9;  break;}
            case sf::Keyboard::Escape: { output = esc; break;}
            case sf::Keyboard::Tab:    { output = tab; break;}
            case sf::Keyboard::Delete: { output = del; break;}
        }
        if (event.key.control) output.ctrl  = true;
        if (event.key.alt)     output.alt   = true;
        if (event.key.shift)   output.shift = true;
        if ((event.key.code == sf::Keyboard::LControl) || (event.key.code == sf::Keyboard::RControl)) output.ctrl  = true;
        if ((event.key.code == sf::Keyboard::LShift)   || (event.key.code == sf::Keyboard::RShift))   output.shift = true;
        if ((event.key.code == sf::Keyboard::LAlt)     || (event.key.code == sf::Keyboard::RAlt))     output.alt   = true;
        if (event.type == sf::Event::KeyReleased)
        {
            output = release(output);
        }
        if (event.type == sf::Event::KeyPressed)  output = press(output);
        key_out = output;
        return true;
    }
}