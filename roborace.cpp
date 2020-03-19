#include "roborace.h"



Pole::Pole( wxFrame * frame, int cel_w, int cel_h, int scr_h, int scr_w) : wxPanel(frame)
{
  cells_width = cel_w;
  cells_height = cel_h;

  screen_width = scr_w;
  screen_height = scr_h;

  turn = PLAYER1_ID;

  p1.player_id = PLAYER1_ID;
  p2.player_id = PLAYER2_ID;

  parent = frame;

// ============================================================================================[
  // создание шаблонной карты для игры
  

  for (int i = 0; i <= screen_height + 1; i++)
  {
    for (int j = 0; j <= screen_width + 1; j++)
    {
      if ((j <= 12 && j >= 6 && i >= 0 && i <= 20))
      {
        Map[i][j] = TRACK;
      }
      else if ((j >= 6 && j < screen_width - 1 && i >= 14 && i <= 21))
      {
        Map[i][j] = TRACK;
      }
      else
      {
        Map[i][j] = GRASS;
      }
    }
  }
//отрисовка границ
/*
  for (int i = 6; i < screen_height; i++)
  {
    Map[i][20] = GRASS;
  }

  for (int i = 12; i < screen_height; i++)
  {
    Map[i][14] = GRASS;
  }

  for (int j = 0; j < 20; j++)
  {
    Map[6][j] = GRASS;
  }

  for (int j = 0; j < 14; j++)
  {
    Map[12][j] = GRASS;
  }
*/
  for (int j = 14; j <= 21; j++)
  {
    Map[j][screen_width-1] = FINISH;
  }

  for (int i = 6; i < 13; i++)
  {
    Map[0][i] = START;
  }
  p1.shiftx = 0; p1.shifty = 0;//first player
  p1.curx = 7; p1.cury = 0;
  p1.startx = 7; p1.starty = 0;
  p2.shiftx = 0; p2.shifty = 0;//second player
  p2.curx = 10; p2.cury = 0;
  p2.startx = 10; p2.starty = 0;
  Map[p1.cury][p1.curx] = PLAYER1_ID;
  Map[p2.cury][p2.curx] = PLAYER2_ID;





// ]===================================================================================
};




void Pole::get_new_coord_player1(int x, int y)
{
  p1.curx = x;
  p1.cury = y;
  p1.shiftx = 0;
  p1.shifty = 0;
  p1.key_pressed = 0;
  p1.startx = x;
  p1.starty = y;
};

void Pole::get_new_coord_player2(int x, int y)
{
  p2.curx = x;
  p2.cury = y;
  p2.shiftx = 0;
  p2.shifty = 0;
  p2.key_pressed = 0;
  p2.startx = x;
  p2.starty = y;
};


void Pole::is_collapse() //проверка на столкновение с другим или с травой
{
  if (p1.curx == p2.curx && p1.cury == p2.cury)//столкновение гонщиков
  {
    Map[p1.cury][p1.curx] = TRACK;
    Map[p2.cury][p2.curx] = TRACK;
    p1.curx = p1.startx;
    p1.cury = p1.starty;
    p1.shiftx = 0;
    p1.shifty = 0;
    p2.curx = p2.startx;
    p2.cury = p2.starty;
    p2.shiftx = 0;
    p2.shifty = 0;
    //Map[p1.cury][p1.curx] = PLAYER1_ID;
    //Map[p2.cury][p2.curx] = PLAYER2_ID;
  }                     //два следующие ифа неверные
  /*else if(Map[p1.cury][p1.curx] == GRASS)
  {
    Map[p1.cury][p1.curx] = GRASS;
    p1.curx = p1.startx; p1.cury = p1.starty;
    p1.shiftx = 0; p1.shifty = 0;
    Map[p1.curx][p1.cury] =PLAYER1_ID;
  }

  if (Map[p2.curx][p2.cury] == GRASS)
  {
    Map[p2.curx][p2.cury] = GRASS;
    p2.curx = p2.startx; p2.cury = p2.starty;
    p2.shiftx = 0; p2.shifty = 0;
    Map[p2.curx][p2.cury] = PLAYER2_ID;
  }*/

                                                          //нахера тут контролировать выход за границы экрана??
  /*if (p1.curx >= screen_width || p1.cury >= screen_height )//тут была ошибка
  {
    Map[p1.curx][p1.cury] = GRASS;
    p1.cury = p1.starty;
    p1.curx = p1.startx;
    p1.shiftx = 0;
    p1.shifty = 0;
    Map[p1.curx][p1.cury] = PLAYER1_ID;
  }
  if (p2.curx >= screen_width || p2.cury >= screen_height)
  {
    Map[p2.curx][p2.cury] = GRASS;
    p2.curx = p2.startx;
    p2.cury = p2.starty;
    p2.shiftx = 0;
    p2.shifty = 0;
    Map[p2.curx][p2.cury] = PLAYER2_ID;
  }*/
};

//тут параметры это текущий итоговый вектор сдвига, а не его изменение

void Pole::move() ///сделать ход: сначала меняем скорость, потом двигаемся, заодно проверяем, что не ушли влево или вверх за границу массива
{
	wxPaintDC dc(this);
  if (turn  == PLAYER1_ID)
  {
    //Map[p1.cury][p1.curx] = TRACK;
    p1.curx += p1.shiftx;
    p1.cury += p1.shifty;


  if ( check_win() )
  {
    // cout << "PLAYER " << turn-2 <<" WON \n GAME OVER \n";
    wxMessageBox(wxString::Format("Player %i won\n GAME OVER", turn-2));
    exit(0);
    /*
    mainFrame * temp = parent;

    (temp->image_panel)->Show();

    (temp->but_panel)->Show();
    
    temp->SetSizer(sizer);
 
    temp->Layout();

    this->Destroy();
*/
  }



    // cout << "my1 coord " << p1.curx << " " << p1.cury << endl;
    if (p1.curx < 0 || p1.cury < 0 || p1.cury >= screen_width || p1.curx >= screen_height || Map[p1.cury][p1.curx] == GRASS)
    {
      p1.curx = p1.startx;
      p1.cury = p1.starty;
      p1.shiftx = 0;
      p1.shifty = 0;
    }
    is_collapse();
    //Map[p1.cury][p1.curx] = PLAYER1_ID;
    turn = PLAYER2_ID;
  }
  else
  {
  //Map[p2.cury][p2.curx] = TRACK;
    p2.curx += p2.shiftx;
    p2.cury += p2.shifty;



  if ( check_win() )
  {
    //cout << "PLAYER " << turn-2 <<" WON \n GAME OVER \n";
    wxMessageBox(wxString::Format("Player %i won\n GAME OVER", turn-2));
    exit(0);
    
/*
    mainFrame * temp = parent;

    (temp->image_panel)->Show();

    (temp->but_panel)->Show();
    
    temp->SetSizer(sizer);
 
    temp->Layout();

    this->Destroy();
    */
  }


    // cout << "want2 to change " << p2.shiftx << " " << p2.shifty << endl;
    // cout << "my2 coord " << p2.curx << " " << p2.cury << endl;
    if (p2.curx < 0 || p2.cury < 0 || p2.cury >= screen_width || p2.curx >= screen_height || Map[p2.cury][p2.curx] == GRASS)
    {
      p2.curx = p2.startx;
      p2.cury = p2.starty;
      p2.shiftx = 0;
      p2.shifty = 0;
    }
    is_collapse();
  //  Map[p2.cury][p2.curx] = PLAYER2_ID;
    turn = PLAYER1_ID;
  }
};




int Pole::check_win()//проверка пересечения финиша игроком сделавшим последний ход
{
  if (turn == PLAYER2_ID)//выиграл ли второй игрок (если последний ход его)
  {
    int prevx = p2.curx - p2.shiftx;
    int prevy = p2.cury - p2.cury;

    if (prevx > p2.curx && prevy > p2.cury)
    {
      for (int i = p2.curx; i <= prevx; i++)
      {
        for (int j = p2.cury; j <= prevy; j++)
        {
          if (Map[j][i] == FINISH)
          {
            // cout << "oops\n";
            return PLAYER2_ID;
          }
        }
      }
    }
    if (prevx <= p2.curx && prevy > p2.cury)
    {
      for (int i = prevx; i <= p2.curx; i++)
      {
        for (int j = p2.cury;  j <= prevy; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER2_ID;
          }
        }
      }
    }
    if (prevy <= p2.cury && prevx > p2.curx)
    {
      for (int i = p2.curx; i <= prevx; i++)
      {
        for (int j = prevy;  j <= p2.cury; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER2_ID;
          }
        }
      }
    }
    if (prevy <= p2.cury && prevx <= p2.curx)
    {
      for (int i = prevx; i <= p2.curx; i++)
      {
        for (int j = prevy;  j <= p2.cury; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER2_ID;
          }
        }
      }
    }
  }
//======================
  if (turn == PLAYER1_ID)//выиграл ли первый игрок сделав свой ход
  {
    int prevx = p1.curx - p1.shiftx;
    int prevy = p1.cury - p1.cury;

    if (prevx > p1.curx && prevy > p1.cury)
    {
      for (int i = p1.curx; i <= prevx; i++)
      {
        for (int j = p1.cury; j <= prevy; j++)
        {
          if (Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER1_ID;
          }
        }
      }
    }
    if (prevx <= p1.curx && prevy > p1.cury)
    {
      for (int i = prevx; i <= p1.curx; i++)
      {
        for (int j = p1.cury;  j <= prevy; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER1_ID;
          }
        }
      }
    }
    if (prevy <= p1.cury && prevx > p1.curx)
    {
      for (int i = p1.curx; i <= prevx; i++)
      {
        for (int j = prevy;  j <= p1.cury; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER1_ID;
          }
        }
      }
    }
    if (prevy <= p1.cury && prevx <= p1.curx)
    {
      for (int i = prevx; i <= p1.curx; i++)
      {
        for (int j = prevy;  j <= p1.cury; j++)
        {
          if(Map[j][i] == FINISH)
          {
            //cout << "oops\n";
            return PLAYER1_ID;
          }
        }
      }
    }
  }

  return 0; // пока не выиграл никто, продолжаем игру

};

// =========================================================================================================================

void Pole::Paint(wxPaintEvent & event)
{
  // cout << "paint event" << endl;

  wxPaintDC dc(this);
  
  /*
  int w, h;
  dc.GetSize(&w, &h);
cells_width = w / screen_width;
cells_height = h / screen_height;
*/
  
	wxColour gray, white, red, blue, orange, green, brown;
	gray.Set(wxT("#d4d4d4"));
	white.Set(wxT("#ffffff"));
	red.Set(wxT("#ff0000"));
	orange.Set(wxT("#fa8e00"));
	green.Set(wxT("#619e1b"));
	brown.Set(wxT("#715b33"));
	blue.Set(wxT("#0d0060"));
	dc.SetPen(wxPen(gray));
  for (int i = 0; i <= screen_height+10; i++)
  {
  	for (int j = 0; j <= screen_width+10; j++)
  	{
    		dc.DrawRectangle(cells_width*j,cells_height*i, cells_width, cells_height);
  	}
	}
	for (int j = 0; j <= screen_height+10; j++)
  	{
    	for (int i = 0; i <= screen_width+10; i++)
    	{
      		if (this->Map[j][i] == GRASS){
      			dc.SetBrush(wxBrush(green));
      			dc.DrawRectangle(cells_width*i, cells_height*j, cells_width, cells_height);
      		}

      		/*if (this->Map[j][i] == PLAYER1_ID){
      			dc.SetBrush(wxBrush(red));
      			dc.DrawRectangle(cells_width*i, cells_height*j, cells_width, cells_height);
      		}

      		if (this->Map[j][i] == PLAYER2_ID){
      			dc.SetBrush(wxBrush(blue));
      			dc.DrawRectangle(cells_width*i, cells_height*j, cells_width, cells_height);
      		}*/

      		if (this->Map[j][i] == FINISH){
      			dc.SetBrush(wxBrush(orange));
      			dc.DrawRectangle(cells_width*i, cells_height*j, cells_width, cells_height);
      		}

      		if (this->Map[j][i] == START){
      			dc.SetBrush(wxBrush(brown));
      			dc.DrawRectangle(cells_width*i, cells_height*j, cells_width, cells_height);
      		}
    	}
  	}
    dc.SetBrush(wxBrush(red));
    dc.DrawRectangle(cells_width*p1.curx, cells_height*p1.cury, cells_width, cells_height);
    dc.SetBrush(wxBrush(blue));
    dc.DrawRectangle(cells_width*p2.curx, cells_height*p2.cury, cells_width, cells_height);
  	render(dc); //новая часть для стрелок

}



void Pole::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}


void Pole::render(wxDC&  dc)
{

  double x1, x2, y1, y2, x3 , x4, y3, y4;
  //cout << "turn of " << turn << endl;

  if (turn == PLAYER1_ID)
  {
    //cout << "current position " << p1.curx << " " << p1.curx << endl;
    switch (p1.key_pressed)
    {
        case 1:
        // cout << p1.shiftx << " " << p1.shifty << "up" << endl;
           x2 = (p1.curx + p1.shiftx+ 0.5)*cells_width;
           y2 = (p1.cury + p1.shifty - 0.5)*cells_height;
            break;
        case 2:
        // cout << p1.shiftx << " " << p1.shifty  << "left" << endl;
          x2 = (p1.curx + p1.shiftx - 0.5)*cells_width;
           y2 = (p1.cury + p1.shifty + 0.5)*cells_height;
            break;
        case 3:
        // cout << p1.shiftx << " " << p1.shifty  << "down" << endl;
            x2 = (p1.curx + p1.shiftx + 0.5)*cells_width;
           y2 = (p1.cury + p1.shifty + 1.5)*cells_height;
            break;
        case 4:
        // cout << p1.shiftx << " " << p1.shifty  << "right" << endl;
            x2 = (p1.curx + p1.shiftx+ 1.5)*cells_width;
           y2 = (p1.cury + p1.shifty + 0.5)*cells_height;
            break;
    }
    x1 = (p1.curx + 0.5)*cells_width;
    y1 = (p1.cury + 0.5)*cells_height;
  }
  else
  {
    x1 = (p2.curx + 0.5)*cells_width;
    y1 = (p2.cury + 0.5)*cells_height;
    // cout << "current position " << p1.curx << " " << p1.curx << endl;
   switch (p2.key_pressed)
    {
        case 1:
         // cout << p2.shiftx << " " << p2.shifty  << "up" << endl;
           x2 = (p2.curx + p2.shiftx+ 0.5)*cells_width;
           y2 = (p2.cury + p2.shifty - 0.5)*cells_height;
            break;
        case 2:
        // cout << p2.shiftx << " " << p2.shifty  << "left" << endl;
          x2 = (p2.curx + p2.shiftx - 0.5)*cells_width;
           y2 = (p2.cury + p2.shifty + 0.5)*cells_height;
            break;
        case 3:
        // cout << p2.shiftx << " " << p2.shifty  << "down" << endl;
            x2 = (p2.curx + p2.shiftx + 0.5)*cells_width;
           y2 = (p2.cury + p2.shifty + 1.5)*cells_height;
            break;
        case 4:
        // cout << p2.shiftx << " " << p2.shifty  << "right" << endl;
            x2 = (p2.curx + p2.shiftx+ 1.5)*cells_width;
           y2 = (p2.cury + p2.shifty + 0.5)*cells_height;
            break;
      }
      x1 = (p2.curx + 0.5)*cells_width;
      y1 = (p2.cury + 0.5)*cells_height;
    }

    if (x2 == x1)
    {
        x3 = x2 + 20;
        x4 = x2 - 20;
        if (y2 < y1) y3 = y4 = y2 + 20;
        else y3 = y4 = y2 - 20;
    }
    if (y2 == y1)
    {
        y3 = y2 + 20;
        y4 = y2 - 20;
        if (x2 < x1) x3 = x4 = x2 + 20;
        else x3 = x4 = x2 - 20;
    }
    if (x2 == x1 && y2 == y1)
    {
        x3 = x2;
        x4 = x2;
        y3 = y2;
        y4 = y2;
    }
    if (x2 != x1 && y2 != y1)
    {
        double k = (y2 - y1) / (x2 - x1);
        double x_tr = x2 - 20 / sqrt(1 + k * k);
        if (x2 < x1) x_tr += 40 / sqrt(1 + k * k);

        double y_tr;
        y_tr = y2 + k * (x_tr - x2);
        x3 = x_tr - 20 / sqrt(1 + 1/k/k);
        x4 = x_tr + 20 / sqrt(1 + 1/k/k);

        y3 = y_tr - 1 / k * (x3 - x_tr);
        y4 = y_tr - 1 / k * (x4 - x_tr);
    }
    wxPoint polygon[] = {wxPoint(x2, y2), wxPoint(x3, y3), wxPoint(x4, y4)};
    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) );
    dc.DrawLine(x1, y1, x2, y2);

    dc.SetBrush(*wxBLUE_BRUSH);
    dc.SetPen(*wxBLUE_PEN);
    dc.DrawPolygon(3, polygon);
}



void Pole::keyPressed(wxKeyEvent& event)
{
  wxPaintDC dc(this);
  // 

  // cout << "draw turn of" << turn << endl;
   if (turn == PLAYER1_ID)
   {
    switch (event.GetKeyCode())
    {
        case WXK_LEFT:
            p1.key_pressed = 2;
            Refresh();
            render(dc);
            break;
        case WXK_RIGHT:
            p1.key_pressed = 4;
            Refresh();
            render(dc);
            break;
        case WXK_UP:
            p1.key_pressed = 1;
            //p1.shifty-= 1;
            Refresh();
            render(dc);
            break;
        case WXK_DOWN:
            p1.key_pressed = 3;
            Refresh();
            render(dc);
            break;
        case WXK_RETURN:
        switch (p1.key_pressed)
        {
        case 1:
            p1.shifty -= 1;
            break;
        case 2:
            p1.shiftx -= 1;
            break;
        case 3:
            p1.shifty += 1;
            break;
        case 4:
            p1.shiftx += 1;
            break;
        }
        this->move();
        Refresh();
        break;
        default:
            event.Skip();
      }
  }
  else
  {
     switch (event.GetKeyCode())
     {
        case WXK_LEFT:
            p2.key_pressed = 2;
            Refresh();
            render(dc);
            break;
        case WXK_RIGHT:
            p2.key_pressed = 4;
            Refresh();
            render(dc);
            break;
        case WXK_UP:
            p2.key_pressed = 1;
            Refresh();
            render(dc);
            break;
        case WXK_DOWN:
            p2.key_pressed = 3;
            Refresh();
            render(dc);
            break;
        case WXK_RETURN:
        switch (p2.key_pressed)
        {
        case 1:
            p2.shifty -= 1;
            break;
        case 2:
            p2.shiftx -= 1;
            break;
        case 3:
            p2.shifty += 1;
            break;
        case 4:
            p2.shiftx += 1;
            break;
        }
        this->move();
        Refresh();
        break;
        default:
            event.Skip();
      }
    }


}


// ==========================================================================================================================



IMPLEMENT_APP(MyApp)
