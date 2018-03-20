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
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkWidget *window_ponto;
GtkWidget *window_linha;
GtkWidget *window_poligono;
Window window;

GtkWidget *button_clear;
GtkWidget *button_color;
GtkWidget *button_name;
GtkWidget *poligono_s;

//Display de objetos
GtkWidget *display;
GtkListStore *store;

//Botoes de coordenadas
GtkWidget *button_ponto;
GtkWidget *criar_ponto;
GtkWidget *button_linha;
GtkWidget *criar_linha;
GtkWidget *button_poligono;
GtkWidget *criar_poligono;
GtkWidget *linha_x;
GtkWidget *linha_y;
GtkWidget *linha_x1;
GtkWidget *linha_y1;
GtkWidget *ponto_x;
GtkWidget *ponto_y;
GtkWidget *poligono_x;
GtkWidget *poligono_y;

//Botoes de controle da window
GtkWidget *button_up;
GtkWidget *button_down;
GtkWidget *button_left;
GtkWidget *button_right;
GtkWidget *button_in;
GtkWidget *button_out;
GtkWidget *passo;

//Lista de objetos graficos
ListaEnc lista;

double XvpMIN = 0;
double YvpMIN = 0;
double XvpMAX = 500;
double YvpMAX = 500;

double xis[100];
double ypsilon[100];
int size;
int pontosAdd;

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

static vector<double> translacao(double x,double y, double dX,double dY){
    double p[3] = {x,y,1};
    double m[3][3] = 
    {
        {1,0,0},
        {0,1,0},
        {dX,dY,1}
    };
    
    vector<double> r(3);
    
    
    r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);

    return r;
}

static vector<double> escalonamento(double x,double y, double sX,double sY){
    double p[3] = {x,y,1};
    double m[3][3] = 
    {
        {sX,0,0},
        {0,sY,0},
        {0,0,1}
    };

    vector<double> r(3);
    
    r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);
    
    return r;
    
}

static vector<double> rotacao(double x,double y, double t){
    double p[3] = {x,y,1};
    double m[3][3] = 
    {
        {cos(t),-sin(t),0},
        {sin(t),cos(t),0},
        {0,0,1}
    };
    
    vector<double> r(3);
    
    r[0] = (p[0]*m[0][0])+(p[1]*m[1][0])+(p[2]*m[2][0]);
    r[1] = (p[0]*m[0][1])+(p[1]*m[1][1])+(p[2]*m[2][1]);
    r[2] = (p[0]*m[0][2])+(p[1]*m[1][2])+(p[2]*m[2][2]);
    
    return r;
}

/*Function that will be called when the ok button is pressed*/
static void draw_linha(double a,double b,double c, double d){
    
    cairo_t *cr;
    cr = cairo_create (surface);
    cairo_set_line_width (cr, 5);
    //gdk_cairo_set_source_rgba (cr,const GdkRGBA *rgba);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); /* Round dot*/
    cairo_move_to (cr, a, b);
    cairo_line_to (cr, c, d);/* a very short line is a dot */
    cairo_stroke (cr);    
    gtk_widget_queue_draw (window_widget);
 }
 
  static void draw_window(){
    double aX = 0;
    double aY = 0;
    double bX = 500;
    double bY = 500;
    
    draw_linha(aX,aY,bX,aY);
    draw_linha(bX,aY,bX,bY);
    draw_linha(bX,bY,aX,bY);
    draw_linha(aX,bY,aX,aY);
    
     
 }
 
 static void showDisplay(){
    
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, "teste", -1);
    
    
    
    gtk_tree_view_set_model (GTK_TREE_VIEW (display), GTK_TREE_MODEL (store));
    gtk_widget_show_all (window_widget);
 }
 
 static void atualiza_surface(){
    clear_surface();
    double a;
    double b;
    double c;
    double d;
    
    //desenha os pontos
    for(int i =0;i<lista.getsizeP();i++){
        a = lista.getpX(i);
        b = lista.getpY(i);
        draw_linha(transformadaX_viewport(a),transformadaY_viewport(b),transformadaX_viewport(a),transformadaY_viewport(b));    
    } 
    
    //desenha as linhas
    for(int i =0;i<lista.getsizeL();i++){
        a = lista.getlX(i);
        b = lista.getlY(i);
        c = lista.getlX2(i);
        d = lista.getlY2(i);
        draw_linha(transformadaX_viewport(a),transformadaY_viewport(b),transformadaX_viewport(c),transformadaY_viewport(d));    
    } 
    /*
    for(int i =0;i<lista.getsizePL();i++){
        
        double primeiro[2];
        primeiro[0] = lista.getXdoPoligono(i,0);
        primeiro[1] = lista.getYdoPoligono(i,0);
        double anterior[2];
        anterior[0] = primeiro[0];
        anterior[1] = primeiro[1];
        double atual[2];
        
        for(int z = 1; z<lista.getSdoPoligono(i);z++){
            atual[0] = lista.getXdoPoligono(i,z);
            atual[1] = lista.getYdoPoligono(i,z);
            draw_linha(transformadaX_viewport(anterior[0]),transformadaY_viewport(anterior[1]),transformadaX_viewport(anterior[0]),transformadaY_viewport(anterior[0]));
            anterior[0] = atual[0];
            anterior[1] = atual[1];
        }
        //draw_linha(transformadaX_viewport(anterior[0]),transformadaY_viewport(anterior[1]),transformadaX_viewport(primeiro[0]),transformadaY_viewport(primeiro[1]));
    } */
    
    //desenha a window
    //draw_window();
    
 }
 
static void clear(){
    lista.clearL();
    lista.clearP();
    
    atualiza_surface();
}

static void cria_ponto(){
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    if(!lista.temObjeto(n)){
        gtk_widget_show_all(window_ponto);
    }
}

static void desenha_ponto(){
    int a = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(ponto_x));
    int b = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(ponto_y));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    
    lista.addP(a,b,n);
    
    gtk_widget_hide(window_ponto);
    atualiza_surface();
    
    
}

/*Function that will be called when the ok button is pressed*/
static void cria_linha(){
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    if(!lista.temObjeto(n)){
        gtk_widget_show_all(window_linha);
    }
 }
static void desenha_linha(){
    double a = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(linha_x));
    double b = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(linha_y));
    double c = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(linha_x1));
    double d = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(linha_y1));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    
    lista.addL(a,b,c,d,n);
    
    gtk_widget_hide(window_linha);
    atualiza_surface();
    
 }
 
static void cria_poligono(){
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    
    if(!lista.temObjeto(n)){
        size = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(poligono_s));
        pontosAdd = 0;
        gtk_widget_show_all(window_poligono);
    }
}

static void desenha_poligono(){
    double x = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(poligono_x));
    double y = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(poligono_y));
    string n = gtk_entry_get_text (GTK_ENTRY (button_name));
    
    xis[pontosAdd] = x;
    ypsilon[pontosAdd] = y;
    pontosAdd+= 1;
    
    if(pontosAdd == size){
        lista.addPL(size,xis,ypsilon,n);
        gtk_widget_hide(window_poligono);
        atualiza_surface();
    
    }
} 
 static void move_up(){
     
    int Xmax = window.getXmax();
    int Ymax = window.getYmax();
    int Xmin = window.getXmin();
    int Ymin = window.getYmin();
    
    Ymax += 10;
    Ymin += 10;
    
    
    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);
    
    atualiza_surface();
    
}

static void move_down(){
    int Xmax = window.getXmax();
    int Ymax = window.getYmax();
    int Xmin = window.getXmin();
    int Ymin = window.getYmin();
    
    Ymax -= 10;
    Ymin -= 10;
    
    
    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);
    
    atualiza_surface();
}

 static void move_left(){
    int Xmax = window.getXmax();
    int Ymax = window.getYmax();
    int Xmin = window.getXmin();
    int Ymin = window.getYmin();
    
    Xmax += 10;
    Xmin += 10;
    
    
    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);
    
    atualiza_surface();
}

static void move_right(){
    int Xmax = window.getXmax();
    int Ymax = window.getYmax();
    int Xmin = window.getXmin();
    int Ymin = window.getYmin();
    
    Xmax -= 10;
    Xmin -= 10;
    
    
    window.setMAX(Xmax,Ymax);
    window.setMIN(Xmin,Ymin);
    
    atualiza_surface();
 }
 
  static void zoom_in(){
    int p = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(passo));

 }
 
static void zoom_out(){
    int p = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(passo));

 }
 
    

int main(int argc, char *argv[]){
  window = Window(500,500,0,0);
  lista = ListaEnc();
  
  //Builder
  GtkBuilder  *gtkBuilder;
  gtk_init(&argc, &argv);
  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "CG_Application.glade", NULL);
  
  //Área de desenho
  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
  window_ponto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_ponto") );
  window_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_linha") );
  window_poligono = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "window_poligono") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
  display = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "display") );
  store = GTK_LIST_STORE( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "store") );
  
  button_color = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_color") );
  button_name = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_name") );
  
  //Botoes de coordenadas
  button_clear = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_clear") );
  button_ponto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_ponto") );
  criar_ponto = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_ponto") );
  button_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_linha") );
  criar_linha = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_linha") );
  button_poligono = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_poligono") );
  criar_poligono = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "criar_poligono") );
  ponto_x = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "ponto_x") );
  ponto_y = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "ponto_y") );
  linha_x = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "linha_x") );
  linha_y = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "linha_y") );
  linha_x1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "linha_x1") );
  linha_y1 = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "linha_y1") );
  poligono_x = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "ponto_x") );
  poligono_y = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "ponto_y") );
  poligono_s = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "poligono_s") );
  
  //Botoes de controle da window
  button_up = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_up") );
  button_down = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_down") );
  button_left = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_left") );
  button_right = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_right") );
  passo = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "passo") );
  button_in = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_in") );
  button_out = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_out") );
  
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  
  //desenhar
  g_signal_connect (button_ponto, "button-release-event", G_CALLBACK (cria_ponto),NULL);
  g_signal_connect (criar_ponto, "button-release-event", G_CALLBACK (desenha_ponto),NULL);
  g_signal_connect (button_linha, "button-release-event", G_CALLBACK (cria_linha),NULL);
  g_signal_connect (criar_linha, "button-release-event", G_CALLBACK (desenha_linha),NULL);
  g_signal_connect (button_poligono, "button-release-event", G_CALLBACK (cria_poligono),NULL);
  g_signal_connect (criar_poligono, "button-release-event", G_CALLBACK (desenha_poligono),NULL);
  g_signal_connect (button_clear, "button-release-event", G_CALLBACK (clear),NULL);
  
  
  //sinais controle da window
  g_signal_connect (button_up, "button-release-event", G_CALLBACK (move_up),NULL);
  g_signal_connect (button_down, "button-release-event", G_CALLBACK (move_down),NULL);
  g_signal_connect (button_left, "button-release-event", G_CALLBACK (move_left),NULL);
  g_signal_connect (button_right, "button-release-event", G_CALLBACK (move_right),NULL);
  g_signal_connect (button_in, "button-release-event", G_CALLBACK (zoom_in),NULL);
  g_signal_connect (button_out, "button-release-event", G_CALLBACK (zoom_out),NULL);
  
  gtk_builder_connect_signals(gtkBuilder, NULL);
  gtk_widget_show_all(window_widget);
  //draw_window();
  gtk_main ();
  
  
  
  
  return 0;
}	 	  	     	  	      	     	 	    	        	 	

