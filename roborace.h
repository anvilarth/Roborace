#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
// #include "static_image.h"
#include "image.h"


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <cctype>
#include <utility>

#include <wx/textfile.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/string.h>

using namespace std;


//константы
#define PLAYER1_ID 3
#define PLAYER2_ID 4
#define TRACK 0  //road
#define GRASS 7  //not road
#define START 1
#define FINISH 2


struct player
{
  int player_id;
  int shiftx;
  int shifty;
  int startx;
  int starty;
  int curx;
  int cury;
  int key_pressed;
};





class Pole : public wxPanel
{
private:
  int cells_width;
  int cells_height;
  int screen_height;
  int screen_width;
  player p1;
  player p2;

  

public:
  // friend myFrame;

  wxFrame* parent;
  int Map[300][300];
  int turn; // -- чей ход сейчас
//эти две переменные надо загнать в приват


  Pole(wxFrame* frame, int cel_w, int cel_h, int scr_h, int scr_w);
 
  void is_collapse(); //проверка на столкновение с другим игроком или с травой
  void move(); ///сделать ход: сначала меняем скорость, потом двигаемся, заодно проверяем, что не ушли влево или вверх за границу массива
  int check_win();
  void print_map();

  void get_new_coord_player1(int, int);//////////////////
  void get_new_coord_player2(int, int);/////////////////


  void Paint(wxPaintEvent & event);
  void paintNow();
  void render(wxDC& dc);
  void keyPressed(wxKeyEvent& event);

  DECLARE_EVENT_TABLE();

};

BEGIN_EVENT_TABLE(Pole, wxPanel)

EVT_KEY_DOWN(Pole::keyPressed)

EVT_PAINT(Pole::Paint)

END_EVENT_TABLE()


// ==========================================================================================================================

class ButtonPanel : public wxPanel
{
public:
  ButtonPanel(wxFrame *parent);
  void OnLoad(wxCommandEvent& event);
  wxButton *m_newb;
  wxButton *m_loadb;
  wxButton *m_exitb;
};

const int ID_LOAD = 1;

 ButtonPanel::ButtonPanel(wxFrame* parent) :
 wxPanel(parent)
 {
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  m_newb = new wxButton(this, wxID_NEW, wxT("Новая игра"));
  m_loadb = new wxButton(this, ID_LOAD, wxT("Загрузить"));
  m_exitb = new wxButton(this, wxID_EXIT, wxT("Выход"));

  // Connect(ID_LOAD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ButtonPanel::OnLoad));

        vbox->Add(-1, 20);
        vbox->Add(m_newb);
        vbox->Add(m_loadb, 0, wxTOP, 40);
        vbox->Add(m_exitb, 0, wxTOP, 40);
        SetSizer(vbox);
 }


class mainFrame : public wxFrame
{
  int victory_was;

public:
  mainFrame(const wxString& title);
  void OnExit(wxCommandEvent& event);
  void OnNew(wxCommandEvent& event);
  void OnLoad(wxCommandEvent& event);


  wxImagePanel *image_panel;
  Pole *map;
  ButtonPanel *but_panel;
  wxBoxSizer* sizer;  

};

mainFrame::mainFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 500))
{
   victory_was = 0;

   image_panel = new wxImagePanel(this, wxT("background.jpg"), wxBITMAP_TYPE_JPEG);
            
   but_panel = new ButtonPanel(this);

   sizer = new wxBoxSizer(wxHORIZONTAL);

   sizer->Add(image_panel, 5, wxEXPAND);
   sizer->Add(but_panel, 0,  wxRIGHT | wxALIGN_RIGHT);

   Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED,   wxCommandEventHandler(mainFrame::OnExit));
   Connect(wxID_NEW, wxEVT_COMMAND_BUTTON_CLICKED,   wxCommandEventHandler(mainFrame::OnNew));
   Connect(ID_LOAD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(mainFrame::OnLoad));


   this->SetSizer(sizer);
 }


 void mainFrame::OnNew(wxCommandEvent& event)
 {
  //wxString str = wxGetTextFromUser(wxT("Введите название игры"));
    image_panel->Hide();
    but_panel->Hide();
    //map->Hide();
    // Refresh();
    // Show();
    
    // this->SetSizer(NULL,true);
    //sizer->Clear(true);
    

    map = new Pole(this, 25, 25, 50, 50);
    
    sizer->Add(map, 5, wxEXPAND);
    this->SetSizer(sizer);
    // Refresh();
    // Show();
    // map->Show();
    // Update();
    Layout();
 }

 void mainFrame::OnLoad(wxCommandEvent& event)
 {
      wxFileDialog * openFileDialog = new wxFileDialog(this);
      if (openFileDialog->ShowModal() == wxID_OK)
      {
            wxString fileName = openFileDialog->GetPath();
            wxFileInputStream input(fileName);
            wxTextInputStream intext(input);
            int cel_width, cel_height, scr_height, scr_width;
            intext >> cel_width >> cel_height >> scr_width >> scr_height;
            cout << cel_width << cel_height <<  scr_width << scr_height<< endl;
            Refresh();
            int temp;
            map = new Pole(this, cel_width, cel_height, scr_width, scr_height);
            for (int i = 0; i < 300; i++)
            {
              for (int j = 0; j < 300; j++)
              {
                map->Map[i][j] = GRASS;
              }
            }
            for (int i = 0; i <scr_width; i++)
            {
              for (int j = 0; j < scr_height; j++)
              {
                intext >> temp;
                if (temp == PLAYER1_ID)
                {
                  map->get_new_coord_player1(j, i);
                  map->Map[i][j] = START;
                }
                else if (temp == PLAYER2_ID)
                {
                  map->get_new_coord_player2(j, i);
                  map->Map[i][j] = START;
                }
                else
                {
                  map->Map[i][j] = temp;
                }
              }
            }
      }
      but_panel->Destroy();
      image_panel->Destroy();
      this->GetSizer()->Add(map, 5, wxEXPAND);
      map->Show();
      this->Layout();
 };

 void mainFrame::OnExit(wxCommandEvent& event)
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL,   wxT("Вы точно хотите выйти?"), wxT("Question"),
                  wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
  
  dial->SetYesNoLabels(_("&No"), _("&Yes"));
  if(dial->ShowModal() == wxID_NO)
  {
    Close(true);
  }
 }

// ================================================================================================================================

class MyApp : public wxApp
{
  virtual bool OnInit();
};


bool MyApp::OnInit()
{
  wxInitAllImageHandlers();

  //wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  //sizer->Add(drawPane, 1, wxEXPAND);

  // frame->SetSizer(sizer);
  // frame->SetAutoLayout(true);
  mainFrame* frame = new mainFrame(wxT("NFS"));

  // int shift_x1 = 0, shift_y1 = 1;
    // int shift_x2 = 1, shift_y2 = 0;

    frame->Show();
    
    // (frame->map)->Hide();
    //(frame->image_panel)->Hide();
    // (frame->but_panel)->Hide();
    // frame->Refresh();
    // map->Refresh();

    return true;

}



// ================================================================================================================================



/*
void game( wxFrame* frame, int a, int b, int c, int d)
{

  
          
      // while(!new_map->check())
      //{
      //    new_map->move(1,1);
      //}  

      //cout << new_map->check();
      //cout << new_map->is_collapse();
     
      //new_map->move(shift_x1, shift_y1);

      //new_map->move(shift_x2, shift_y2);
    

    //sleep(500);
    
    

    //new_map->check()


  //cout << new_map->check() << "won" << "\n";
  //cout << "GAME OVER\n";


};
*/
//================================================================================================================


/*
void Pole::OnMouseEvent(wxMouseEvent& event)
{
    const wxPoint pt = wxGetMousePosition();
  int mouseX = pt.x - this->GetScreenPosition().x;
  int mouseY = pt.y - this->GetScreenPosition().y;

  wxPaintDC dc(this);
    //wxMessageBox(wxString::Format("x: %i, y: %i",mouseX, mouseY));
    
    wxColour red;

  red.Set(wxT("#ff0000"));
  
    dc.SetBrush(wxBrush(red));  
    dc.DrawRectangle(mouseX, mouseY, cells_height, cells_width);

    //event.Skip();
}
*/




/*

void Pole::print_map()
{
  for (int i = 0; i < screen_height + 1; i++)
  {
    for (int j = 0; j < screen_width + 1; j++)
    {
      cout << setw(3)<< Map[i][j];
    }
    cout << endl;
  }

  cout << endl << endl << endl;
};

*/




//открытие файла и подгрузка в матрицу данных

/*
void load_map()//загрузить карту из файла
{
  file_with_Map.open("./Map1");

  //cout << "loading Map matrix from file\n";

  height = -1;

  if (file_with_Map.is_open())
  {
      while (getline(file_with_Map, line) )
      {
        height++;

        line_lenght = line.size();

        width = -1;

        for (int item = 0; item < line_lenght; item++)
          {
            if (isdigit(line[item]))
            {
              width++;
              Map[height][width] = atoi(&line[item]) ;
              //cout << setw(3) << line[item] <<"->"<< Map[height][width];
            }
          }
          //cout << endl;
      }

      file_with_Map.close();
    }

    else cout << "Unable to open file";
};
*/


//================================================================================================================
