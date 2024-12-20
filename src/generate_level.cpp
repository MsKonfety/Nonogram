#include "generate_level.h"
#include "wrapper.h"

#include <iostream>
#include <typeinfo>

void Generate_level_window::cb_save_button(Graph_lib::Address, Graph_lib::Address addr)
{
    auto* pb = static_cast<Graph_lib::Button*>(addr);
    static_cast<Generate_level_window&>(pb->window()).save_button();
}

void Generate_level_window::save_button()
{
    level_size = sizes[size_box.get_value()];

    str_level_name = level_name.get_string();
    
    if (str_level_name == "")
    {
        str_level_name = "level " + std::to_string(own.db_levels.get_new_id(level_size));
    }

    if(filename == "")
    {
        image_chooser.change_state(Graph_lib::File_chooser_box::State::ERR);
        redraw();
        fl_alert("please specify the path to the file");
    }
    else
    {
        Level level {str_level_name, level_size, filename};
        level.init();

        Database_levels::Response res = own.db_levels.add_level(level);
        if (res == Database_levels::ALREADY_EXISTS) 
        {
            image_chooser.change_state(Graph_lib::File_chooser_box::State::OK);
            level_name.set_color(FL_RED);
            redraw();
            fl_alert("this level is already exsists");
            return;
        }
        button_pushed = true;
    }
}

Generate_level_window::Generate_level_window(Graph_lib::Point xy, int w, int h,
                                             const std::string &title, Windows_wrapper &own)
    : Window_with_back{xy, w, h, title}, own{own},
      size_box{size_box_point, size_box_width, size_box_height, "size: "},
      image_chooser{image_chooser_point, image_chooser_width, image_chooser_height, " ", "choose image", "Image Files (*.{jpg,png})", save_image, cb_choose_file},
      level_name{level_name_point, level_name_width, level_name_height, "level name: "}
{
    Window_with_back::size_range(w, h, w, h);

    attach(size_box);
    size_box.set_font_size(18);
    for (unsigned int i = 10; i < 30; i += 5)
    {
        Size s {i, i};
        sizes.push_back(s);
        size_box.add(to_string(s));
    }
    size_box.set_value(0);

    attach(image_chooser);
    image_chooser.set_font_size(gen_lvl_text_size);

    Graph_lib::Button save_button{save_button_point, save_button_width, save_button_height, "save", cb_save_button};
    attach(save_button);
    attach(level_name);
    save_button.set_font_size(gen_lvl_text_size);
    level_name.set_font_size(gen_lvl_text_size);
}

void Generate_level_window::cb_choose_file(Graph_lib::Address, Graph_lib::Address addr)
{
    auto* pb = static_cast<Graph_lib::Button*>(addr);
    static_cast<Generate_level_window&>(pb->window()).image_chooser.choose_file();
}

void Generate_level_window::save_image(Graph_lib::Window *own, const std::string& filename) {
    static_cast<Generate_level_window*>(own)->filename = filename;
};