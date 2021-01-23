/*******************************************
    The Program is design and coded by Eng :


    1-Tarek AbdAlmajed Idrees

    2-Mohamad Mahmoud Ali Mahmoud

    2-Mohamed AbdAlmajed Idrees


    Copyright 2021,All rights reserved.
/******************************************/
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool random = false;

string data = "";

int counter = 0;

int menu=0;

int counter_click_rectangle=0;

struct rect_point
{
    int x;
    int y;
    rect_point()
    {
        x=0;
        y=0;
    }
};

static rect_point left_bottom;
static rect_point right_top;
static rect_point right_bottom;
static rect_point left_top;

//Header of All Function//

//Line Window point //
void DrawLine_window(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color);
bool Clipping(HDC hdc,double x,double y,rect_point left_bottom,rect_point left_top,rect_point right_top,rect_point right_bottom,COLORREF color);


//Circle clip point//
void DrawCircle_Clipping(HDC hdc, int xc,int yc, int R, COLORREF color);


//Clipping Line by line //
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom);
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi);
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi);
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom,COLORREF c);

//Round pixel//
int ROUND(double x);

//Draw Point//
void Draw_8_point(HDC hdc,int xc,int yc,int x,int y,COLORREF c);
void Draw_4_Points(HDC hdc, int x, int y, int xc,int yc, COLORREF c);
void Draw_8_point_Filling(HDC hdc,int xc,int yc,int x,int y,int quarter,COLORREF color);

//Draw Line//
void DrawLine_DDA(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color);
void DrawLine_MidPoint(HDC hdc, int x1,int y1,int x2,int y2,COLORREF c);
void DrawLine_Parametric(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color);

//Draw Circle//
void DrawCircle_Direct(HDC hdc, int xc,int yc, int R, COLORREF color);
void DrawCircle_polar(HDC hdc, int xc,int yc, int R, COLORREF color);
void DrawCircle_MidPoint(HDC hdc,int xc,int yc,int R,COLORREF color);
void DrawCircle_Filling(HDC hdc,int xc,int yc,int R,int quarter,COLORREF color);

//Draw Ellipse//
void DrawEllipse_Direct(HDC hdc,int xc,int yc,int R,int r2,COLORREF c);
void DrawEllipse_Direct(HDC hdc,int xc,int yc,int R,int r2,COLORREF c);
void DrawEllipse_Polar(HDC hdc,int xc,int yc,int r,int r2,COLORREF c);


//Save Points//
struct Save_Point
{
    string Function_Name;
    int x1,x2;
    int y1,y2;
    int R,r2;
    int Rc,Gc,Bc;
    int quarter;
    rect_point left_bottom;
    rect_point right_top;
    rect_point right_bottom;
    rect_point left_top;
    int xleft;
    int ytop;
    int xright;
    int ybottom;

    Save_Point()
    {
        this->Function_Name="";
        this->x1=0;
        this->x2=0;
        this->y1=0;
        this->y2=0;
        this->R=0;
        this->r2=0;
        this->Rc=255;
        this->Gc=255;
        this->Bc=255;

    }

    Save_Point(string Function_Name,int x1,int y1,int R,int r2,int Rc,int Gc,int Bc,char e) //Ellipse
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->r2=r2;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
    Save_Point(string Function_Name,int x1,int y1,int R,int Rc,int Gc,int Bc) //Circle
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
    Save_Point(string Function_Name,int x1,int y1,int R,int quarter,int Rc,int Gc,int Bc,string f) //Filling
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
        this->quarter=quarter;
    }
    Save_Point(string Function_Name,int x1,int y1,int x2,int y2,int Rc,int Gc,int Bc)  //Line
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->x2=x2;
        this->y1=y1;
        this->y2=y2;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
    Save_Point(string Function_Name,rect_point left_bottom,rect_point right_bottom,rect_point left_top,rect_point right_top,int Rc,int Gc,int Bc)//Rectangle
    {
        this->Function_Name=Function_Name;
        this->left_bottom=left_bottom;
        this->right_top=right_top;
        this->right_bottom=right_bottom;
        this->left_top=left_top;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;

    }
    Save_Point(string Function_Name,int x1,int y1,int x2,int y2,int xleft,int ytop,int xright,int ybottom,int Rc,int Gc,int Bc)//clipping line
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->x2=x2;
        this->y1=y1;
        this->y2=y2;
        this->xleft=xleft;
        this->xright=xright;
        this->ytop=ytop;
        this->ybottom=ybottom;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
};
const vector<string> Split(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;
    for(auto n:s)
    {
        if(n != c) buff+=n;
        else if(n == c && buff != "")
        {
            v.push_back(buff);
            buff = "";
        }
    }
    if(buff != "") v.push_back(buff);

    return v;
}
vector<Save_Point>Arr_Save_Point;
void Save()
{
    data = "";

    for (auto it=Arr_Save_Point.begin(); it!=Arr_Save_Point.end(); ++it)
    {
        /* Save Function of All Line */
        if(it->Function_Name=="DDLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DPLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DMLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /* save Function of ALL Circle */

        if(it->Function_Name=="DDCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if (it->Function_Name=="DPCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if (it->Function_Name=="DMCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /*save Function of All Filling */

        if(it->Function_Name=="DPCircleFilling")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->quarter)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /* save Function of ALL Ellipse */
        if(it->Function_Name=="DDEllipse")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->r2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DPEllipse")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->r2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }

        /* Save Rectangle Drawing */
        if(it->Function_Name=="RectDraw")
        {
            data+=it->Function_Name+","+to_string(it->left_bottom.x)+","+to_string(it->left_bottom.y)+","+to_string(it->right_bottom.x)+","+to_string(it->right_bottom.y)+","+to_string(it->left_top.x)+","+to_string(it->left_top.y)+","+to_string(it->right_top.x)+","+to_string(it->right_top.y)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";


        }
        /* Save line clipped on Rectangle Window by point*/

        if(it->Function_Name=="DrawLine_window")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        /* Save Circle clipped on Rectangle Window by point */

        if(it->Function_Name=="DrawCircle_Clipping")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }

        /* save line clipped on rectangle using line*/
        if(it->Function_Name=="CohenSuth")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->xleft)+","+to_string(it->ytop)+","+to_string(it->xright)+","+to_string(it->ybottom)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }

    }
    ofstream myfile;
    myfile.open ("loadFile.txt");
    myfile << data;
    myfile.close();

}

//Load Points//
void Load(HDC hdc)
{
    string Line;
    ifstream LoadFile;
    LoadFile.open("loadFile.txt");
    if (!LoadFile)
    {
        cout << "Unable to open file";
        return;
    }
    while(getline(LoadFile,Line))
    {
        vector<string> Fun_Load;
        Fun_Load=Split(Line,',');

        /* Load Function of All Line */
        if(Fun_Load[0]=="DDLine")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawLine_DDA(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);


        }
        else if(Fun_Load[0]=="DPLine")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawLine_Parametric(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);
        }
        else if(Fun_Load[0]=="DMLine")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawLine_MidPoint(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);
        }


        /* Load Function of ALL Circle */

        if(Fun_Load[0]=="DDCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_Direct(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
        else if (Fun_Load[0]=="DPCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_polar(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
        else if (Fun_Load[0]=="DMCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_MidPoint(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }


        /*Load Function of All Filling */
        if(Fun_Load[0]=="DPCircleFilling")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawCircle_Filling(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);

            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),"e");
            Arr_Save_Point.push_back(x);

        }


        /* Load Function of ALL Ellipse */

        if(Fun_Load[0]=="DDEllipse")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawEllipse_Direct(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),'e');
            Arr_Save_Point.push_back(x);
        }
        else if(Fun_Load[0]=="DPEllipse")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawEllipse_Polar(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),'e');
            Arr_Save_Point.push_back(x);
        }

        /* Load Function of Draw Rectangle */

        if(Fun_Load[0]=="RectDraw")
        {
            COLORREF c =RGB(stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            DrawLine_DDA(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),c);
            DrawLine_DDA(hdc,stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),stoi(Fun_Load[8]),c);
            DrawLine_DDA(hdc,stoi(Fun_Load[7]),stoi(Fun_Load[8]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            DrawLine_DDA(hdc,stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[1]),stoi(Fun_Load[2]),c);
            /* Save points in array after load */
            left_bottom.x=stoi(Fun_Load[1]);
            left_bottom.y=stoi(Fun_Load[2]);
            left_top.x=stoi(Fun_Load[5]);
            left_top.y=stoi(Fun_Load[6]);
            right_bottom.x=stoi(Fun_Load[3]);
            right_bottom.y=stoi(Fun_Load[4]);
            right_top.x=stoi(Fun_Load[7]);
            right_top.y=stoi(Fun_Load[8]);

            Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            Arr_Save_Point.push_back(x);

        }
        if(Fun_Load[0]=="DrawLine_window")
        {

            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawLine_window(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);

            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);
        }
        if(Fun_Load[0]=="DrawCircle_Clipping")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_Clipping(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
         if(Fun_Load[0]=="CohenSuth")
        {

            COLORREF c =RGB(stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            CohenSuth(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),stoi(Fun_Load[8]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),stoi(Fun_Load[8]),stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            Arr_Save_Point.push_back(x);
        }
    }
}

//Clean Points//
void Clean(HDC hdc,vector<Save_Point> &Arr_Save_Point)
{

    COLORREF  c=RGB(255,255,255);

    for (auto it=Arr_Save_Point.begin(); it!=Arr_Save_Point.end(); ++it)
    {
        // Clean Function of All Line //

        if(it->Function_Name=="DDLine")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawLine_DDA(hdc,it->x1,it->y1,it->x2,it->y2,c);
        }
        else if(it->Function_Name=="DPLine")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawLine_Parametric(hdc,it->x1,it->y1,it->x2,it->y2,c);

        }
        else if(it->Function_Name=="DMLine")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawLine_MidPoint(hdc,it->x1,it->y1,it->x2,it->y2,c);

        }


        // Clean Function of ALL Circle //

        if(it->Function_Name=="DDCircle")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawCircle_Direct(hdc,it->x1,it->y1,it->R,c);
        }
        else if (it->Function_Name=="DPCircle")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawCircle_polar(hdc,it->x1,it->y1,it->R,c);
        }
        else if (it->Function_Name=="DMCircle")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawCircle_MidPoint(hdc,it->x1,it->y1,it->R,c);
        }



        // Clean Function of ALL Circle Filling //
        if(it->Function_Name=="DPCircleFilling")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawCircle_Filling(hdc,it->x1,it->y1,it->R,it->quarter,c);

        }



        // Clean Function of ALL Ellipse //

        if(it->Function_Name=="DDEllipse")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawEllipse_Direct(hdc,it->x1,it->y1,it->R,it->r2,c);
        }
        else if(it->Function_Name=="DPEllipse")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawEllipse_Polar(hdc,it->x1,it->y1,it->R,it->r2,c);
        }

        // Clean Function of Drawing Rectangle
        if(it->Function_Name=="RectDraw")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawLine_DDA(hdc,it->left_bottom.x,it->left_bottom.y,it->left_top.x,it->left_top.y,c);
            DrawLine_DDA(hdc,it->left_top.x,it->left_top.y,it->right_top.x,it->right_top.y,c);
            DrawLine_DDA(hdc,it->right_top.x,it->right_top.y,it->right_bottom.x,it->right_bottom.y,c);
            DrawLine_DDA(hdc,it->right_bottom.x,it->right_bottom.y,it->left_bottom.x,it->left_bottom.y,c);
        }

        // Clean Function of Line on Rectangle of point

        if(it->Function_Name=="DrawLine_window")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawLine_window(hdc,it->x1,it->y1,it->x2,it->y2,c);
        }
        // Clean Function of Circle on Rectangle of point

        if(it->Function_Name=="DrawCircle_Clipping")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            DrawCircle_Clipping(hdc,it->x1,it->y1,it->R,c);
        }

        // Clean Function of Line on Rectangle of Line
        if(it->Function_Name=="CohenSuth")
        {
            it->Rc=255;
            it->Gc=255;
            it->Bc=255;
            CohenSuth(hdc,it->x1,it->y1,it->x2,it->y2,it->xleft,it->ytop,it->xright,it->ybottom,c);
        }
    }
    Arr_Save_Point.clear();
}
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Graphics Project_Bio_Coder");
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{

    HWND hwnd; /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) RGB(255,255,0);
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Graphics Project_Bio_Coder"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
//*******************************************************************//
void addMenu(HWND hwnd);
HMENU hMenu;
//*******************************************************************//
int Rc,Gc,Bc;
COLORREF c = 0;
COLORREF cF = 0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HDC hdc = GetDC(hwnd);
    static int x1,x2,x3;
    static int y1,y2,y3;
    int r,r2;
    int R;
    static int   quarter;
    if(random)
    {
        Rc = (rand()* 10000) % 255;
        Gc = (rand()* 10000) % 180;
        Bc = (rand()* 10000) % 255;
        c = RGB(Rc,Gc,Bc);
    }
    switch (message)                  /* handle the messages */
    {

    case WM_CREATE:
        addMenu(hwnd);
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case(0):
            cout<< "Saving Process" << std::endl;
            Save();
            cout<< "Save Done !" << std::endl;
            break;
        case(1):
            cout<< "loading Process" << std::endl;
            Load(hdc);
            cout<<"load Done !"<<endl;
            break;
        case(2):
            menu = 2;
            cout << "Draw Line by DDA" << endl;
            break;
        case(3):
            menu = 3;
            cout << "Draw Line by Mid Point" << endl;
            break;
        case(4):
            menu = 4;
            cout << "Draw Line by Parametric" << endl;
            break;
        case(5):
            menu = 5;
            cout << "Draw ellipse by Direct" << endl;
            break;
        case(6):
            menu = 6;
            cout << "Draw ellipse by Polar" << endl;
            break;
        case(7):
            menu = 7;
            cout << "Draw circle Direct " << endl;
            break;
        case(8):
            menu = 8;
            cout << "Draw circle polar " << endl;
            break;
        case(9):
            menu = 9;
            cout << "Draw circle midpoint " << endl;
            break;
        case(10):
            cout << "Draw by the Black color" << endl;
            random = false;
            c = 0;
            break;
        case(11):
            cout << "Draw by the Red color" << endl;
            random = false;
            c = RGB(255,0,0);
            break;
        case(12):
            cout << "Draw by the Blue color" << endl;
            random = false;
            c = RGB(0,0,255);
            break;
        case(13):
            cout << "Random Color :" << endl;
            random = true;
            break;
        case(14):
            menu = 14;
            cout << "Fill circle by the Black color" << endl;
            cF = 0;
            break;
        case(15):
            menu = 15;
            cout << "fill circle by the Red color" << endl;
            cF = RGB(255,0,0);
            break;
        case(16):
            menu = 16;
            cout << "Fill circle by the Blue color" << endl;
            cF = RGB(0,0,255);
            break;
        case(17):
            menu = 17;
            cout << "Fill circle by Random color" << endl;
            random=true;
            break;
        case(18):
            menu = 18;
            cout <<"Clean the window"<< endl;
            Clean(hdc,Arr_Save_Point);
            break;
        case(19):
            menu = 19;
            cout << "Fill circle by the Green color" << endl;
            cF = RGB(0,255,0);
            break;
        case(20):
            menu=20;
            cout << "Draw by the Green color" << endl;
            random = false;
            c = RGB(0,255,0);
            break;
        case(21):
            menu=21;
            cout<<"Filling Circle with First Quarter :"<<endl;
            quarter=1;
            break;
        case(22):
            menu=22;
            cout<<"Filling Circle with Secound Quarter :"<<endl;
            quarter=2;
            break;
        case(23):
            menu=23;
            cout<<"Filling Circle with Third Quarter :"<<endl;
            quarter=3;
            break;
        case(24):
            menu=24;
            cout<<"Filling Circle with Fourth Quarter :"<<endl;
            quarter=4;
            break;
        case(25):
            menu=25;
            cout<<"Draw Rectangle with four point  :"<<endl;
            break;
        case(26):
            menu=26;
            cout<<"Draw Line after clipping   :"<<endl;
            break;
        case(27):
            menu=27;
            cout<<"Draw Circle after clipping   :"<<endl;
            break;
        case(28):
            menu=28;
            cout<<"Draw Circle Clipping by Line :"<<endl;
            break;
        }
        break;

    case WM_LBUTTONDOWN:
        if(menu == 7 ||menu ==8 || menu==9 ) // Circle(Directr,Polar,Midpoint)
        {
            x1 = LOWORD(lParam);
            y1 = HIWORD(lParam);
            break;
        }
        if(menu== 2 || menu==3 || menu==4) //  Line(Directr,Polar,Midpoint)
        {
            x1 = LOWORD(lParam);
            y1 = HIWORD(lParam);
            break;
        }
        if(menu==5 || menu==6)   // Ellipse(Directr,Polar)
        {
            x1 = LOWORD(lParam);
            y1 = HIWORD(lParam);
            break;
        }
        if(menu==14 || menu==15 || menu==16 || menu==17 || menu==19)
        {

            x1 = LOWORD(lParam);
            y1 = HIWORD(lParam);
            break;
        }
        if(menu==25)
        {
            if(counter_click_rectangle==0)
            {
                left_bottom.x=LOWORD(lParam);
                left_bottom.y=HIWORD(lParam);

                counter_click_rectangle++;

                left_top.x=left_bottom.x;
                left_top.y= left_bottom.y - 200;

                right_bottom.y=left_bottom.y;
                right_top.y=left_top.y;

                right_top.x=left_top.x + 400;
                right_bottom.x=right_top.x;
                if(c==RGB(255,0,0))
                {
                    Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {
                    Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("RectDraw",left_bottom,right_bottom,left_top,right_top,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }


                break;
            }
        }
        if(menu==26)
        {
            x1=LOWORD(lParam);
            y1=HIWORD(lParam);
        }
        if(menu==27)
        {
            x1=LOWORD(lParam);
            y1=HIWORD(lParam);
        }
        if(menu==28)
        {
            x1=LOWORD(lParam);
            y1=HIWORD(lParam);
        }
        break;
    case WM_LBUTTONUP:
        if(menu==7 || menu==8 || menu==9) //Circle(Direct,Polar,Midpoint)
        {
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            R=ROUND(std::sqrt(std::pow(x2-x1,2.0)+pow(y2-y1,2.0)));
            if(menu == 7) //Circle(Direct)
            {
                cout<< "Draw Direct Circle"<< endl;
                DrawCircle_Direct(hdc,x1,y1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DDCircle",x1,y1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DDCircle",x1,y1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DDCircle",x1,y1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DDCircle",x1,y1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DDCircle",x1,y1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw Direct Circle Done !"<<endl;
            }
            else if(menu == 8)//Circle(Polar)
            {
                cout<< "Draw Polar Circle"<< endl;
                DrawCircle_polar(hdc,x1,y1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DPCircle",x1,y1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DPCircle",x1,y1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DPCircle",x1,y1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DPCircle",x1,y1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DPCircle",x1,y1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw Polar Circle Done !"<<endl;
            }
            else if(menu == 9)//Circle(MidPooint)
            {
                cout<< "Draw MidPoint Circle "<< endl;
                DrawCircle_MidPoint(hdc,x1,y1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DMCircle",x1,y1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DMCircle",x1,y1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DMCircle",x1,y1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {
                    Save_Point x("DMCircle",x1,y1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DMCircle",x1,y1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw MidPoint Circle Done !"<<endl;
            }
            break;
        }
        else if (menu==2 || menu==3 || menu==4)  //Line(Directr,Polar,Midpoint)
        {
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            if(menu == 2)//Line(Direct)
            {
                cout<< " Draw DDA line "<< endl;
                DrawLine_DDA(hdc,x1,y1,x2,y2,c);
                if(c==RGB(255,0,0))
                {

                    Save_Point x("DDLine",x1,y1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DDLine",x1,y1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DDLine",x1,y1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DDLine",x1,y1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DDLine",x1,y1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<< " Draw DDA line Done !"<< endl;
            }
            else if(menu == 3)////Line(Polar)
            {
                cout<< " Draw MidPoint Line "<< endl;
                DrawLine_MidPoint(hdc,x1,y1,x2,y2,c);
                if(c==RGB(255,0,0))
                {

                    Save_Point x("DMLine",x1,y1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DMLine",x1,y1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DMLine",x1,y1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {
                    Save_Point x("DMLine",x1,y1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DMLine",x1,y1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<< "Draw mid point line Done !"<< endl;
            }
            else if(menu == 4)////Line(MidPoint)
            {
                cout<< " Draw parametric Line"<< endl;
                DrawLine_Parametric(hdc,x1,y1,x2,y2,c);
                if(c==RGB(255,0,0))
                {

                    Save_Point x("DPLine",x1,y1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DPLine",x1,y1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DPLine",x1,y1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DPLine",x1,y1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DPLine",x1,y1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<< "Draw parametric line Done !"<< endl;
            }
            break;
        }
        else if (menu==14 || menu==15 || menu==16 || menu==17 ||menu== 19) //Filling Circle
        {
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            R=ROUND(std::sqrt(std::pow(x2-x1,2.0)+pow(y2-y1,2.0)));
            if(menu==14)//Filling Circle(Black color)
            {

                cout<< "Filling  Circle with black Color"<< endl;
                DrawCircle_Filling(hdc,x1,y1,R,quarter,RGB(0,0,0));
                Save_Point x("DPCircleFilling",x1,y1,R,quarter,0,0,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with black Color Done"<< endl;


            }
            else if (menu==15)//Filling Circle(Red color)
            {
                cout<< "Filling  Circle with Red Color"<< endl;
                DrawCircle_Filling(hdc,x1,y1,R,quarter,RGB(255,0,0));
                Save_Point x("DPCircleFilling",x1,y1,R,quarter,255,0,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Red Color Done"<< endl;
            }
            else if (menu== 16)//Filling Circle(Blue color)
            {
                cout<< "Filling  Circle with Blue Color"<< endl;
                DrawCircle_Filling(hdc,x1,y1,R,quarter,RGB(0,0,255));
                Save_Point x("DPCircleFilling",x1,y1,R,quarter,0,0,255,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Blue Color Done"<< endl;
            }
            else if (menu== 17) //Filling Circle(Random color)
            {
                cout<< "Filling  Circle with Random Color"<< endl;
                DrawCircle_Filling(hdc,x1,y1,R,quarter,c);
                Save_Point x("DPCircleFilling",x1,y1,R,quarter,Rc,Gc,Bc,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Random Color Done"<< endl;
            }
            else if (menu== 19)//Filling Circle(Green color)
            {
                cout<< "Filling  Circle with Green Color"<< endl;
                DrawCircle_Filling(hdc,x1,y1,R,quarter,RGB(0,255,0));
                Save_Point x("DPCircleFilling",x1,y1,R,quarter,0,255,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Green Color Done"<< endl;
            }

        }
        else if (menu==25)
        {
            if(counter_click_rectangle==1)
            {
                DrawLine_DDA(hdc,left_bottom.x,left_bottom.y,left_top.x,left_top.y,c);
                DrawLine_DDA(hdc,left_top.x,left_top.y,right_top.x,right_top.y,c);
                DrawLine_DDA(hdc,right_top.x,right_top.y,right_bottom.x,right_bottom.y,c);
                DrawLine_DDA(hdc,right_bottom.x,right_bottom.y,left_bottom.x,left_bottom.y,c);
                counter_click_rectangle++;
                cout<<"Draw Window Done !"<<endl;
                break;
            }
        }
        else if (menu==26)
        {
            x2=LOWORD(lParam);
            y2=HIWORD(lParam);

            DrawLine_window(hdc,x1,y1,x2,y2,c);
            if(c==RGB(255,0,0))
            {

                Save_Point x("DrawLine_window",x1,y1,x2,y2,255,0,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,255,0))
            {
                Save_Point x("DrawLine_window",x1,y1,x2,y2,0,255,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,255))
            {

                Save_Point x("DrawLine_window",x1,y1,x2,y2,0,0,255);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,0))
            {

                Save_Point x("DrawLine_window",x1,y1,x2,y2,0,0,0);
                Arr_Save_Point.push_back(x);
            }
            else
            {

                Save_Point x("DrawLine_window",x1,y1,x2,y2,Rc,Gc,Bc);
                Arr_Save_Point.push_back(x);
            }

        }
        else if(menu==27)
        {

            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            R=ROUND(std::sqrt(std::pow(x2-x1,2.0)+pow(y2-y1,2.0)));
            DrawCircle_Clipping(hdc,x1,y1,R,c);
            if(c==RGB(255,0,0))
            {
                Save_Point x("DrawCircle_Clipping",x1,y1,R,255,0,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,255,0))
            {

                Save_Point x("DrawCircle_Clipping",x1,y1,R,0,255,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,255))
            {

                Save_Point x("DrawCircle_Clipping",x1,y1,R,0,0,255);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,0))
            {

                Save_Point x("DrawCircle_Clipping",x1,y1,R,0,0,0);
                Arr_Save_Point.push_back(x);
            }
            else
            {

                Save_Point x("DrawCircle_Clipping",x1,y1,R,Rc,Gc,Bc);
                Arr_Save_Point.push_back(x);
            }

        }
        else if(menu==28)
        {
            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            CohenSuth(hdc,x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,c);
            if(c==RGB(255,0,0))
            {
                CohenSuth(hdc,x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,c);
                Save_Point x("CohenSuth",x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,255,0,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,255,0))
            {

                Save_Point x("CohenSuth",x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,0,255,0);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,255))
            {

                Save_Point x("CohenSuth",x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,0,0,255);
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,0))
            {

                Save_Point x("CohenSuth",x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,0,0,0);
                Arr_Save_Point.push_back(x);
            }
            else
            {

                Save_Point x("CohenSuth",x1,y1,x2,y2,left_bottom.x,left_top.y,right_bottom.x,right_bottom.y,Rc,Gc,Bc);
                Arr_Save_Point.push_back(x);
            }
        }
        break;
    case WM_RBUTTONDOWN:
        if(menu == 5 ||menu == 6)
        {
            x3 = LOWORD(lParam);
            y3 = HIWORD(lParam);
        }
        break;
    case WM_RBUTTONUP:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        r = ROUND(sqrt(pow(x2-x1,2.0)+pow(y2-y1,2.0)));
        r2 = ROUND(sqrt(pow(x3-x1,2.0)+pow(y3-y1,2.0)));
        if(menu == 5)//Direct Ellipse
        {
            cout<< "Draw Direct Ellipse "<< endl;
            DrawEllipse_Direct(hdc,x1,y1,r,r2,c);
            if(c==RGB(255,0,0))
            {

                Save_Point x("DDEllipse",x1,y1,r,r2,255,0,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,255,0))
            {

                Save_Point x("DDEllipse",x1,y1,r,r2,0,255,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,255))
            {

                Save_Point x("DDEllipse",x1,y1,r,r2,0,0,255,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,0))
            {

                Save_Point x("DDEllipse",x1,y1,r,r2,0,0,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else
            {

                Save_Point x("DDEllipse",x1,y1,r,r2,Rc,Gc,Bc,'e');
                Arr_Save_Point.push_back(x);
            }

            cout<<"Draw Direct Ellipse Done !"<<endl;
        }
        else if(menu == 6)//Polar Ellipse
        {
            cout<< "Draw Polar Ellipse "<< endl;
            DrawEllipse_Polar( hdc, x1, y1, r, r2, c);
            if(c==RGB(255,0,0))
            {
                Save_Point x("DPEllipse",x1,y1,r,r2,255,0,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,255,0))
            {

                Save_Point x("DPEllipse",x1,y1,r,r2,0,255,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,255))
            {

                Save_Point x("DPEllipse",x1,y1,r,r2,0,0,255,'e');
                Arr_Save_Point.push_back(x);
            }
            else if (c==RGB(0,0,0))
            {

                Save_Point x("DPEllipse",x1,y1,r,r2,0,0,0,'e');
                Arr_Save_Point.push_back(x);
            }
            else
            {

                Save_Point x("DPEllipse",x1,y1,r,r2,Rc,Gc,Bc,'e');
                Arr_Save_Point.push_back(x);
            }
            cout<<"Draw Polar Ellipse Done !"<<endl;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}




//Menu of Window//

void addMenu(HWND hwnd)
{
    hMenu = CreateMenu();

    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu,MF_STRING,0,"Save");
    AppendMenu(hFileMenu,MF_STRING,1,"Load");
    AppendMenu(hFileMenu,MF_STRING,18,"Clean");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"File");

    HMENU hLineMenu = CreateMenu();
    AppendMenu(hLineMenu,MF_STRING,2,"DDA");
    AppendMenu(hLineMenu,MF_STRING,3,"Mid Point");
    AppendMenu(hLineMenu,MF_STRING,4,"Parametric");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLineMenu,"Line Algorithms");

    HMENU hEllipseMenu = CreateMenu();
    AppendMenu(hEllipseMenu,MF_STRING,5,"Direct");
    AppendMenu(hEllipseMenu,MF_STRING,6,"Polar");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEllipseMenu,"Ellipse Algorithms");

    HMENU hCircleMenu = CreateMenu();
    AppendMenu(hCircleMenu,MF_STRING,7,"Direct,");
    AppendMenu(hCircleMenu,MF_STRING,8,"Polari iterative Polar");
    AppendMenu(hCircleMenu,MF_STRING,9,"Midpoint");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircleMenu,"Circle Algorithms");


    HMENU hColorMenu = CreateMenu();
    AppendMenu(hColorMenu,MF_STRING,10,"Black");
    AppendMenu(hColorMenu,MF_STRING,11,"Red");
    AppendMenu(hColorMenu,MF_STRING,12,"Blue");
    AppendMenu(hColorMenu,MF_STRING,20,"Green");
    AppendMenu(hColorMenu,MF_STRING,13,"Random");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hColorMenu,"Colors");

    HMENU hFilling = CreateMenu();
    AppendMenu(hFilling,MF_STRING,14,"Black");
    AppendMenu(hFilling,MF_STRING,15,"Red");
    AppendMenu(hFilling,MF_STRING,16,"Blue");
    AppendMenu(hFilling,MF_STRING,19,"Green");
    AppendMenu(hFilling,MF_STRING,17,"Random");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFilling,"Fill Circle Shapes");


    HMENU hquarter = CreateMenu();
    AppendMenu(hquarter,MF_STRING,21,"First");
    AppendMenu(hquarter,MF_STRING,22,"Secound");
    AppendMenu(hquarter,MF_STRING,23,"Third");
    AppendMenu(hquarter,MF_STRING,24,"Fourth");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hquarter,"Quarter Circle Shapes");

    HMENU hwindow = CreateMenu();
    AppendMenu(hwindow,MF_STRING,25,"Draw Rectangle");
    AppendMenu(hwindow,MF_STRING,26,"Draw Line Clipping by poinyt");
    AppendMenu(hwindow,MF_STRING,27,"Draw Circle Clipping by point");
    AppendMenu(hwindow,MF_STRING,28,"Draw Circle Clipping by Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hwindow,"Clipping");



    SetMenu(hwnd,hMenu);

}


//Functions of drawing all shapes//



int ROUND(double x)
{
    return (int)(x+0.5);
}




void Draw_8_point(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-x,yc-y,c);
    SetPixel(hdc,xc+y,yc-x,c);
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
    SetPixel(hdc,xc+y,yc+x,c);
}



void Draw_4_Points(HDC hdc, int x, int y, int xc,int yc, COLORREF c)
{

    SetPixel(hdc, xc+x, yc+y, c);
    SetPixel(hdc, xc-x, yc+y, c);
    SetPixel(hdc, xc-x, yc-y, c);
    SetPixel(hdc, xc+x, yc-y, c);

}



bool Clipping(HDC hdc,double x,double y,rect_point left_bottom,rect_point left_top,rect_point right_top,rect_point right_bottom,COLORREF color)
{
    if((x>left_bottom.x && x>left_top.x) && (y<left_bottom.y && y>left_top.y) && (x>left_top.x && x<right_top.x) &&(y>left_top.y && y>right_top.y) && (x<right_top.x && x<right_bottom.x) &&(y>right_top.y && y<right_bottom.y)&&(x>left_bottom.x && x<right_bottom.x) && (y<left_bottom.y && y<right_bottom.y))
        return false;
    return true;
}




void DrawLine_window(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color)
{
    double dt=(double)1.0/std::max(abs(x2-x1),abs(y2-y1));
    for(double t = 0; t<=1; t+=dt)
    {
        double x=x1+t*(x2-x1);
        double y=y1+t*(y2-y1);
        if(!Clipping(hdc,x,y,left_bottom,left_top,right_top,right_bottom,color))
        {
            SetPixel(hdc,ROUND(x),ROUND(y),color);
        }
    }


}



void DrawCircle_Clipping(HDC hdc, int xc,int yc, int R, COLORREF color)
{
    double dtheta=1.0/R;
    for( double theta=0; theta< 6.28; theta+=dtheta)
    {
        int x=ROUND(xc+R*std::cos(theta));
        int y=ROUND(yc+R*std::sin(theta));
        if(!Clipping(hdc,x,y,left_bottom,left_top,right_top,right_bottom,color))
        {
            SetPixel(hdc,x,y,color);

        }

    }
}



void DrawLine_DDA(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color)
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(std::abs(dx)>=std::abs(dy))
    {
        double m=(double)dy/dx;
        if(x1>x2)
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        int x=x1;
        double y=y1;
        SetPixel(hdc,x1,y1,color);
        while(x<x2)
        {
            x++;
            y=y+m;
            SetPixel(hdc,x,ROUND(y),color);
        }
    }
    else
    {
        double m=(double)dx/dy;
        if(y1>y2)
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        double x=x1;
        int y=y1;
        SetPixel(hdc,x1,y1,color);
        while(y<y2)
        {
            y++;
            x=x+m;
            SetPixel(hdc,ROUND(x),y,color);
        }
    }


}




void Draw_8_point_Filling(HDC hdc,int xc,int yc,int x,int y,int quarter,COLORREF color)
{
    if(quarter==2)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc-x,color);

    }
    if(quarter==4)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc+x,color);

    }
    if(quarter==3)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc+x,color);

    }
    if(quarter==1)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc-x,color);
    }
}



void DrawLine_MidPoint(HDC hdc, int x1,int y1,int x2,int y2,COLORREF c)
{
    int  dx=x2-x1, dy=y2-y1;
    if(abs(dx) >= abs(dy))
    {
        if(dx < 0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx = -dx;
            dy = -dy;
        }



        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dy>=0)
        {
            int d = dx - 2 * dy;
            int d1 = 2*dx - 2*dy;
            int d2 = -2*dy;
            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y++;
                }
                else
                {
                    d+=d2;
                    x++;
                }
                SetPixel(hdc, x, y, c);

            }
        }
        else
        {
            int d = dx + 2 * dy;
            int d1 = 2*dx + 2*dy;
            int d2 = 2*dy;

            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y--;
                }
                else
                {
                    d+=d2;
                    x++;
                }
                SetPixel(hdc, x, y, c);

            }
        }
    }
    else
    {
        if(dy<0)
        {
            swap(x1,x2);
            swap(y1,y2);

            dx = -dx;
            dy = -dy;
        }

        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dx>=0)
        {
            double d = -2*dx + dy;
            int d1 = -2*dx + 2*dy;
            int d2 = -2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y++;
                }
                else
                {
                    d+=d2;
                    y++;
                }
                SetPixel(hdc, x, y, c);

            }
        }
        else
        {
            double d = 2*dx + dy;
            int d1 = 2*dx + 2*dy;
            int d2 = 2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x--;
                    y++;
                }
                else
                {
                    d+=d2;
                    y++;
                }
                SetPixel(hdc, x, y, c);
            }
        }
    }
}



void DrawLine_Parametric(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dt=(double)1.0/std::max(abs(x2-x1),abs(y2-y1));
    for(double t = 0; t<=1; t+=dt)
    {
        double x=x1+t*(x2-x1);
        double y=y1+t*(y2-y1);
        SetPixel(hdc,ROUND(x),ROUND(y),color);
    }
}



void DrawCircle_Direct(HDC hdc, int xc,int yc, int R, COLORREF color)
{
    double x=0;
    double y=R;
    Draw_8_point(hdc,xc,yc,R,0,color);
    while(x<y)
    {
        x++;
        y=std::sqrt(R*R-x*x);
        Draw_8_point(hdc,xc,yc,x,y,color);
    }
}



void DrawCircle_polar(HDC hdc, int xc,int yc, int R, COLORREF color)
{
    double dtheta=1.0/R;
    double c= std::cos(dtheta);
    double s= std::sin(dtheta);
    double x=R;
    double y=0;
    Draw_8_point(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw_8_point(hdc,xc,yc,ROUND(x),ROUND(y),color);
    }
}



void DrawCircle_MidPoint(HDC hdc,int xc,int yc,int R,COLORREF color)
{
    int x=0;
    int y=R;
    int d=1-R;
    int d1=3;
    int d2=5-2*R;
    Draw_8_point(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=d1;
            d1+=2;
            d2+=2;
            x++;
        }
        else
        {
            d+=d2;
            d2+=4;
            d1+=2;
            x++;
            y--;
        }
        Draw_8_point(hdc,xc,yc,x,y,color);
    }
}



void DrawCircle_Filling(HDC hdc,int xc,int yc,int R,int quarter,COLORREF color)
{
    double dtheta=1.0/R;
    double c=std::cos(dtheta);
    double s=std::sin(dtheta);
    double x=R;
    double y=0;
    Draw_8_point_Filling(hdc,xc,yc,R,0,quarter,color);
    while (x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw_8_point_Filling(hdc,xc,yc,ROUND(x),ROUND(y),quarter,color);
    }
}




void DrawEllipse_Direct(HDC hdc,int xc,int yc,int R,int r2,COLORREF c)
{

    double x=R;
    int y=0;

    Draw_4_Points( hdc,  R,  0,  xc, yc,  c);
    while(x>=0)
    {
        x-=0.01;
        y= abs(r2*sqrt(1-(pow(x,2)/pow(R,2))));
        Draw_4_Points( hdc,  x,  y,  xc, yc,  c);
    }
}



void DrawEllipse_Polar(HDC hdc,int xc,int yc,int r,int r2,COLORREF c)
{
    double dtheta=1.0/r;
    for(double theta=0.0; theta<6.28; theta+=dtheta)
    {
        double x=xc+r2*cos(theta);
        double y=yc+r*sin(theta);
        SetPixel(hdc,ROUND(x),ROUND(y),c);
    }
}



OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;
    else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;
    else if(y>ybottom)out.bottom=1;
    return out;
}




void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{

    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}


void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}


void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom,COLORREF c)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);

    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {



            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {



            if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }

    if(!out1.All && !out2.All)
    {
        DrawLine_DDA(hdc,x1,y1,x2,y2,c);
    }
}
