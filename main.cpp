using namespace std;
#include <gtk/gtk.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <math.h>
#include "Window.h"
#include "ListaEnc.h"
#include "Poligono.h"
#include "Linha.h"
#include "Ponto.h"


static cairo_surface_t *surface = NULL;

//Windows
GtkWidget *window_widget;
GtkWidget *window_desenho;
GtkWidget *window_transformacao;
GtkWidget *window_objetos;
Window window;

//Display de objetos
GtkWidget *display;
GtkListStore *store;
GtkWidget *drawing_area;
GtkWidget *button_objetos;

//Botoes de Desenho
GtkWidget *button_draw;
GtkWidget *criar_ponto;
GtkWidget *criar_linha;
GtkWidget *criar_poligono;
GtkWidget *xi;
GtkWidget *yi;
GtkWidget *xf; 
GtkWidget *yf;
GtkWidget *button_clear;
GtkWidget *button_name;
GtkWidget *pontos;

//Botoes de desenho curva
GtkWidget *curvas;
GtkWidget *tipo;
GtkWidget *criar_curva;

//Botoes de Transformacao
GtkWidget *button_transformar;
GtkWidget *transformacao_x;
GtkWidget *transformacao_y;
GtkWidget *button_translacao;
GtkWidget *button_escalonamento;
GtkWidget *button_rotacao;
GtkWidget *button_rotacaoCObj;
GtkWidget *button_rotacaoPQ;
GtkWidget *angulo;

//Botoes de controle da window
GtkWidget *button_up;
GtkWidget *button_down;
GtkWidget *button_left;
GtkWidget *button_right; 
GtkWidget *button_in;
GtkWidget *button_out;
GtkWidget *button_rotaciona_direita;
GtkWidget *button_rotaciona_esquerda;

//Lista de objetos graficos
ListaEnc lista;

//Variaveis auxiliares;
double XvpMIN = 0;
double YvpMIN = 0;
double XvpMAX = 300;
double YvpMAX = 300;

double xis[100];
double ypsilon[100];
int size;
int pontosAdd;

double xisC[100];
double ypsilonC[100];
int pontosAddC;
int sizeCurva;
int type;
bool clip = true;

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

static void draw_linha(double a,double b,double c, double d){

    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, 3);
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
static void fill_model(string n){

    GtkTreeIter iter;
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,COL_NAME, n.c_str(),-1);
    gtk_widget_queue_draw(window_widget);

}

static GtkTreeModel * create_and_fill_model (void){

    GtkTreeIter    iter;
    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING);

    return GTK_TREE_MODEL (store);
}
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

//---------------------------------------------------------Transformadas de viewport--------------------------------------------------
static double transformadaX_viewport(double Xw){
    double XwMIN = window.getXmin();
    double XwMAX = window.getXmax();

    double aux = (Xw - XwMIN) / (XwMAX - XwMIN);

    double Xvp =  aux * double(XvpMAX-XvpMIN);
    return Xvp;
}

static double transformadaY_viewport(double Yw){
    double aux = (Yw - double(window.getYmin())) / double(window.getYmax() - window.getYmin());
    double Yvp = (1-aux) * double(YvpMAX-YvpMIN);
    return Yvp;
}

static void draw_window(){
   double Xmax = window.getXmax();
   double Ymax = window.getYmax();
   double Xmin = window.getXmin();
   double Ymin = window.getYmin();
   
   draw_linha(transformadaX_viewport(Xmax),transformadaY_viewport(Ymax),transformadaX_viewport(Xmax), transformadaY_viewport(Ymin));
   draw_linha(transformadaX_viewport(Xmax), transformadaY_viewport(Ymin),transformadaX_viewport(Xmin), transformadaY_viewport(Ymin));
   draw_linha(transformadaX_viewport(Xmin), transformadaY_viewport(Ymin),transformadaX_viewport(Xmin), transformadaY_viewport(Ymax));
   draw_linha(transformadaX_viewport(Xmin), transformadaY_viewport(Ymax),transformadaX_viewport(Xmax),transformadaY_viewport(Ymax));
 }
//-------------------------------------------------------------Atualiza��o da viewport-------------------------------------------------
static void atualiza_ponto(){
    double a;
    double b;

    //desenha os pontos
    for(int i =0;i<lista.getsizeP();i++){
        fill_model(lista.getNP(i));
        
        if(!window.getState()){
            a = lista.getXP(i);
            b = lista.getYP(i);
        }
        else{
            a = lista.getUP(i);
            b = lista.getVP(i);
        }
        if(window.clipPonto(a,b)){
            draw_linha(transformadaX_viewport(a),transformadaY_viewport(b),transformadaX_viewport(a),transformadaY_viewport(b));
        }
    }
}

static void atualiza_linha(){
    double a;
    double b;
    double c;
    double d;
    Linha aux;

    //desenha as linhas
    for(int i =0;i<lista.getsizeL();i++){
        fill_model(lista.getNL(i));
        
        if(!window.getState()){
            a = lista.getXL(i,true);
            b = lista.getYL(i,true);
            c = lista.getXL(i,false);
            d = lista.getYL(i,false);

        }
        else{
            a = lista.getUL(i,true);
            b = lista.getVL(i,true);
            c = lista.getUL(i,false);
            d = lista.getVL(i,false);

        }
        
        //draw_linha(transformadaX_viewport(a),transformadaY_viewport(b),transformadaX_viewport(c),transformadaY_viewport(d));
        aux = window.liangBarsky(a,b,c,d);
        
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getX(true)),transformadaY_viewport(aux.getY(true)),transformadaX_viewport(aux.getX(false)),transformadaY_viewport(aux.getY(false)));
        }
    }
}

static void atualiza_poligono(){
    double a;
    double b;
    double c;
    double d;
    Linha aux;

    //desenha os pontos
    for(int i =0;i<lista.getsizePL();i++){
        fill_model(lista.getNPL(i));
        
        
        for(int z = 0; z < lista.getSPL(i) - 1; z++ ){
            if(!window.getState()){
                a = lista.getXPL(i,z);
                b = lista.getYPL(i,z);
                c = lista.getXPL(i,z+1);
                d = lista.getYPL(i,z+1);
            }
            else{
                a = lista.getUPL(i,z);
                b = lista.getVPL(i,z);
                c = lista.getUPL(i,z+1);
                d = lista.getVPL(i,z+1);
            }
            
            aux = window.liangBarsky(a,b,c,d);
        
            if(aux.getEstado()){
                draw_linha(transformadaX_viewport(aux.getX(true)),transformadaY_viewport(aux.getY(true)),transformadaX_viewport(aux.getX(false)),transformadaY_viewport(aux.getY(false)));
            }
        }
        
        if(!window.getState()){
            a = lista.getXPL(i,lista.getSPL(i)-1);
            b = lista.getYPL(i,lista.getSPL(i)-1);
            c = lista.getXPL(i,0);
            d = lista.getYPL(i,0);
            }
        else{
            a = lista.getUPL(i,lista.getSPL(i)-1);
            b = lista.getVPL(i,lista.getSPL(i)-1);
            c = lista.getUPL(i,0);
            d = lista.getVPL(i,0);
        }
            
        aux = window.liangBarsky(a,b,c,d);
        
        if(aux.getEstado()){
            draw_linha(transformadaX_viewport(aux.getX(true)),transformadaY_viewport(aux.getY(true)),transformadaX_viewport(aux.getX(false)),transformadaY_viewport(aux.getY(false)));
        }
        
    }
}

static void atualiza_curva(){
    double anterior[2];
    double atual[2];
    Linha aux;

    int numC = 0;
    double t2 = 0;
    double t3 = 0;



    for(int i = 0;i<lista.getsizeC();i++){

        numC = ((lista.getSC(i) - 4)/3)+1;
        
        
        for(int z = 0; z < numC ;z++){
            anterior[0] = lista.getXC(i,z*3+0);
            anterior[1] = lista.getYC(i,z*3+0);

            for(double t=0; t <= 1;t = t + 0.002){
                t2 = t * t;
                t3 = t2 *t;

                atual[0] = (-t3 +3*t2 -3*t +1) * lista.getXC(i,z*3+0)  + (3*t3 -6*t2 +3*t) * lista.getXC(i,z*3+1) + (-3*t3 +3*t2) * lista.getXC(i,z*3+2) + (t3)*lista.getXC(i,z*3+3);
                atual[1] = (-t3 +3*t2 -3*t +1) * lista.getYC(i,z*3+0) + (3*t3 -6*t2 +3*t) * lista.getYC(i,z*3+1) + (-3*t3 +3*t2) * lista.getYC(i,z*3+2) + (t3)*lista.getYC(i,z*3+3);
                
                aux = window.liangBarsky(anterior[0],anterior[1],atual[0],atual[1]);
                
                if(aux.getEstado()){
                    draw_linha(transformadaX_viewport(aux.getX(true)),transformadaY_viewport(aux.getY(true)),transformadaX_viewport(aux.getX(false)),transformadaY_viewport(aux.getY(false)));
                }
        
                anterior[0] = atual[0];
                anterior[1] = atual[1];
            }
        fill_model(lista.getNC(i));    
        }
    }
}

static void atualiza_surface(){
    clear_surface();
    gtk_list_store_clear (store);
    
    draw_window();

    atualiza_ponto();
    atualiza_linha();
    atualiza_poligono();
    atualiza_curva();

}

static void clear(){
    gtk_widget_hide(window_objetos);
    lista.clearL();
    lista.clearP();
    lista.clearPL();
    lista.clearC();

    atualiza_surface();
}


//-------------------------------------------------------------------------Controle das Windows(programa)----------------------------------------------
static void editor_de_mundo(){
    gtk_widget_show_all(window_objetos);
}

static void desenha(){
    gtk_widget_hide(window_objetos);
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    if(!lista.temObjeto(n)){
        size = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(pontos));
        sizeCurva = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(curvas));
        sizeCurva = sizeCurva * 4;
        type = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(tipo));
        pontosAdd = 0;
        pontosAddC = 0;
        gtk_widget_show_all(window_desenho);
    }

}


static void transforma(){
    gtk_widget_hide(window_objetos);
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    if(lista.temObjeto(n)){
        gtk_widget_show_all(window_transformacao);
    }

}


//-------------------------------------------------------------M�todos de transforma��o(multiplica��o de matrizes)---------------------------------------------------------
static double * calculoCentroGeo(){
    double somaX = 0;
    double somaY = 0;

    for (int i = 0; i<size;i++){
        somaX = somaX + xis[i];
        somaY = somaY + ypsilon[i];
    }

    double r[2];
    r[0] = somaX/size;
    r[1] = somaY/size;
    return r;
}

static double * translacao(double x,double y, double dX,double dY){
    double p[3] = {x,y,1};
    double m[3][3] =
            {
                    {1,0,0},
                    {0,1,0},
                    {dX,dY,1}
            };

    double r[3];
    for (int i = 0; i < 3; ++i) {
        r[i] = (p[0]*m[0][i])+(p[1]*m[1][i])+(p[2]*m[2][i]);
    }
    //r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    //r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    //r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);
    return r;
}

static double * escalonamento(double x,double y, double sX,double sY){
    double p[3] = {x,y,1};
    double m[3][3] =
            {
                    {sX,0,0},
                    {0,sY,0},
                    {0,0,1}
            };

    double r[3];
    for (int i = 0; i < 3; ++i) {
        r[i] = (p[0]*m[0][i])+(p[1]*m[1][i])+(p[2]*m[2][i]);
    }
    //r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    //r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    //r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);
    return r;

}

static double * rotacao(double x,double y, double t){
    double p[3] = {x,y,1};
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
    //r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    //r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    //r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);

    return r;
}
//----------------------------------------------------------------------Transformacao do Objeto(translacao,rotacao e escalonamento)---------------------------------------------------------------------------------
static void translada(){
    double dX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_x));
    double dY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_y));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    double x;
    double y;
    double a;
    double b;
    double * aux;

    int type = lista.getType(n);

    if(type == 0){
        x = lista.getXP(n);
        y = lista.getYP(n);
        aux = translacao(x,y, dX,dY);
        lista.setP(*(aux),*(aux+1), n);
        gtk_widget_hide(window_transformacao);
        atualiza_surface();

    }
    else if(type == 1){
        x = lista.getXL(n,true);
        y = lista.getYL(n,true);
        aux = translacao(x,y, dX,dY);
        lista.setL(*(aux),*(aux+1), n,true);

        a = lista.getXL(n,false);
        b = lista.getYL(n,false);
        aux = translacao(a,b, dX,dY);
        lista.setL(*(aux),*(aux+1), n,false);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();

    }
    else if(type == 2){
        int q = lista.getSPL(n);

        for (int i =0; i<q; i++){
            x = lista.getXPL(n,i);
            y = lista.getYPL(n,i);
            aux = translacao(x,y, dX,dY);
            lista.setPL(*(aux),*(aux+1), n,i);
        }
        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
}

static void escalona(){
    double sX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_x));
    double sY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_y));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    double x;
    double y;
    double a;
    double b;
    double * aux;
    double * centro;

    int type = lista.getType(n);

    if(type == 0){
        x = lista.getXP(n);
        y = lista.getYP(n);

        aux = escalonamento(x,y,sX,sY);

        lista.setP(*(aux),*(aux+1), n);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();

    }
    else if(type == 1){
        x = lista.getXL(n,true);
        y = lista.getYL(n,true);
        a = lista.getXL(n,false);
        b = lista.getYL(n,false);

        xis[0] = x;
        ypsilon[0] = y;
        xis[1] = a;
        ypsilon[1] = b;
        size = 2;
        centro = calculoCentroGeo();

        aux = translacao(x,y, -*(centro),-*(centro+1));
        aux = escalonamento(*(aux),*(aux+1),sX,sY);
        aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
        lista.setL(*(aux),*(aux+1), n,true);

        aux = translacao(a,b, -*(centro),-*(centro+1));
        aux = escalonamento(*(aux),*(aux+1),sX,sY);
        aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
        lista.setL(*(aux),*(aux+1), n,false);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();


    }
    else if(type == 2){
        int q = lista.getSPL(n);
        size = q;
        for (int i =0; i<q; i++){
            xis[i] = lista.getXPL(n,i);
            ypsilon[i] = lista.getYPL(n,i);
        }
        centro = calculoCentroGeo();

        for (int i =0; i<q; i++){
            x = lista.getXPL(n,i);
            y = lista.getYPL(n,i);
            aux = translacao(x,y, -*(centro),-*(centro+1));
            aux = escalonamento(*(aux),*(aux+1),sX,sY);
            aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
            lista.setPL(*(aux),*(aux+1), n,i);
        }

        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
}

static void rotaciona(){
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(angulo));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    double x;
    double y;
    double a;
    double b;
    double * aux;
    double * centro;

    int type = lista.getType(n);

    if(type == 0){
        x = lista.getXP(n);
        y = lista.getYP(n);

        aux =  rotacao(x,y,teta);
        lista.setP(*(aux),*(aux+1), n);
        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
    else if(type == 1){
        x = lista.getXL(n,true);
        y = lista.getYL(n,true);
        aux = rotacao(x,y,teta);
        lista.setL(*(aux),*(aux+1), n,true);

        a = lista.getXL(n,false);
        b = lista.getYL(n,false);
        aux = rotacao(a,b,teta);
        lista.setL(*(aux),*(aux+1), n,false);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();

    }
    else if(type == 2){
        int q = lista.getSPL(n);

        for (int i =0; i<q; i++){
            x = lista.getXPL(n,i);
            y = lista.getYPL(n,i);
            aux = rotacao(x,y,teta);
            lista.setPL(*(aux),*(aux+1), n,i);
        }
        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
}

static void rotacionaCObj(){
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(angulo));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    double x;
    double y;
    double a;
    double b;
    double * aux;
    double * centro;

    int type = lista.getType(n);
    if(type == 0){
        gtk_widget_hide(window_transformacao);
    }
    else if(type == 1){
        x = lista.getXL(n,true);
        y = lista.getYL(n,true);
        a = lista.getXL(n,false);
        b = lista.getYL(n,false);

        xis[0] = x;
        ypsilon[0] = y;
        xis[1] = a;
        ypsilon[1] = b;
        size = 2;
        centro = calculoCentroGeo();

        aux = translacao(x,y, -*(centro),-*(centro+1));
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
        lista.setL(*(aux),*(aux+1), n,true);

        aux = translacao(a,b, -*(centro),-*(centro+1));
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
        lista.setL(*(aux),*(aux+1), n,false);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
    else if(type == 2){
        int q = lista.getSPL(n);
        size = q;
        for (int i =0; i<q; i++){
            xis[i] = lista.getXPL(n,i);
            ypsilon[i] = lista.getYPL(n,i);
        }
        centro = calculoCentroGeo();

        for (int i =0; i<q; i++){
            x = lista.getXPL(n,i);
            y = lista.getYPL(n,i);
            aux = translacao(x,y, -*(centro),-*(centro+1));
            aux = rotacao(*(aux),*(aux+1),teta);
            aux = translacao(*(aux),*(aux+1), *(centro),*(centro+1));
            lista.setPL(*(aux),*(aux+1), n,i);
        }

        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }

}

static void rotacionaPQ(){
    double pX = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_x));
    double pY = gtk_spin_button_get_value (GTK_SPIN_BUTTON(transformacao_y));
    double teta = gtk_spin_button_get_value (GTK_SPIN_BUTTON(angulo));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    double x;
    double y;
    double a;
    double b;
    double * aux;

    int type = lista.getType(n);

    if(type == 0){
        x = lista.getXP(n);
        y = lista.getYP(n);

        aux = translacao(x,y, -pX,-pY);
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = translacao(*(aux),*(aux+1), pX,pY);

        lista.setP(*(aux),*(aux+1), n);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();

    }
    else if(type == 1){
        x = lista.getXL(n,true);
        y = lista.getYL(n,true);
        a = lista.getXL(n,false);
        b = lista.getYL(n,false);

        aux = translacao(x,y, -pX,-pY);
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = translacao(*(aux),*(aux+1), pX,pY);
        lista.setL(*(aux),*(aux+1), n,true);

        aux = translacao(a,b, -pX,-pY);
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = translacao(*(aux),*(aux+1),pX,pY);
        lista.setL(*(aux),*(aux+1), n,false);

        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }
    else if(type == 2){
        int q = lista.getSPL(n);

        for (int i =0; i<q; i++){
            x = lista.getXPL(n,i);
            y = lista.getYPL(n,i);
            aux = translacao(x,y, -pX,-pY);
            aux = rotacao(*(aux),*(aux+1),teta);
            aux = translacao(*(aux),*(aux+1), pX,pY);
            lista.setPL(*(aux),*(aux+1), n,i);
        }

        gtk_widget_hide(window_transformacao);
        atualiza_surface();
    }

}

//------------------------------------------------------------------------------Rotac�o e normaliza��o do mundo(rota��o da window)--------------------------------------------------------------------
static void rotacionaPonto(){
    double x;
    double y;
    double * aux;
    double teta = window.getTeta();

    for(int i =0;i<lista.getsizeP();i++){
        x = lista.getXP(i);
        y = lista.getYP(i);
        aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = escalonamento(*(aux),*(aux+1), window.getXmax(),window.getYmax());

        lista.setPSCN(*(aux),*(aux+1),i);
    }

}
static void rotacionaLinha(){
    double a;
    double b;
    double c;
    double d;
    double * aux;
    double teta = window.getTeta();

    //desenha as linhas
    for(int i =0;i<lista.getsizeL();i++){
        a = lista.getXL(i,true);
        b = lista.getYL(i,true);
        aux = translacao(a,b, -window.getXcentro(),-window.getYcentro());
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = escalonamento(*(aux),*(aux+1), window.getXmax(),window.getYmax());
        lista.setLSCN(*(aux),*(aux+1), i,true);

        c = lista.getXL(i,false);
        d = lista.getYL(i,false);
        aux = translacao(c,d, -window.getXcentro(),-window.getYcentro());
        aux = rotacao(*(aux),*(aux+1),teta);
        aux = escalonamento(*(aux),*(aux+1), window.getXmax(),window.getYmax());

        lista.setLSCN(*(aux),*(aux+1), i,false);
    }

}
static void rotacionaPoligono(){
    double x;
    double y;
    double * aux;
    double teta = window.getTeta();

    //desenha poligonos

    for(int i =0;i<lista.getsizePL();i++){

        for(int z = 1; z<lista.getSPL(i);z++){
            x = lista.getXPL(i,z);
            y = lista.getYPL(i,z);

            aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
            aux = rotacao(*(aux),*(aux+1),teta);
            aux = escalonamento(*(aux),*(aux+1), window.getXmax(),window.getYmax());

            lista.setPLSCN(*(aux),*(aux+1),i,z);
        }
    }
}
//--------------------------------------------------------------------------Criadores de Objetos(ponto,linha,poligono)------------------------------------------------------------------------------------------------

static void desenha_ponto(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(xi));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(yi));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));

    lista.addP(a,b,n);
    if(window.getState()){
        rotacionaPonto();
    }

    gtk_widget_hide(window_desenho);
    atualiza_surface();
}

static void desenha_linha(){
    double a = gtk_spin_button_get_value (GTK_SPIN_BUTTON(xi));
    double b = gtk_spin_button_get_value (GTK_SPIN_BUTTON(yi));
    double c = gtk_spin_button_get_value (GTK_SPIN_BUTTON(xf));
    double d = gtk_spin_button_get_value (GTK_SPIN_BUTTON(yf));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));

    lista.addL(a,b,c,d,n);
    
    if(window.getState()){
        rotacionaLinha();

    }

    gtk_widget_hide(window_desenho);
    atualiza_surface();

}

static void desenha_poligono(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(xi));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(yi));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));

    xis[pontosAdd] = x;
    ypsilon[pontosAdd] = y;
    pontosAdd+= 1;

    if(pontosAdd == size){
        lista.addPL(size,xis,ypsilon,n);
        if(window.getState()){
            rotacionaPoligono();
        }
        gtk_widget_hide(window_desenho);
        atualiza_surface();
    }
}

static void desenha_curva(){
    double x = gtk_spin_button_get_value (GTK_SPIN_BUTTON(xi));
    double y = gtk_spin_button_get_value (GTK_SPIN_BUTTON(yi));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    bool t = false;
    
    if(type = 1){
        size = sizeCurva;
        t = true;
    }
    
    xisC[pontosAddC] = x;
    ypsilonC[pontosAddC] = y;
    pontosAddC+= 1;

    if(pontosAddC == size){
        lista.addC(xisC,ypsilonC,n,size,t);
        
        gtk_widget_hide(window_desenho);
        atualiza_surface();
    }

}

//---------------------------------------------------------------------------------------Controle da Window(rota��o,movimenta��o,zoom)-----------------------------------------------------------------------------------------------------
static void rotacionaWindowEsquerda(){
    double teta = -15;
    window.addTeta(teta);

    double x;
    double y;
    double * aux;

    window.calculaCentro();

    /*----------------------------------------------Rotaciona Window
    x = window.getXmax();
    y = window.getYmax();
    aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
    aux = rotacao(*(aux),*(aux+1),teta);
    window.setMAX(*(aux),*(aux+1));
    
    x = window.getXmin();
    y = window.getYmin();
    aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
    aux = rotacao(*(aux),*(aux+1),teta);
    window.setMIN(*(aux),*(aux+1));
    */
    window.rotate();

    //-----------------------------------Rotaciona o Mundo
    rotacionaPonto();
    rotacionaLinha();
    rotacionaPoligono();

    /*/-------------------------------------atualiza Surface
    x = window.getXmax();
    y = window.getYmax();
    aux = translacao(x,y, window.getXcentro(),window.getYcentro());
    window.setMAX(*(aux),*(aux+1));
    
    x = window.getXmin();
    y = window.getYmin();
    aux = translacao(x,y, window.getXcentro(),window.getYcentro());
    window.setMIN(*(aux),*(aux+1));*/

    atualiza_surface();
}

static void rotacionaWindowDireita(){
    double teta = 15;
    window.addTeta(teta);

    double x;
    double y;
    double * aux;

    window.calculaCentro();

    /*/----------------------------------------------Rotaciona Window
    x = window.getXmax();
    y = window.getYmax();
    aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
    aux = rotacao(*(aux),*(aux+1),teta);
    window.setMAX(*(aux),*(aux+1));
    
    
    x = window.getXmin();
    y = window.getYmin();
    aux = translacao(x,y, -window.getXcentro(),-window.getYcentro());
    aux = rotacao(*(aux),*(aux+1),teta);
    window.setMIN(*(aux),*(aux+1));
    */
    window.rotate();

    //-----------------------------------Rotaciona o Mundo
    rotacionaPonto();
    rotacionaLinha();
    rotacionaPoligono();

    /*/-------------------------------------atualiza Surface
    
    x = window.getXmax();
    y = window.getYmax();
    aux = translacao(x,y, window.getXcentro(),window.getYcentro());
    window.setMAX(*(aux),*(aux+1));
    
    x = window.getXmin();
    y = window.getYmin();
    aux = translacao(x,y, window.getXcentro(),window.getYcentro());
    window.setMIN(*(aux),*(aux+1));
    */
    atualiza_surface();


}

static void move_up(){

    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Ymax += 10;
    Ymin += 10;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();

}

static void move_down(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Ymax -= 10;
    Ymin -= 10;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void move_left(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Xmax -= 10;
    Xmin -= 10;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void move_right(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    Xmax += 10;
    Xmin += 10;

    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);

    atualiza_surface();
}

static void zoom_in(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    double dX = 10;
    double dY = 10;

    window.setMAX(Xmax-dX,Ymax-dY);
    window.setMIN(Xmin+dX,Ymin+dY);

    atualiza_surface();
}

static void zoom_out(){
    double Xmax = window.getXmax();
    double Ymax = window.getYmax();
    double Xmin = window.getXmin();
    double Ymin = window.getYmin();

    double dX = 10;
    double dY = 10;

    window.setMAX(Xmax+dX,Ymax+dY);
    window.setMIN(Xmin-dX,Ymin-dY);

    atualiza_surface();

}

int main(int argc, char *argv[]){
    window = Window(500,500,0,0);
    lista = ListaEnc();

    //Builder
    GtkBuilder  *gtkBuilder;
    gtk_init(&argc, &argv);
    gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder, "CG_Application.glade", NULL);

    //Display
    window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
    window_desenho = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_desenho") );
    window_transformacao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_transformacao") );
    window_objetos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_objetos") );
    drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
    display = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "display") );
    create_Model();
    button_objetos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_objetos") );


    //Botoes de desenho
    button_name = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_name") );
    button_clear = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_clear") );
    button_draw = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_draw") );
    criar_ponto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_ponto") );
    criar_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_linha") );
    criar_poligono = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_poligono") );
    xi = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "xi") );
    yi = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "yi") );
    xf = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "xf") );
    yf = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "yf") );
    pontos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "pontos") );
    
    //Botoes de desenho de curva
    criar_curva = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_curva") );
    curvas = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "curvas") );
    tipo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "tipo") );
    
    
    
    
    //Botoes de controle da window
    button_up = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_up") );
    button_down = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_down") );
    button_left = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_left") );
    button_right = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_right") );
    button_in = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_in") );
    button_out = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_out") );
    button_rotaciona_direita = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotaciona_direita") );
    button_rotaciona_esquerda = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotaciona_esquerda") );

    //Botoes de Transformacao
    button_transformar= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_transformar") );
    transformacao_x= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "transformacao_x") );
    transformacao_y= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "transformacao_y") );
    button_translacao= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_translacao") );
    button_escalonamento= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_escalonamento") );
    button_rotacao= GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacao") );
    button_rotacaoCObj = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacaoCObj") );
    button_rotacaoPQ = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_rotacaoPQ") );
    angulo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "angulo") );

    //sinais de display
    g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);

    //sinais de desenho
    g_signal_connect (button_objetos, "button-release-event", G_CALLBACK (editor_de_mundo),NULL);
    g_signal_connect (button_draw, "button-release-event", G_CALLBACK (desenha),NULL);
    g_signal_connect (criar_ponto, "button-release-event", G_CALLBACK (desenha_ponto),NULL);
    g_signal_connect (criar_linha, "button-release-event", G_CALLBACK (desenha_linha),NULL);
    g_signal_connect (criar_poligono, "button-release-event", G_CALLBACK (desenha_poligono),NULL);
    g_signal_connect (button_clear, "button-release-event", G_CALLBACK (clear),NULL);
    g_signal_connect (criar_curva, "button-release-event", G_CALLBACK (desenha_curva),NULL);
 
    //sinais de transformacao
    g_signal_connect (button_transformar, "button-release-event", G_CALLBACK (transforma),NULL);
    g_signal_connect (button_translacao, "button-release-event", G_CALLBACK (translada),NULL);
    g_signal_connect (button_escalonamento, "button-release-event", G_CALLBACK (escalona),NULL);
    g_signal_connect (button_rotacao, "button-release-event", G_CALLBACK (rotaciona),NULL);
    g_signal_connect (button_rotacaoCObj, "button-release-event", G_CALLBACK (rotacionaCObj),NULL);
    g_signal_connect (button_rotacaoPQ, "button-release-event", G_CALLBACK (rotacionaPQ),NULL);

    //sinais controle da window
    g_signal_connect (button_up, "button-release-event", G_CALLBACK (move_up),NULL);
    g_signal_connect (button_down, "button-release-event", G_CALLBACK (move_down),NULL);
    g_signal_connect (button_left, "button-release-event", G_CALLBACK (move_left),NULL);
    g_signal_connect (button_right, "button-release-event", G_CALLBACK (move_right),NULL);
    g_signal_connect (button_in, "button-release-event", G_CALLBACK (zoom_in),NULL);
    g_signal_connect (button_out, "button-release-event", G_CALLBACK (zoom_out),NULL);
    g_signal_connect (button_rotaciona_direita, "button-release-event", G_CALLBACK (rotacionaWindowDireita),NULL);
    g_signal_connect (button_rotaciona_esquerda, "button-release-event", G_CALLBACK (rotacionaWindowEsquerda),NULL);

    gtk_builder_connect_signals(gtkBuilder, NULL);
    gtk_widget_show_all(window_widget);
    gtk_main ();
    return 0;
}	 	  	     	  	      	     	 	    	        	 	
