#pragma once

namespace nbi
{
    enum mouse_t
    {
        mouse_move,
        mouse_lclick,
        mouse_rclick,
        mouse_rrelease,
        mouse_lrelease,
        mouse_uscroll,
        mouse_dscroll,
        mouse_ldrag,
        mouse_rdrag
    };
    
    static std::string get_mouse_str(const mouse_t& mous)
    {
        switch(mous)
        {
            case mouse_move:     {return "mouse_move";}
            case mouse_lclick:   {return "mouse_lclick";}
            case mouse_rclick:   {return "mouse_rclick";}
            case mouse_rrelease: {return "mouse_rrelease";}
            case mouse_lrelease: {return "mouse_lrelease";}
            case mouse_uscroll:  {return "mouse_uscroll";}
            case mouse_dscroll:  {return "mouse_dscroll";}
            case mouse_ldrag:    {return "mouse_ldrag";}
            case mouse_rdrag:    {return "mouse_rdrag";}
        }
        return "";
    }
    
    struct mouse_state_t
    {
        bool left = false;
        bool right = false;
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            left = true;
        }
        void on_lrelease(const sf::Vector2f& pos, canvas_t& data)
        {
            left = false;
        }
        void on_rclick(const sf::Vector2f& pos, canvas_t& data)
        {
            right = true;
        }
        void on_rrelease(const sf::Vector2f& pos, canvas_t& data)
        {
            right = false;
        }
    };
    
    static bool is_mouse_event(const sf::Event& event, mouse_t& mouse_out, const mouse_state_t& mouse_state)
    {
        bool output = false;
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                mouse_out = mouse_lclick;
                output = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                mouse_out = mouse_rclick;
                output = true;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                mouse_out = mouse_lrelease;
                output = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                mouse_out = mouse_rrelease;
                output = true;
            }
        }
        if (event.type == sf::Event::MouseMoved)
        {
            mouse_out = mouse_move;
            output = true;
        }
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                int delta = event.mouseWheelScroll.delta;
                if (delta < 0) mouse_out = mouse_dscroll;
                if (delta > 0) mouse_out = mouse_uscroll;
                output = true;
            }
        }
        if (mouse_out == mouse_move && mouse_state.left)  mouse_out = mouse_ldrag;
        if (mouse_out == mouse_move && mouse_state.right) mouse_out = mouse_rdrag;
        return output;
    }
}