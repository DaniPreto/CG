using namespace std;
#include <gtk/gtk.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <math.h>
#include "Window.h"
#include "Poligono.h"
#include "Linha.h"
#include "Ponto.h"
#include "Curva.h"
#include "Objeto3D.h"
#include <algorithm>
#include "List.h"
#include "Coordenada.h"
#include "Faceta.h"
#include "DescritorObj.h"
#include "Superficie.h"
static cairo_surface_t *surface = NULL;

//Windows
GtkWidget *window_widget;
GtkWidget *window_desenho;
GtkWidget *window_transformacao;
GtkWidget *window_objeto;
GtkWidget *window_desenho3D;
GtkWidget *window_transformacao3D;
GtkWidget *window_objeto3D;
GtkWidget *window_faceta;
GtkWidget *window_arquivo;
GtkWidget *window_opcao;

//Opções de usuário
GtkWidget *button_opcao;
GtkWidget *button_apply;
GtkWidget *button_passo;
GtkWidget *button_tipo_recorte;
GtkWidget *button_tipo_superficie;
GtkWidget *button_tipo_vizualizacao;
GtkWidget *button_t_linha;


//Display de objetos
GtkWidget *display;
GtkWidget *display_faceta;
GtkListStore *store;
GtkListStore *store_faceta;
GtkWidget *drawing_area;
GtkWidget *button_objetos;
GtkWidget *button_objetos3D;

//Botoes de Desenho
GtkWidget *button_desenhar;
GtkWidget *button_ponto;
GtkWidget *button_linha;
GtkWidget *button_poligono;
GtkWidget *button_poligono1;
GtkWidget *button_x;
GtkWidget *button_y;
GtkWidget *button_xf; 
GtkWidget *button_yf;
GtkWidget *button_clear;
GtkWidget *button_nome;

//Botoes de Transformacao
GtkWidget *button_transformar;
GtkWidget *button_x1;
GtkWidget *button_y1;
GtkWidget *button_translacao;
GtkWidget *button_escalonamento;
GtkWidget *button_rotacao;
GtkWidget *button_rotacaoCObj;
GtkWidget *button_rotacaoPQ;
GtkWidget *button_angulo;

//Botoes de Desenho3D
GtkWidget *button_desenhar1;
GtkWidget *button_npontos1;
GtkWidget *button_objeto3D;
GtkWidget *button_objeto3D1;
GtkWidget *button_superficie3D;
GtkWidget *button_vertice;
GtkWidget *button_faceta;
GtkWidget *button_faceta1;
GtkWidget *button_x2;
GtkWidget *button_y2;
GtkWidget *button_z;
GtkWidget *button_clear1;
GtkWidget *button_nome1;


//Botoes de Transformacao3D
GtkWidget *button_transformar1;
GtkWidget *button_x3;
GtkWidget *button_y3;
GtkWidget *button_z1;
GtkWidget *button_translacao1;
GtkWidget *button_escalonamento1;
GtkWidget *button_rotacao3DEQ;
GtkWidget *button_rotacao3DX;
GtkWidget *button_rotacao3DY;
GtkWidget *button_rotacao3DZ;
GtkWidget *button_angulo1;

//Botoes de desenho curva
GtkWidget *button_npontos;
GtkWidget *button_tipo;
GtkWidget *button_curva;



//Botoes de controle da window
GtkWidget *button_up;
GtkWidget *button_down;
GtkWidget *button_left;
GtkWidget *button_right; 
GtkWidget *button_in;
GtkWidget *button_out;
GtkWidget *button_rotaciona_direita;
GtkWidget *button_rotaciona_esquerda;

//Botoes de leitura de arquivo
GtkWidget *button_nome2;
GtkWidget *button_desenhar2;
GtkWidget *button_leitorOBJ;

Window window;

//Lista de objetos graficos
List lista_objetos;

//Variaveis auxiliares;
//----------------------------------------------------Viewport
double XvpMIN = 0;
double YvpMIN = 0;
double XvpMAX = 300;
double YvpMAX = 300;

//-----------------------------------------------------Desenho
Poligono * poligono_aux;
Curva * curva_aux;
Objeto3D * objeto3D;
Superficie * superficie;
Faceta faceta_aux;
int index;

//----------------------------------------------------Opções do usuário
int modelo3D = 1;
bool t_clipping = false;
bool t_superficie = true;
int passo_window = 1;
double t_linha = 1;


//---------------------------------------------------Matrizes
double m_transform[4][4];
double  v_transform[4];
             

    

//----------------------------------------Controle da surface--------------------------------------------------------------
/*Clear the surface, removing the scribbles*/
static void clear_surface (){
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}

/*Creates the surface*/
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data){
    if (surface)
        cairo_surface_destroy (surface);

    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));
    clear_surface ();
    return TRUE;
}

/* Redraw the screen from the surface */
static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr,  gpointer   data){
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

//Desenha uma Linha
static void draw_linha(double a,double b,double c, double d){

    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, t_linha);
    //cairo_set_line_width (cr, 1);
    //gdk_cairo_set_source_rgba (cr,const GdkRGBA *rgba);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); /* Round dot*/
    cairo_move_to (cr, a, b);
    cairo_line_to (cr, c, d);/* a very short line is a dot */
    cairo_stroke (cr);
    gtk_widget_queue_draw (window_widget);
}

//-----------------------------------------------Controle do display(nomes de objetos)---------------------------------------------------
enum
{
    COL_NAME = 0,
    NUM_COLS
} ;

//Adiciona o nome do objeto a lista display
static void fill_model(string n){

    GtkTreeIter iter;
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,COL_NAME, n.c_str(),-1);
    gtk_widget_queue_draw(window_widget);

}

//Cria a lista do display e adiciona nome do objeto
static GtkTreeModel * create_and_fill_model (void){

    GtkTreeIter    iter;
    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING);

    return GTK_TREE_MODEL (store);
}

//Cria a lista do display
static void create_Model(){
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *view;

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (display),
                                                 -1,
                                                 "Name",
                                                 renderer,
                                                 "text", COL_NAME,
                                                 NULL);

    model = create_and_fill_model ();

    gtk_tree_view_set_model (GTK_TREE_VIEW (display), model);

}

//Multiplicação de Matriz 4x4
static void multMatrix4x4(double mat1 [4][4], double mat2 [4][4]){
    
    for(int a = 0; a < 4; a++){
        for (int b=0; b<4; b++){
            m_transform[a][b] = 0;
        }
    }
        
    for(int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            for (int k=0; k<4; k++) {
                m_transform[i][j] = m_transform[i][j] + (mat1[i][k] * mat2[k][j]);
            }
        }
    }
}
//Multiplicação de Matriz 4x4 por Matriz 1x4
static void multMatrix1x4(double m[4][4], double p[4]){


    for (int i = 0; i < 4; ++i) {
        v_transform[i] = (p[0]*m[0][i]) + (p[1]*m[1][i]) + (p[2]*m[2][i]) + (p[3]*m[3][i]);
    }
}
//---------------------------------------------------------Transformadas de viewport--------------------------------------------------
static double transformadaX_viewport(double Xw){
    double XwMIN;
    double XwMAX;;
    
    if(window.getEstado()){
        XwMIN = -1;
        XwMAX = 1;
    }
    else{
        XwMIN = window.getXmin();
        XwMAX = window.getXmax();
    }

    double aux = (Xw - XwMIN) / (XwMAX - XwMIN);

    double Xvp =  aux * double(XvpMAX-XvpMIN);
    return Xvp;
}

static double transformadaY_viewport(double Yw){
    
    double w_Ymax;
    double w_Ymin;
    
    if(window.getEstado()){
        w_Ymax = 1;
        w_Ymin = -1;
        
    }
    else{
        w_Ymax = window.getYmax();
        w_Ymin = window.getYmin();
    }
    
    double aux = (Yw - w_Ymin )/ (w_Ymax - w_Ymin);
    double Yvp = (1-aux) * double(YvpMAX-YvpMIN);
    return Yvp;
}

//Desenha a window na viewport
static void draw_window(){
    
   double Xmax = window.getXmax();
   double Ymax = window.getYmax();
   double Xmin = window.getXmin();
   double Ymin = window.getYmin();
   
   if(window.getEstado()){
        Xmax = 1;
        Ymax = 1;
        Xmin = -1;
        Ymin = -1;
        
    }
   
   draw_linha(transformadaX_viewport(Xmax),transformadaY_viewport(Ymax),transformadaX_viewport(Xmax), transformadaY_viewport(Ymin));
   draw_linha(transformadaX_viewport(Xmax), transformadaY_viewport(Ymin),transformadaX_viewport(Xmin), transformadaY_viewport(Ymin));
   draw_linha(transformadaX_viewport(Xmin), transformadaY_viewport(Ymin),transformadaX_viewport(Xmin), transformadaY_viewport(Ymax));
   draw_linha(transformadaX_viewport(Xmin), transformadaY_viewport(Ymax),transformadaX_viewport(Xmax),transformadaY_viewport(Ymax));
 }
 
//-------------------------------------------------------------Metodos de transformacao(multiplica��o de matrizes)---------------------------------------------------------
//Rotacao 2D
static Coordenada rotacao(Coordenada c, double t){
    double p[3] = {c.x,c.y,1};
    double m[3][3] =
            {
                    {cos(t),-sin(t),0},
                    {sin(t),cos(t),0},
                    {0,0,1}
            };

    double r[3];
    for (int i = 0; i < 3; ++i) {
        r[i] = (p[0]*m[0][i])+(p[1]*m[1][i])+(p[2]*m[2][i]);
    }
    Coordenada a(r[0],r[1],0);

    return a;
}

//Translacao Genérica
static Coordenada translacao(Coordenada c, double dX,double dY,double dZ){
    double p[4] = {c.x,c.y,c.z,1};
    double m[4][4] =
            {
                    {1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {dX,dY,dZ,1}
            };
    
    multMatrix1x4(m, p);
    Coordenada a(v_transform[0],v_transform[1],v_transform[2]);
    
    return a;
}

//Escalonamento Genérico
static Coordenada escalonamento(Coordenada c, double sX,double sY,double sZ){
    double p[4] = {c.x,c.y,c.z,1};
    double m[4][4] =
            {
                    {sX,0,0,0},
                    {0,sY,0,0},
                    {0,0,sZ,0},
                    {0,0,0,1}
            };

    multMatrix1x4(m, p);
    Coordenada a(v_transform[0],v_transform[1],v_transform[2]);
    
    return a;

}

//Rotacao 3D em torno do eixo X
static Coordenada rotacao3DX(Coordenada c, double t){
    double p[4] = {c.x,c.y,c.z,1};
    double m[4][4] =
            {
                    {1,0,0,0},
                    {0,cos(t),sin(t),0},
                    {0,-sin(t),cos(t),1},
                    {0,0,0,1}
            };

    multMatrix1x4(m, p);
    Coordenada a(v_transform[0],v_transform[1],v_transform[2]);
    
    return a;
}

//Rotacao 3D em torno do eixo Y
static Coordenada rotacao3DY(Coordenada c, double t){
    double p[4] = {c.x,c.y,c.z,1};
    double m[4][4] =
            {
                    {cos(t),0,-sin(t),0},
                    {0,1,0,0},
                    {sin(t),0,cos(t),0},
                    {0,0,0,1}
            };

    multMatrix1x4(m, p);
    Coordenada a(v_transform[0],v_transform[1],v_transform[2]);
    
    return a;
}

//Rotacao 3D em torno do eixo Z
static Coordenada rotacao3DZ(Coordenada c, double t){
    double p[4] = {c.x,c.y,c.z,1};
    double m[4][4] =
            {
                    {cos(t),sin(t),0,0},
                    {-sin(t),cos(t),0,0},
                    {0,0,1,0},
                    {0,0,0,1}
            };

    multMatrix1x4(m, p);
    Coordenada a(v_transform[0],v_transform[1],v_transform[2]);
    
    return a;
}

//------------------------------------------------------------------------------Rotacao e normalizacao do mundo(rotacao da window)--------------------------------------------------------------------
static Coordenada rotacionaPonto(double x, double y){
    Coordenada aux(x,y,0);
    double teta = window.getTeta();
    
     Coordenada w1 = window.getCoord(0);
    Coordenada w2 = window.getCoord(1);
    Coordenada w3 = window.getCoord(2);
    Coordenada w4 = window.getCoord(3);
    
    double altura = max(abs(w1.y - w3.y),abs(w2.y-w4.y));
    double largura = max(abs(w2.x-w1.x),abs(w4.x - w3.x));
    
    double x_norm = largura/2;
    double y_norm = altura/2;
    
    aux = translacao(aux, -window.getXcentro(),-window.getYcentro(),0);
    aux = rotacao(aux,-teta);
    aux = escalonamento(aux , 1/x_norm,1/y_norm,1);
    
    return aux;
}
//--------------------------------------------------------Calculo das matrizes de transformacao 3D---------------------------------
static void matriz_Perspectiva(){
    Coordenada vrp = window.getCentro();
    Coordenada cop = window.getCOP();
    double vrpT[4] = {vrp.x,vrp.y,vrp.z,1};
    double x;
    double y;
    double z;
        
    Linha aux;
        
    Coordenada w1 = window.getCoord(0);
    Coordenada w2 = window.getCoord(1);
    Coordenada w3 = window.getCoord(2);
    Coordenada w4 = window.getCoord(3);
        
    //determinando matriz de translação
    double trans[4][4] = {
                    {1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {-cop.x,-cop.y,-cop.z,1}};
        
    double  m_w1[4] = {w1.x,w1.y,w1.z,1};
    double  m_w2[4] = {w2.x,w2.y,w2.y,1};
    double  m_w3[4] = {w3.x,w3.y,w3.z,1};
    double  m_w4[4] = {w4.x,w4.y,w4.z,1};
        
    //Transladando VRP para origem
        
    multMatrix1x4(trans, m_w1);
    double w1t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w2);
    double w2t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w3);
    double w3t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w4);
    double w4t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, vrpT);
    vrp.x = v_transform[0];vrp.y = v_transform[1]; vrp.z = v_transform[2];
    
    
    
    
    //determinando VPN
    x = ((vrp.y-w1t[1]) * (w2t[2]-vrp.z))-((vrp.z-w1t[2]) * (w2t[1]-vrp.y));
    y = ((vrp.z-w1t[2]) * (w2t[0]-vrp.x))-((vrp.x-w1t[0]) * (w2t[2]-vrp.z));
    z = ((vrp.x-w1t[0]) * (w2t[1]-vrp.y))-((vrp.y-w1t[1]) * (w2t[0]+vrp.x));
    Coordenada vpn( x,y,z);  

    //determinando angulos
    double tetaX = atan(vpn.y/vpn.z);
    double tetaY = atan(vpn.x/vpn.z);
    
    //determinando matriz de rotacao X
    double rX[4][4] = {
        {1,0,0,0},
        {0,cos(tetaX),sin(tetaX),0},
        {0,-sin(tetaX),cos(tetaX),1},
        {0,0,0,1}};
        
    //determinando matriz de rotacao y
    double rY[4][4] ={
        {cos(tetaY),0,-sin(tetaY),0},
        {0,1,0,0},
        {sin(tetaY),0,cos(tetaY),0},
        {0,0,0,1}};
        
    //determinando matriz de rotacao transform
    multMatrix4x4(trans, rX);
    multMatrix4x4(m_transform, rY);
        
        
        
    //Transformando a window
    Coordenadas c_transform;
    multMatrix1x4( m_transform,m_w1);
    w1 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w2);
    w2 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w3);
    w3 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w4);
    w4 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    c_transform.push_back(w1);
    c_transform.push_back(w2);
    c_transform.push_back(w3);
    c_transform.push_back(w4);
    window.setTransforms(c_transform);
       
}
static void matriz_Ortogonal(){
 

        
    Coordenada vrp = window.getCentro();
    double vrpT[4] = {vrp.x,vrp.y,vrp.z,1};
        
    Linha aux;
        
    Coordenada w1 = window.getCoord(0);
    Coordenada w2 = window.getCoord(1);
    Coordenada w3 = window.getCoord(2);
    Coordenada w4 = window.getCoord(3);
        
//determinando matriz de translação
    double trans[4][4] = {
                    {1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {-vrp.x,-vrp.y,-vrp.z,1}};
        
    double  m_w1[4] = {w1.x,w1.y,w1.z,1};
    double  m_w2[4] = {w2.x,w2.y,w2.y,1};
    double  m_w3[4] = {w3.x,w3.y,w3.z,1};
    double  m_w4[4] = {w4.x,w4.y,w4.z,1};
        
//Transladando VRP para origem
        
    multMatrix1x4(trans, m_w1);
    double w1t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w2);
    double w2t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w3);
    double w3t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    multMatrix1x4(trans, m_w4);
    double w4t[4] = {v_transform[0],v_transform[1],v_transform[2],v_transform[3]};
    
    
    //determinando VPN
    Coordenada vpn((-w1t[1] * w2t[2])-(-w1t[2] * w2t[1]),(-w1t[2] * w2t[0])-(-w1t[0] * w2t[2]),(-w1t[0] * w2t[1])-(-w1t[1] * w2t[0]));  

    //determinando angulos
    double tetaX = atan(vpn.y/vpn.z);
    double tetaY = atan(vpn.x/vpn.z);
    
    //determinando matriz de rotacao X
    double rX[4][4] = {
        {1,0,0,0},
        {0,cos(tetaX),sin(tetaX),0},
        {0,-sin(tetaX),cos(tetaX),1},
        {0,0,0,1}};
        
    //determinando matriz de rotacao y
    double rY[4][4] ={
        {cos(tetaY),0,-sin(tetaY),0},
        {0,1,0,0},
        {sin(tetaY),0,cos(tetaY),0},
        {0,0,0,1}};
        
    //determinando matriz de rotacao transform
    multMatrix4x4(trans, rX);
    multMatrix4x4(m_transform, rY);
        
        
        
    //Transformando a window
    Coordenadas c_transform;
    multMatrix1x4( m_transform,m_w1);
    w1 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w2);
    w2 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w3);
    w3 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    multMatrix1x4(m_transform, m_w4);
    w4 = Coordenada(v_transform[0],v_transform[1],v_transform[2]);
    c_transform.push_back(w1);
    c_transform.push_back(w2);
    c_transform.push_back(w3);
    c_transform.push_back(w4);
    window.setTransforms(c_transform);
    
}
//-------------------------------------------------------------Atualizacao da viewport (Desenhos)-------------------------------------------------
static void atualiza_ponto(int i){
   
    double a;
    double b;
    
    
    ObjGrafico * obj = lista_objetos.getObj(i);
        
    fill_model(obj->getNome());
      
    Coordenada  c = obj->getCoord(0);
    
        

    
    if(window.getEstado()){
        c = rotacionaPonto(c.x,c.y);

    }
        
    if(window.clipPonto(c.x,c.y)){
        draw_linha(transformadaX_viewport(c.x),transformadaY_viewport(c.y),transformadaX_viewport(c.x),transformadaY_viewport(c.y));
    }
    
}


static void atualiza_linha(int i){

    double a;
    double b;
    double c;
    double d;
    Linha aux;

    ObjGrafico * obj = lista_objetos.getObj(i);
        
    fill_model(obj->getNome());
      
    Coordenada b1 = obj->getCoord(0);
    Coordenada b2 = obj->getCoord(1);
        

    
    if(window.getEstado()){
        b1 = rotacionaPonto(b1.x, b1.y);
        b2 = rotacionaPonto(b2.x, b2.y);
    }
    
    if(t_clipping)
        aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
    else
        aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
        
    if(aux.getEstado()){
        draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
    }
}

static void atualiza_poligono(int i){

    
    Linha aux;
    
    ObjGrafico * obj = lista_objetos.getObj(i);
        
    fill_model(obj->getNome());
    
    Coordenada b1;
    Coordenada b2;
    
    Coordenada inicio = obj->getCoord(0);
    
    for(int z = 0; z < obj->getCoordsSize() - 1; z++ ){

        b1 = obj->getCoord(z);
        b2 = obj->getCoord(z+1);
        
        
        if(window.getEstado()){
            b1 = rotacionaPonto(b1.x, b1.y);
            b2 = rotacionaPonto( b2.x, b2.y);
        }
        
        if(t_clipping)
            aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
        else
            aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
    }

    
    if(window.getEstado()){
            inicio = rotacionaPonto(inicio.x, inicio.y);
    }
    
    if(t_clipping)
        aux = window.cohenSutherland(inicio.x,inicio.y,b2.x,b2.y);
    else
        aux = window.liangBarsky(inicio.x,inicio.y,b2.x,b2.y);
    
    if(aux.getEstado()){
        draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
    }

}

static void desenhaCurvaFwdDiff(int n, double x,double dx,double dx2,double dx3,double y,double dy,double dy2,double dy3){
    Linha aux;
    int i = 1;
    double x0 = x;
    double y0 = y;
    
    double xf = x;
    double yf = y;
    
    Coordenada xy;
    double a;
    double b;
    double c;
    double d;
    
    while(i<n){
        i++;
        xf = xf + dx; dx = dx+dx2; dx2 = dx2+dx3;
        
        yf = yf +dy; y = dy+dy2; dy2 = dy2+dy3;
        
        a = x0;
        b = y0;
        c = xf;
        d = yf;  
        
       if(window.getEstado()){
            xy = rotacionaPonto(a, b);
            a = xy.x; 
            b = xy.y;
            xy = rotacionaPonto(c, d);
            c = xy.x; 
            d = xy.y;
        }
       
       
       if(t_clipping)
            aux = window.cohenSutherland(a,b,c,d);
        else
            aux = window.liangBarsky(a,b,c,d);
                
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
        x0 = xf;
        y0 = yf;
        
    }
    
}

//Desenha a curva 2D de acordo com o tipo (True = Bezier, False = B-spline). B-spline através do fwd difference
static void atualiza_curva(int i){

    double anterior[2];
    double atual[2];
    Linha aux;

    int numC = 0;
    double t2 = 0;
    double t3 = 0;
    
    double const1 = 0.002;
    double const2 = const1 * const1;
    double const3 = const2 * const1;
    
    Coordenada b0;
    Coordenada b1;
    Coordenada b2;
    Coordenada b3;
    
    Coordenada xy;
    double a;
    double b;
    double c;
    double d;
    
    ObjGrafico * obj = lista_objetos.getObj(i);

    fill_model(obj->getNome());
    
    if(obj -> getEstado()){
        numC = ((obj->getCoordsSize() - 4)/3)+1;
        
        for(int z = 0; z < numC; z++ ){
            
            b0 = obj->getCoord(z*3+0);
            b1 = obj->getCoord(z*3+1);
            b2 = obj->getCoord(z*3+2);
            b3 = obj->getCoord(z*3+3);
            
            anterior[0] = b0.x;
            anterior[1] = b0.y;
        
            for(double t=0; t <= 1;t = t + const1){
                t2 = t * t;
                t3 = t2 *t;
                atual[0] = (-t3 +3*t2 -3*t +1) * b0.x + (3*t3 -6*t2 +3*t) * b1.x + (-3*t3 +3*t2) * b2.x + (t3)*b3.x;
                atual[1] = (-t3 +3*t2 -3*t +1) * b0.y + (3*t3 -6*t2 +3*t) * b1.y + (-3*t3 +3*t2) * b2.y + (t3)*b3.y;
                
                a = anterior[0];
                b = anterior[1];
                c = atual[0];
                d = atual[1];  
        
                if(window.getEstado()){
                    xy = rotacionaPonto(a, b);
                    a = xy.x; 
                    b = xy.y;
                    xy = rotacionaPonto(c, d);
                    c = xy.x; 
                    d = xy.y;
                }
       
       
                if(t_clipping)
                    aux = window.cohenSutherland(a,b,c,d);
                else
                    aux = window.liangBarsky(a,b,c,d);
                
                if(aux.getEstado()){
                    draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
                }
        
                anterior[0] = atual[0];
                anterior[1] = atual[1];
            }
        }
    }
    else{
            
        
        for(int v = 3;v<obj->getCoordsSize();v++){
            b0 = obj->getCoord(v-3);
            b1 = obj->getCoord(v-2);
            b2 = obj->getCoord(v-1);
            b3 = obj->getCoord(v);
            //----------------------------------------------------------- X
            //----------------------------Calculo do coeficiente Cx
           
            double a =  b0.x * (-1/6) +  b1.x * (3/6) +  b2.x * (-3/6) + b3.x * (1/6);
            double b =  b0.x * (3/6) +  b1.x * (-1) +   b3.x * (4/6);
            double c =  b0.x * (-3/6) +  b1.x * (3/6) +  b2.x * (3/6) + b3.x * (1/6);
            double d =  b0.x * (1/6);
            
            //----------------------------Calculo de fx,dx,dx2,dx3
            double x = b * const3 + c * (6 * const3) + d * (6*const3);
            double dx = b * const2 + c * (2 * const2);
            double dx2 =  b * const1;
            double dx3 = a;
            //----------------------------------------------------------- Y
            //----------------------------Calculo do coeficiente Cy
            a = b0.y * (-1/6) + b1.y * (3/6) + b2.y * (-3/6) + b3.y * (1/6);
            b = b0.y * (3/6) + b1.y * (-1) +  b3.y * (4/6);
            c = b0.y * (-3/6) + b1.y * (3/6) + b2.y * (3/6) + b3.y * (1/6);
            d = b0.y * (1/6);
            //----------------------------Calculo de fy,dy,dy2,dy3
            double y = b * const3 + c * (6 * const3) + d * (6*const3);
            double dy = b * const2 + c * (2 * const2);
            double dy2 =  b * const1;
            double dy3 = a;
                    
            double n = 1 / const1;
                    
            desenhaCurvaFwdDiff(n,x,dx,dx2,dx3,y,dy,dy2,dy3);
                    
        }
        
        
    }
}

static void branching(Coordenada b1,Coordenada b2,Coordenada c1,Coordenada c2){
    Linha aux;
    if(t_clipping)
            aux = window.cohenSutherland(b1.x + b1.z,b1.y + b1.z,c1.x + c1.z,c1.y + c1.z);
        else
            aux = window.liangBarsky(b1.x + b1.z,b1.y + b1.z,c1.x + c1.z,c1.y + c1.z);
    
    if(aux.getEstado()){
        draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
    }
    
    double dist1 = (b2.x + b2.z) - (b1.x + b1.z);
    double dist2 = (c2.x + c2.z) - (c1.x + c1.z);
         
        
    if(dist2 < dist1){
        if(t_clipping)
            aux = window.cohenSutherland(c1.x + c1.z,c1.y + c1.z,b2.x + b2.z,b2.y + b2.z);
        else
            aux = window.liangBarsky(c1.x + c1.z,c1.y + c1.z,b2.x + b2.z,b2.y + b2.z);
    }else{
        if(t_clipping)
            aux = window.cohenSutherland(b1.x + b1.z,b1.y + b1.z,c2.x + c2.z,c2.y + c2.z);
        else
            aux = window.liangBarsky(b1.x + b1.z,b1.y + b1.z,c2.x + c2.z,c2.y + c2.z);
    }
    
    if(aux.getEstado()){
        draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
    }
}


static void atualiza_objeto3D_arame(int i){
    ObjGrafico * obj = lista_objetos.getObj(i);

    fill_model(obj->getNome());
    
    Faceta f;
    Faceta f1;
    Coordenada b1;
    Coordenada b2;
    Coordenada c1;
    Coordenada c2;
    Linha aux;


    for(int i = 0;i < obj->getFacetsSize()-1;i++){
        
        
        f = obj->getFaceta(i);
        f1 = obj->getFaceta(i+1);
        
        for(int j = 0; j<f.getSize()-1;j++){
            
            b1 = obj->getCoord(f.getVertice(j)-1);
            b2 = obj->getCoord(f.getVertice(j+1)-1);
            c1 = obj->getCoord(f1.getVertice(j)-1);
            c2 = obj->getCoord(f1.getVertice(j+1)-1);
            
            //------------------desenha linhas das Faceta
            if(t_clipping)
                aux = window.cohenSutherland(b1.x + b1.z,b1.y + b1.z,b2.x + b2.z,b2.y + b2.z);
            else
                aux = window.liangBarsky(b1.x + b1.z,b1.y + b1.z,b2.x + b2.z,b2.y + b2.z);
            if(aux.getEstado()){
                draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
            }

            if(t_clipping)
                aux = window.cohenSutherland(c1.x + c1.z,c1.y + c1.z,c2.x + c2.z,c2.y + c2.z);
            else
                aux = window.liangBarsky(c1.x + c1.z,c1.y + c1.z,c2.x + c2.z,c2.y + c2.z);
            
            if(aux.getEstado()){
                draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
            }
                
                //-------------------Liga facetas
                
           branching(b1,b2,c1,c2); 
              
        }
        //------------------------------------ Últimas arestas
        b1 = obj->getCoord(f.getVertice(0)-1);
        b2 = obj->getCoord(f.getVertice(f.getSize()-1)-1);
        c1 = obj->getCoord(f1.getVertice(0)-1);
        c2 = obj->getCoord(f1.getVertice(f1.getSize()-1)-1);
                
        //------------------desenha linhas das Faceta
        if(t_clipping)
            aux = window.cohenSutherland(b1.x + b1.z,b1.y + b1.z,b2.x + b2.z,b2.y + b2.z);
        else
            aux = window.liangBarsky(b1.x + b1.z,b1.y + b1.z,b2.x + b2.z,b2.y + b2.z);
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
            
        if(t_clipping)
            aux = window.cohenSutherland(c1.x + c1.z,c1.y + c1.z,c2.x + c2.z,c2.y + c2.z);
        else
            aux = window.liangBarsky(c1.x + c1.z,c1.y + c1.z,c2.x + c2.z,c2.y + c2.z);
        
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
                
        //-------------------Liga facetas
        
          branching(b1,b2,c1,c2); 
               
    }
}


static void atualiza_objeto3D_Perspectiva(int i){
    bool estado =  window.getEstado();
    window.setEstado(true);
    ObjGrafico * obj = lista_objetos.getObj(i);
    fill_model(obj->getNome());
    
    Coordenada w1 = window.getTransform(0);
    Coordenada w2 = window.getTransform(1);
    Coordenada w3 = window.getTransform(2);
    Coordenada w4 = window.getTransform(3);
    
    double altura = max(abs(w1.y - w3.y),abs(w2.y-w4.y));
    double largura = max(abs(w2.x-w1.x),abs(w4.x - w3.x));
        
        
    Faceta f;
    Coordenada b1;
    Coordenada b2;
    Linha aux;
    
    double bt[4];
    bt[3] = 1;
    Coordenada cop = window.getCOP();
    
    double xp;
    double yp;
    
    for(int i = 0;i < obj->getFacetsSize();i++){
        
        
        f = obj->getFaceta(i);
        
        for(int j = 0; j < f.getSize()-1;j++){
            
            b1 = obj->getCoord(f.getVertice(j)-1);
            b2 = obj->getCoord(f.getVertice(j+1)-1);
            
            
            bt[0] = b1.x;
            bt[1]  = b1.y;
            bt[2] = b1.z;
            multMatrix1x4(m_transform, bt);
            b1.x = v_transform[0];b1.y = v_transform[1];
            
            //--------------------------------------------calculo de xp e yp
            b1.x = (cop.z*b1.x) / b1.z;
            b1.y = (cop.z*b1.y) / b1.z;
            b1 = escalonamento(b1, 1/(largura/2), 1/(altura/2), 1);
            
            
            
            bt[0] = b2.x;
            bt[1]  = b2.y;
            bt[2] = b2.z;
            multMatrix1x4(m_transform, bt);
            b2.x = v_transform[0];b2.y = v_transform[1];
            
            //--------------------------------------------calculo de xp e yp
            b2.x = (cop.z*b2.x) / b2.z;
            b2.y = (cop.z*b2.y) / b2.z;
            b2 = escalonamento(b2, 1/(largura/2),1/(altura/2),1);
            
            

            if(t_clipping)
                aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
            else
                aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
            if(aux.getEstado()){
                draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
            }
        }
        
        b1 = obj->getCoord(f.getVertice(0)-1);
        b2 = obj->getCoord(f.getVertice(f.getSize()-1)-1);
        //draw_linha(transformadaX_viewport(b1.x+b1.z),transformadaY_viewport(b1.y+b1.z),transformadaX_viewport(b2.x+b2.z),transformadaY_viewport(b2.y+b2.z));
        bt[0] = b1.x;
        bt[1]  = b1.y;
        bt[2] = b1.z;
        multMatrix1x4(m_transform, bt);
        b1.x = v_transform[0];b1.y = v_transform[1];
        b1.x = (cop.z*b1.x) / b1.z;
        b1.y = (cop.z*b1.y) / b1.z;
        b1 = escalonamento(b1, 1/(largura/2), 1/(altura/2), 1);
        
        bt[0] = b2.x;
        bt[1]  = b2.y;
        bt[2] = b2.z;
        multMatrix1x4(m_transform, bt);
        b2.x = v_transform[0];b2.y = v_transform[1];
        b2.x = (cop.z*b2.x) / b2.z;
        b2.y = (cop.z*b2.y) / b2.z;
        b2 = escalonamento(b2, 1/(largura/2),1/(altura/2),1);
        
        if(t_clipping)
            aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
        else
            aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
        
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
    }
    window.setEstado(estado);
    
}

static void atualiza_objeto3D_Ortogonal(int i){
    bool estado =  window.getEstado();
    window.setEstado(true);
    ObjGrafico * obj = lista_objetos.getObj(i);
    fill_model(obj->getNome());
    
    Coordenada w1 = window.getTransform(0);
    Coordenada w2 = window.getTransform(1);
    Coordenada w3 = window.getTransform(2);
    Coordenada w4 = window.getTransform(3);
    
    double altura = max(abs(w1.y - w3.y),abs(w2.y-w4.y));
    double largura = max(abs(w2.x-w1.x),abs(w4.x - w3.x));
        
        
    Faceta f;
    Coordenada b1;
    Coordenada b2;
    Linha aux;
    
    double bt[4];
    bt[3] = 1;
    
    for(int i = 0;i < obj->getFacetsSize();i++){
        
        
        f = obj->getFaceta(i);
        
        for(int j = 0; j < f.getSize()-1;j++){
            
            b1 = obj->getCoord(f.getVertice(j)-1);
            b2 = obj->getCoord(f.getVertice(j+1)-1);
            
            bt[0] = b1.x;
            bt[1]  = b1.y;
            bt[2] = b1.z;
            multMatrix1x4(m_transform, bt);
            b1.x = v_transform[0];b1.y = v_transform[1];
            b1 = escalonamento(b1, 1/(largura/2), 1/(altura/2), 1);
        
            bt[0] = b2.x;
            bt[1]  = b2.y;
            bt[2] = b2.z;
            multMatrix1x4(m_transform, bt);
            b2.x = v_transform[0];b2.y = v_transform[1];
            b2 = escalonamento(b2, 1/(largura/2),1/(altura/2),1);
            
            if(t_clipping)
                aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
            else
                aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
            if(aux.getEstado()){
                draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
            }
        }
        
        b1 = obj->getCoord(f.getVertice(0)-1);
        b2 = obj->getCoord(f.getVertice(f.getSize()-1)-1);
        
        bt[0] = b1.x;
        bt[1]  = b1.y;
        bt[2] = b1.z;
        multMatrix1x4(m_transform, bt);
        b1.x = v_transform[0];b1.y = v_transform[1];
        b1 = escalonamento(b1, 1/(largura/2),1/(altura/2),1);
        
        bt[0] = b2.x;
        bt[1]  = b2.y;
        bt[2] = b2.z;
        multMatrix1x4(m_transform, bt);
        b2.x = v_transform[0];b2.y = v_transform[1];
        b2 = escalonamento(b2, 1/(largura/2),1/(altura/2),1);
        

        if(t_clipping)
            aux = window.cohenSutherland(b1.x,b1.y,b2.x,b2.y);
        else
            aux = window.liangBarsky(b1.x,b1.y,b2.x,b2.y);
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
        }
           
    }
    window.setEstado(estado);
    
}

static void atualiza_superficieBezier(int i){
    
    ObjGrafico * obj = lista_objetos.getObj(i);
    fill_model(obj->getNome());
    Linha aux;
    
    double t2 = 0;
    double t3 = 0;
    double s2 = 0;
    double s3 = 0;
    double const1 = 0.002;
    
    Coordenada anterior(0,0,0);
    Coordenada atual(0,0,0);
    
    Coordenadas c;
    
    for(int i = 0; i<16;i++){
        c.push_back(obj->getCoord(i));
    }
    
    double m[4][4] = {
                    {-1,3,-3,1},
                    {3,-6,3,0},
                    {-3,3,0,0},
                    {1,0,0,0}};
                    
    double gX[4][4] = {
                    {c[0].x,c[1].x,c[2].x,c[3].x},
                    {c[4].x,c[5].x,c[6].x,c[7].x},
                    {c[8].x,c[9].x,c[10].x,c[11].x,},
                    {c[12].x,c[13].x,c[14].x,c[15].x}};
                    
    double gY[4][4] = {
                    {c[0].y,c[1].y,c[2].y,c[3].y},
                    {c[4].y,c[5].y,c[6].y,c[7].y},
                    {c[8].y,c[9].y,c[10].y,c[11].y,},
                    {c[12].y,c[13].y,c[14].y,c[15].y}};
                    
    double gZ[4][4] = {
                    {c[0].z,c[1].z,c[2].z,c[3].z},
                    {c[4].z,c[5].z,c[6].z,c[7].z},
                    {c[8].z,c[9].z,c[10].z,c[11].z,},
                    {c[12].z,c[13].z,c[14].z,c[15].z}};
    
    for(double s=0; s <= 1;s = s + const1){
        s2 = s * s;
        s3 = s2 *s;
        double S[4] = {s3,s2,s,1};
        
        for(double t=0; t <= 1;t = t + const1){
            t2 = t * t;
            t3 = t2 *t;
            
            multMatrix1x4(m,S);
            multMatrix1x4(gX, v_transform);
            multMatrix1x4(m, v_transform);
            atual.x = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];
            
            
            multMatrix1x4(m,S);
            multMatrix1x4(gY, v_transform);
            multMatrix1x4(m, v_transform);
            atual.y = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];
            
            multMatrix1x4(m,S);
            multMatrix1x4(gZ, v_transform);
            multMatrix1x4(m, v_transform);
            atual.z = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];

    
                
            if(t!=0){
                if(t_clipping)
                    aux = window.cohenSutherland(anterior.x + anterior.z,anterior.y + anterior.z,atual.x + atual.z,atual.y + atual.z);
                else
                    aux = window.liangBarsky(anterior.x + anterior.z,anterior.y + anterior.z,atual.x + atual.z,atual.y + atual.z);
                if(aux.getEstado()){
                    draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
                }
            }
            
            anterior.x = atual.x;
            anterior.y = atual.y;
            anterior.z = atual.z;
        }
    }
    
    
}

static void atualiza_superficieSpline(int i){
    
    ObjGrafico * obj = lista_objetos.getObj(i);
    fill_model(obj->getNome());
    Linha aux;
    
    double t2 = 0;
    double t3 = 0;
    double s2 = 0;
    double s3 = 0;
    double const1 = 0.002;
    
    Coordenada anterior(0,0,0);
    Coordenada atual(0,0,0);
    
    Coordenadas c;
    
    for(int i = 0; i<16;i++){
        c.push_back(obj->getCoord(i));
    }
    
    double m[4][4] = {
                    {-1/6,1/3,-1/3,1/6},
                    {1/3,-1,1/3,0},
                    {-1/3,0,1/3,0},
                    {1/6,2/3,1/6,0}};
                    
    double mT[4][4] = {
                    {-1/6,1/3,-1/3,1/6},
                    {1/3,-1,0,2/3},
                    {-1/3,1/3,1/3,1/6},
                    {1/6,0,0,0}};
                    
    double gX[4][4] = {
                    {c[0].x,c[1].x,c[2].x,c[3].x},
                    {c[4].x,c[5].x,c[6].x,c[7].x},
                    {c[8].x,c[9].x,c[10].x,c[11].x,},
                    {c[12].x,c[13].x,c[14].x,c[15].x}};
                    
    double gY[4][4] = {
                    {c[0].y,c[1].y,c[2].y,c[3].y},
                    {c[4].y,c[5].y,c[6].y,c[7].y},
                    {c[8].y,c[9].y,c[10].y,c[11].y,},
                    {c[12].y,c[13].y,c[14].y,c[15].y}};
                    
    double gZ[4][4] = {
                    {c[0].z,c[1].z,c[2].z,c[3].z},
                    {c[4].z,c[5].z,c[6].z,c[7].z},
                    {c[8].z,c[9].z,c[10].z,c[11].z,},
                    {c[12].z,c[13].z,c[14].z,c[15].z}};
    
    for(double s=0; s <= 1;s = s + const1){
        s2 = s * s;
        s3 = s2 *s;
        double S[4] = {s3,s2,s,1};
        
        for(double t=0; t <= 1;t = t + const1){
            t2 = t * t;
            t3 = t2 *t;
            
            multMatrix1x4(m,S);
            multMatrix1x4(gX, v_transform);
            multMatrix1x4(mT, v_transform);
            atual.x = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];
            
            
            multMatrix1x4(m,S);
            multMatrix1x4(gY, v_transform);
            multMatrix1x4(mT, v_transform);
            atual.y = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];
            
            multMatrix1x4(m,S);
            multMatrix1x4(gZ, v_transform);
            multMatrix1x4(mT, v_transform);
            atual.z = v_transform[0] * t + v_transform[1] * t2 + v_transform[2] * t3 + v_transform[3];

    
                
            if(t!=0){
                if(t_clipping)
                    aux = window.cohenSutherland(anterior.x + anterior.z,anterior.y + anterior.z,atual.x + atual.z,atual.y + atual.z);
                else
                    aux = window.liangBarsky(anterior.x + anterior.z,anterior.y + anterior.z,atual.x + atual.z,atual.y + atual.z);
                
                if(aux.getEstado()){
                    draw_linha(transformadaX_viewport(aux.getCoord(0).x),transformadaY_viewport(aux.getCoord(0).y),transformadaX_viewport(aux.getCoord(1).x),transformadaY_viewport(aux.getCoord(1).y));
                }
            }
            
            anterior.x = atual.x;
            anterior.y = atual.y;
            anterior.z = atual.z;
        }
    }
    
    
}

//Limpa a surface e desenha os objetos novamente, verifica o tipo do objeto
static void atualiza_surface(){
    clear_surface();
    gtk_list_store_clear (store);
    
    draw_window();
    ObjGrafico * obj;
    
    if(modelo3D == 2){
        matriz_Ortogonal();
    }
    else if(modelo3D == 3){
        matriz_Perspectiva();
    }
    
    for(int i = 0; i< lista_objetos.size();i++){
        obj = lista_objetos.getObj(i);
        if(obj->getTipo() == 0)
            atualiza_ponto(i);
        else if(obj->getTipo() == 1)
            atualiza_linha(i);
        else if(obj->getTipo() == 2)
            atualiza_poligono(i);
        else if(obj->getTipo() == 3)
            atualiza_curva(i);
        else if(obj->getTipo() == 4){
            //Tipo da vizualizacao
            if(modelo3D == 1){
                atualiza_objeto3D_arame(i);
            }
            else if(modelo3D == 2){
                matriz_Ortogonal();
                atualiza_objeto3D_Ortogonal(i);
            }
            else{
                matriz_Perspectiva();
                atualiza_objeto3D_Perspectiva(i);
            }
        }else if(obj->getTipo() == 6){
            if(t_superficie)
                atualiza_superficieBezier(i);
            else
                atualiza_superficieSpline(i);
        }
    }
}
//Limpa surface e lista
static void clear(){
    gtk_widget_hide(window_objeto);
    gtk_list_store_clear (store);
    lista_objetos.clearL();
    

    atualiza_surface();
}



//-------------------------------------------------------------------------Controle das Windows(programa)----------------------------------------------
static void editor_de_mundo(){
    gtk_widget_show_all(window_objeto);
}

static void editor_de_mundo3D(){
    gtk_widget_show_all(window_objeto3D);
}

static void ler_arquivo(){
    gtk_widget_show_all(window_arquivo);
}
static void opcoes(){
    gtk_widget_show_all(window_opcao);
}


static bool temObjeto(std::string n){
    ObjGrafico * obj;
    
    for(int i = 0; i< lista_objetos.size();i++){
        obj = lista_objetos.getObj(i);
        
        if(obj->getNome() == n){
            index = i;
            return true;
            
        }
    }
    
    return false;
}

static void configuracoes(){
    modelo3D = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_tipo_vizualizacao));
    int s = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_tipo_superficie));
    int c = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_tipo_recorte));
    if(c = 1)
        t_clipping = true;
    else
        t_clipping = false;
    
    if(s = 1)
        t_superficie = true;
    else
        t_superficie = false;
    passo_window = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_passo));
    t_linha = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_t_linha));
    gtk_widget_hide(window_opcao);
}
static void desenha(){
    gtk_widget_hide(window_objeto);
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome));
    
    if(!temObjeto(n)){
        int s = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_npontos));
        int tipo = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_tipo));
        Coordenada p(0,0,0);

        
        poligono_aux = new Poligono(n);
        
        if(tipo == 1){
            curva_aux = new Curva( n, true);
            for (int i = 0; i< s*4;i++)
                curva_aux -> addCoordenada(p);
        }
        else{
            if(s < 4){
                s = 4;
            }
            curva_aux = new Curva(n,false);
            for (int i = 0; i< s;i++)
                curva_aux -> addCoordenada(p);
        }
    

        gtk_widget_show_all(window_desenho);
    }

}

static void desenha3D(){
    gtk_widget_hide(window_objeto3D);
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    Coordenada p(0,0,0);
    
    
    
    if(!temObjeto(n)){
        int n_faceta = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_npontos1));
        
        index = 0;
        objeto3D = new Objeto3D(n);
        for (int i = 0; i< n_faceta;i++){
            faceta_aux.addVertice(0);
        }
        
        superficie = new Superficie(n,true);
        for (int i = 0; i< 16;i++)
            superficie -> addCoordenada(p);
        
        gtk_widget_show_all(window_desenho3D);
    }

}

static void transforma(){
    gtk_widget_hide(window_objeto);
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome));
    
    if(temObjeto(n)){
        gtk_widget_show_all(window_transformacao);
    }

}
static void transforma3D(){
    gtk_widget_hide(window_objeto3D);
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    
    if(temObjeto(n)){
        gtk_widget_show_all(window_transformacao3D);
    }

}


//--------------------------------------------------------------------------Criadores de Objetos(ponto,linha,poligono)------------------------------------------------------------------------------------------------

static void desenha_ponto(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome));

    Ponto * obj = new Ponto(a,b,n);
    lista_objetos.addObj(obj);


    gtk_widget_hide(window_desenho);
    atualiza_surface();
}

static void desenha_linha(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y));
    double c = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_xf));
    double d = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_yf));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome));

    Linha * obj = new Linha(a,b,c,d,n);
    lista_objetos.addObj(obj);
    


    gtk_widget_hide(window_desenho);
    atualiza_surface();

}

static void adiciona_vertice_poligono(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y));
    
    Coordenada p(x,y,0);
    poligono_aux->addCoordenada(p);

}

static void desenha_poligono(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y));
    
    Coordenada p(x,y,0);
    poligono_aux->addCoordenada(p);
    
    lista_objetos.addObj(poligono_aux);
    gtk_widget_hide(window_desenho);
    atualiza_surface();
}

static void desenha_curva(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y));
    
    Coordenada p(x,y,0);
    curva_aux->setCoordenada(p,curva_aux->getPosicao());
    curva_aux->addPosicao();

    if(curva_aux->getPosicao() == curva_aux->getCoordsSize()){
        lista_objetos.addObj(curva_aux);
        
        gtk_widget_hide(window_desenho);
        atualiza_surface();
    }

}


static void adiciona_vertice_objeto3D(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x2));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y2));
    double c = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z));
    
    Coordenada p(a,b,c);
    objeto3D->addCoordenada(p);
    
    
}

static void adiciona_verticeFinal_objeto3D(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x2));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y2));
    double c = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z));
    
    Coordenada p(a,b,c);
    objeto3D->addCoordenada(p);
    
    gtk_widget_hide(window_desenho3D);
    gtk_widget_show_all(window_faceta);
    
}

static void adiciona_faceta_objeto3D(){
    int v = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(button_vertice));
    
    if( v < (objeto3D->getCoordsSize()+1)){
        faceta_aux.setVertice(v,index);
        index ++;
    
        if( index = faceta_aux.getSize()){
            objeto3D -> addFaceta(faceta_aux);
            index = 0;
        }
    }
    
}

static void desenha_objeto3D(){
    
    double v = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_vertice));
    
    if( v <= objeto3D->getCoordsSize()){
        faceta_aux.setVertice(v,index);
        index ++;
    
        if( index = faceta_aux.getSize()){
            objeto3D -> addFaceta(faceta_aux);
            lista_objetos.addObj(objeto3D);
            atualiza_surface();
            gtk_widget_hide(window_faceta);
        }
    }
    
}

static void desenha_superficie3D(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x2));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y2));
    double z = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z));
    
    Coordenada p(x,y,z);
    superficie->setCoordenada(p,superficie->getPosicao());
    superficie->addPosicao();

    if(superficie->getPosicao() == superficie->getCoordsSize()){
        lista_objetos.addObj(superficie);
        
        gtk_widget_hide(window_desenho3D);
        atualiza_surface();
    }
    
}

static void desenha_arquivo(){
    std:string fileName = gtk_entry_get_text (GTK_ENTRY (button_nome2));
    DescritorObj reader(fileName);
    std::vector<ObjGrafico*>& objetos = reader.getObjs();
    for(int i = 0;i< objetos.size();i++){
        //draw_linha(0,0,100,i*10);
        if(objetos[i] -> getTipo() == 5){
            for(int j = 0; j < objetos[i]-> getCoordsSize();j++ ){
                window.setCoordenada(objetos[i]-> getCoord(j),j);
            }
        }
        else if(!temObjeto(objetos[i] -> getNome()))
                lista_objetos.addObj(objetos[i]);
        
    }
    atualiza_surface();
    gtk_widget_hide(window_arquivo);
}


//----------------------------------------------------------------------Transformacao do Objeto(translacao,rotacao e escalonamento)---------------------------------------------------------------------------------
static void translada(){
    double dX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x1));
    double dY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y1));
    
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;

    for (int i =0; i< obj->getCoordsSize(); i++){
        c = obj->getCoord(i);
        c = translacao(c, dX,dY,0);
        obj->setCoordenada(c,i);
    }
    
    gtk_widget_hide(window_transformacao);
    atualiza_surface();
}

static void escalona(){
    double sX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x1));
    double sY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y1));
    Coordenada centro;

    
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;

    centro = obj -> getCentro();

    for (int i =0; i<obj->getCoordsSize(); i++){
        c = obj->getCoord(i);
        c = translacao(c, -centro.x,-centro.y,-centro.z);
        c = escalonamento(c,sX,sY,1);
        c = translacao(c, centro.x,centro.y,centro.z);
        obj->setCoordenada(c,i);
    }

    gtk_widget_hide(window_transformacao);
    atualiza_surface();
    
}

static void rotaciona(){
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo));

    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    
    for (int i =0; i<obj->getCoordsSize(); i++){
        c = obj->getCoord(i);
        c = rotacao(c,teta);
        obj->setCoordenada(c,i);
    }

    gtk_widget_hide(window_transformacao);
    atualiza_surface();
    
    
}

static void rotacionaCObj(){
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo));
    Coordenada centro;
    
    
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    
    centro = obj -> getCentro();

    for (int i =0; i<obj->getCoordsSize(); i++){
        c = obj->getCoord(i);
        c = translacao(c, -centro.x,-centro.y,-centro.z);
        c = rotacao(c,teta);
        c = translacao(c, centro.x,centro.y,centro.z);
        obj->setCoordenada(c,i);
    }

    gtk_widget_hide(window_transformacao);
    atualiza_surface();


}

static void rotacionaPQ(){
    double pX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x1));
    double pY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y1));
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo));
    double x;
    double y;
    double * aux;
    
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    
    for (int i =0; i<obj->getCoordsSize(); i++){
        c = obj->getCoord(i);
        c = translacao(c, -pX,-pY,0);
        c = rotacao(c,teta);
        c = translacao(c, pX,pY,0);
        obj->setCoordenada(c,i);
    }

    gtk_widget_hide(window_transformacao);
    atualiza_surface();
    
    

}

//-----------------------------------------------Transformação 3D
static void translada3D(){
    double dX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x3));
    double dY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y3));
    double dZ = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z1));

    
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        
        c = obj -> getCoord(i);
        c = translacao(c, dX,dY,dZ);
        obj->setCoordenada(c,i);
        
    }
    gtk_widget_hide(window_transformacao3D);
    atualiza_surface();
    
}

static void escalona3D(){
    double sX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x3));
    double sY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y3));
    double sZ = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z1));
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    Coordenada centro = obj -> getCentro();
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        c = obj -> getCoord(i);
        c = translacao(c, -centro.x,-centro.y,-centro.z);
        c = escalonamento(c,sX,sY,sZ);
        c = translacao(c, centro.x,centro.y,centro.z);
        obj->setCoordenada(c,i);
    }

    gtk_widget_hide(window_transformacao3D);
    atualiza_surface(); 
    
}

static void rotaciona3DX(){
    
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo1));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    Coordenada centro = obj -> getCentro();
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        c = obj -> getCoord(i);
        c = rotacao3DX(c,teta);
        obj->setCoordenada(c,i);
        
    }
    
    gtk_widget_hide(window_transformacao3D);
    atualiza_surface();
    
    
}

static void rotaciona3DY(){
   double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo1));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    Coordenada centro = obj -> getCentro();
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        c = obj -> getCoord(i);
        c = rotacao3DY(c,teta);
        obj->setCoordenada(c,i);
        
    }
    
    gtk_widget_hide(window_transformacao3D);
    atualiza_surface();
}

static void rotaciona3DZ(){
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo1));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    ObjGrafico * obj = lista_objetos.getObj(index);;
    Coordenada  c;
    Coordenada centro = obj -> getCentro();
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        c = obj -> getCoord(i);
        c = rotacao3DZ(c,teta);
        obj->setCoordenada(c,i);
        
    }
    
    gtk_widget_hide(window_transformacao3D);
    atualiza_surface();
    
}

static void rotaciona3DEQ(){

    double pX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_x3));
    double pY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_y3));
    double pZ = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_z1));
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(button_angulo1));
    string n = gtk_entry_get_text (GTK_ENTRY (button_nome1));
    double * aux;
    double opX;
    double adX;
    double opZ;
    double adZ;
    double tetaX;
    double tetaZ;
    Coordenada  c;
    ObjGrafico * obj = lista_objetos.getObj(index);;
    
    
    for (int i =0; i<obj ->getCoordsSize(); i++){
        //- -------------------------------------------------Ponto 1
        c = obj -> getCoord(i);
        c = translacao(c, -pX,-pY,-pZ);
            
        //----------------------------------------------Calculo do Angulo
        opX = c.y; // y
        adX = c.z; // z
        tetaX = atan(opX / adX);
            
        opZ = c.x; //x
        adZ = sqrt(opX*opX + adX*adX); //hipotenusa
        tetaZ = atan(opZ / adZ);
            
        //------------------------------------------------Transformações
        c = rotacao3DX(c,tetaX);
        c = rotacao3DZ(c,tetaZ);
        c = rotacao3DY(c,teta);
        c = rotacao3DZ(c,-tetaZ);
        c = rotacao3DX(c,-tetaX);
        c = translacao(c, pX,pY,pZ);
        obj->setCoordenada(c,i);

    }

    gtk_widget_hide(window_transformacao3D);
    atualiza_surface();
    

}


//---------------------------------------------------------------------------------------Controle da Window(rota��o,movimenta��o,zoom)-----------------------------------------------------------------------------------------------------
static void rotacionaWindowEsquerda(){
    window.addTeta(passo_window);
    atualiza_surface();

}

static void rotacionaWindowDireita(){
    window.addTeta(passo_window);
    atualiza_surface();
}

static void move_up(){

    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Ymax += passo_window;
    Ymin += passo_window;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();

}

static void move_down(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Ymax -= passo_window;
    Ymin -= passo_window;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void move_left(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Xmax -= passo_window;
    Xmin -= passo_window;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void move_right(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Xmax += passo_window;
    Xmin += passo_window;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void zoom_in(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    double dX = passo_window;
    double dY = passo_window;

    window.setMAX(Xmax-dX,Ymax-dY);
    window.setMIN(Xmin+dX,Ymin+dY);

    atualiza_surface();
}

static void zoom_out(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    double dX = passo_window;
    double dY = passo_window;

    window.setMAX(Xmax+dX,Ymax+dY);
    window.setMIN(Xmin-dX,Ymin-dY);

    atualiza_surface();

}

int main(int argc, char *argv[]){
    window = Window(50,50,-10,-10);
    //Builder
    GtkBuilder  *gtkBuilder;
    gtk_init(&argc, &argv);
    gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder, "CG_Application.glade", NULL);

    //Display
    window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
    window_desenho = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_desenho") );
    window_transformacao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_transformacao") );
    window_objeto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_objeto") );
    window_desenho3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_desenho3D") );
    window_transformacao3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_transformacao3D") );
    window_objeto3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_objeto3D") );
    window_faceta = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_faceta") );
    window_arquivo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_arquivo") );
    window_opcao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_opcao") );
    drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
    display = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "display") );
    create_Model(); 
    button_objetos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_objetos") );
    button_objetos3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_objetos3D") );
    button_leitorOBJ = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_leitorOBJ") );
 

    //Botoes de desenho
    button_desenhar = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_desenhar") );
    button_nome = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_nome") );
    button_clear = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_clear") );
    button_ponto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_ponto") );
    button_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_linha") );
    button_poligono = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_poligono") );
    button_poligono1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_poligono1") );
    button_x = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_x") );
    button_y = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_y") );
    button_xf = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_xf") );
    button_yf = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_yf") );
    

    //Botoes de Transformacao
    button_transformar= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_transformar") );
    button_x1= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_x1") );
    button_y1= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_y1") );
    button_translacao= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_translacao") );
    button_escalonamento= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_escalonamento") );
    button_rotacao= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao") );
    button_rotacaoCObj = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacaoCObj") );
    button_rotacaoPQ = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacaoPQ") );
    button_angulo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_angulo") );
    
    //Botoes de desenho3D
    button_desenhar1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_desenhar1") );
    button_nome1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_nome1") );
    button_desenhar2 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_desenhar2") );
    button_nome2 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_nome2") );
    button_clear1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_clear1") );
    button_objeto3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_objeto3D") );
    button_objeto3D1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_objeto3D1") );
    button_superficie3D = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_superficie3D") );
    button_faceta = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_faceta") );
    button_faceta1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_faceta1") );
    button_x2 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_x2") );
    button_y2 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_y2") );
    button_z = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_z") );
    button_npontos1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_npontos1") );
    button_vertice = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_vertice") );
     
    
    //Botoes de Transformacao3D
    button_transformar1= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_transformar1") );
    button_x3= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_x3") );
    button_y3= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_y3") );
    button_translacao1= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_translacao1") );
    button_escalonamento1= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_escalonamento1") );
    button_rotacao3DEQ= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao3DEQ") );
    button_rotacao3DX = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao3DX") );
    button_rotacao3DY = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao3DY") );
    button_rotacao3DZ = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao3DZ") );
    button_angulo1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_angulo1") );
    button_z1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_z1") );
    
    //Botoes de desenho de curva
    button_curva = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_curva") );
    button_tipo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_tipo") );
    button_npontos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_npontos") );
    
    
    //Botoes de controle da window
    button_up = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_up") );
    button_down = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_down") );
    button_left = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_left") );
    button_right = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_right") );
    button_in = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_in") );
    button_out = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_out") );
    button_rotaciona_direita = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotaciona_direita") );
    button_rotaciona_esquerda = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotaciona_esquerda") );
    
    //Botoes Opcoes do usuario
    button_opcao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_opcao") );
    button_apply = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_apply") );
    button_passo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_passo") );
    button_tipo_recorte = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_tipo_recorte") );
    button_tipo_superficie = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_tipo_superficie") );
    button_t_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_t_linha") );
    button_tipo_vizualizacao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_tipo_vizualizacao") );
    
    //sinais de display
    g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);

    //sinais de desenho
    g_signal_connect (button_objetos, "button-release-event", G_CALLBACK (editor_de_mundo),NULL);
    g_signal_connect (button_objetos3D, "button-release-event", G_CALLBACK (editor_de_mundo3D),NULL);
    g_signal_connect (button_leitorOBJ, "button-release-event", G_CALLBACK (ler_arquivo),NULL);
    g_signal_connect (button_desenhar, "button-release-event", G_CALLBACK (desenha),NULL);
    g_signal_connect (button_desenhar1, "button-release-event", G_CALLBACK (desenha3D),NULL);
    g_signal_connect (button_desenhar2, "button-release-event", G_CALLBACK (desenha_arquivo),NULL);
    g_signal_connect (button_ponto, "button-release-event", G_CALLBACK (desenha_ponto),NULL);
    g_signal_connect (button_linha, "button-release-event", G_CALLBACK (desenha_linha),NULL);
    g_signal_connect (button_poligono, "button-release-event", G_CALLBACK (adiciona_vertice_poligono),NULL);
    g_signal_connect (button_poligono1, "button-release-event", G_CALLBACK (desenha_poligono),NULL);
    g_signal_connect (button_clear, "button-release-event", G_CALLBACK (clear),NULL);
    g_signal_connect (button_curva, "button-release-event", G_CALLBACK (desenha_curva),NULL);
    g_signal_connect (button_objeto3D, "button-release-event", G_CALLBACK (adiciona_vertice_objeto3D),NULL);
    g_signal_connect (button_objeto3D1, "button-release-event", G_CALLBACK (adiciona_verticeFinal_objeto3D),NULL);
    g_signal_connect (button_faceta, "button-release-event", G_CALLBACK (adiciona_faceta_objeto3D),NULL);
    g_signal_connect (button_faceta1, "button-release-event", G_CALLBACK (desenha_objeto3D),NULL);
    g_signal_connect (button_superficie3D, "button-release-event", G_CALLBACK (desenha_superficie3D),NULL);
 
    //sinais de transformacao
    g_signal_connect (button_transformar, "button-release-event", G_CALLBACK (transforma),NULL);
    g_signal_connect (button_transformar1, "button-release-event", G_CALLBACK (transforma3D),NULL);
    g_signal_connect (button_translacao, "button-release-event", G_CALLBACK (translada),NULL);
    g_signal_connect (button_escalonamento, "button-release-event", G_CALLBACK (escalona),NULL);
    g_signal_connect (button_rotacao, "button-release-event", G_CALLBACK (rotaciona),NULL);
    g_signal_connect (button_rotacaoCObj, "button-release-event", G_CALLBACK (rotacionaCObj),NULL);
    g_signal_connect (button_rotacaoPQ, "button-release-event", G_CALLBACK (rotacionaPQ),NULL);
    g_signal_connect (button_translacao1, "button-release-event", G_CALLBACK (translada3D),NULL);
    g_signal_connect (button_escalonamento1, "button-release-event", G_CALLBACK (escalona3D),NULL);
    g_signal_connect (button_rotacao3DX, "button-release-event", G_CALLBACK (rotaciona3DX),NULL);
    g_signal_connect (button_rotacao3DY, "button-release-event", G_CALLBACK (rotaciona3DY),NULL);
    g_signal_connect (button_rotacao3DZ, "button-release-event", G_CALLBACK (rotaciona3DZ),NULL);
    g_signal_connect (button_rotacao3DEQ, "button-release-event", G_CALLBACK (rotaciona3DEQ),NULL);

    //sinais controle da window
    g_signal_connect (button_up, "button-release-event", G_CALLBACK (move_up),NULL);
    g_signal_connect (button_down, "button-release-event", G_CALLBACK (move_down),NULL);
    g_signal_connect (button_left, "button-release-event", G_CALLBACK (move_left),NULL);
    g_signal_connect (button_right, "button-release-event", G_CALLBACK (move_right),NULL);
    g_signal_connect (button_in, "button-release-event", G_CALLBACK (zoom_in),NULL);
    g_signal_connect (button_out, "button-release-event", G_CALLBACK (zoom_out),NULL);
    g_signal_connect (button_rotaciona_direita, "button-release-event", G_CALLBACK (rotacionaWindowDireita),NULL);
    g_signal_connect (button_rotaciona_esquerda, "button-release-event", G_CALLBACK (rotacionaWindowEsquerda),NULL);
    
    //sinais de opcao do usuario
    g_signal_connect (button_opcao, "button-release-event", G_CALLBACK (opcoes),NULL);
    g_signal_connect (button_apply, "button-release-event", G_CALLBACK (configuracoes),NULL);

    gtk_builder_connect_signals(gtkBuilder, NULL);
    gtk_widget_show_all(window_widget);
    gtk_main ();
    return 0;
}	 	  	     	  	      	     	 	    	        	 	

            